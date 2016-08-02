// ADC_to_Serial
// Uses I2C backpack on a 20x4 lcd screen

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define COLUMNS  20
#define LINES    4

#define POT_PIN  A0

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
  
  pinMode(POT_PIN,INPUT);
}

void loop(){
  int val = map(analogRead(POT_PIN),0,1023,0,255);
  
  lcd.setCursor(0,2);
  lcd.print("ADC:           ");
  lcd.setCursor(5,2);
  lcd.print(String(val));
  
  Serial.println(val);
  delay(50);  
}
