#include <SoftwareSerial.h>
#include <Nextion.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char* host = "esp8266-webupdate";
const char* ssid = "ASUS_HUN";
const char* password = "Kicsim1986";

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

byte fStart = 5;
byte fStop = 5;
byte tStart = 5;
byte tStop = 5;
byte hiszter = 3;
byte fanDelay = 10;
boolean thermostat = true;
boolean debug = false;
float fTemp = 850.7;
float vTemp = 42.5;
byte profile = 1;

/// VEZÉRLÉS ///
const int heatPin = 10;
const int fanPin = 11;
const int motorPin = 12;
const int ledPin = 13;
boolean motorState = HIGH; 
boolean fanState = HIGH;
boolean manual = false;
boolean reqHeat = false;
/// Hőmérséklet szabályozás ///
byte setvTemp = 60;
int setfTemp = 800;
float tempC = 0;

////////////////

boolean mainMenu = true;

long previousMillis = 0; 
long interval = 1000; 
boolean stdby = true;

SoftwareSerial nextion(4, 5);// Nextion TX to pin 2 and RX to pin 3 of Arduino

Nextion myNextion(nextion, 115200); 

void setup() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }
  MDNS.begin(host);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  
  Serial.begin(115200);
  EEPROM.begin(512);
  myNextion.init();
  sensors.begin();
  Serial.println("START...");
  myNextion.setComponentText("fTemp", "PROGRAM");
  myNextion.setComponentText("vTemp", "START");
  profile = EEPROM.read(0);
  memRead();
  delay(1000);
  updMain();
}

void loop() {
// NEXTION UPDATE //
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis; 
    if(mainMenu==true) { 
   /////Stdby/////
   if(stdby) {
    stdby=false;
   } else {
    stdby=true;
   }
   ///////////////
    updMain();
    }
    sensors.setWaitForConversion(false);
    sensors.requestTemperatures();
    Serial.println(sensors.getTempCByIndex(0));
  }
////////////////////

  String message = myNextion.listen();

  if (message != "") { // if a message is received...
    Serial.print("MESSAGE: ");
    Serial.println(message); //...print it out
    dataIn(message);
  }  
  httpServer.handleClient(); // WEB UPDATE
}
