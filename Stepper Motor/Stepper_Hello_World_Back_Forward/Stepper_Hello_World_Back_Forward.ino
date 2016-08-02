#define Pin1  8
#define Pin2  9
#define Pin3  10
#define Pin4  11

#define delaytime  8


void setup() {
  pinMode(Pin1,OUTPUT);
  pinMode(Pin2,OUTPUT);
  pinMode(Pin3,OUTPUT);
  pinMode(Pin4,OUTPUT);
}

void loop() {
  int numberOfSteps = 48;
  Step_OFF();
  
  while(numberOfSteps){
    forward();
    numberOfSteps--;
  }
  
  while(numberOfSteps<48){
    backward();
    numberOfSteps++;
  }
  
  delay(2000);
}
