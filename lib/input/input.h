#define test 21
#define real 22

String test_value, real_value;
int test_in, real_in;

void input_setup() {
    pinMode(test,INPUT);
    pinMode(real,INPUT);
}

void input_loop() {
    test_in = digitalRead(test);
    real_in = digitalRead(real);

    if(test_in==1) {
        test_value = "true";
        // Serial.println("TEST");
    } else {
        test_value = "false";
    }
   
    if(real_in==1) {
        real_value = "true";
        // Serial.println("REAL");
    } else {
        real_value = "false";
    }
}