#include <SPI.h>
#include <String.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "pSat.h"

RF24 radio(CE_PIN,CSN_PIN);

void setup(void){
  Serial.begin(57600);
  Serial.println("Hello!");

  psat_InitAntenna(radio, RF24_1MBPS, RF24_PA_HIGH, CHANNEL);
  
  radio.openReadingPipe(PIPE,addr[PIPE]);
  radio.startListening();
}

void loop(void){
  if(radio.available()){
    bool done = false;
    uint16_t counter;
    
    while(!done){
      done = radio.read(buf, BUFFER_SIZE);
      counter = (buf[1]<<8 & 0xFF00) + (buf[0] & 0x00FF);
      
      Serial.print("Counter: "); Serial.println(counter);
    }
  }
}


