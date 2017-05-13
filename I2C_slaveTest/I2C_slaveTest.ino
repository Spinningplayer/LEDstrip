#include <TinyWireS.h>

#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 16 )
#endif

#define ADDRESS 0x7
int x;

void setup() {
  TinyWireS.begin(ADDRESS);
  TinyWireS.onReceive(receiveEvent);
  pinMode(9, OUTPUT);
}

void loop() {
  TinyWireS_stop_check();
}

void receiveEvent(uint8_t howMany) {

  while(1 < TinyWireS.available()) {
    x = TinyWireS.receive();   
  }
  digitalWrite(9, x);
  
}

