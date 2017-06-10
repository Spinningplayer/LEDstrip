#include <ESP8266WiFi.h>
#include <Wire.h>

const char* ssid = "networkThijs";
const char* pass = "Welkom01";

WiFiServer server(80);
IPAddress ip(192, 168, 1, 102);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

int led = 7;
bool ledState = false;
boolean Switch = false;
int RGBAX[5] = {255, 255, 255};

void setup() {
  Serial.begin(115200);
  Serial.println("Serial started");
  
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
  WiFi.begin(ssid, pass);
  //WiFi.config(ip, gateway, subnet);
  
  while(WiFi.status() != WL_CONNECTED) {
    if(ledState) {
      digitalWrite(led, HIGH);
      ledState = !ledState;
    } else {
      digitalWrite(led, LOW);
      ledState = !ledState;
    }
    delay(500);
  }
  Serial.println("Wifi connected");
  
  server.begin();
  Serial.println("Server started");
  
  Wire.begin();
  
}

void loop() {
  WiFiClient client = server.available();
  if(!client){
    return;
  }
  Serial.println("============");
  
  String request = client.readStringUntil('\r');
  Serial.println(request);
  
  Serial.println("state: ");

  if(request.indexOf("/strip/on") > 0) {
    Serial.print("trigger on");
    for(int i = 0; i < 4; i++) {
      RGBAX[i] = 0;
    }
  }
  
  if(request.indexOf("/strip/off") > 0) {
    Serial.print("trigger off");
    for(int i = 0; i < 4; i++) {
      RGBAX[i] = 255;
    }
  }
  /*
  if(Switch) {
    Switch = false;
    RGBAX[1] = 0;
    Serial.println("off");
  } else {
    Switch = true;
    RGBAX[1] = 255;
    Serial.println("on");
  }
  */
  
  
  Wire.beginTransmission(7);
    Wire.write(RGBAX[0]);
    Wire.write(RGBAX[1]);
    Wire.write(RGBAX[2]);
    Wire.write(RGBAX[3]);
  Serial.println("return: ");
  Serial.println(Wire.endTransmission());
  
  Serial.println();
  client.flush();
}
