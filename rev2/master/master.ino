#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h> 

//ip config
IPAddress ip(192, 168, 1, 3);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//network credentials
const char *ssid = "networkThijs";
const char *password = "Welkom01";

WiFiServer server(80);

boolean Switch = false;
int RGBAX[5] = {255, 255, 255};
int counter;
boolean switchOn = false;

void setup() {
//  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  server.begin();
  
  Wire.begin();
  Serial.begin(115200);
  Serial.println("esp started");
  sendLedData();
}

void loop() {
  if(switchOn && counter == 1000) {
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
    counter = 0;
    sendLedData();
  } else {
    counter++;
  }

  WiFiClient client = server.available();

  if(client) {
    String httpHeader = "";
    while(client.connected()) {
      if(client.available()){
        String line = client.readStringUntil('\r');
        httpHeader += line;
        if(line.length() == 1 && line[0] == '\n') {
          if(httpHeader.indexOf("POST") >= 0) {
            String body = client.readStringUntil('\r');
            StaticJsonBuffer<300> JSONBuffer;

            JsonObject& json = JSONBuffer.parseObject(body);
            if(json.success()) {
              switchOn = json["ledState"];
            }
          }
        }
      }
      client.stop();
    }
  }
  
  Serial.println();
}

void loopData(int data) {
  for(int i = 0; i < 5; i++) {
    RGBAX[i] = data;
  }
}

void sendLedData() {
  Wire.beginTransmission(7);
    Wire.write(RGBAX[0]);
    Wire.write(RGBAX[1]);
    Wire.write(RGBAX[2]);
    Wire.write(RGBAX[3]);
  Serial.println("return: ");
  Serial.println(Wire.endTransmission());
}

