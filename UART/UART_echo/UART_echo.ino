void setup(){
  Serial.begin(115200);
  Serial.println("ola!");
}

void loop(){
  if(Serial.available()){
    Serial.write(Serial.read());
  }
}
