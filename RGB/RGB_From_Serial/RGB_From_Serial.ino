// RGB_From_Serial
// Uses I2C backpack on a 20x4 lcd screen

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define COLUMNS  20
#define LINES    4

#define RED_PIN    11
#define GREEN_PIN  10
#define BLUE_PIN   9

char stream[COLUMNS] = "";
unsigned int Red_Value,Green_Value,Blue_Value;
unsigned int aux1,aux2;

void setRGBValues();

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  Serial.begin(9600);  
  
  // Initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.begin(COLUMNS,LINES); 
  // Backlight on    
  lcd.backlight(); 
  
  lcd.setCursor(3,0);
  lcd.print("RGB From Serial");
  lcd.setCursor(1,1);
  lcd.print("By: Andre Botelho");
  
  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(BLUE_PIN,OUTPUT);
  
  analogWrite(RED_PIN,0xF);
  analogWrite(GREEN_PIN,0xF);
  analogWrite(BLUE_PIN,0xF);
}

void loop(){
  int counter = 0;
  for(;counter<COLUMNS;counter++) stream[counter] = '\0';
  counter = 0;
  
  while(!Serial.available()); 
    
  lcd.setCursor(0,2);
  lcd.print("oi                  ");
  lcd.setCursor(0,3);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  
  while(Serial.available()){
    char temp = Serial.read();
    lcd.write(temp);
    stream[counter++] = temp;
  }
  
  lcd.setCursor(0,3);
  //for(int counterTemp = 0; counterTemp < counter; counterTemp++)  lcd.write(stream[counterTemp]);
  
  setRGBValues();
  //lcd.print("R " + String(Red_Value,HEX) + " G " + String(Green_Value,HEX) + " B " + String(Blue_Value,HEX));
  
  analogWrite(RED_PIN,Red_Value);
  analogWrite(GREEN_PIN,Green_Value);
  analogWrite(BLUE_PIN,Blue_Value);
}




void setRGBValues(){
  if( stream[2] == '0' || stream[2] == '1' || 
      stream[2] == '2' || stream[2] == '3' || 
      stream[2] == '4' || stream[2] == '5' || 
      stream[2] == '6' || stream[2] == '7' || 
      stream[2] == '8' || stream[2] == '9') aux1 = ( stream[2] - '0' ) << 4; 
  else if( stream[2] == 'A' ) aux1 = 0xA0;
  else if( stream[2] == 'B' ) aux1 = 0xB0;
  else if( stream[2] == 'C' ) aux1 = 0xC0;
  else if( stream[2] == 'D' ) aux1 = 0xD0;
  else if( stream[2] == 'E' ) aux1 = 0xE0;
  else if( stream[2] == 'F' ) aux1 = 0xF0;
  
  if( stream[3] == '0' || stream[3] == '1' || 
      stream[3] == '2' || stream[3] == '3' || 
      stream[3] == '4' || stream[3] == '5' || 
      stream[3] == '6' || stream[3] == '7' || 
      stream[3] == '8' || stream[3] == '9') aux2 = ( stream[3] - '0' ); 
  else if( stream[3] == 'A' ) aux2 = 0xA;
  else if( stream[3] == 'B' ) aux2 = 0xB;
  else if( stream[3] == 'C' ) aux2 = 0xC;
  else if( stream[3] == 'D' ) aux2 = 0xD;
  else if( stream[3] == 'E' ) aux2 = 0xE;
  else if( stream[3] == 'F' ) aux2 = 0xF;
  
  Red_Value = aux1 + aux2;
  
  if( stream[7] == '0' || stream[7] == '1' || 
      stream[7] == '2' || stream[7] == '3' || 
      stream[7] == '4' || stream[7] == '5' || 
      stream[7] == '6' || stream[7] == '7' || 
      stream[7] == '8' || stream[7] == '9') aux1 = ( stream[7] - '0' ) << 4; 
  else if( stream[7] == 'A' ) aux1 = 0xA0;
  else if( stream[7] == 'B' ) aux1 = 0xB0;
  else if( stream[7] == 'C' ) aux1 = 0xC0;
  else if( stream[7] == 'D' ) aux1 = 0xD0;
  else if( stream[7] == 'E' ) aux1 = 0xE0;
  else if( stream[7] == 'F' ) aux1 = 0xF0;
  
  if( stream[8] == '0' || stream[8] == '1' || 
      stream[8] == '2' || stream[8] == '3' || 
      stream[8] == '4' || stream[8] == '5' || 
      stream[8] == '6' || stream[8] == '7' || 
      stream[8] == '8' || stream[8] == '9') aux2 = ( stream[8] - '0' ); 
  else if( stream[8] == 'A' ) aux2 = 0xA;
  else if( stream[8] == 'B' ) aux2 = 0xB;
  else if( stream[8] == 'C' ) aux2 = 0xC;
  else if( stream[8] == 'D' ) aux2 = 0xD;
  else if( stream[8] == 'E' ) aux2 = 0xE;
  else if( stream[8] == 'F' ) aux2 = 0xF;
  
  Green_Value = aux1 + aux2;
  
  if( stream[12] == '0' || stream[12] == '1' || 
      stream[12] == '2' || stream[12] == '3' || 
      stream[12] == '4' || stream[12] == '5' || 
      stream[12] == '6' || stream[12] == '7' || 
      stream[12] == '8' || stream[12] == '9') aux1 = ( stream[12] - '0' ) << 4; 
  else if( stream[12] == 'A' ) aux1 = 0xA0;
  else if( stream[12] == 'B' ) aux1 = 0xB0;
  else if( stream[12] == 'C' ) aux1 = 0xC0;
  else if( stream[12] == 'D' ) aux1 = 0xD0;
  else if( stream[12] == 'E' ) aux1 = 0xE0;
  else if( stream[12] == 'F' ) aux1 = 0xF0;
  
  if( stream[13] == '0' || stream[13] == '1' || 
      stream[13] == '2' || stream[13] == '3' || 
      stream[13] == '4' || stream[13] == '5' || 
      stream[13] == '6' || stream[13] == '7' || 
      stream[13] == '8' || stream[13] == '9') aux2 = ( stream[13] - '0' ); 
  else if( stream[13] == 'A' ) aux2 = 0xA;
  else if( stream[13] == 'B' ) aux2 = 0xB;
  else if( stream[13] == 'C' ) aux2 = 0xC;
  else if( stream[13] == 'D' ) aux2 = 0xD;
  else if( stream[13] == 'E' ) aux2 = 0xE;
  else if( stream[13] == 'F' ) aux2 = 0xF;
  
  Blue_Value = aux1 + aux2;
}
