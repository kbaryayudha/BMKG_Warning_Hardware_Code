// #include <Arduino.h>

// #define dc_voltage 35
// #define ac_voltage 34

// int ac_voltage_value;
// float dc_voltage_value;

// void voltmeter_setup() {
//   Serial.begin(115200);
//   pinMode(dc_voltage, INPUT);
//   pinMode(ac_voltage, INPUT);
// }

// float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
//   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }

// void voltmeter_loop() {
//   dc_voltage_value = mapfloat(analogRead(dc_voltage), 0, 2688, 0, 24);
//   ac_voltage_value = map(analogRead(ac_voltage),0,2047,200,240);
// }