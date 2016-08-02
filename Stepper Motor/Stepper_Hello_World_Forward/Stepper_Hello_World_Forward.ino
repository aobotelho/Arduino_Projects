#include <Stepper.h>

#define  Pin1  8
#define  Pin2  9
#define  Pin3  10
#define  Pin4  11

#define Steps  1024

Stepper myStepper = Stepper(Steps,Pin1,Pin2,Pin3,Pin4);

void setup(){
  myStepper.setSpeed(15);
}

void loop(){
  myStepper.step(Steps/2);
  
  delay(1000);
}
