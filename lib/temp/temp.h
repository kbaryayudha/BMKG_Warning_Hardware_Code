#include <Arduino.h>
#include <DallasTemperature.h>

#define fan 26

OneWire ds18b20(33);
DallasTemperature temp(&ds18b20);

float temp_value;

void temp_setup() {
    temp.begin();
    pinMode(fan, OUTPUT);
}

void temp_loop() {
    temp.requestTemperatures();
    temp_value = temp.getTempCByIndex(0);

    if(temp_value >= 35.0) {
        digitalWrite(fan,LOW);
    } else {
        digitalWrite(fan,HIGH);
    }
}