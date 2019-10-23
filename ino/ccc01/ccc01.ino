
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"


// The network information
const char* ssid = "Deco3000";
const char* password = "42563808";

// IP address of the raspberry pi
const char* mqtt_server = "192.168.8.104";

// Uniqe client ID 
const char *ID = "lobster";

//Perams
float Voltage = 0;


WiFiClient esplobster;
PubSubClient client(esplobster);
long lastMsg = 0;
char msg[50];
int value = 0;


//ESP32 onboard led pin 
const int ledPin = 13;

int r;
int g;
int b;
int n;


//Neopixel pin number and number of led's
#define PIXEL_PIN    14
#define PIXEL_COUNT 18
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);
// WIFI setup
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

// Neopixel setup
  pinMode(ledPin, OUTPUT);
  strip.begin();
  strip.show();
  
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();


if (String(topic) == "lobster/mac/output") {
    Serial.print("Changing output to ");


    if(messageTemp == "2"){
      Serial.println("rainbowCycle");
       rainbowCycle(20);
    }

    else if(messageTemp == "1"){
      Serial.println("rainbow");
      rainbow(20);
    }
    else if(messageTemp != "1" || messageTemp!="2"){
    Serial.println("rainbow");
    r = messageTemp.substring(0,3).toInt();
    Serial.println(r);
    g = messageTemp.substring(3,6).toInt();
    b = messageTemp.substring(6,9).toInt();
    n = messageTemp.substring(9,12).toInt();
    strip.setPixelColor((n * 4) + 0, r, g, b);
    strip.setPixelColor((n * 4) + 1, r, g, b);
    strip.setPixelColor((n * 4) + 2, r, g, b);
    strip.setPixelColor((n * 4) + 3, r, g, b);
    
    strip.show();
    
    }
    
  }
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("lobster")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("lobster/mac/output");
      

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}

// neopixel fill colour annimation
 void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// neopixel rainbow annimation relies on wheel 
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Wheel for colour changing annimations 
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
