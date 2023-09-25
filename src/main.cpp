#include <Arduino.h>
#include <SPI.h>
#include <input.h>
#include <output.h>
#include <database.h>

void setup() {
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