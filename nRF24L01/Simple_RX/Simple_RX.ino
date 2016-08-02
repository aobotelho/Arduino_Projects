#include <SPI.h>
#include <String.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

RF24 radio(9,10);

#define BUFFER_SIZE  32
#define PIPE         0
#define AUTO_ACK     false
#define CHANNEL      120

uint8_t  buf[BUFFER_SIZE];

#define RELAY_PIN  8

static const uint64_t addr[6] = {
                              0xDEADBEEF00LL,
                              0xABAB555500LL,
                              0xABAB555533LL,
                              0xABAB555566LL,
                              0xABAB555599LL,
                              0xABAB5555CCLL};

void setup(void){
  Serial.begin(57600);
  Serial.println("Hello!");
  
  pinMode(RELAY_PIN,OUTPUT);
  digitalWrite(RELAY_PIN,LOW);

  radio.begin();
  radio.setPayloadSize(BUFFER_SIZE);
  radio.setChannel(CHANNEL);
  radio.setCRCLength(RF24_CRC_16);
  radio.setDataRate( RF24_1MBPS);
  radio.setAutoAck(PIPE,AUTO_ACK);
  radio.setPALevel(RF24_PA_HIGH);
  
  radio.openReadingPipe(PIPE,addr[PIPE]);
  radio.startListening();
}

void loop(void){
  if(radio.available()){
    bool done = false;
    
    while(!done){
      // Fetch the payload, and see if this was the last one.
      done = radio.read(buf, BUFFER_SIZE);
      Serial.print("Buffer 0: ");Serial.print(buf[0]);
      Serial.print(" Buffer 1: "); Serial.println(buf[1]);
      if(buf[0] == '1'){
        digitalWrite(RELAY_PIN,HIGH);
        Serial.print("pin high");
      }
      else{
        digitalWrite(RELAY_PIN,LOW);
        Serial.print("Pin Low");
      }
    }
  }
}


