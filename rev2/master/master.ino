#include <Wire.h>

boolean Switch = false;
int RGBAX[5] = {255, 255, 255};

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while(!Serial);
  Serial.println("esp started");
}

void loop() {
  Serial.println("============");
  
  Serial.println("state: ");
  if(Switch) {
    Switch = false;
    RGBAX[1] = 0;
    Serial.println("off");
  } else {
    Switch = true;
    RGBAX[1] = 255;
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
