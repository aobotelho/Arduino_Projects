// Tutorial 01
// Blink on board LED

int ledPin = 13;

void setup(){
 // Initialize pins as outputs
 pinMode(ledPin,OUTPUT); 
}

void loop(){
  digitalWrite(ledPin,HIGH);
  delay(100);
  digitalWrite(ledPin,LOW);
  delay(100);
}
