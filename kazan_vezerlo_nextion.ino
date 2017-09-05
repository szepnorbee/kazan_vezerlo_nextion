#include <SoftwareSerial.h>
#include <Nextion.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"

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

#define MAXDO   1
#define MAXCS   3
#define MAXCLK  16

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

byte fStart = 5;
byte fStop = 5;
byte tStart = 5;
byte tStop = 5;
byte hiszter = 3;
byte fanDelay = 10;
boolean thermostat = true;
boolean debug = false;
double fTemp = 222;
float vTemp = 42.5;
byte profile = 1;

/// VEZÉRLÉS ///
const int heatPin = 13;
const int fanPin = 12;
const int motorPin = 14;
boolean motorState = HIGH; 
boolean fanState = HIGH;
boolean manual = false;
boolean reqHeat = true;
/// Hőmérséklet szabályozás ///
byte setvTemp = 60;
int setfTemp = 800;
float tempC = 0;

const long egyezer = 1000;       //másodperc
const long hatvanezer = 60000;   //perc
unsigned long elozoMillis = 0;
unsigned long OnTime = 0;
unsigned long OffTime = 0;
////////////////

boolean mainMenu = true;

long previousMillis = 0; 
long interval = 1000; 
boolean stdby = true;

SoftwareSerial nextion(4, 5);

Nextion myNextion(nextion, 115200); 

void setup() {
  pinMode(fanPin, OUTPUT);
  pinMode(heatPin, INPUT);
  pinMode(motorPin, OUTPUT);
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
}

void loop() {
  readInput();
  updVar();
////// NEXTION UPDATE ///////////////////////////
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis; 
    if(mainMenu==true) { 
   /////Stdby symbol/////
   if(stdby) {
    stdby=false;
   } else {
    stdby=true;
   }
/////////////////////////////////////////////////
    updMain();
    }
    sensors.setWaitForConversion(false);
    sensors.requestTemperatures();
    vTemp = sensors.getTempCByIndex(0);
    fTemp = thermocouple.readCelsius();
  }
//////////////////////////////////////////////////

  String message = myNextion.listen();
  if (message != "") { // if a message is received...
    dataIn(message);
  }  
//// CSIGA VEZÉRLÉS ////////
  unsigned long aktualisMillis = millis();

  if ((motorState == LOW) && (aktualisMillis - elozoMillis >= OnTime))
  {
    if (manual == false) {
      motorState = HIGH;  // Turn it off
      elozoMillis = aktualisMillis;
      digitalWrite(motorPin, motorState);
    }
  }
  else if ((motorState == HIGH) && (aktualisMillis - elozoMillis >= OffTime))
  {
    if (manual == false) {
      motorState = LOW;  // turn it on
      elozoMillis = aktualisMillis;
      digitalWrite(motorPin, motorState);
    }
  }
//// LOOP without delay ///////////////////////  
  httpServer.handleClient(); // WEB UPDATE

///////////////////////////////////////////////
}
