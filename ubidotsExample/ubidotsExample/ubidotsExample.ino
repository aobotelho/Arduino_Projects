#include <SPI.h>
#include <Ethernet.h>

#define BTN_PIN A0

String onOffID = "581b74bd762542745c01a6ea";
String token = "L0Hm3Z2QeTYen33w8XaR4M3eh0UNlh";

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char server[] = "things.ubidots.com";

EthernetClient client;
IPAddress ip(192,168,1,35);
IPAddress myDNS(192,168,1,1);
IPAddress myGateway(192,168,1,1);


void setup() {
  pinMode(BTN_PIN,INPUT);
  Serial.begin(9600);

  Serial.println("Starting");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
   Ethernet.begin(mac,ip);
  }

  Serial.print("Servidor iniciado em: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  if(digitalRead(BTN_PIN)){
    Serial.println("Button UP");
    save_value("UP");
  }
  else{
    Serial.println("Button DOWN");
    save_value("DOWN");
  }

  delay(2000);
}


void save_value(String resultString){
  Serial.println("Sending data...");
  // if you get a connection, report back via serial:
  int num = 0;
  delay(2000);

  //String varString = "[{\"variable\":"
  String varString = "[{\"variable\":" + onOffID + "\", \"value\":" + resultString + "}";
  
  num = varString.length();
 
  Serial.println("Connecting...");
 
  if (client.connect(server,80)) {
    //client.println("POST /api/v1.6/variables/"+tempVarId+"/values HTTP/1.1");
    //Serial.println("POST /api/v1.6/variables/"+tempVarId+"/values HTTP/1.1");
    client.println("POST /api/v1.6/collections/values HTTP/1.1");
    Serial.println("POST /api/v1.6/collections/values HTTP/1.1");
    client.println("Content-Type: application/json");
    Serial.println("Content-Type: application/json");
    client.println("Content-Length: "+String(num));
    Serial.println("Content-Length: "+String(num));
    client.println("X-Auth-Token: "+token);
    Serial.println("X-Auth-Token: "+token);
    client.println("Host: things.ubidots.comn");
    Serial.println("Host: things.ubidots.comn");
    client.print(varString);
   Serial.print(varString+"n");
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 
  boolean sta = client.connected();
  Serial.println("Connection ["+String(sta)+"]");
  if (!client.connected())   {
   Serial.println();
   Serial.println("disconnecting.");
   client.stop();
  }
 
  Serial.println("Reading..");
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  client.flush();
  client.stop();
}
