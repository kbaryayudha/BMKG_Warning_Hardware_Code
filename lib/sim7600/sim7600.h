#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <SoftwareSerial.h>

#define DEBUG true

SoftwareSerial SIM7600(2,3);

const char* bmkg_server = "ntp.bmkg.go.id";
const long gmtOffset_sec = 25200;
const int daylightOffset_sec = 0;

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  SIM7600.println(command);
  
  long int time = millis();
  while ( (time + timeout) > millis()) {
    while (SIM7600.available()) {
      char c = SIM7600.read();
      response += c;
    }
  }
  if (debug) {
    Serial.print(response);
    delay(1000);
  }
  return response;
}

void sim7600_http_post() {  
    struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.print("Not Connected To BMKG Time Server");
    delay(1000);
    return;
  }

  String time = (&timeinfo, "%d/%m/%y, %H:%M:%S");

  int ac_voltage = random(200,240);
  int dc_voltage = random(22, 26);
  int ping = random(1, 50);
  int gas = random (200, 400);
  int temp = random (25, 35);
  int rssi = random (10, 50);
  
  String http_str = "AT+HTTPPARA=\"URL\",\"http://13.250.20.205/api/v1/arduino/send-data?location=daerah_istimewa_yogyakarta&tower=tower_underpassYIA&ac_voltage=" + (String) ac_voltage + "&dc_voltage=" + (String) dc_voltage + "&ping=" + (String) ping + "&temp=" + (String) temp + "&gas=" + (String) gas + "&rssi=" + (String) rssi + "&time=" + (String) time + "\"\r\n";
  Serial.println(http_str);

  sendData("AT+CCLK?", 2000, DEBUG);
  sendData("AT+HTTPINIT", 2000, DEBUG);
  sendData(http_str, 2000, DEBUG);
  sendData("AT+HTTPACTION=1", 2000, DEBUG);
  sendData("AT+HTTPREAD=0,250", 2000, DEBUG);
  sendData("AT+HTTPTERM", 2000, DEBUG);
}

void sim7600_http_get() {
  String http_str = "AT+HTTPPARA=\"URL\",\"http://13.250.20.205/api/v1/arduino/get-state?tower=tower_parangtritis\"\r\n";
  Serial.println(http_str);
 
  sendData("AT+HTTPINIT", 2000, DEBUG);
  sendData(http_str, 2000, DEBUG);
  sendData("AT+HTTPACTION=0", 2000, DEBUG);
  sendData("AT+HTTPREAD=0,200", 2000, DEBUG);
  sendData("AT+HTTPTERM", 2000, DEBUG);
}

void sim7600_setup() {
  Serial.begin(9600);
  SIM7600.begin(9600);
 
  sendData("AT+CICCID", 3000, DEBUG);
  sendData("AT+CREG?", 3000, DEBUG);
  sendData("AT+CGATT=1", 1000, DEBUG);
  sendData("AT+CGACT=1,1", 1000, DEBUG);
  sendData("AT+CGDCONT=1,\"IP\",\"3data\"", 1000, DEBUG);
  
  configTime(gmtOffset_sec, daylightOffset_sec, bmkg_server);

  Serial.println("4G HTTP Test Begin!");
 
  delay(1000);
}
 
void sim7600_loop() {  
  sim7600_http_post();
}