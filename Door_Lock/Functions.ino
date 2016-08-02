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

bool CheckArray(byte array1[PASS_SIZE], byte array2[PASS_SIZE]){
  for(int counter = 0; counter < PASS_SIZE; counter++){
    if(array1[counter] != array2[counter])
      return false;
  }  
  return true;  
}
