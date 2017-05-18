#include <TinyWireS.h>


#define ADDRESS 0x7
#define R 0
#define G 1
#define B 2
#define A 3

int RGBAX[5];

void setup() {
  TinyWireS.begin(ADDRESS);
  TinyWireS.onReceive(receiveEvent);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A, OUTPUT);
  
}

void loop() {
  TinyWireS_stop_check();

  switch(RGBAX[5]) {
    case 1:
      
    break;
  }
}

void receiveEvent(uint8_t howMany) {
  int i = 0;
  while(1 < TinyWireS.available()) {
    RGBAX[i] = TinyWireS.receive();   
  }
  
  analogWrite(R, RGBAX[0]);
  analogWrite(G, RGBAX[1]);
  analogWrite(B, RGBAX[2]);
  analogWrite(A, RGBAX[3]);
  
}

void wave(){
  if(RGBAX[R] == 0 && RGBAX[G] == 0 && RGBAX[B] == 0) {
    RGBAX[R]++;
  } else if(RGBAX[R] == 255 && RGBAX[G] == 0 && RGBAX[B] == 0) {
    RGBAX[G]++;
  } else if(RGBAX[R] == 255 && RGBAX[G] == 255 && RGBAX[B] == 0) {
    RGBAX[R]--;
  } else if(RGBAX[R] == 0 && RGBAX[G] == 255 && RGBAX[B] == 0) {
    RGBAX[B]++;
  } else if(RGBAX[R] == 0 && RGBAX[G] == 255 && RGBAX[B] == 255) {
    RGBAX[G]--;
  } else if(RGBAX[R] == 0 && RGBAX[G] == 0 && RGBAX[B] == 255) {
    RGBAX[R]++;
  } else if(RGBAX[R]
  
}

