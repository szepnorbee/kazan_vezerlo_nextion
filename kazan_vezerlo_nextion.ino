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
#include <BlynkSimpleEsp8266.h>
//#include<EasyDDNS.h>

const char* host = "esp8266-webupdate";
const char* ssid = "TP-Link";
const char* password = "zolika11";
char auth[] = "14ac0aae450047428d80a82676a2fa93";   // Blynk

//const char* ssid = "ENIKO";
//const char* password = "Kicsim1986";

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
boolean fanStart = true;
byte fanCounter = 0;
boolean fanFlag = 0;
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
byte wl = 0;
SoftwareSerial nextion(4, 5);

Nextion myNextion(nextion, 115200);

void runBlynk() {
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
  }
}

void sendBlynk() {
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.virtualWrite(V2, millis() / 1000);
    if(reqHeat) {
     Blynk.virtualWrite(V7, "Futes"); 
    } else {
     Blynk.virtualWrite(V7, "Tartas");  
    }
    Blynk.virtualWrite(V0, fTemp); 
    Blynk.virtualWrite(V1, vTemp); 
  }
}

void handle_root() {
  httpServer.send(200, "text/plain", "A kazan vezerlo uzemel...");
}

void setup() {
  pinMode(fanPin, OUTPUT);
  pinMode(heatPin, INPUT);
  pinMode(motorPin, OUTPUT);
  digitalWrite(fanPin, HIGH); // kikapcsoljuk a kimeneteket
  digitalWrite(motorPin, HIGH);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  delay(1000);
  if (WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.begin(ssid, password);
  }
  httpServer.on("/", handle_root);
  MDNS.begin(host);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  Blynk.config(auth);
  Blynk.connect(10);
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
  httpServer.handleClient(); // WEB UPDATE
  runBlynk();
  ////// NEXTION UPDATE ///////////////////////////
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    if (mainMenu == true) {
      /////Stdby symbol/////
      if (stdby) {
        stdby = false;
      } else {
        stdby = true;
      }
      /////////////////////////////////////////////////
      updMain();
    }
    sensors.setWaitForConversion(false);
    sensors.requestTemperatures();
    vTemp = sensors.getTempCByIndex(0);
    fTemp = thermocouple.readCelsius();
    byte t1 = fTemp / 2;
    byte t2 = vTemp / 2;
    
    nextion.print("add 1,0,");
    nextion.print(map(t1,0,1200,10,115)); 
    nextion.write(0xFF);
    nextion.write(0xFF);
    nextion.write(0xFF);

    nextion.print("add 4,0,");
    nextion.print(map(t2,0,128,10,115));
    nextion.write(0xFF);
    nextion.write(0xFF);
    nextion.write(0xFF);
///////// Ventillátor késleltetés tartó üzemen/////
     if (fanStart == true) {
       fanCounter++;
     }
    sendBlynk();
    }
   
  
  //////////////////////////////////////////////////

  String message = myNextion.listen();
  if (message != "") { // if a message is received...
    dataIn(message);
  }
  //// CSIGA VEZÉRLÉS ////////
  unsigned long aktualisMillis = millis();

if (manual == false) {
  if ((motorState == LOW) && (aktualisMillis - elozoMillis >= OnTime))
  {
    motorState = HIGH;  // Turn it off
    elozoMillis = aktualisMillis;
    digitalWrite(motorPin, motorState);
  }
  else if ((motorState == HIGH) && (aktualisMillis - elozoMillis >= OffTime))
  {
    motorState = LOW;  // turn it on
    elozoMillis = aktualisMillis;
    digitalWrite(motorPin, motorState);
  }
  //// LOOP without delay ///////////////////////
 
  if (reqHeat == false & motorState == LOW) {
    digitalWrite(fanPin, LOW);         // Ventillátor bekapcsolása
    fanStart = true;
  }

  if (fanCounter > fanDelay) {
    digitalWrite(fanPin, HIGH);         // Ventillátor kikapcsolása
    fanCounter = 0;
    fanStart = false;
  }
 }
  ///////////////////////////////////////////////
}
