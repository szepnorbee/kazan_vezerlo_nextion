#include <SoftwareSerial.h>
#include <Nextion.h>
#include <OneWire.h>
#include <DallasTemperature.h>
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

boolean mainMenu = true;

long previousMillis = 0; 
long interval = 1000; 
boolean stdby = true;

SoftwareSerial nextion(4, 5);// Nextion TX to pin 2 and RX to pin 3 of Arduino

Nextion myNextion(nextion, 115200); 

void setup() {
  Serial.begin(115200);
  myNextion.init();
  sensors.begin();
  Serial.println("START...");
  myNextion.setComponentText("fTemp", "PROGRAM");
  myNextion.setComponentText("vTemp", "START");
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
    sensors.setWaitForConversion(false);
    sensors.requestTemperatures();
    Serial.println(sensors.getTempCByIndex(0));
    updMain();
    }
  }
////////////////////

  String message = myNextion.listen();

  if (message != "") { // if a message is received...
    Serial.print("MESSAGE: ");
    Serial.println(message); //...print it out
    dataIn(message);
  }  
}
