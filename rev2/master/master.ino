#include <Wire.h>

boolean Switch = false;
int data = 0;

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
    data = 0;
    Serial.println("off");
  } else {
    Switch = true;
    data = 255;
    Serial.println("on");
  }
  
  Wire.beginTransmission(7);
    Wire.write(data);
    Wire.write(data);
    Wire.write(data);
    Wire.write(data);
  Serial.println("return: ");
  Serial.println(Wire.endTransmission());
  
  Serial.println();
  delay(500);
}
