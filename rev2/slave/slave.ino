#include <TinyWireS.h>



#define ADDRESS 0x7
#define R 8
#define G 7
#define B 5
#define A 3

int RGBAX[5];
int counter = 0;

void setup() {
  TCCR1B = TCCR1B & B11111000 | 0x01; 
  TCCR0B = TCCR0B & B11111000 | 0x02; 
  
  TinyWireS.begin(ADDRESS);
  TinyWireS.onReceive(receiveEvent);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A, OUTPUT);
  
}

void loop() {
  TinyWireS_stop_check();
//  modeStep();
}

void receiveEvent(uint8_t howMany) {
  int i = 0;
  while(TinyWireS.available() > 0) {
    RGBAX[i] = TinyWireS.receive();   
    i++;
  }
//  modeStep();
  setLed();
}

void setLed() {
  analogWrite(R, RGBAX[0]);
  analogWrite(G, RGBAX[1]);
  analogWrite(B, RGBAX[2]);
  analogWrite(A, RGBAX[3]);
}
//
//void modeStep(){
//   switch(RGBAX[4]){
//    case 1:
//      
//  }
//}



