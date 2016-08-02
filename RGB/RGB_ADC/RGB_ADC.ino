// RGB_ADC
// Uses I2C backpack on a 20x4 lcd screen

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

enum{
  Red = 0,
  Green,
  Blue,
  All
};

#define RED_PIN    11
#define GREEN_PIN  10
#define BLUE_PIN   9

#define RED_ADC     A0
#define GREEN_ADC   A1
#define BLUE_ADC    A2

#define COLUMNS  20
#define LINES    4

void checkADC();

int Old_Red_ADC_Value, Old_Green_ADC_Value, Old_Blue_ADC_Value;
unsigned int TimeOut = 0;



LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  // Initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.begin(COLUMNS,LINES); 
  // Backlight on    
  lcd.backlight(); 
  
  lcd.setCursor(6,0);
  lcd.print("RGB ADC");
  lcd.setCursor(2,1);
  lcd.print("By: Andre Botelho");
  lcd.clearLine(1,COLUMNS);
  lcd.clearLine(3,COLUMNS);
  delay(1000);
  
  lcd.setCursor(0,1);
  lcd.print("Red: 0              ");   
  lcd.setCursor(0,2);
  lcd.print("Green: 0            "); 
  lcd.setCursor(0,3);
  lcd.print("Blue: 0             "); 
  
  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(BLUE_PIN,OUTPUT);
  
  pinMode(RED_ADC,INPUT);
  pinMode(GREEN_ADC,INPUT);
  pinMode(BLUE_ADC,INPUT);
  
 analogWrite(RED_PIN,0);
 analogWrite(GREEN_PIN,0);
 analogWrite(BLUE_PIN,0);
}

void loop() { 
  checkADC();
  if(++TimeOut == 0x0F){
    updateADC();
  }
}




void checkADC(){
  int Red_ADC_Value, Green_ADC_Value, Blue_ADC_Value;
  
  Red_ADC_Value = map(analogRead(A0),0,1024,0,255);
  Green_ADC_Value = map(analogRead(A1),0,1024,0,255); 
  Blue_ADC_Value = map(analogRead(A2),0,1024,0,255);
  
  if(((Red_ADC_Value <= Old_Red_ADC_Value-2) || (Red_ADC_Value >= Old_Red_ADC_Value+2)) && Red_ADC_Value){
    if(Red_ADC_Value < 3) Red_ADC_Value = 0;
    Old_Red_ADC_Value = Red_ADC_Value; 
    analogWrite(RED_PIN,Red_ADC_Value);
    
    lcd.setCursor(0,1);
    lcd.print("Red:                ");  
    
    lcd.setCursor(5,1);
    lcd.print(String(Red_ADC_Value));
    TimeOut = 0;
  }
  
  if(((Green_ADC_Value <= Old_Green_ADC_Value-2) || (Green_ADC_Value >= Old_Green_ADC_Value+2)) && Green_ADC_Value){
    if(Green_ADC_Value < 3) Green_ADC_Value = 0;
    Old_Green_ADC_Value = Green_ADC_Value;
    analogWrite(GREEN_PIN,Green_ADC_Value);
    
    lcd.setCursor(0,2);
    lcd.print("Green:              ");  
    
    lcd.setCursor(7,2);
    lcd.print(String(Green_ADC_Value));
    TimeOut = 0;
  }
  
  if(((Blue_ADC_Value <= Old_Blue_ADC_Value-2) || (Blue_ADC_Value >= Old_Blue_ADC_Value+2)) && Blue_ADC_Value){
    if(Blue_ADC_Value < 3) Blue_ADC_Value = 0;
    Old_Blue_ADC_Value = Blue_ADC_Value;    
     analogWrite(BLUE_PIN,Blue_ADC_Value);
    
    lcd.setCursor(0,3);
    lcd.print("Blue:               ");  
    
    lcd.setCursor(6,3);
    lcd.print(String(Blue_ADC_Value));
    TimeOut = 0;
  }
}

void updateADC(){  
  Old_Red_ADC_Value = map(analogRead(A0),0,1024,0,255);
  Old_Green_ADC_Value = map(analogRead(A1),0,1024,0,255); 
  Old_Blue_ADC_Value = map(analogRead(A2),0,1024,0,255);
  
  if(Old_Red_ADC_Value < 3) Old_Red_ADC_Value = 0;
  if(Old_Green_ADC_Value < 3) Old_Green_ADC_Value = 0;
  if(Old_Blue_ADC_Value < 3) Old_Blue_ADC_Value = 0;
  
  analogWrite(RED_PIN,Old_Red_ADC_Value);
  analogWrite(GREEN_PIN,Old_Green_ADC_Value);
  analogWrite(BLUE_PIN,Old_Blue_ADC_Value);
    
  lcd.setCursor(0,1);
  lcd.print("Red:                ");    
  lcd.setCursor(5,1);
  lcd.print(String(Old_Red_ADC_Value));
  lcd.setCursor(0,2);
  lcd.print("Green:              ");    
  lcd.setCursor(7,2);
  lcd.print(String(Old_Green_ADC_Value));
  lcd.setCursor(0,3);
  lcd.print("Blue:               ");    
  lcd.setCursor(6,3);
  lcd.print(String(Old_Blue_ADC_Value));
  
  TimeOut = 0;
}

