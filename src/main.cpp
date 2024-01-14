#include <Arduino.h>
// #include <siren.h>
#include <wifi_connection.h>
// #include <server_connection.h>
// #include <voltmeter.h>
// #include <temp.h>
// #include <gas.h>
// #include <firebase.h>
// #include <ethernet_connection.h>
#include <sim7600.h>
// #include <bmkg_time.h>
// #include <inverter.h>

unsigned long previousMillis;

void setup() {
  // wifi_connection_setup();
  // server_connection_setup();
  // voltmeter_setup();
  // temp_setup();
  // gas_setup();
  // firebase_setup();

  // pinMode(BUILTIN_LED,OUTPUT);
  // wifi_connection_setup();
  // siren_setup();
  // firebase_setup();

  // ethernet_connection_setup();
  sim7600_setup();
}

void loop() {
  // server_connection_loop();
  // voltmeter_loop();
  // temp_loop();
  // ethernet_connection_loop();
  // if(Firebase.ready() && credentialOK) {
  //   digitalWrite(BUILTIN_LED,HIGH);
  //   if(millis() - previousMillis > 10000 || previousMillis == 0) {
  //     previousMillis = millis();
  //     server_connection_loop();
  //     voltmeter_loop();
  //     temp_loop();
  //     // gas_loop();    
  //     // firebase_write_loop();  
  //   }
  // } else {
  //   digitalWrite(BUILTIN_LED,LOW);
  // }
  
  // if(Firebase.ready() && credentialOK) {
  //   digitalWrite(BUILTIN_LED,HIGH);
  //   firebase_read_loop();
  // } else {
  //   digitalWrite(BUILTIN_LED,LOW);
  // }

  // ethernet_connection_loop();
  //sim7600_loop();
}