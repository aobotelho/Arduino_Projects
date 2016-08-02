#define LED1  7
#define LED2  8
#define BUTTON  2

int currentState = 3;
 
long debouncing_time = 150; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(BUTTON,INPUT_PULLUP);
  
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  
  attachInterrupt(0,debounce,FALLING);
}

void loop() {
  
}


void debounce(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    pushButton();
    last_micros = micros();
  }
}


void pushButton(){
   
  //while(digitalRead(BUTTON) == LOW);
  
    switch(currentState){
    case 0:
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,LOW);
    break;
    case 1:
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,LOW);
    break;
    case 2:
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,HIGH);
    break;
    case 3:
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,HIGH);
    break;
  }
  
  if(currentState == 3) currentState = 0;
  else currentState++;
}
