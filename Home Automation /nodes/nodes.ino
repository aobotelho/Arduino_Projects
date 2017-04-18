/********************************************************************************
 *                      André Botelho - Home automation project                 *
 *                                                                              *   
 * This code requires a few third party libraries                               *
 *      https://github.com/knolleary/pubsubclient                               *
 *      https://github.com/tzapu/WiFiManager                                    *  
 *                                                                              *
 ********************************************************************************/
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

//Defines all type of nodes possible
enum nodeType{
  lightType=0,
  switchType,
  doubleSwitch
};

//Defines all possible status for the nodes
enum nodeStatus{
  nodeON = 0,
  nodeOFF
};

//Each node will have one type, defined by the enum abovesudp
const nodeType NODE_TYPE = switchType;


//This is the specific topic in which the server will use to update the status of every node
String STATUS_TOPIC = "/update/status";
//This is the specific topic in which the server will be listening to in order to update every node status
String STATUS_CALLBACK = "/update/callback/";
//This is the specific topic in which the server will receive the information that a new device is available
String DISCOVER_TOPIC = "/discover/newDevices";
//This is the specific topic in which the server will send the confirmation (or not) that the device was added
String DISCOVER_CALLBACK = "/discover/registerDevice/";
//This is the specific topic in which the server will send the output status
//The update topic is in the format "/NodeID/status", set further on on the code
String UPDATE_TOPIC;
//This is the specific topic in whic the server will send a reset message
String RESET_TOPIC = "/update/reset/";



//Here you have to UNCOMMENT the specific part for your project
#if NODE_TYPE == lightType
#endif
#if NODE_TYPE == switchType
  /////////////////////////////////////////////////////
  // CHANGE BACK TO 2!!!!!!!!!!!
  /////////////////////////////////////////////////
  const int outputPin = LED_BUILTIN;
  boolean outputStatus = nodeOFF;
  const int inputPin = 3;
#endif
#if NODE_TYPE == doubleSwitch
  const int outputPin1 = 2;
  boolean outputStatus1 = nodeOFF;
  const int inputPin1 = 3;
  const int outputPin2 = 0;
  boolean outputStatus2 = nodeOFF;
  const int inputPin2 = 1;
#endif


//Boolean variable that checks if the node is registered or not
boolean nodeRegistered = false;

//Define the string that will be sent as payload for the server when registering
//This string will simulate a JSON object that will be parsed by the server side using python
//Status will start as OFF since it will only turn on after registration
//const String registerPayload = "{""id"":" + String(NODE_ID) + ",""type"":" + NODE_TYPE +  + ",""status"":""OFF""}";
String registerPayload;

//Define the MQTT variables and values
const char* mqtt_server = "192.168.1.35";
#define MQTT_PORT 1883
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
unsigned int resetCounter = 0;

WiFiClient wificlient;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient client(wificlient);
WiFiManager wifiManager;

const String CLOUD_USER = "andrefrejat@gmail.com";
const String CLOUD_PASS = "45f66b00";
void cloudCallback(char* topic, byte* payload, unsigned int length);
PubSubClient cloudClient("mqtt.dioty.co", 1883, cloudCallback, wificlient);


//WI-FI settings
uint8_t macAddr[6];
String macAddrStr;

//# to define if serial is being used or not
#define USE_SERIAL  true

unsigned long previousMillis = 0;
const long interval = 1000;           


void setup(){
  switch(NODE_TYPE){
    case lightType:
      break;
    case switchType:
      pinMode(outputPin,OUTPUT);
      digitalWrite(outputPin,outputStatus);
      pinMode(inputPin,INPUT);
    case doubleSwitch:
      pinMode(outputPin1,OUTPUT);
      digitalWrite(outputPin1,outputStatus1);
      pinMode(inputPin1,INPUT);
      pinMode(outputPin2,OUTPUT);
      digitalWrite(outputPin2,outputStatus2);
      pinMode(inputPin2,INPUT);
      break;
  }

  //Configure serial if USE_SERIAL is set to true
  #if USE_SERIAL
    Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY);
    Serial.println("\nHello!!");
    Serial.print("My current node type is: ");
    if(NODE_TYPE==lightType) Serial.println("light!");
    else if(NODE_TYPE==switchType) Serial.println("Switch!");
    else if(NODE_TYPE==doubleSwitch) Serial.println("Double switch!");
  #endif

  if(!connectWifi()){
    wifiManager.resetSettings();
  }
  
  WiFi.macAddress(macAddr);

  {
  #if USE_SERIAL
    Serial.println("Wifi connected!");
    Serial.print("IP Address: ");    
    Serial.println(WiFi.localIP());
    Serial.println("Mac Address: " + String(macAddr[0],16) + ":" + String(macAddr[1],16) + ":" + String(macAddr[2],16) + ":" + 
                                     String(macAddr[3],16) + ":" + String(macAddr[4],16) + ":" + String(macAddr[5],16));
  #endif
  }

  //Once the MAC address has been figured out we have to update the topics to subscribe
  for(int i=0;i<6;i++){
    macAddrStr += String(macAddr[i],16);  
    if(String(macAddr[i],16)=="0"){
      macAddrStr += '0';
    }
  }

  DISCOVER_CALLBACK += macAddrStr;
  STATUS_CALLBACK += macAddrStr;
  UPDATE_TOPIC = "/" + macAddrStr + "/status";
  registerPayload = "{\"id\":\"" + macAddrStr + "\",\"type\":" + NODE_TYPE +  ",\"status\":\"OFF\"}";
  
  //Once connected to WiFi connects to MQTT server
  #if USE_SERIAL
    Serial.println("Trying to connect to MQTT server...");
  #endif

  #if USE_SERIAL
      (connectMQTT() ? Serial.println("Connected!") : Serial.println("Not connected...."));
  #else
      connectMQTT();
  #endif
  
  #if USE_SERIAL
      (connectCloudMQTT() ? Serial.println("Connected to Cloud MQTT!") : Serial.println("Not connected to Cloud MQTT...."));
      Serial.println("\nNow trying to register with the server");
  #else
      connectCloudMQTT();
  #endif

  client.subscribe((char*)DISCOVER_CALLBACK.c_str());
  
  
  while(!registerDevice());

  client.subscribe((char*)STATUS_TOPIC.c_str());
  client.subscribe((char*)UPDATE_TOPIC.c_str());
  client.subscribe((char*)RESET_TOPIC.c_str());
  #if USE_SERIAL
    Serial.println("Subscribing to: ");
    Serial.println(STATUS_TOPIC);
    Serial.println(UPDATE_TOPIC);
    Serial.println(RESET_TOPIC);
  #endif
}


void loop(){  
  //maintain MQTT connection
  client.loop();
  cloudClient.loop();

  //MUST delay to allow ESP8266 WIFI functions to run
  delay(50);
  /*
  switch(NODE_TYPE){
    case lightType:
      break;
    case switchType:
      if(!digitalRead(inputPin)){
        while(!digitalRead(inputPin)){delay(10);}
        delay(100);
        #if USE_SERIAL
          Serial.println("Changing the output status");
        #endif
        outputStatus = !outputStatus;
        digitalWrite(outputPin,outputStatus);
        client.publish((char*)STATUS_CALLBACK.c_str(),(outputStatus?"OFF":"ON"));
        cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK).c_str(),(outputStatus?"OFF":"ON"));
          
      }
    case doubleSwitch:
      if(!digitalRead(inputPin1)){
        while(!digitalRead(inputPin1)){delay(10);}
        delay(100);
        #if USE_SERIAL
          Serial.println("Changing the output status");
        #endif
        outputStatus1 = !outputStatus1;
        digitalWrite(outputPin1,outputStatus1);      
        
        client.publish((char*)(STATUS_CALLBACK + "/switch1").c_str(),(outputStatus1?"OFF":"ON"));
        cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK + "/switch1").c_str(),(outputStatus1?"OFF":"ON"));
        
      }
      if(!digitalRead(inputPin2)){
        while(!digitalRead(inputPin2)){delay(10);}
        delay(100);
        #if USE_SERIAL
          Serial.println("Changing the output status");
        #endif
        outputStatus2 = !outputStatus2;
        digitalWrite(outputPin2,outputStatus2);
        
        client.publish((char*)(STATUS_CALLBACK + "/switch2").c_str(),(outputStatus2?"OFF":"ON"));
        cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK + "/switch2").c_str(),(outputStatus2?"OFF":"ON"));
        
      }
      break;
  }

  */
  //The input pin has a pull-up resistor
  //If read low then the momentary switch is pressed
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if(!client.connected() && !cloudClient.connected()){
      if(!connectMQTT()){
        if(!connectCloudMQTT()){
          ESP.restart();
        }
      }
    }

    if(!wificlient.connected()){connectWifi();}
  }
 
}





//Function to loop while it tries to connect to Wifi network
boolean connectWifi(){

  #if not USE_SERIAL
    wifiManager.setDebugOutput(false);
  #endif
  
  if(!wifiManager.autoConnect("Home Automation Setup")){
    wifiManager.resetSettings();
    wifiManager.addParameter(&custom_mqtt_server);
  }
  wifiManager.addParameter(&custom_mqtt_server);
  return true;
}

//Function to loop while it tries to connect to MQTT server
boolean connectMQTT(){
  mqtt_server = custom_mqtt_server.getValue();
  client.setServer(mqtt_server, 1883);
  
  unsigned int counter=0;
  while(!client.connect((char*)macAddrStr.c_str())){
    delay(100);
    if(counter == 20){
      #if USE_SERIAL
        Serial.println("Failed to connect");
      #endif
      return false;
    }
    else{
      counter++;
    }
  }

  client.setCallback(callback);
  
  //If got out of the while loop it managed to connect
  return true;
}


boolean connectCloudMQTT(){  
  unsigned int counter=0;
  while(!cloudClient.connect((char*)CLOUD_USER.c_str(),(char*)CLOUD_USER.c_str(),(char*)CLOUD_PASS.c_str())){
    delay(100);
    if(counter == 20){
      #if USE_SERIAL
        Serial.println("Failed to connect Cloud");
      #endif
      return false;
    }
    else{
      counter++;
    }
  }

  //cloudClient.setCallback(cloudCallback);
  cloudClient.publish((char*)("/" + CLOUD_USER+"/test").c_str(),"To vivao");  
  Serial.print("/" + CLOUD_USER+"/test");
  cloudClient.subscribe((char*)("/" + CLOUD_USER +"/#").c_str());
  
  //If got out of the while loop it managed to connect
  return true;
}

//Function to loop while it tries to regioutputStatusster with the server
boolean registerDevice(){
    unsigned int counter = 0;
    while(!nodeRegistered){
      #if USE_SERIAL
        Serial.println("..Device still not registered...");
      #endif

      char tempChar[50];

      registerPayload.toCharArray(tempChar,50);
      
      //client.publish((char*)DISCOVER_TOPIC.c_str(),(char*)registerPayload.c_str());
      #if USE_SERIAL
        Serial.print("Publishing: ");
        Serial.println(tempChar);
      #endif
      client.publish((char*)DISCOVER_TOPIC.c_str(),tempChar);
      //maintain MQTT connection
      client.loop();
      delay(1500);    

      if(counter==100){
        #if USE_SERIAL
          Serial.println("Failed to register");
        #endif
        ESP.restart();
      }
      else{
        counter++;
      }
    }

    return true;
}


void callback(char* topic, byte* payload, unsigned int length) {
  String payloadStr = "";
  for(int i=0;i<length;i++){
    payloadStr += (char)payload[i];
  }
  
  #if USE_SERIAL
    Serial.println("Received MQTT message!");
    Serial.print("Topic: ");
    Serial.println(String(topic));
    Serial.print("Payload: ");
    Serial.println(payloadStr);
  #endif

  if(String(topic)==(DISCOVER_CALLBACK) && !nodeRegistered){
    #if USE_SERIAL
      Serial.println("Discover topic received");
    #endif
    if(payloadStr == "Registered"){
      nodeRegistered = true;
      client.unsubscribe((char*)DISCOVER_CALLBACK.c_str());
      #if USE_SERIAL
        Serial.println("Node registration successfull! =D");
      #endif
    }
    else{
      #if USE_SERIAL
        Serial.println("Node registration NOT successfull");
      #endif
    }
  }
  switch(NODE_TYPE){
    case  lightType:
      break;
    case switchType:
      if(String(topic)==STATUS_TOPIC){
        #if USE_SERIAL
          Serial.println("Status topic received");
        #endif 
        client.publish((char*)STATUS_CALLBACK.c_str(),(outputStatus?"OFF":"ON"));
        cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK).c_str(),(outputStatus?"OFF":"ON"));
      }
  
      else if(String(topic)==UPDATE_TOPIC){
        #if USE_SERIAL
          Serial.println("Update topic received");
        #endif 
        if(payloadStr == "ON"){
          outputStatus = nodeON;
          digitalWrite(outputPin,outputStatus);
          client.publish((char*)STATUS_CALLBACK.c_str(),(outputStatus?"OFF":"ON"));
          cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK).c_str(),(outputStatus?"OFF":"ON"));
          #if USE_SERIAL
            Serial.println("Output is ON");
          #endif
        }
        else if(payloadStr == "OFF"){
          outputStatus = nodeOFF;
          digitalWrite(outputPin,outputStatus);
          client.publish((char*)STATUS_CALLBACK.c_str(),(outputStatus?"OFF":"ON"));
          cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK).c_str(),(outputStatus?"OFF":"ON"));
          #if USE_SERIAL
            Serial.println("Output is OFF");
          #endif
        }
        else{
          #if USE_SERIAL
            Serial.println("Command not recognized");
          #endif
        }
      }
      break;
  case doubleSwitch:
      if(String(topic)==STATUS_TOPIC){
        #if USE_SERIAL
          Serial.println("Status topic received");
        #endif 
        client.publish((char*)(STATUS_CALLBACK + "/switch1").c_str(),(outputStatus1?"OFF":"ON"));
        client.publish((char*)(STATUS_CALLBACK + "/switch2").c_str(),(outputStatus2?"OFF":"ON"));
        cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK + "/switch1").c_str(),(outputStatus1?"OFF":"ON"));
        cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK + "/switch2").c_str(),(outputStatus2?"OFF":"ON"));
      }  
      else if(String(topic)==(UPDATE_TOPIC + "/switch1")){
        #if USE_SERIAL
          Serial.println("Update topic received");
        #endif 
        if(payloadStr == "ON"){
          outputStatus1 = nodeON;
          digitalWrite(outputPin1,outputStatus1);
          client.publish((char*)(STATUS_CALLBACK + "/switch1").c_str(),(outputStatus1?"OFF":"ON"));
          cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK + "/switch1").c_str(),(outputStatus1?"OFF":"ON"));
          #if USE_SERIAL
            Serial.println("Output is ON");
          #endif
        }
        else if(payloadStr == "OFF"){
          outputStatus1 = nodeOFF;
          digitalWrite(outputPin1,outputStatus1);
          client.publish((char*)(STATUS_CALLBACK + "/switch1").c_str(),(outputStatus1?"OFF":"ON"));
          cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK + "/switch1").c_str(),(outputStatus1?"OFF":"ON"));
          #if USE_SERIAL
            Serial.println("Output is OFF");
          #endif
        }
      }
      
      else if(String(topic)==(UPDATE_TOPIC + "/switch2")){
        #if USE_SERIAL
          Serial.println("Update topic received");
        #endif 
        if(payloadStr == "ON"){
          outputStatus2 = nodeON;
          digitalWrite(outputPin2,outputStatus2);
          client.publish((char*)(STATUS_CALLBACK + "/switch2").c_str(),(outputStatus2?"OFF":"ON"));
          cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK + "/switch2").c_str(),(outputStatus2?"OFF":"ON"));
          #if USE_SERIAL
            Serial.println("Output is ON");
          #endif
        }
        else if(payloadStr == "OFF"){
          outputStatus2 = nodeOFF;
          digitalWrite(outputPin2,outputStatus2);
          client.publish((char*)(STATUS_CALLBACK + "/switch2").c_str(),(outputStatus2?"OFF":"ON")); 
          cloudClient.publish((char*)("/" + CLOUD_USER + "//" + STATUS_CALLBACK + "/switch2").c_str(),(outputStatus2?"OFF":"ON"));
          #if USE_SERIAL
            Serial.println("Output is OFF");
          #endif
        }
      }
      break;  
  }



  if(String(topic) == RESET_TOPIC){
    #if USE_SERIAL
      Serial.println("Resetting! ..... See you soon x)");
    #endif
    delay(10);
    ESP.restart();
  }
}

void cloudCallback(char* topic, byte* payload, unsigned int length){
  String payloadStr = "";
  for(int i=0;i<length;i++){
    payloadStr += (char)payload[i];
  }
  
  #if USE_SERIAL
    Serial.println("Received CLOUD MQTT message!");
    Serial.print("Topic: ");
    Serial.println(String(topic));
    Serial.print("Payload: ");
    Serial.println(payloadStr);
  #endif
}

