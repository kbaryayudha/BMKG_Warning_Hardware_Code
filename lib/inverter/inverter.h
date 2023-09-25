#include <Arduino.h>

#define trig 23
#define inv 19

void inverter_setup() {
    pinMode(trig,INPUT);
    pinMode(inv, INPUT);
}

void inverter_loop() {
    // lcd.setCursor(0,0);
    // lcd.print("Transistor : ");
    // if(digitalRead(trig)==1) {
    //     lcd.print("ON");
    // } else if(digitalRead(trig)==0){
    //     lcd.print("OFF");
    // } 
    // lcd.setCursor(0,1);
    // lcd.print("Inverter : ");
    // if(digitalRead(inv)==1) {
    //     lcd.print("ON");
    // } else {
    //     lcd.print("OFF");
    // }
}