#include <Arduino.h>
#include <esp_task_wdt.h>
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
    esp_task_wdt_init(120, true);
    esp_task_wdt_add(NULL);
    bmkg_time_setup();
    rtc_setup();
    temp_setup();
    voltage_setup();
    siren_setup();
    database_setup();
}

void loop() {
    esp_task_wdt_reset();
    bmkg_time_loop();
    rtc_loop();
    temp_loop();
    voltage_loop();
    database_loop();
}