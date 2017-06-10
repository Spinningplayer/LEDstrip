#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <Wire.h>

// 
// AP credentials
//
const char *ssid = "networkThijs";
//const char *password = "WTHcc8SW";
const char *password = "Welkom01";

IPAddress ip(192, 168, 1, 3);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

//
// Globale variables
//
boolean ledStatus = false;
unsigned int fakeCounter = 0;
int RGBAX[5] = {255, 255, 255};

// 
// Server@port
//
const int PORT = 8080;
WiFiServer server(PORT);

//
// Types
//
typedef enum { METHOD_ERR = 0, GET, POST } HTTP_METHOD;

//
// Functie prototypes
//
String getResponseString();
String postResponseString();
HTTP_METHOD parseHeader(String header);

//
// Initial setup
//
void setup() {

  // 
  // Setup serial
  //
  Serial.begin(115200);
  delay(100);
  Serial.printf("\n\r>%s: Trying to connected to %s\n\r", __FUNCTION__, ssid);

  // 
  // Connect to AP
  //
  //WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.begin();
  Serial.printf("\n\r>%s: BIEM! connected.\n\r", __FUNCTION__);

  // 
  // Done in setup
  //
  Serial.printf("<%s: Let the fun begin. Connect to %s:%d\n\r", 
      __FUNCTION__, 
      WiFi.localIP().toString().c_str(), 
      PORT);
  //
  //  Start I2C bus
  //
  Wire.begin();
  setLed();
}

//
// Loop forever
//
void loop() {

   WiFiClient client = server.available();

   if( client ) {
      String httpHeader = "";
          
      while(client.connected()) {
      
         if(client.available()) {
        
            String line = client.readStringUntil('\r'); 
            httpHeader += line;        
        
            if( line.length() == 1 && line[0] == '\n') {

               // Handle GET
               if(GET == parseHeader(httpHeader)) {
                  client.println(getResponseString());
                  break;
               }

               // Handle POST
               if(POST == parseHeader(httpHeader)) {
                  String body = client.readStringUntil('\r'); 

                  // Maak een static buffer aan voor de JSON parser
                  StaticJsonBuffer<300> JSONBuffer;

                  // Vertaal de bodystring in een JSON object
                  JsonObject& json = JSONBuffer.parseObject(body);

                  // Bij success query alle KEY-VALUE paren.
                  if( json.success() ) {
                     boolean ledState = json["ledState"];
                     if(ledState) {
                      for(int i = 0; i < 5; i++){
                        RGBAX[i] = 0;
                      }
                     } else {
                      for(int i = 0; i < 5; i++){
                        RGBAX[i] = 255;
                      }
                     }
                     setLed();
                  } else {
                     Serial.println("JSON parser err ..."); 
                  }
                  
                  client.println(postResponseString()); 
                  break;
               }          
            }
         }
      }
      client.stop();
   }
}

HTTP_METHOD parseHeader(String header) {
   if( header.indexOf("GET") >= 0 ) return GET;
   if( header.indexOf("POST") >= 0 ) return POST;
   return METHOD_ERR;
}

void setLed() {
   
    Wire.beginTransmission(7);
    Wire.write(RGBAX[0]);
    Wire.write(RGBAX[1]);
    Wire.write(RGBAX[2]);
    Wire.write(RGBAX[3]);
  Serial.println("return: ");
  Serial.println(Wire.endTransmission());
    
   //state ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW);
}


//
// HTTP GET response message
//
String getResponseString() {
  String res = "";
  res += "HTTP/1.1 200 OK\r\n";
  res += "Connection: close\r\n";
  res += "Content-Type: application/json\r\n";
  res += "\r\n";
  
  String str = "{\"counter\":";
  str += fakeCounter;
  str += "}\r\n";
  
  res += str;
  res += "\r\n";
  return res;
}

//
// HTTP POST response message
//
String postResponseString() {
  String res = "";
  res += "HTTP/1.1 200 OK\r\n";
  res += "Connection: close\r\n";
  res += "Content-Type: application/json\r\n";
  res += "\r\n";
  res += "{\"msg\":\"Have a nice day\"}\r\n";
  res += "\r\n";
  return res;
}


