
enum{
  Red = 0,
  Green,
  Blue
};

enum{
  Effect_Program = 0,
  ADC_Program,
  Serial_Program
};

#define ARRAY_SIZE  12

#define RED_PIN    9
#define GREEN_PIN  10
#define BLUE_PIN   11  

#define RED_ADC     A0
#define GREEN_ADC   A1
#define BLUE_ADC    A2

#define LED1  7
#define LED2  8

#define BUTTON  2

#define  MIN  2
#define  SETPOINT  50
#define  MAX  250
#define  DELAY  5

// Effect Variables
signed int pace = 1;
unsigned int duty = 100;
unsigned int currentPin = Red;
boolean Up = true;

long debouncing_time = 150; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;

int Old_Red_ADC_Value, Old_Green_ADC_Value, Old_Blue_ADC_Value;
unsigned int TimeOut = 0;

volatile unsigned int currentProgram = Effect_Program;
volatile unsigned int lastProgram = currentProgram;
volatile bool changedProgram = false;

char stream[ARRAY_SIZE] = "";
unsigned int Red_Value,Green_Value,Blue_Value;
unsigned int aux1,aux2;
unsigned int colorCount = 0;

void Effect_Program_Function();
void ADC_Program_Function();
void Serial_Program_Function();
void setRGBValues();

void setup() {
  Serial.begin(9600);
  
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(BUTTON,INPUT_PULLUP);
  
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,LOW);
  
  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(BLUE_PIN,OUTPUT);
  
  pinMode(RED_ADC,INPUT);
  pinMode(GREEN_ADC,INPUT);
  pinMode(BLUE_ADC,INPUT);
  
  attachInterrupt(0,debounce,FALLING);
}

void loop(){
  switch(currentProgram){
    case Effect_Program:
      if(lastProgram != currentProgram) changedProgram = true;
      else changedProgram = false;
      Effect_Program_Function();
      lastProgram = currentProgram;
    break;
    
    case ADC_Program:
      if(lastProgram != currentProgram) changedProgram = true;
      else changedProgram = false;
      ADC_Program_Function();
      if(++TimeOut == 0x0F){
        Old_Red_ADC_Value = map(analogRead(A0),0,1024,0,255);
        Old_Green_ADC_Value = map(analogRead(A1),0,1024,0,255); 
        Old_Blue_ADC_Value = map(analogRead(A2),0,1024,0,255);
        
        if(Old_Red_ADC_Value < 3) Old_Red_ADC_Value = 0;
        if(Old_Green_ADC_Value < 3) Old_Green_ADC_Value = 0;
        if(Old_Blue_ADC_Value < 3) Old_Blue_ADC_Value = 0;
        
        analogWrite(RED_PIN,Old_Red_ADC_Value);
        analogWrite(GREEN_PIN,Old_Green_ADC_Value);
        analogWrite(BLUE_PIN,Old_Blue_ADC_Value);
        
        TimeOut = 0;
      }
      lastProgram = currentProgram;
    break;    
    case Serial_Program:
      if(lastProgram != currentProgram) changedProgram = true;
      else changedProgram = false;
      Serial_Program_Function();
      lastProgram = currentProgram;
    break;
  }
}



void Effect_Program_Function() { 
  
  switch(currentPin){
   case Red:
      analogWrite(RED_PIN,duty);
      delay(DELAY);
      
      duty += pace;
      
      if(duty == MAX){
        pace = -pace;
        Up = false;
      }
      else if(duty <= SETPOINT && Up == false){
        analogWrite(GREEN_PIN,SETPOINT-duty);        
      }
      
      if(duty == MIN && Up == false){
        Up = true;
        pace = -pace;
        currentPin = Green;
        duty = SETPOINT;
         
        analogWrite(GREEN_PIN,SETPOINT); 
      }
   break;
   
   case Green:
      analogWrite(GREEN_PIN,duty);
      delay(DELAY);
      
      duty += pace;
      
      if(duty == MAX){
        pace = -pace;
        Up = false;
      }
      else if(duty <= SETPOINT && Up == false){
        analogWrite(BLUE_PIN,SETPOINT-duty);
      }
      
      if(duty == MIN && Up == false){
        Up = true;
        pace = -pace;
        currentPin = Blue;
        duty = SETPOINT;
        
        analogWrite(BLUE_PIN,SETPOINT);
      }
   break;
   
   case Blue:
      analogWrite(BLUE_PIN,duty);
      delay(DELAY);
      
      duty += pace;
      
      if(duty == MAX){
        pace = -pace;
        Up = false;
      }
      else if(duty <= SETPOINT && Up == false){
        analogWrite(RED_PIN,SETPOINT-duty);
      }
      
      if(duty == MIN && Up == false){
        Up = true;
        pace = -pace;
        currentPin = Red;
        duty = SETPOINT;
        
        analogWrite(RED_PIN,SETPOINT);
      }
   break;  
  }
}

void ADC_Program_Function(){
  int Red_ADC_Value, Green_ADC_Value, Blue_ADC_Value;
  
  Red_ADC_Value = map(analogRead(A0),0,1024,0,255);
  Green_ADC_Value = map(analogRead(A1),0,1024,0,255); 
  Blue_ADC_Value = map(analogRead(A2),0,1024,0,255);
  
  if(((Red_ADC_Value <= Old_Red_ADC_Value-2) || (Red_ADC_Value >= Old_Red_ADC_Value+2)) && Red_ADC_Value){
    if(Red_ADC_Value < 3) Red_ADC_Value = 0;
    Old_Red_ADC_Value = Red_ADC_Value; 
    analogWrite(RED_PIN,Red_ADC_Value);
    TimeOut = 0;
  }
  
  if(((Green_ADC_Value <= Old_Green_ADC_Value-2) || (Green_ADC_Value >= Old_Green_ADC_Value+2)) && Green_ADC_Value){
    if(Green_ADC_Value < 3) Green_ADC_Value = 0;
    Old_Green_ADC_Value = Green_ADC_Value;
    analogWrite(GREEN_PIN,Green_ADC_Value);
    TimeOut = 0;
  }
  
  if(((Blue_ADC_Value <= Old_Blue_ADC_Value-2) || (Blue_ADC_Value >= Old_Blue_ADC_Value+2)) && Blue_ADC_Value){
    if(Blue_ADC_Value < 3) Blue_ADC_Value = 0;
    Old_Blue_ADC_Value = Blue_ADC_Value;    
     analogWrite(BLUE_PIN,Blue_ADC_Value);
    TimeOut = 0;
  }
}





void Serial_Program_Function(){
  int counter = 0;
  
  for(counter = 0;counter<ARRAY_SIZE;counter++) stream[counter] = '\0';
  counter=0;
  
  while(!Serial.available() && currentProgram == Serial_Program);
  
  if(!changedProgram){
    Serial.readBytesUntil('\n',stream,ARRAY_SIZE);
  
    //while(stream[counter++]) {Serial.print(int('0'));Serial.print(' ');}
    
    setRGBValues();
    
    analogWrite(RED_PIN,Red_Value);
    analogWrite(GREEN_PIN,Green_Value);
    analogWrite(BLUE_PIN,Blue_Value);
  }
}

void debounce(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    pushButton();
    last_micros = micros();
  }
}
void setRGBValues(){
  unsigned int auxCounter =0;

  do{    
    if(stream[auxCounter] == 'R'){
      if( int(stream[auxCounter+1]) >= int('0') || int(stream[auxCounter+1]) <= int('9'))
        aux1 = ( stream[auxCounter+1] - '0' ) << 4; 
      
      else  aux1 = ( stream[auxCounter+1] - 'A' + 0xA) << 4;
      
      if( int(stream[auxCounter+2]) >= int('0') || int(stream[auxCounter+2]) <= int('9'))
        aux2 = ( stream[auxCounter+2] - '0' );
      
      else  aux2 = ( stream[auxCounter+2] - 'A' + 0xA);
      
      
      Red_Value = aux1 + aux2;
      colorCount++;
    }
    else if(stream[auxCounter] == 'G'){
      if( int(stream[auxCounter+1]) >= int('0') || int(stream[auxCounter+1]) <= int('9'))
        aux1 = ( stream[auxCounter+1] - '0' ) << 4;      
      else  aux1 = ( stream[auxCounter+1] - 'A' + 0xA) << 4;
      
      
      if( int(stream[auxCounter+2]) >= int('0') || int(stream[auxCounter+2]) <= int('9'))
        aux2 = ( stream[auxCounter+2] - '0' );
      else   aux2 = ( stream[auxCounter+2] - 'A' + 0xA);
      
      
      Green_Value = aux1 + aux2;
      colorCount++;
    }
    else if(stream[auxCounter] == 'B'){
      if( int(stream[auxCounter+1]) >= int('0') || int(stream[auxCounter+1]) <= int('9'))
        aux1 = ( stream[auxCounter+1] - '0' ) << 4;
      else
        aux1 = ( stream[auxCounter+1] - 'A' + 0xA) << 4;
      
      
      if( int(stream[auxCounter+2]) >= int('0') || int(stream[auxCounter+2]) <= int('9'))
        aux2 = ( stream[auxCounter+2] - '0' );
      else  aux2 = ( stream[auxCounter+2] - 'A' + 0xA);
      
      
      Blue_Value = aux1 + aux2;
      colorCount++;
    }
  }while(colorCount <3 && stream[auxCounter++]);
  
  colorCount = 0;
}




void pushButton(){  
  lastProgram = currentProgram;
  
  if(currentProgram == ADC_Program) currentProgram = Effect_Program;
  else currentProgram++;
  
  switch(currentProgram){
    case Effect_Program:
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,LOW);
    break;
    case ADC_Program:
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,HIGH);
    break;
    case Serial_Program:
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,HIGH);
    break;
  }
  
  
}
