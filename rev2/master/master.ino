#include <Wire.h>

boolean Switch = false;
int RGBAX[5] = {255, 255, 255};

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while(!Serial);
  Serial.println("esp started");
}

void loop() {
  Serial.println("============");
  
  Serial.println("state: ");
  if(Switch) {
    Switch = false;
    loopData(0);
    Serial.println("off");
  } else {
    Switch = true;
    loopData(255);
    Serial.println("on");
  }
  
  Wire.beginTransmission(7);
    Wire.write(RGBAX[0]);
    Wire.write(RGBAX[1]);
    Wire.write(RGBAX[2]);
    Wire.write(RGBAX[3]);
  Serial.println("return: ");
  Serial.println(Wire.endTransmission());
  
  Serial.println();
  delay(500);
}

void loopData(int data) {
  for(int i = 0; i < 5; i++) {
    RGBAX[i] = data;
  }
}

