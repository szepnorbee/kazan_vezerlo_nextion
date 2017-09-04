void updMain() {
  int a =0;
  int b =0;
  a = map(fTemp,1,1200,1,100);
  b = map(vTemp,1,128,1,100);
  myNextion.setComponentValue("fVar", a);
  myNextion.setComponentValue("vVar", b);
  myNextion.setComponentText("fTemp", String(fTemp));
  myNextion.setComponentText("vTemp", String(vTemp));
  if (stdby) {
    myNextion.sendCommand("run.val=1");
  } else {
    myNextion.sendCommand("run.val=0");
  }
  myNextion.setComponentValue("n0", profile);
  Serial.println("DATA SENDING...");
}

void updTimers1() {
  myNextion.setComponentText("fStartNr", String(fStart));
  myNextion.setComponentText("fStopNr", String(fStop));
  Serial.println("DATA SENDING...");
}

void updTimers2() {
  myNextion.setComponentText("tStartNr", String(tStart));
  myNextion.setComponentText("tStopNr", String(tStop));
  Serial.println("DATA SENDING...");
}

void updThermostat() {
  myNextion.setComponentText("setTemp", String(setvTemp));
  myNextion.setComponentText("setFire", String(setfTemp));
  Serial.println("DATA SENDING...");
}

void updSettings() {
  myNextion.setComponentText("hiszt", String(hiszter));
  myNextion.setComponentText("levK", String(fanDelay));
  //delay(50);
  if (thermostat==true){
    myNextion.setComponentText("termState", "BE");
  } else {
    myNextion.setComponentText("termState", "KI");
  }
  if (debug==true){
    myNextion.setComponentText("debugState", "BE");
  } else {
    myNextion.setComponentText("debugState", "KI");
  }
  Serial.println("DATA SENDING...");
}

void dataIn(String message) {
  Serial.println("INCOMING");
// MENU NAVIGATION //////
  if (message=="65 0 3 0 ff ff ff") {
    myNextion.sendCommand("page Settings");
    mainMenu=false;
    updSettings();
  }
/// Vissza gombok ///
  if (message=="65 4 2 0 ff ff ff" || message=="65 5 1 0 ff ff ff" || message=="65 6 6 0 ff ff ff") {
    myNextion.sendCommand("page Main");
    mainMenu=true;
  }
  if (message=="65 2 b 0 ff ff ff" || message=="65 1 3 0 ff ff ff" || message=="65 3 3 0 ff ff ff") {
    myNextion.sendCommand("page Main");
    mainMenu=true;
  }
/////////////////////
  if (message=="65 2 9 0 ff ff ff" || message=="65 0 2 0 ff ff ff") {
    myNextion.sendCommand("page Timers1");
    mainMenu=false;
    updTimers1();
  }
  if (message=="65 1 1 0 ff ff ff") {
    myNextion.sendCommand("page Timers2");
    mainMenu=false;
    updTimers2();
  }
  if (message=="65 0 1 0 ff ff ff") {
    mainMenu=false;
    myNextion.sendCommand("page Manual");
  }
  if (message=="65 1 2 0 ff ff ff" || message=="65 2 a 0 ff ff ff" || message=="65 6 5 0 ff ff ff") {
    mainMenu=false;
    myNextion.sendCommand("page Saved");
  }
  if (message=="65 5 2 0 ff ff ff") {
    memWrite();
    myNextion.sendCommand("page Main");
    mainMenu=true;
  }
  
  if (message=="65 0 4 0 ff ff ff" || message=="65 0 5 0 ff ff ff") {
    myNextion.sendCommand("page Thermostat");
    mainMenu=false;
    updThermostat();
  }

/////////////////////////
// SET VALUES ///////////
/// Thermostat ///
  if (message=="65 6 2 0 ff ff ff") {
    setvTemp++;
    myNextion.setComponentText("setTemp", String(setvTemp));
  }
  if (message=="65 6 1 0 ff ff ff") {
    setvTemp--;
    myNextion.setComponentText("setTemp", String(setvTemp));
  }
  if (message=="65 6 4 0 ff ff ff") {
    setfTemp+=10;
    myNextion.setComponentText("setFire", String(setfTemp));
  }
  if (message=="65 6 3 0 ff ff ff") {
    setfTemp-=10;
    myNextion.setComponentText("setFire", String(setfTemp));
  }

/// Fütés start ///
  if (message=="65 1 6 0 ff ff ff") {
    fStart++;
    myNextion.setComponentText("fStartNr", String(fStart));
  }
  if (message=="65 1 7 0 ff ff ff") {
    fStart+=5;
    myNextion.setComponentText("fStartNr", String(fStart));
  }
  if (message=="65 1 4 0 ff ff ff") {
    fStart--;
    myNextion.setComponentText("fStartNr", String(fStart));
  }
  if (message=="65 1 5 0 ff ff ff") {
    fStart-=5;
    myNextion.setComponentText("fStartNr", String(fStart));
  }

/// Fütés stop ///
  if (message=="65 1 a 0 ff ff ff") {
    fStop++;
    myNextion.setComponentText("fStopNr", String(fStop));
  }
  if (message=="65 1 b 0 ff ff ff") {
    fStop+=5;
    myNextion.setComponentText("fStopNr", String(fStop));
  }
  if (message=="65 1 8 0 ff ff ff") {
    fStop--;
    myNextion.setComponentText("fStopNr", String(fStop));
  }
  if (message=="65 1 9 0 ff ff ff") {
    fStop-=5;
    myNextion.setComponentText("fStopNr", String(fStop));
  }

/// Tartás start ///
  if (message=="65 2 4 0 ff ff ff") {
    tStart++;
    myNextion.setComponentText("tStartNr", String(tStart));
  }
  if (message=="65 2 3 0 ff ff ff") {
    tStart+=5;
    myNextion.setComponentText("tStartNr", String(tStart));
  }
  if (message=="65 2 1 0 ff ff ff") {
    tStart--;
    myNextion.setComponentText("tStartNr", String(tStart));
  }
  if (message=="65 2 2 0 ff ff ff") {
    tStart-=5;
    myNextion.setComponentText("tStartNr", String(tStart));
  }

/// Tartás stop ///
  if (message=="65 2 8 0 ff ff ff") {
    tStop++;
    myNextion.setComponentText("tStopNr", String(tStop));
  }
  if (message=="65 2 7 0 ff ff ff") {
    tStop+=5;
    myNextion.setComponentText("tStopNr", String(tStop));
  }
  if (message=="65 2 6 0 ff ff ff") {
    tStop--;
    myNextion.setComponentText("tStopNr", String(tStop));
  }
  if (message=="65 2 5 0 ff ff ff") {
    tStop-=5;
    myNextion.setComponentText("tStopNr", String(tStop));
  }
/// Settings page ///
  if (message=="65 4 6 0 ff ff ff") {
    if (thermostat==true) {
      thermostat=false;
    } else {
      thermostat=true;
    }
  updSettings();
  }
  
  if (message=="65 4 7 0 ff ff ff") {
    if (debug==true) {
      debug=false;
    } else {
      debug=true;
    }
  updSettings();
  }

  if (message=="65 4 a 0 ff ff ff") {
    hiszter++;
    myNextion.setComponentText("hiszt", String(hiszter));
  }
  if (message=="65 4 8 0 ff ff ff") {
    hiszter--;
    myNextion.setComponentText("hiszt", String(hiszter));
  }
  if (message=="65 4 b 0 ff ff ff") {
    fanDelay++;
    myNextion.setComponentText("levK", String(fanDelay));
  }
  if (message=="65 4 9 0 ff ff ff") {
    fanDelay--;
    myNextion.setComponentText("levK", String(fanDelay));
  }
/// Profile change ///
  if (message=="65 4 3 0 ff ff ff") {
    profile = 1;
    EEPROM.write(0, profile);
    EEPROM.commit();
    memRead();
    myNextion.sendCommand("page Main");
    mainMenu=true;
  }
  if (message=="65 4 4 0 ff ff ff") {
    profile = 2;
    EEPROM.write(0, profile);
    EEPROM.commit();
    memRead();
    myNextion.sendCommand("page Main");
    mainMenu=true;
  }
  if (message=="65 4 5 0 ff ff ff") {
    profile = 3;
    EEPROM.write(0, profile);
    EEPROM.commit();
    memRead();
    myNextion.sendCommand("page Main");
    mainMenu=true;
  }
//////////////////////
}

void memWrite () {
  Serial.println("SAVE TO EEPROM");
  /// Save data ///
  switch (profile) {
    case 1:
      EEPROM.write(1, fStart);
      EEPROM.write(2, fStop);
      EEPROM.write(3, tStart);
      EEPROM.write(4, tStop);
      EEPROM.write(5, hiszter);
      EEPROM.write(6, fanDelay);
      EEPROM.write(19, setvTemp);
      EEPROM.commit();
      break;

    case 2:
      EEPROM.write(7, fStart);
      EEPROM.write(8, fStop);
      EEPROM.write(9, tStart);
      EEPROM.write(10, tStop);
      EEPROM.write(11, hiszter);
      EEPROM.write(12, fanDelay);
      EEPROM.write(21, setvTemp);
      EEPROM.commit();
      break;

    case 3:
      EEPROM.write(13, fStart);
      EEPROM.write(14, fStop);
      EEPROM.write(15, tStart);
      EEPROM.write(16, tStop);
      EEPROM.write(17, hiszter);
      EEPROM.write(18, fanDelay);
      EEPROM.write(23, setvTemp);
      EEPROM.commit();
      break;
  }
}

void memRead () {
  /// Read data ///
  switch (profile) {
    case 1:
      fStart = EEPROM.read(1);
      fStop = EEPROM.read(2);
      tStart = EEPROM.read(3);
      tStop = EEPROM.read(4);
      hiszter = EEPROM.read(5);
      fanDelay = EEPROM.read(6);  
      setvTemp = EEPROM.read(19);    
      break;
      
    case 2:
      fStart = EEPROM.read(7);
      fStop = EEPROM.read(8);
      tStart = EEPROM.read(9);
      tStop = EEPROM.read(10);
      hiszter = EEPROM.read(11);
      fanDelay = EEPROM.read(12);   
      setvTemp = EEPROM.read(21);   
      break;
      
    case 3:
      fStart = EEPROM.read(13);
      fStop = EEPROM.read(14);
      tStart = EEPROM.read(15);
      tStop = EEPROM.read(16);
      hiszter = EEPROM.read(17);
      fanDelay = EEPROM.read(18);     
      setvTemp = EEPROM.read(23); 
      break;
  }
}

void updVar() {

  if (reqHeat == true) { 
    OnTime = fStart * egyezer;
    OffTime = fStop * egyezer;    
  } else {
    OnTime = tStart * egyezer;
    OffTime = tStop * hatvanezer;   //ez percben van
  }
}

//////////////////////////ÜZEMMÓD VÁLTÁS - BEMENETI JEL//////////////////////////
void readInput() {
  if (thermostat == false) {         // Bemenet vezérelt üzemmód

    if (digitalRead(heatPin) == LOW) {       // Ha alacsony akkor fűtűnk
      delay(50);
      if (digitalRead(heatPin) == LOW) {
        reqHeat = true;
        digitalWrite(fanPin, LOW);         // Ventillátor bekapcsolása
      } else {
        reqHeat = false;
        digitalWrite(fanPin, HIGH);        // Ventillátor kikapcsolása
      }
    }
  } else if (thermostat == true) {   // Thermostat vezérelt üzemmód
    //valami
    if (tempC < setvTemp - hiszter && digitalRead(heatPin) == LOW) {
      reqHeat = true;
      digitalWrite(fanPin, LOW);         // Ventillátor bekapcsolása
    }
    if (tempC >= setvTemp || digitalRead(heatPin) == HIGH) {
      reqHeat = false;
      digitalWrite(fanPin, HIGH);         // Ventillátor kikapcsolása
    }
  }
}
