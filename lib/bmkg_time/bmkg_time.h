#include <Arduino.h>

const char* bmkg_server = "ntp.bmkg.go.id";
const long gmtOffset_sec = 25200;
const int daylightOffset_sec = 0;

void bmkg_time_setup() {
  configTime(gmtOffset_sec, daylightOffset_sec, bmkg_server);
}

void bmkg_time_loop() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    // lcd.setCursor(0,0);
    // lcd.print("Not Connected To");
    // lcd.setCursor(0,1);
    // lcd.print("BMKG Time Server");
    delay(1000);
    // lcd.clear();
    return;
  }
  // lcd.setCursor(0,0);
  // switch(timeinfo.tm_wday) {
  //   case 0: lcd.print(&timeinfo, "Minggu, %d/%m/%y"); break;
  //   case 1: lcd.print(&timeinfo, "Senin, %d/%m/%y"); break;
  //   case 2: lcd.print(&timeinfo, "Selasa, %d/%m/%y"); break;
  //   case 3: lcd.print(&timeinfo, "Rabu, %d/%m/%y"); break;
  //   case 4: lcd.print(&timeinfo, "Kamis, %d/%m/%y"); break;
  //   case 5: lcd.print(&timeinfo, "Jumat, %d/%m/%y"); break;
  //   case 6: lcd.print(&timeinfo, "Sabtu, %d/%m/%y"); break;
  // }
  // lcd.setCursor(0,1);
  // lcd.print(&timeinfo, "%H:%M:%S WIB");
}