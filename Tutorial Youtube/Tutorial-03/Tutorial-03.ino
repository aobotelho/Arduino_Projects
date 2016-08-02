// Tutorial 03
/* Button and Serial

unsigned int button_IO = 8;

void setup(){
  //Set pins
  pinMode(button_IO,INPUT);
  
  // Set Serial
  Serial.begin(9600);
}


void loop(){
  // Print Current State
  Serial.println(digitalRead(button_IO));
  delay(1000);
}*/

// Voltage divider
unsigned int pot_IO = 0;

void setup(){
  //Set pins
  pinMode(pot_IO,INPUT);
  
  // Set Serial
  Serial.begin(9600);
}


void loop(){
  // Print Current State
  Serial.println(analogRead(pot_IO));
  delay(1000);
}
