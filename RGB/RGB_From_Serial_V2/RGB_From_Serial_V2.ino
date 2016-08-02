// RGB_From_Serial_V2

#define ARRAY_SIZE  12

#define RED_PIN    11
#define GREEN_PIN  10
#define BLUE_PIN   9

char stream[ARRAY_SIZE] = "";
unsigned int Red_Value,Green_Value,Blue_Value;
unsigned int aux1,aux2;
unsigned int colorCount = 0;

int counter = 0;

void setRGBValues();


void setup() {
  Serial.begin(9600);  
  
  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(BLUE_PIN,OUTPUT);
  
  analogWrite(RED_PIN,0xF);
  analogWrite(GREEN_PIN,0xF);
  analogWrite(BLUE_PIN,0xF);
}

void loop(){  
  int counter = 0;
  
  for(counter = 0;counter<ARRAY_SIZE;counter++) stream[counter] = '\0';
  counter=0;
  
  while(!Serial.available());
  
  Serial.readBytesUntil('\n',stream,ARRAY_SIZE);

  //while(stream[counter++]) {Serial.print(int('0'));Serial.print(' ');}
  
  setRGBValues();
  
  analogWrite(RED_PIN,Red_Value);
  analogWrite(GREEN_PIN,Green_Value);
  analogWrite(BLUE_PIN,Blue_Value);
}




void setRGBValues(){
  unsigned int auxCounter =0;

  do{    
    if(stream[auxCounter] == 'R'){
      Serial.print("R-- ");
      
      if( int(stream[auxCounter+1]) >= int('0') || int(stream[auxCounter+1]) <= int('9')){
        aux1 = ( stream[auxCounter+1] - '0' ) << 4; 
        Serial.print(" Aux1 is a number ");
      }
      else{
        aux1 = ( stream[auxCounter+1] - 'A' + 0xA) << 4;
        Serial.print(" Aux1 is a letter ");
      }
      
      Serial.print(" Aux1: ");
      Serial.print(aux1,HEX);
      
      if( int(stream[auxCounter+2]) >= int('0') || int(stream[auxCounter+2]) <= int('9')){
        aux2 = ( stream[auxCounter+2] - '0' ); 
        Serial.print(" Aux2 is a number ");
      }
      else{
        aux2 = ( stream[auxCounter+2] - 'A' + 0xA);
        Serial.print(" Aux2 is a letter ");
      }
      
      Serial.print(" Aux2: ");
      Serial.println(aux2,HEX);
      
      Red_Value = aux1 + aux2;
      colorCount++;
    }
    else if(stream[auxCounter] == 'G'){
      Serial.print("G-- ");
      
      if( int(stream[auxCounter+1]) >= int('0') || int(stream[auxCounter+1]) <= int('9')){
        aux1 = ( stream[auxCounter+1] - '0' ) << 4;
        Serial.print(" Aux1 is a number ");
      }
      else{
        aux1 = ( stream[auxCounter+1] - 'A' + 0xA) << 4;
        Serial.print(" Aux1 is a letter ");
      }
      
      Serial.print(" Aux1: ");
      Serial.print(aux1,HEX);
      
      if( int(stream[auxCounter+2]) >= int('0') || int(stream[auxCounter+2]) <= int('9')){
        aux2 = ( stream[auxCounter+2] - '0' );
        Serial.print(" Aux2 is a number ");
      }
      else{
        aux2 = ( stream[auxCounter+2] - 'A' + 0xA);
        Serial.print(" Aux2 is a letter ");
      }
      
      Serial.print(" Aux2: ");
      Serial.println(aux2,HEX);
      
      Green_Value = aux1 + aux2;
      colorCount++;
    }
    else if(stream[auxCounter] == 'B'){
      Serial.print("B-- ");
      
      if( int(stream[auxCounter+1]) >= int('0') || int(stream[auxCounter+1]) <= int('9')){
        aux1 = ( stream[auxCounter+1] - '0' ) << 4;
        Serial.print(" Aux1 is a number ");
      }
      else{
        aux1 = ( stream[auxCounter+1] - 'A' + 0xA) << 4;
        Serial.print(" Aux1 is a letter ");
      }
      
      Serial.print(" Aux1: ");
      Serial.print(aux1,HEX);
      
      if( int(stream[auxCounter+2]) >= int('0') || int(stream[auxCounter+2]) <= int('9')){
        aux2 = ( stream[auxCounter+2] - '0' );
        Serial.print(" Aux2 is a number ");
      }
      else{
        aux2 = ( stream[auxCounter+2] - 'A' + 0xA);
        Serial.print(" Aux2 is a letter ");
      }
      
      Serial.print(" Aux2: ");
      Serial.println(aux2,HEX);
      
      Blue_Value = aux1 + aux2;
      colorCount++;
    }
  }while(colorCount <3 && stream[auxCounter++]);
  
  colorCount = 0;
}
