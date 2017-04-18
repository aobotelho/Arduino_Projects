#include <MCP3304.h>
#include <SPI.h>
//CLK Pin13
//MISO(DO) Pin12
//MOSI(DI) Pin11
//CS - Selected below

#define CS_PIN  10

enum Channels{
  CH1=0,
  CH2,
  CH3,
  CH4,
  CH5,
  CH6,
  CH7
};

MCP3304 ADCChip(CS_PIN);  //creat an instance with pin 10 as CS

int readings[8];
float voltages[8];

void setup(){  
  Serial.begin(9600);
  Serial.println("Hello world!");
  Serial.println("This sketch reads the voltage value on all 8 channels of the MCP3304 chip");
  Serial.println(" ");
}

void loop(){
  for(int i=0; i<8; i++){
    readings[i] = ADCChip.readSgl(i);
    voltages[i] = readings[i]/4095.0 * 5.0;
    Serial.print("Voltage on CH" + String(i) + ": ");
    Serial.println(voltages[i],3);
  }

  Serial.println(" ");
  delay(1000);    //delay for 1s so you dont get to much lines in the serial monitor
}
  
