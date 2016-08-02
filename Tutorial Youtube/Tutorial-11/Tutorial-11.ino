// Tutorial 11
// SD Card and data logging

#include <SD.h>

// SPI Settings
// MOSI,MISO,SCK set by default
int CS_pin = 10;
int pow_pin = 8; 

void setup(){
  Serial.begin(9600);
  Serial.println("Initializing SD Card");
  //CS Pin as an output
  pinMode(CS_pin, OUTPUT);
  
  //Power up SD
  pinMode(pow_pin,OUTPUT);
  digitalWrite(pow_pin,HIGH);
  
  // Check if card is ready
  if(!SD.begin(CS_pin)){
    Serial.println("Card Failed!");
    return;
  }
  
  Serial.println("Card Ready!");
}

void loop(){
  String dataString = "Whats Up?";
  
  // Open a file to write to
  // Only one file can be open at time
  // If second parameter is ommited you can only read from file
  File dataFile = SD.open("log.txt",FILE_WRITE);
  if(dataFile){
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString + " Wrote OK!");
  }
  else{
    Serial.println("Couldnt access file =/");
  }
  
  delay(5000);
}
