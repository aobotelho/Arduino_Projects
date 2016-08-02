// Tutorial 10
// LED and distance sensor - INTERRUPT!

// Setup button
// Pin2
int buttonInt = 0;

// Setup LED
int YellowLED = 11; 
int Red LED = 10;
int GreenLED = 9;
int nullLED = 6;

// Setup distance sensor
int Distpin = 0;

void Setup(){
  pinMode(RedLED,OUTPUT); 
  pinMode(GreenLED,OUTPUT);
  pinMode(YellowLED,OUTPUT);
  pinMode(nullLED,OUTPUT);
  
  // Attach the interrupt
  // (PORT, Function to run when the interrupt is called, When the interrupt should be called)
  attachInterrupt(buttonInt,swap,RISING);
}

void Loop(){
  
}
