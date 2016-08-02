#include <SPI.h>
#include <String.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "HomeAutomation.h"


// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

char buffer[12];

//-----------------------------------------------------
// Declaring Addressess
// Values defined at HomeAutomation.h
//-----------------------------------------------------
static const uint64_t addr[6] = {0xDEADBEEF00LL,0xABAB555500LL,0xABAB555533LL,0xABAB555566LL,0xABAB555599LL,0xABAB5555CCLL};

void setup(void){
  Serial.begin(57600);
  Serial.println("Hello!");

  radio.begin();
  radio.setPayloadSize(12);
  radio.setChannel(20);
  radio.setCRCLength(RF24_CRC_16);
  radio.setDataRate( RF24_1MBPS);
  radio.setAutoAck(1,false);
  radio.setPALevel(RF24_PA_HIGH);
}

void loop(void){
  static unsigned int current=0; 
  
  radio.openReadingPipe(current,addr[current]);
  radio.startListening();
  delay(5);
  if(radio.available()){
    radio.stopListening();
    radio.read( buffer, 12 );
    Serial.print("Pipe");
    Serial.println(current);
  }
  radio.stopListening(); 
  current++;
  if(current == 6) current = 0;
}


