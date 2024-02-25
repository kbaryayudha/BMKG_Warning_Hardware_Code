#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

SoftwareSerial software_serial(25, 26);
DFRobotDFPlayerMini DFPlayer;

void siren_setup() {
    software_serial.begin(9600);
    
    if(DFPlayer.begin(software_serial)) {
        Serial.println("DFPLayer is connected");
    }

    while(!DFPlayer.begin(software_serial)) {
        Serial.println("DFPLayer is not connected");
        delay(0);
        if(DFPlayer.begin(software_serial)) {
            Serial.println("DFPLayer is connected");
            break;
        }
    }

    DFPlayer.volume(30);
}