// Tutorial 04
// Voltage Divider

#define MAX_ADC  1023
#define MIN_ADC  0
#define MAX_PWM  255
#define MIN_PWM  0


unsigned int SensePin = 0;
unsigned int LED_IO = 11;

void setup(){
 analogReference(DEFAULT);  // Isn't necessary
 
 pinMode(LED_IO,OUTPUT);
 
 Serial.begin(9600); 
}

void loop(){
 unsigned int val = analogRead(SensePin);
 // map: MAX_ADC is converted to MAX_PWM, MIN_ADC is converted to MIN_PWM
 unsigned int LEDLevel = map(val, MAX_ADC,MIN_ADC,MAX_PWM,MIN_PWM);
 Serial.println(analogRead(SensePin)); 
 analogWrite(LED_IO,analogRead(SensePin)*255/1023);
 delay(100);
}




