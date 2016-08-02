#define  SCL_Pin  8
#define  SDO_Pin  9

byte CurrentKey = 0;
byte OldKey = 0;
bool NewKey = false;

void setup(){
  
  Serial.begin(9600);
  
  pinMode(SCL_Pin,OUTPUT);
  pinMode(SDO_Pin,INPUT);  
  
  Serial.println("Hello World!!");
}

void loop() {
  CurrentKey = CheckKey();
  
  if(CurrentKey == 0){
    OldKey = CurrentKey;
    if(!NewKey){
      NewKey = true;
      Serial.println("No Key");
    }
  }
  else if(NewKey){
   Serial.print("Pressed: ");
   Serial.println(CurrentKey);
   OldKey = CurrentKey;
   NewKey = false;
  }
    
  
}

byte CheckKey(){
 byte Key = 0;
 int Counter = 0;
 
 for(Counter = 0; Counter <=16 ; Counter++){
   digitalWrite(SCL_Pin,HIGH);
   
   if(!digitalRead(SDO_Pin)){
     Key = Counter;     
   }   
   digitalWrite(SCL_Pin,LOW);
 }
 
 delay(3);
 return Key;
}

