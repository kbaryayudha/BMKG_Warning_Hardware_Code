#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyDDIJw4IGELpeVDw87izacw_R9jimiT3CE"
#define DATABASE_URL "https://bmkg-warning-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "bmkg.kygi@gmail.com"
#define USER_PASSWORD "stageofsleman"

FirebaseData fbdo, fbdo_s1, fbdo_s2, fbdo_s3;
FirebaseAuth auth;
FirebaseConfig config;

bool credentialOK = false;
bool real_status = false, voice_status = false, test_status = false;

void firebase_setup() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  if(Firebase.signUp(&config, &auth, "bmkg.kygi@gmail.com", "stageofsleman")) {
    Serial.println("Sign up successfully");
    credentialOK = true;
  } else if(config.signer.signupError.message == "EMAIL_EXISTS") {
    Serial.println("Sign in successfully");
    credentialOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;

  Firebase.begin(&config, &auth);
  
  // if(!Firebase.RTDB.beginStream(&fbdo_s1, "siren_activator/daerah_istimewa_yogyakarta/tower_underpassYIA/real")) Serial.printf("Stream 1 begin error, %s\n\n", fbdo_s1.errorReason().c_str());
  // if(!Firebase.RTDB.beginStream(&fbdo_s2, "siren_activator/daerah_istimewa_yogyakarta/tower_underpassYIA/voice")) Serial.printf("Stream 2 begin error, %s\n\n", fbdo_s2.errorReason().c_str());
  // if(!Firebase.RTDB.beginStream(&fbdo_s3, "siren_activator/daerah_istimewa_yogyakarta/tower_underpassYIA/test")) Serial.printf("Stream 3 begin error, %s\n\n", fbdo_s3.errorReason().c_str());
}

void firebase_write_loop() {
    Serial.println();

    if (Firebase.isTokenExpired()){
      Firebase.refreshToken(&config);
      Serial.println("Refresh token\n");
    }

    if(Firebase.RTDB.setFloat(&fbdo, "siren_monitor/daerah_istimewa_yogyakarta/tower_underpassYIA/temp", temp_value)) {
      Serial.print(temp_value);
      Serial.print(" - successfully saved to : ");
      Serial.print(fbdo.dataPath());
      Serial.print(" (");
      Serial.print(fbdo.dataType());
      Serial.println(")");
    } else {
      Serial.print("FAILED : ");
      Serial.println(fbdo.errorReason());
    }

    if(Firebase.RTDB.setInt(&fbdo, "siren_monitor/daerah_istimewa_yogyakarta/tower_underpassYIA/gas", analog_gas_value)) {
      Serial.print(analog_gas_value);
      Serial.print(" - successfully saved to : ");
      Serial.print(fbdo.dataPath());
      Serial.print(" (");
      Serial.print(fbdo.dataType());
      Serial.println(")");
    } else {
      Serial.print("FAILED : ");
      Serial.println(fbdo.errorReason());
    }

    if(Firebase.RTDB.setInt(&fbdo, "siren_monitor/daerah_istimewa_yogyakarta/tower_underpassYIA/rssi", rssi_value)) {
      Serial.print(rssi_value);
      Serial.print(" - successfully saved to : ");
      Serial.print(fbdo.dataPath());
      Serial.print(" (");
      Serial.print(fbdo.dataType());
      Serial.println(")");
    } else {
      Serial.print("FAILED : ");
      Serial.println(fbdo.errorReason());
    }    

    if(Firebase.RTDB.setFloat(&fbdo, "siren_monitor/daerah_istimewa_yogyakarta/tower_underpassYIA/dc_voltage", dc_voltage_value)) {
      Serial.print(dc_voltage_value);
      Serial.print(" - successfully saved to : ");
      Serial.print(fbdo.dataPath());
      Serial.print(" (");
      Serial.print(fbdo.dataType());
      Serial.println(")");
    } else {
      Serial.print("FAILED : ");
      Serial.println(fbdo.errorReason());
    }

    if(Firebase.RTDB.setInt(&fbdo, "siren_monitor/daerah_istimewa_yogyakarta/tower_underpassYIA/ac_voltage", ac_voltage_value)) {
      Serial.print(ac_voltage_value);
      Serial.print(" - successfully saved to : ");
      Serial.print(fbdo.dataPath());
      Serial.print(" (");
      Serial.print(fbdo.dataType());
      Serial.println(")");
    } else {
      Serial.print("FAILED : ");
      Serial.println(fbdo.errorReason());
    }

    if(Firebase.RTDB.setFloat(&fbdo, "siren_monitor/daerah_istimewa_yogyakarta/tower_underpassYIA/ping", ping_value)) {
      Serial.print(ping_value);
      Serial.print(" - successfully saved to : ");
      Serial.print(fbdo.dataPath());
      Serial.print(" (");
      Serial.print(fbdo.dataType());
      Serial.println(")");
    } else {
      Serial.print("FAILED : ");
      Serial.println(fbdo.errorReason());
    }
}

void firebase_read_loop() {
  if(Firebase.isTokenExpired()){
    Firebase.refreshToken(&config);
    Serial.println("Refresh token\n");
  }  

  if(!Firebase.RTDB.readStream(&fbdo_s1)) Serial.printf("Stream 1 read error, %s\n\n", fbdo_s1.errorReason().c_str());
    if(fbdo_s1.streamAvailable()) {
      if(fbdo_s1.dataType() == "boolean") {
        real_status = fbdo_s1.boolData();
        Serial.print("Successful READ from ");
        Serial.print(fbdo_s1.dataPath());
        Serial.print(" : ");
        Serial.print(real_status);
        Serial.print(" (");
        Serial.print(fbdo_s1.dataType());
        Serial.println(")");
        if(real_status==true) {
          // myDFPlayer.play(3);
        } else {
          // myDFPlayer.pause();
        }  
      }
    }

  // if(!Firebase.RTDB.readStream(&fbdo_s2)) Serial.printf("Stream 2 read error, %s\n\n", fbdo_s2.errorReason().c_str());
  //   if(fbdo_s2.streamAvailable()) {
  //     if(fbdo_s2.dataType() == "boolean") {
  //       voice_status = fbdo_s2.boolData();
  //       Serial.print("Successful READ from ");
  //       Serial.print(fbdo_s2.dataPath());
  //       Serial.print(" : ");
  //       Serial.print(voice_status);
  //       Serial.print(" (");
  //       Serial.print(fbdo_s2.dataType());
  //       Serial.println(")");
  //     }
  //   }    

  if(!Firebase.RTDB.readStream(&fbdo_s3)) Serial.printf("Stream 3 read error, %s\n\n", fbdo_s3.errorReason().c_str());
    if(fbdo_s3.streamAvailable()) {
      if(fbdo_s3.dataType() == "boolean") {
        test_status = fbdo_s3.boolData();
        Serial.print("Successful READ from ");
        Serial.print(fbdo_s3.dataPath());
        Serial.print(" : ");
        Serial.print(test_status);
        Serial.print(" (");
        Serial.print(fbdo_s3.dataType());
        Serial.println(")");
        if(test_status==true) {
          // myDFPlayer.play(2);
        } 
        if(test_status==false) {
          // myDFPlayer.pause();
        }
      } 
    }
}