
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"


// The network information
const char* ssid = "Deco3000";
const char* password = "42563808";

// IP address of the raspberry pi
const char* mqtt_server = "192.168.1.100";

// Uniqe client ID 
const char *ID = "ccc01";

//Perams
float Voltage = 0;


WiFiClient espClient01;
PubSubClient client(espClient01);
long lastMsg = 0;
char msg[50];
int value = 0;


//ESP32 onboard led pin 
const int ledPin = 13;



//Neopixel pin number and number of led's
#define PIXEL_PIN    14
#define PIXEL_COUNT 8
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

if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }

    
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ccc01")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
      client.subscribe("esp32/output/neopixel");

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



  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;


    Voltage = analogRead(A13);
    Voltage = (Voltage * 2)/1000;
    
    char voltageString[8];
    dtostrf(Voltage, 1, 2, voltageString);
    Serial.print("Volatge: ");
    Serial.println(voltageString);
    client.publish("esp32/voltage1", voltageString);

   
    


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


