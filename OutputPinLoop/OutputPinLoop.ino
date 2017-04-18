void setup() {
  Serial.begin(9600);
  for(int i=2;i<10;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
}

void loop() {
  for(int i=2;i<10;i++){
    digitalWrite(i,HIGH);
    Serial.println("Pin " + String(i) + " High");
    delay(1000);
    digitalWrite(i,LOW);
  }
}
