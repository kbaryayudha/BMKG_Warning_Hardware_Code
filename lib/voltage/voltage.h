#define primary_voltage 35
#define secondary_voltage 34
#define accu_voltage 15

float primary_voltage_value, secondary_voltage_value, accu_voltage_value;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void voltage_setup() {
    pinMode(primary_voltage,INPUT);
    pinMode(secondary_voltage,INPUT);
    pinMode(accu_voltage,INPUT);
}

void voltage_loop() {
    if(analogRead(primary_voltage)>512) {
        primary_voltage_value = mapfloat(analogRead(primary_voltage),0,4095,0,13.8);
    } else {
        primary_voltage_value = 0;
    }

    if(analogRead(secondary_voltage)>512) {
        secondary_voltage_value = mapfloat(analogRead(secondary_voltage),0,4095,0,13.8);
    } else {
        secondary_voltage_value = 0;
    }
    
    if(analogRead(accu_voltage)>512) {
        accu_voltage_value = mapfloat(analogRead(accu_voltage),0,4095,0,28);
    } else {
        accu_voltage_value = 0;
    }
}