#include <Arduino.h>
#include <esp_task_wdt.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SSLClient.h>
#include "trust_anchors.h"
#include <bmkg_time.h>
#include <input.h>
#include <output.h>
#include <database.h>

void setup() {
    delay(3000);
    
    Serial.begin(115200);
    esp_task_wdt_init(120, true);
    esp_task_wdt_add(NULL);
    bmkg_time_setup();
    input_setup();
    output_setup();
    database_setup();
}

void loop() {
    esp_task_wdt_reset();
    bmkg_time_loop();
    input_loop();
    output_loop();
    database_loop();
}