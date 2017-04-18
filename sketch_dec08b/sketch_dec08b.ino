String response;
int ADCValue;
void setup() {
  Serial.begin(115200);
  pinMode(A1,INPUT);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop() {  
  while(!Serial.available());
  digitalWrite(13,HIGH);
  Serial.read();
  Serial.print("ADC:");
  ADCValue = analogRead(A1);
  Serial.print(ADCValue);
  delay(500);
  digitalWrite(13,LOW);
}
