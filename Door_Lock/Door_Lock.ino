#include <Stepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define  Pin1  8
#define  Pin2  9
#define  Pin3  10
#define  Pin4  11

#define Steps  1024

#define  SCL_Pin  6
#define  SDO_Pin  7

#define PASS_SIZE  4

byte CurrentKey = 0;
byte OldKey = 0;
byte KeyCounter = 0;

bool NewKey = false;
bool OpenDoor = false;

byte Code[PASS_SIZE] = {1,6,11, 16};
byte InputCode[PASS_SIZE] = {0,0,0,0};

Stepper myStepper = Stepper(Steps,Pin1,Pin2,Pin3,Pin4);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

void setup(){
  pinMode(SCL_Pin,OUTPUT);
  pinMode(SDO_Pin,INPUT); 
  
  myStepper.setSpeed(15);
  
  lcd.begin(20,4);
  lcd.backlight();  
  
  lcd.setCursor(0,0); 
  lcd.print("Door Lock Project");
  
  lcd.setCursor(0,1);
  lcd.print("Code:               ");
  lcd.setCursor(7,1);
  lcd.print(Code[0]);
  lcd.setCursor(10,1);
  lcd.print(Code[1]);
  lcd.setCursor(13,1);
  lcd.print(Code[2]);
  lcd.setCursor(16,1);
  lcd.print(Code[3]);
  
  lcd.setCursor(0,2);
  lcd.print("Input:              ");
  
  lcd.setCursor(0,3);
  lcd.print("By: Andre Botelho");

}

void loop(){
  CurrentKey = CheckKey();
  
  // If retrurns zero is because no key is pressed
  if(CurrentKey == 0){
    // Store last key pressed
    OldKey = CurrentKey;
    if(!NewKey){
      NewKey = true;
    }
  }
  else if(NewKey){
    if(KeyCounter < PASS_SIZE){
      lcd.setCursor(7+3*KeyCounter,2);
      lcd.print(CurrentKey);
      InputCode[KeyCounter] = CurrentKey;
      KeyCounter++;
      OldKey = CurrentKey;
      NewKey = false;
    }
    
    if(KeyCounter == PASS_SIZE){
      OpenDoor = CheckArray(Code,InputCode);
      if( OpenDoor == true){
        myStepper.step(Steps/2);
        delay(1000);
        myStepper.step(-1*Steps/2);
      }
      
      for(int i=0;i<PASS_SIZE;i++)
        InputCode[i] = 0;        
        
      lcd.setCursor(0,2);
      lcd.print("Input:              ");
      KeyCounter = 0;
      OpenDoor = false;
      
    }
  }
    
}
