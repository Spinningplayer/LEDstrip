#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include <ArduinoJson.h>
#include <Wire.h>

//ip configuration
IPAddress ip(192, 168, 1, 102);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//network credentials
const char *ssid = "xxx";
const char *password = "xxx";

//RGB array plus extra optional values
int RGBAX[5] = {255, 255, 255};

WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            //client disconnected
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            //client connected, show IP address
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            // Received text
            Serial.printf("[%u] get Text: %s\n", num, payload);
            
            // Create jsonbuffer and parse object
            StaticJsonBuffer<300> JSONBuffer;

            JsonObject& json = JSONBuffer.parseObject(payload);

            //if parsing succeeded retrieve RGB and address values
            if (json.success()) {
              JsonObject& rgb = json["color"];

              // Normalize RGB data before storing
              RGBAX[0] = normalize(rgb["red"]);
              RGBAX[1] = normalize(rgb["green"]);
              RGBAX[2] = normalize(rgb["blue"]);
              
              int address = json["address"];

              //Send RGB values to slave address
              sendLedData(address);
            } else {
              Serial.println("json error");
            }
        
            break;
    }

}

void setup() {
    // Setup Serial connection
    Serial.begin(115200);
    delay(100);
    Serial.println();
    
    // Setup WiFi configuration and connect
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(ssid, password);

    // Log IP to serial
    Serial.println(ipToString(WiFi.localIP()));

    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    // Begin MDNS service
    if(MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }

    //Start i2c bus
    Wire.begin();

    // Add service to MDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);
}

void loop() {
    // Handle Websockets
    webSocket.loop();
}

/*
 * int sendLedData
 * input: address of i2c slave
 * output: i2c result code
 * description: Sends the values of RGBAX array to the
 * i2c slave on the given address
 */
int sendLedData(int address) {
  Wire.beginTransmission(address);
  for (int i = 0; i < sizeof(RGBAX); i++) {
    Wire.write(RGBAX[i]);
  }
  return Wire.endTransmission();
}

/*  
 * int normalize   
 * input: integer to be converted
 * output: converted integer
 * description: Method to normalize data for the LED strips.
 * NOTE: Might move this task to the slaves
 */
int normalize(int data) {
  int i = 255 - data;
  return i;
}


/*
 * String ipToString
 * input: IPAddress
 * output string version of given IPAdress
 * description: converts IPAddress to String
 */
String ipToString(IPAddress ip) {
  String s = "";
  for (int i = 0; i < 4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}
