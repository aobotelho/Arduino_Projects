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

uint16_t counter = 0;

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
  
  radio.begin();
  radio.setPayloadSize(BUFFER_SIZE);
  radio.setChannel(CHANNEL);
  radio.setCRCLength(RF24_CRC_16);
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(CHANNEL,AUTO_ACK);
  radio.setPALevel(RF24_PA_HIGH);
  
  radio.openWritingPipe(addr[PIPE]); 
}

void loop(void){
    counter++;
    buf[1] = (counter >> 8);
    buf[0] = (counter & 0x00FF);
   
    Serial.println(counter);
    radio.write(buf, BUFFER_SIZE);
    
    delay(50);
}
