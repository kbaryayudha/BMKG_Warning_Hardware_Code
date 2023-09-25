#define rly_1 21
#define rly_2 22
#define rly_3 25
#define rly_4 26

void output_setup() {
    pinMode(rly_1,OUTPUT);
    pinMode(rly_2,OUTPUT);
    pinMode(rly_3,OUTPUT);
    pinMode(rly_4,OUTPUT);

    digitalWrite(rly_1,LOW);
    digitalWrite(rly_2,LOW);
    digitalWrite(rly_3,LOW);
    digitalWrite(rly_4,LOW);
}

void output_loop() {

}