// Tutorial 02
/* LED toggle when pressing button

int ledPin = 13;
int switchPin = 8;
boolean lastButton = LOW;
boolean ledOn = false;
boolean currentButton = LOW;

void setup(){
 // Initialize pins
 pinMode(ledPin,OUTPUT); 
 pinMode(switchPin,INPUT);
}

boolean debounce(boolean last){
 boolean current = digitalRead(switchPin);
 if(last != current){
   delay(5);
   current = digitalRead(switchPin);
 }
 
 return current;
}

void loop(){
  currentButton = debounce(lastButton);
  
  if(!lastButton && currentButton){
    ledOn = !ledOn;
  }
  
  lastButton = currentButton;
  
  digitalWrite(ledPin,ledOn);
}*/

// LED and PWM
unsigned int ledPin = 11;
unsigned int switchPin = 8;
boolean lastButton = LOW;
unsigned int ledLevel = 0;
boolean currentButton = LOW;

void setup(){
 // Initialize pins
 pinMode(ledPin,OUTPUT); 
 pinMode(switchPin,INPUT);
}

boolean debounce(boolean last){
 boolean current = digitalRead(switchPin);
 if(last != current){
   delay(5);
   current = digitalRead(switchPin);
 }
 
 return current;
}

void loop(){
  currentButton = debounce(lastButton);
  
  if(!lastButton && currentButton){
    ledLevel += 51;
  }
  
  lastButton = currentButton;
  
  if(ledLevel >255) ledLevel = 0;
  
  analogWrite(ledPin,ledLevel);
}
