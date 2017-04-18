/********************************************************************************
 *                      André Botelho - Home automation project                 *
 *                                                                              *   
 * This code requires PubSubClient found here:                                  *
 *      https://github.com/knolleary/pubsubclient                               *  
 *                                                                              *
 * -Simple testing code                                                         *
 * -Uses ID to configure MQTT topic                                             *
 * -Defines all topics and how to set them supposing this node being a          *
 *      simple light controllet (ON/OFF option)                                 *
 ********************************************************************************/
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

//Defines all type of nodes possible
enum nodeType{
  lightType=0
};

//Defines all possible status for the nodes
enum nodeStatus{
  nodeOFF = 0,
  nodeOn
};

//Each node will have one type, defined by the enum above
#define NODE_TYPE lightType

//This is the specific topic in which the server will use to update the status of every node
#define STATUS_TOPIC_DEF "/update/status"
String STATUS_TOPIC = STATUS_TOPIC_DEF;
//This is the specific topic in which the server will be listeing to in order to update every node status
#define STATUS_CALLBACK_DEF "/update/callback/"
String STATUS_CALLBACK = STATUS_CALLBACK_DEF;
//This is the specific topic in which the server will receive the information that a new device is available
#define DISCOVER_TOPIC_DEF "/discover/newDevices"
String DISCOVER_TOPIC = DISCOVER_TOPIC_DEF;
//This is the specific topic in which the server will send the confirmation (or not) that the device was added
#define DISCOVER_CALLBACK_DEF "/discover/registerDevice/"
String DISCOVER_CALLBACK = DISCOVER_CALLBACK_DEF;
//This is the specific topic in which the server will send the output status
#define UPDATE_TOPIC_DEF "/ID GOES HERE/status"
String UPDATE_TOPIC;


//Define the pin that will turn the light on or off
const int outputPin = D2;
boolean outputStatus = nodeOFF;

//Boolean variable that checks if the node is registered or not
boolean nodeRegistered = false;

//Define the string that will be sent as payload for the server when registering
//This string will simulate a JSON object that will be parsed by the server side using python
//Status will start as OFF since it will only turn on after registration
//const String registerPayload = "{""id"":" + String(NODE_ID) + ",""type"":" + NODE_TYPE +  + ",""status"":""OFF""}";
String registerPayload;

//Define the MQTT variables and values
#define MQTT_IP "192.168.1.33"
#define MQTT_PORT 1883
WiFiClient wificlient;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient client(MQTT_IP,MQTT_PORT,callback,wificlient);


//WI-FI settings
const char* ssid = "Sumidouro de Baixo";
const char* password = "abc123def4";
uint8_t macAddr[6];
String macAddrStr;

//# to define if serial is being used or not
#define USE_SERIAL  true

void setup(){
  //Setting output pin mode
  pinMode(outputPin,OUTPUT);
  digitalWrite(outputPin,outputStatus);
  
  //Configure serial if USE_SERIAL is set to true
  #if USE_SERIAL
    Serial.begin(115200);
    Serial.println("\nHello!!");
    Serial.println("\nConnecting to Wifi: " + String(ssid));
  #endif
  
  //Try to connect to wifi
  WiFi.begin(ssid,password);

  while(!connectWifi());

  #if USE_SERIAL
    Serial.println("Wifi connected!");
    WiFi.macAddress(macAddr);
    Serial.print("IP Address: ");    
    Serial.println(WiFi.localIP());
    Serial.println("Mac Address: " + String(macAddr[0],16) + ":" + String(macAddr[1],16) + ":" + String(macAddr[2],16) + ":" + 
                                     String(macAddr[3],16) + ":" + String(macAddr[4],16) + ":" + String(macAddr[5],16));
  #endif

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
  registerPayload = "{""id"":" + macAddrStr + ",""type"":" + NODE_TYPE +  + ",""status"":""OFF""}";
  
  //Once connected to WiFi connects to MQTT server
  #if USE_SERIAL
    Serial.println("Trying to connect to MQTT server...");
  #endif

  while(!connectMQTT());

  #if USE_SERIAL
    Serial.println("Connected!");
    Serial.println("\nNow trying to register with the server");
  #endif

  client.subscribe((char*)DISCOVER_CALLBACK.c_str());
  
  while(!registerDevice());

  client.subscribe((char*)STATUS_TOPIC.c_str());
  client.subscribe((char*)UPDATE_TOPIC.c_str());
  #if USE_SERIAL
    Serial.println("Subscribing to: ");
    Serial.println(STATUS_TOPIC);
    Serial.println(UPDATE_TOPIC);
  #endif
}


void loop(){  
  //maintain MQTT connection
  client.loop();

  //MUST delay to allow ESP8266 WIFI functions to run
  delay(10); 
}





//Function to loop while it tries to connect to Wifi network
boolean connectWifi(){
  unsigned int counter=0;
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    
    #if USE_SERIAL
      Serial.println("Attempt: " + String(counter++));
    #endif

    //Tries to connect up to 20 times. If they all fail enters halt state
    if(counter==20){
      #if USE_SERIAL
        Serial.println("Not able to connect to wifi!!!!!");
      #endif
      errorLED();
      }  
  }

  //If it got out of the while loop it managed to connect
  return true;
}

//Function to loop while it tries to connect to MQTT server
boolean connectMQTT(){
  unsigned int counter=0;
  while(!client.connect((char*)macAddrStr.c_str())){
    delay(100);
    if(counter == 20){
      #if USE_SERIAL
        Serial.println("Failed to connect");
      #endif
      errorLED();
    }
    else{
      counter++;
    }
  }
  
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
      
      client.publish((char*)DISCOVER_TOPIC.c_str(),(char*)registerPayload.c_str());
      //maintain MQTT connection
      client.loop();
      delay(1500);    

      if(counter==100){
        #if USE_SERIAL
          Serial.println("Failed to register");
        #endif
        errorLED();
      }
      else{
        counter++;
      }
    }

    return true;
}

//Simple function to signal the outside world something has gone wrong
void errorLED(){
  #if USE_SERIAL
    Serial.println("Entering Halt state");
  #endif
  
  pinMode(LED_BUILTIN,OUTPUT);
  boolean ledON = true;
 
  while(1){
    ledON = !ledON;
    digitalWrite(LED_BUILTIN,ledON);
    delay(100);
  }
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
  else if(String(topic)==STATUS_TOPIC){
    #if USE_SERIAL
      Serial.println("Status topic received");
    #endif 
    client.publish((char*)STATUS_CALLBACK.c_str(),(outputStatus?"ON":"OFF"));
  }

  else if(String(topic)==UPDATE_TOPIC){
    #if USE_SERIAL
      Serial.println("Update topic received");
    #endif 
    if(payloadStr == "ON"){
      outputStatus = true;
      digitalWrite(outputPin,outputStatus);
      #if USE_SERIAL
        Serial.println("Output is ON");
      #endif
    }
    else if(payloadStr == "OFF"){
      outputStatus = false;
      digitalWrite(outputPin,outputStatus);
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
  
  
}

