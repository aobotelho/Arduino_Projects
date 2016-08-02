#include <PCM.h>

#define SAMPLE_SIZE 50
#define SAMPLE_PERIOD 100

int audioIn = A0;
int audioOut = 9;

int maxOut = 0;

unsigned char sample[SAMPLE_SIZE];

void setup(){
  pinMode(audioOut,OUTPUT);

  for(int i = 0 ; i < SAMPLE_PERIOD ; i++){
    int tempMax = analogRead(audioIn);
    if(maxOut < tempMax){
      maxOut = tempMax;
    }
    delayMicroseconds(100);
  }
}

void loop(){
  for(int i = 0 ; i < SAMPLE_SIZE ; i++){
    sample[i] = map(analogRead(audioIn),0,maxOut,0,255);
    delayMicroseconds(50);
  }
  
  startPlayback(sample, sizeof(sample));
}
