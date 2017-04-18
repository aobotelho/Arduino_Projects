#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

float tempC;
int reading;
const int tempPin = A0;

const int heaterRelayPin = 8;
boolean heaterOn = false;

void setup(){
  analogReference(INTERNAL);
  pinMode(heaterRelayPin,OUTPUT);
  digitalWrite(heaterRelayPin,heaterOn);

  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight

  lcd.backlight(); // finish with backlight on  

  lcd.setCursor(0,0); //Start at character 4 on line 0
  lcd.print("Temperature");
}

void loop(){
  reading = analogRead(tempPin);
  tempC = reading / 9.31;
  
  lcd.setCursor(0,1);
  lcd.print(String(tempC));

  if((tempC > 30) && (heaterOn == false)){
    heaterOn = false;
    digitalWrite(heaterRelayPin,heaterOn);
    //Serial.println("Heater OFF now!");    
  }
  else if((tempC<27) && (heaterOn == true)){
    heaterOn = true;
    digitalWrite(heaterRelayPin,heaterOn);
    //Serial.println("Heater ON now!");
  }
  delay(1000);
}
