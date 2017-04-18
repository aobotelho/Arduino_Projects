
boolean lastInput = LOW;
void setup() {
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY); 
  pinMode(2,OUTPUT);
  digitalWrite(2,lastInput);
  pinMode(3,INPUT);
  //pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  if(digitalRead(3) != lastInput){
    lastInput = !lastInput;
    digitalWrite(2,lastInput);
    Serial.println("New input: " + String(lastInput));
  }
}
