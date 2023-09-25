#define a0 34
#define a1 35
#define d0 16
#define d1 17

String a0_value, a1_value, d0_value, d1_value;

void input_setup() {
    pinMode(a0,INPUT);
    pinMode(a1,INPUT);
    pinMode(d0,INPUT);
    pinMode(d1,INPUT);
}

void input_loop() {
    if(analogRead(a0)>=3072) {
        a0_value = "true";
    } else {
        a0_value = "false";
    }

    if(analogRead(a1)>=3072) {
        a1_value = "true";
    } else {
        a1_value = "false";
    }

    if(digitalRead(d0)==1) {
        d0_value = "true";
    } else {
        d0_value = "false";
    }

    if(digitalRead(d1)==1) {
        d1_value = "true";
    } else {
        d1_value = "false";
    }
}