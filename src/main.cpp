#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SSLClient.h>
#include "trust_anchors.h"
#include <input.h>
#include <output.h>
#include <database.h>

void setup() {
    delay(3000);
    
    Serial.begin(115200);
    input_setup();
    output_setup();
    database_setup();
}

void loop() {
    input_loop();
    output_loop();
    database_loop();
}