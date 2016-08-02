// RGB_Effect
// To use I2C backpack on a 20x4 lcd screen uncomment the define below

//#define USE_I2C_BACKPACK

#if defined USE_I2C_BACKPACK
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
#endif

enum{
  Red = 0,
  Green,
  Blue
};

#define RED_PIN    9
#define GREEN_PIN  10
#define BLUE_PIN   11

#define  MIN  2
#define  SETPOINT  50
#define  MAX  250
#define  DELAY  5

#define COLUMNS  20
#define LINES    4

signed int pace = 1;
unsigned int duty = 100;
unsigned int currentPin = Red;
boolean Up = true;

#if defined USE_I2C_BACKPACK
  LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#endif
void setup() {
  
  // Initialize the lcd for 20 chars 4 lines and turn on backlight
  #if defined USE_I2C_BACKPACK
    lcd.begin(COLUMNS,LINES); 
    // Backlight on    
    lcd.backlight(); 
    
    lcd.setCursor(3,0);
    lcd.print("Hello, world!");
    lcd.setCursor(2,1);
    lcd.print("By: Andre Botelho");
    lcd.setCursor(0,2);
    lcd.print("RGB Effect");
    lcd.setCursor(0,3);
    lcd.write("Red Up");
    delay(3000);
  #endif
  
  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(BLUE_PIN,OUTPUT);
}

void loop() { 
  
  switch(currentPin){
   case Red:
      analogWrite(RED_PIN,duty);
      delay(DELAY);
      
      duty += pace;
      
      if(duty == MAX){
        pace = -pace;
        Up = false;
        
        #if defined USE_I2C_BACKPACK
          lcd.clearLine(3,COLUMNS);
          lcd.setCursor(0,3);
          lcd.print("Red Down");
        #endif
      }
      else if(duty <= SETPOINT && Up == false){
        analogWrite(GREEN_PIN,SETPOINT-duty);        
        
        #if defined USE_I2C_BACKPACK
          lcd.clearLine(3,COLUMNS);
          lcd.setCursor(0,3);
          lcd.print("Green Up - Red Down");
        #endif
      }
      
      if(duty == MIN && Up == false){
        Up = true;
        pace = -pace;
        currentPin = Green;
        duty = SETPOINT;
         
        analogWrite(GREEN_PIN,SETPOINT); 
      }
   break;
   
   case Green:
      analogWrite(GREEN_PIN,duty);
      delay(DELAY);
      
      duty += pace;
      
      if(duty == MAX){
        pace = -pace;
        Up = false;
        
        #if defined USE_I2C_BACKPACK
          lcd.clearLine(3,COLUMNS);
          lcd.setCursor(0,3);
          lcd.print("Green Down");
        #endif
      }
      else if(duty <= SETPOINT && Up == false){
        analogWrite(BLUE_PIN,SETPOINT-duty);        
        
        #if defined USE_I2C_BACKPACK
          lcd.clearLine(3,COLUMNS);
          lcd.setCursor(0,3);
          lcd.print("Blue Up - Green Down");
        #endif
      }
      
      if(duty == MIN && Up == false){
        Up = true;
        pace = -pace;
        currentPin = Blue;
        duty = SETPOINT;
        
        analogWrite(BLUE_PIN,SETPOINT);
      }
   break;
   
   case Blue:
      analogWrite(BLUE_PIN,duty);
      delay(DELAY);
      
      duty += pace;
      
      if(duty == MAX){
        pace = -pace;
        Up = false;
        
        #if defined USE_I2C_BACKPACK
          lcd.clearLine(3,COLUMNS);
          lcd.setCursor(0,3);
          lcd.print("Blue Down");
        #endif
      }
      else if(duty <= SETPOINT && Up == false){
        analogWrite(RED_PIN,SETPOINT-duty);
        
        #if defined USE_I2C_BACKPACK
          lcd.clearLine(3,COLUMNS);
          lcd.setCursor(0,3);
          lcd.print("Blue Down - Red Up");
        #endif
      }
      
      if(duty == MIN && Up == false){
        Up = true;
        pace = -pace;
        currentPin = Red;
        duty = SETPOINT;
        
        analogWrite(RED_PIN,SETPOINT);
      }
   break;  
  }
}

