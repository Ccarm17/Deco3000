/*
 * Created by Clay Griffith-Carmichael
 * 
 * Sketch creates a MQTT connection, subscribes to 3 topics and publishes to 2 topics
 * Receives track numbers and plays track on DfPlayer
 * Receives lighting triggers and plays annimations
 * Ultrasonic sensor input controlls volume of DfPlayer
 * Publihes current battery voltage, and ultrasonic data. 
 * 
 * 
 */


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
const char *ID = "gsrB";

//Perams
//float Voltage = 0;
int dfPlayerVolume = 10;
int trackNo;

// Counter and state of PIR sensor
long int people = 0; 
int pirState = 0;
int lastPirState = 0;

WiFiClient gsrB;
PubSubClient client(gsrB);
long lastMsg = 0;
char msg[50];
int value = 0;


//ESP32 onboard led pin 
const int ledPin = 13;

// PIR Pins
const int pirPin = 13;


//Neopixel pin number and number of led's
#define PIXEL_PIN    21
#define PIXEL_COUNT 403
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

HardwareSerial mySoftwareSerial(1);
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

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

// PIR setup
pinMode(pirPin, INPUT);

// DFplayer setup
 mySoftwareSerial.begin(9600, SERIAL_8N1, 17, 16);  // speed, type, RX, TX

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    
    Serial.println(myDFPlayer.readType(),HEX);
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    //while(true);
  }
  
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(dfPlayerVolume);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  
  int delayms=100;

  /*
  Serial.println(F("myDFPlayer.loop(1)"));
  myDFPlayer.loop(1);  //Loop the first mp3
  delay(delayms);

  Serial.println(F("myDFPlayer.play(1)"));
  myDFPlayer.play(1);  //Play the first mp3
  delay(delayms);


  */

  //----Read imformation----
  Serial.println(F("readState--------------------"));
  Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.println(F("readVolume--------------------"));
  Serial.println(myDFPlayer.readVolume()); //read current volume
  //Serial.println(F("readEQ--------------------"));
  //Serial.println(myDFPlayer.readEQ()); //read EQ setting
  Serial.println(F("readFileCounts--------------------"));
  Serial.println(myDFPlayer.readFileCounts()); //read all file counts in SD card
  Serial.println(F("readCurrentFileNumber--------------------"));
  Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
  Serial.println(F("readFileCountsInFolder--------------------"));
  Serial.println(myDFPlayer.readFileCountsInFolder(3)); //read fill counts in folder SD:/03
  Serial.println(F("--------------------"));

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
   

    if(messageTemp == "green"){
      Serial.println("Greeen");
      colorWipe(strip.Color(0, 255, 0), 50);  // Green
    }

    else if(messageTemp == "greenoff"){
      Serial.println("neopix off");
      rainbow(20);
    }
  }
if (String(topic) == "esp32/output/dfPlayerB") {
    trackNo = messageTemp.toInt();
    myDFPlayer.play(trackNo);
   
}

if (String(topic) == "esp32/output/dfPlayerVolumeGSR") {
    dfPlayerVolume = messageTemp.toInt();
    myDFPlayer.volume(dfPlayerVolume);
    
}
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("gsrB")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
      client.subscribe("esp32/output/neopixelB");
      client.subscribe("esp32/output/dfPlayerB");
      client.subscribe("esp32/output/dfPlayerVolumeGSR");
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
  rainbow(10);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

// PIR loop
pirState = digitalRead(pirPin);

if (pirState != lastPirState){
  if (pirState == HIGH){
    people ++;
    Serial.println(people);

}
lastPirState = pirState;
}

// Voltage update loop
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

/*
    Voltage = analogRead(A13);
    Voltage = (Voltage * 2)/1000;
    
    char voltageString[8];
    dtostrf(Voltage, 1, 2, voltageString);
    Serial.print("Volatge: ");
    Serial.println(voltageString);
    client.publish("esp32/voltage1", voltageString);
*/
    char peopleString[8];
    dtostrf(people, 1, 2, peopleString);
    client.publish("esp32/people/b", peopleString);
    Serial.println(peopleString);
    

  }

  // for use with testing from serial monitor
  /*
  if (Serial.available()) {
    String inData = "";
    inData = Serial.readStringUntil('\n');
    if (inData.startsWith("n")) {
      Serial.println(F("next--------------------"));
      myDFPlayer.next();
      Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
    } 
    else if (inData.startsWith("p")) {
      Serial.println(F("previous--------------------"));
      myDFPlayer.previous();
      Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
    } else if (inData.startsWith("+")) {
      Serial.println(F("up--------------------"));
      myDFPlayer.volumeUp();
      Serial.println(myDFPlayer.readVolume()); //read current volume
    } else if (inData.startsWith("-")) {
      Serial.println(F("down--------------------"));
      myDFPlayer.volumeDown();
      Serial.println(myDFPlayer.readVolume()); //read current volume
    } 
    else if (inData.startsWith("*")) {
      Serial.println(F("pause--------------------"));
      myDFPlayer.pause();
    } else if (inData.startsWith(">")) {
      Serial.println(F("start--------------------"));
      myDFPlayer.start();
    }
 }
*/

/*
 if (myDFPlayer.available()) {
  if (myDFPlayer.readType()==DFPlayerPlayFinished) {
    Serial.println(myDFPlayer.read());
    Serial.println(F("next--------------------"));
     myDFPlayer.next();  //Play next mp3 every 3 second.
    Serial.println(F("readCurrentFileNumber--------------------"));
    Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
    
    }
  }  

  */
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

// error messages from dfplayer
/*
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
*/







  
