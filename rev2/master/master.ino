#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

//ip config
IPAddress ip(192, 168, 1, 102);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

typedef enum { METHOD_ERR = 0, GET, POST } HTTP_METHOD;

//network credentials
const char *ssid = "Connecting....";
const char *password = "Smartphone1234";

WiFiServer server(8080);

boolean Switch = false;
int RGBAX[5] = {255, 255, 255};
int counter;
boolean switchOn = false;
int address;

void setup() {
  Serial.begin(115200);
  delay(100);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();

  Wire.begin();
  Serial.println("esp started on " + ipToString(WiFi.localIP()));
  sendLedData(7);
}

void loop() {
  WiFiClient client = server.available();

  if ( client ) {
    String httpHeader = "";

    while (client.connected()) {

      if (client.available()) {

        String line = client.readStringUntil('\r');
        httpHeader += line;

        if ( line.length() == 1 && line[0] == '\n') {

          // Handle GET
          if (GET == parseHeader(httpHeader)) {
            client.println(getResponseString());
            break;
          }

          if (POST == parseHeader(httpHeader)) {
            String body = client.readStringUntil('\r');

            StaticJsonBuffer<1000> JSONBuffer;

            JsonObject& json = JSONBuffer.parseObject(body);
            if (json.success()) {
              RGBAX[0] = normalize(json["R"]);
              RGBAX[1] = normalize(json["G"]);
              RGBAX[2] = normalize(json["B"]);
              RGBAX[3] = normalize(json["A"]);
              RGBAX[4] = json["X"];
              address = json["address"];

              sendLedData(address);
            } else {
              Serial.println("json error");
            }
            client.println(getResponseString());
            break;
          }
        }
      }
    }
    client.stop();
  }
}

void sendLedData(int address) {
  Wire.beginTransmission(address);
  Wire.write(RGBAX[0]);
  Wire.write(RGBAX[1]);
  Wire.write(RGBAX[2]);
  Wire.write(RGBAX[3]);
  Serial.println("return: ");
  Serial.println(Wire.endTransmission());
}

String postResponseString() {
  String res = "";
  res += "HTTP/1.1 200 OK\r\n";
  res += "Connection: close\r\n";
  res += "Content-Type: application/json\r\n";
  res += "\r\n";
  res += "{\"msg\":\"greetings from the esp8266!\"}\r\n";
  res += "\r\n";
  return res;
}

HTTP_METHOD parseHeader(String header) {
  if ( header.indexOf("GET") >= 0 ) return GET;
  if ( header.indexOf("POST") >= 0 ) return POST;
  return METHOD_ERR;
}

String getResponseString() {
  String res = "";
  res += "HTTP/1.1 200 OK\r\n";
  res += "Connection: close\r\n";
  res += "Content-Type: application/json\r\n";
  res += "\r\n";
  res += "{\"msg\":\"Have a nice day\"}\r\n";
  res += "\r\n";
  return res;
}

int normalize(int data) {
  int i = 255 - data;
  Serial.println("normalized data: " + String(i));
  return i;
}

String ipToString(IPAddress ip) {
  String s = "";
  for (int i = 0; i < 4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}
