#include <SPI.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

WiFiClient wificlient;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient arduinoClient("mqtt.dioty.co", 1883, callback, wificlient);
WiFiManager wifiManager;


void setup() {
   Serial.begin(115200);
   wifiManager.autoConnect("AutoConnectAP");
   Serial.println("connected...yeey :)");
   arduinoClient.connect("andrefrejat@gmail.com","andrefrejat@gmail.com","45f66b00");
   arduinoClient.subscribe("/andrefrejat@gmail.com/arduino");
   Serial.println("Starting");
}


void loop() {
   arduinoClient.loop();
   
   // Publish
   arduinoClient.publish("/andrefrejat@gmail.com/Hello", "Hello World from Arduino!");
   delay(500);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String payloadStr = "";
  for(int i=0;i<length;i++){
    payloadStr += (char)payload[i];
  }
  Serial.println("Received MQTT message!");
  Serial.print("Topic: ");
  Serial.println(String(topic));
  Serial.print("Payload: ");
  Serial.println(payloadStr);
}
