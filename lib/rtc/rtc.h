#include <RTClib.h>
#include <time.h>

RTC_DS1307 DS1307_RTC;

int rtcday, rtcmonth, rtcyear, rtchour, rtcminute, rtcsecond;

void rtc_setup() {
    if(DS1307_RTC.begin()) {
        Serial.println("RTC is connected");
    }

    while(!DS1307_RTC.begin()) {
        Serial.println("RTC is not connected");
        delay(0);
        if(DS1307_RTC.begin()) {
            Serial.println("RTC is connected");
            break;
        }
    }

    // DS1307_RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));

    if(year()>=2023) DS1307_RTC.adjust(DateTime(year(), month(), day(), hour(), minute(), second()));
}

void rtc_loop() {
    DateTime now = DS1307_RTC.now();

    rtcday = now.day();
    rtcmonth = now.month();
    rtcyear = now.year();
    rtchour = now.hour();
    rtcminute = now.minute();
    rtcsecond = now.second();

    if(year()>=2023 && rtcsecond!=second()) DS1307_RTC.adjust(DateTime(year(), month(), day(), hour(), minute(), second()));

    // Serial.print(now.day(), DEC);
    // Serial.print('/');
    // Serial.print(now.month(), DEC);
    // Serial.print('/');
    // Serial.print(now.year(), DEC);
    // Serial.print(' ');
    // Serial.print(now.hour(), DEC);
    // Serial.print(':');
    // Serial.print(now.minute(), DEC);
    // Serial.print(':');
    // Serial.println(now.second(), DEC);
}