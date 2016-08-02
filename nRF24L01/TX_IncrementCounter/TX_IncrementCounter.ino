#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "pSat.h"

RF24 radio(CE_PIN,CSN_PIN);
uint16_t counter = 0;

void setup(void){
  Serial.begin(57600);
  Serial.println("Hello!");
  
  psat_InitAntenna(radio, RF24_1MBPS, RF24_PA_HIGH, CHANNEL);
  radio.openWritingPipe(addr[PIPE]); 
}

void loop(void){
    counter++;
    Serial.println(counter);
    
    buf[0] = (counter & 0x00FF);
    buf[1] = ((counter>>8) & 0x00FF);
    
    radio.write(buf, BUFFER_SIZE);
    
    delay(50);
}
