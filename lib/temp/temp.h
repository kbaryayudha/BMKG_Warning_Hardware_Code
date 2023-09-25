#include <DallasTemperature.h>

#define fan 16

OneWire ds18b20(27);
DallasTemperature temp(&ds18b20);

float temp_value;

void temp_setup() {
    temp.begin();
    pinMode(fan, OUTPUT);
}

void temp_loop() {
    temp.requestTemperatures();
    temp_value = temp.getTempCByIndex(0);

    if(temp_value >= 25.0) {
        digitalWrite(fan,HIGH);
    } else {
        digitalWrite(fan,LOW);
    }

    // Serial.print("Temp : ");
    // Serial.print(temp_value);
    // Serial.println("°C");
}