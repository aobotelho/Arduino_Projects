void Step_A(){
  digitalWrite(Pin1,HIGH);
  digitalWrite(Pin2,LOW);
  digitalWrite(Pin3,LOW);
  digitalWrite(Pin4,LOW);
}

void Step_B(){
  digitalWrite(Pin1,LOW);
  digitalWrite(Pin2,HIGH);
  digitalWrite(Pin3,LOW);
  digitalWrite(Pin4,LOW);
}

void Step_C(){
  digitalWrite(Pin1,LOW);
  digitalWrite(Pin2,LOW);
  digitalWrite(Pin3,HIGH);
  digitalWrite(Pin4,LOW);
}

void Step_D(){
  digitalWrite(Pin1,LOW);
  digitalWrite(Pin2,LOW);
  digitalWrite(Pin3,LOW);
  digitalWrite(Pin4,HIGH);
}

void forward(){
  Step_A();
  delay(delaytime);
  Step_B();
  delay(delaytime);
  Step_C();
  delay(delaytime);
  Step_D();
  delay(delaytime);
}

void backward(){
  Step_D();
  delay(delaytime);
  Step_C();
  delay(delaytime);
  Step_B();
  delay(delaytime);
  Step_A();
  delay(delaytime);
}
void Step_OFF(){
  digitalWrite(Pin1,LOW);
  digitalWrite(Pin2,LOW);
  digitalWrite(Pin3,LOW);
  digitalWrite(Pin4,LOW);
}
