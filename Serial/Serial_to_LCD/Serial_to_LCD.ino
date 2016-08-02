// Serial_to_LCD
// Uses I2C backpack on a 20x4 lcd screen

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define COLUMNS  20
#define LINES    4

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  Serial.begin(9600);  
  
  // Initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.begin(COLUMNS,LINES); 
  // Backlight on    
  lcd.backlight(); 
  
  lcd.setCursor(3,0);
  lcd.print("Serial to LCD");
  lcd.setCursor(2,1);
  lcd.print("By: Andre Botelho");
}

void loop(){
  
  while(!Serial.available()); 
    
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  
  while(Serial.available()){
    lcd.write(Serial.read());
  }
}
