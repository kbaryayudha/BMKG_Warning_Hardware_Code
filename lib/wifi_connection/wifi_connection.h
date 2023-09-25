#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

#define WIFI_SSID "BMKG Warning_Underpass YIA"
#define WIFI_PASSWORD "StageofPGR7"
#define buzzer 25

int rssi_value;

void wifi_connection_setup() {
  Serial.begin(115200);

  pinMode(buzzer, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi SSID : ");
  Serial.println(WIFI_SSID);
  delay(3000);
  
  if(WiFi.status()==WL_CONNECTED) {
    Serial.println("Wi-Fi is connected");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
    Serial.print("Mac Address : ");
    Serial.println(WiFi.macAddress());
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
  } 

  while(WiFi.status()!=WL_CONNECTED) {
    Serial.println("Wi-Fi is not connected");
    digitalWrite(buzzer,HIGH);
    WiFi.disconnect();
    WiFi.reconnect();
    delay(3000);

    if(WiFi.status()==WL_CONNECTED) {
      Serial.println("Wi-Fi is connected");
      Serial.print("IP Address : ");
      Serial.println(WiFi.localIP());
      Serial.print("Mac Address : ");
      Serial.println(WiFi.macAddress());
      digitalWrite(buzzer,HIGH);
      delay(1000);
      digitalWrite(buzzer,LOW);
      break;
    } 
  }
}

void wifi_connection_loop() {
  rssi_value = WiFi.RSSI();

  while(WiFi.status()!=WL_CONNECTED) {
    Serial.println("Wi-Fi is reconnecting");
    digitalWrite(buzzer,HIGH);
    WiFi.disconnect();
    WiFi.reconnect();
    delay(3000);
    
    if(WiFi.status()==WL_CONNECTED) {
      Serial.println("Wi-Fi is reconnected");
      Serial.print("IP Address : ");
      Serial.println(WiFi.localIP());
      Serial.print("Mac Address : ");
      Serial.println(WiFi.macAddress());
      digitalWrite(buzzer,HIGH);
      delay(1000);
      digitalWrite(buzzer,LOW);
      break;
    } 
  }
}