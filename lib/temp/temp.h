#include <OneWire.h>
#include <DallasTemperature.h>

#define fan 26

OneWire ds18b20(33);
DallasTemperature temp(&ds18b20);

float temp_value, raw_temp_value;

void temp_setup() {
    temp.begin();
    pinMode(fan, OUTPUT);
}

void temp_loop() {
    temp.requestTemperatures();
    raw_temp_value = temp.getTempCByIndex(0);

    if(raw_temp_value != -127 && raw_temp_value != 85) {
        temp_value = raw_temp_value;
    } else {
        temp_value = temp_value;
    }

    if(temp_value >= 25.0) {
        digitalWrite(fan,HIGH);
    } else {
        digitalWrite(fan,LOW);
    }

    // Serial.print("Temp : ");
    // Serial.print(raw_temp_value);
    // Serial.println("°C");

    // delay(2000);
}