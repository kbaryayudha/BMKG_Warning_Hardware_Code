#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

SoftwareSerial mySoftwareSerial(25, 26);
DFRobotDFPlayerMini myDFPlayer;

void siren_setup() {
  mySoftwareSerial.begin(9600);

  if(!myDFPlayer.begin(mySoftwareSerial)) {
    // lcd.setCursor(0,0);
    // lcd.print("Not Connected To");
    // lcd.setCursor(0,1);
    // lcd.print("MP3 Player");
    Serial.println("DFPLayer is not connected");
    delay(3000);
    // lcd.clear();    
  } else {
    // lcd.setCursor(0,0);
    // lcd.print("Connected To");
    // lcd.setCursor(0,1);
    // lcd.print("MP3 Player");
    Serial.println("DFPLayer is connected");
    delay(3000);
    // lcd.clear();    
  }

  myDFPlayer.volume(25);
}