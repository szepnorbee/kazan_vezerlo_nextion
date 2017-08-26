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
    updSettings();
  }
  if (message=="65 3 1 0 ff ff ff" || message=="65 1 b 0 ff ff ff" || message=="65 2 b 0 ff ff ff" || message=="65 4 1 0 ff ff ff") {
    myNextion.sendCommand("page Main");
    updMain();
  }
  if (message=="65 0 2 0 ff ff ff" || message=="65 2 9 0 ff ff ff") {
    myNextion.sendCommand("page Timers1");
    updTimers1();
  }
  if (message=="65 1 9 0 ff ff ff") {
    myNextion.sendCommand("page Timers2");
    updTimers2();
  }
  if (message=="65 0 1 0 ff ff ff") {
    myNextion.sendCommand("page Manual");
  }

/////////////////////////
// SET VALUES ///////////

/// Fütés start ///
  if (message=="65 1 4 0 ff ff ff") {
    fStart++;
    myNextion.setComponentText("fStartNr", String(fStart));
  }
  if (message=="65 1 3 0 ff ff ff") {
    fStart+=5;
    myNextion.setComponentText("fStartNr", String(fStart));
  }
  if (message=="65 1 1 0 ff ff ff") {
    fStart--;
    myNextion.setComponentText("fStartNr", String(fStart));
  }
  if (message=="65 1 2 0 ff ff ff") {
    fStart-=5;
    myNextion.setComponentText("fStartNr", String(fStart));
  }

/// Fütés stop ///
  if (message=="65 1 6 0 ff ff ff") {
    fStop++;
    myNextion.setComponentText("fStopNr", String(fStop));
  }
  if (message=="65 1 7 0 ff ff ff") {
    fStop+=5;
    myNextion.setComponentText("fStopNr", String(fStop));
  }
  if (message=="65 1 5 0 ff ff ff") {
    fStop--;
    myNextion.setComponentText("fStopNr", String(fStop));
  }
  if (message=="65 1 8 0 ff ff ff") {
    fStop-=5;
    myNextion.setComponentText("fStopNr", String(fStop));
  }

/// Tartás start ///
  if (message=="65 2 1 0 ff ff ff") {
    tStart++;
    myNextion.setComponentText("tStartNr", String(tStart));
  }
  if (message=="65 2 7 0 ff ff ff") {
    tStart+=5;
    myNextion.setComponentText("tStartNr", String(tStart));
  }
  if (message=="65 2 4 0 ff ff ff") {
    tStart--;
    myNextion.setComponentText("tStartNr", String(tStart));
  }
  if (message=="65 2 6 0 ff ff ff") {
    tStart-=5;
    myNextion.setComponentText("tStartNr", String(tStart));
  }

/// Tartás stop ///
  if (message=="65 2 8 0 ff ff ff") {
    tStop++;
    myNextion.setComponentText("tStopNr", String(tStop));
  }
  if (message=="65 2 5 0 ff ff ff") {
    tStop+=5;
    myNextion.setComponentText("tStopNr", String(tStop));
  }
  if (message=="65 2 2 0 ff ff ff") {
    tStop--;
    myNextion.setComponentText("tStopNr", String(tStop));
  }
  if (message=="65 2 3 0 ff ff ff") {
    tStop-=5;
    myNextion.setComponentText("tStopNr", String(tStop));
  }
/// Settings page ///
  if (message=="65 3 3 0 ff ff ff") {
    if (thermostat==true) {
      thermostat=false;
    } else {
      thermostat=true;
    }
  updSettings();
  }
  
  if (message=="65 3 2 0 ff ff ff") {
    if (debug==true) {
      debug=false;
    } else {
      debug=true;
    }
  updSettings();
  }

  if (message=="65 3 4 0 ff ff ff") {
    hiszter++;
    myNextion.setComponentText("hiszt", String(hiszter));
  }
  if (message=="65 3 7 0 ff ff ff") {
    hiszter--;
    myNextion.setComponentText("hiszt", String(hiszter));
  }
  if (message=="65 3 5 0 ff ff ff") {
    fanDelay++;
    myNextion.setComponentText("levK", String(fanDelay));
  }
  if (message=="65 3 6 0 ff ff ff") {
    fanDelay--;
    myNextion.setComponentText("levK", String(fanDelay));
  }
/// Profile change ///
  if (message=="65 3 d 0 ff ff ff") {
    myNextion.sendCommand("page Main");
  }
  if (message=="65 3 e 0 ff ff ff") {
    myNextion.sendCommand("page Main");
  }
  if (message=="65 3 f 0 ff ff ff") {
    myNextion.sendCommand("page Main");
  }
//////////////////////
}
