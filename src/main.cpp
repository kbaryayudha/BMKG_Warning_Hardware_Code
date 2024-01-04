#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SSLClient.h>
#include "trust_anchors.h"
#include <bmkg_time.h>
#include <rtc.h>
#include <temp.h>
#include <voltage.h>
#include <siren.h>
#include <database.h>

void setup() {
    delay(3000);
    
    Serial.begin(115200);
    bmkg_time_setup();
    rtc_setup();
    temp_setup();
    voltage_setup();
    siren_setup();
    database_setup();
}

void loop() {
    bmkg_time_loop();
    rtc_loop();
    temp_loop();
    voltage_loop();
    database_loop();
}