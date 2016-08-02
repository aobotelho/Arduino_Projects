// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// set the LCD address to 0x20 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


void setup(){
  // initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.begin(20,4);         

  lcd.backlight();  
  
  lcd.setCursor(3,0); 
  lcd.print("Hello, world!");
  lcd.setCursor(8,1);
  lcd.print("=D");
  lcd.setCursor(0,2);
  lcd.print("20 by 4 Line Display");
  lcd.setCursor(3,3);
  lcd.print("Andre Botelho");
}


void loop(){
}
