#include <TinyWireS.h>



#define ADDRESS 0x7
#define R 8
#define G 7
#define B 5
#define A 3

int RGBAX[5];

void setup() {
  TCCR1B = TCCR1B & B11111000 | B00000001; //this changes the PWM frequency to 32kHz
  TCCR0B = TCCR0B & B11111000 | B00000001; //this changes the PWM frequency to 62.5kHz
  TinyWireS.begin(ADDRESS);
  TinyWireS.onReceive(receiveEvent);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A, OUTPUT);
  
}

void loop() {
  TinyWireS_stop_check();

}

void receiveEvent(uint8_t howMany) {
  int i = 0;
  while(TinyWireS.available() > 0) {
    RGBAX[i] = TinyWireS.receive();   
    i++;
  }
  
  analogWrite(R, RGBAX[0]);
  analogWrite(G, RGBAX[1]);
  analogWrite(B, RGBAX[2]);
  analogWrite(A, RGBAX[3]);
}



