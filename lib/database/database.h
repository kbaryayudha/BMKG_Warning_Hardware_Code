#include <ArduinoJson.h>
#include <StreamUtils.h>

char server[] = "semarsiren.id";

#define inet 2

const int rand_pin = 36;

EthernetClient base_client;
SSLClient client(base_client, TAs, (size_t)TAs_NUM, rand_pin);

unsigned long beginMicros, endMicros, byteCount = 0, currentMillis, previousMillis = 0;
bool printWebData = true, i = false;
float seconds, rate;
int len;
bool reset = false;

void database_setup() {
    pinMode(inet,OUTPUT);
    
    client.connect(server,443);

    if(client.connected()) {
        Serial.println("Connected to Server");
    } else {
        Serial.println("Not Connected to Server!");
        ESP.restart();
    }

    Serial.println();

    beginMicros = micros();
}

void database_loop() {
    len = client.available();
    if(len > 0) {
        byte buffer[3000];
        if(len > 3000) {
            len = 3000;
        }
        client.read(buffer, len);
        if(printWebData) {
            Serial.write(buffer, len);
        }
        byteCount = byteCount + len;
    }

    currentMillis = millis();
    if(client.connected()) {
        if(currentMillis - previousMillis >= 1000) {
            previousMillis = currentMillis;
            if(i==false) {
                digitalWrite(inet,HIGH);
                client.println("GET /api/v1/esp32/siren-activator?province=sumatra_utara&site=mobile_tapanuliTengah HTTP/1.1");
                client.println("Host: semarsiren.id");
                client.println("Connection: close");
                if(client.println() == 0) {
                    Serial.println("Failed to send request");
                    client.stop();
                    return;
                }
                client.println();
                
                char status[32] = {0};
                client.readBytesUntil('\r', status, sizeof(status));
                if(strcmp(status + 9, "200 OK") != 0) {
                    Serial.print("Unexpected response: ");
                    Serial.println(status);
                    client.stop();
                    return;
                }

                char endOfHeaders[] = "\r\n\r\n";
                if(!client.find(endOfHeaders)) {
                    Serial.println("Invalid response");
                    client.stop();
                    return;
                }

                JsonDocument doc;
                ReadBufferingStream bufferedFile(client, 64);
                DeserializationError error = deserializeJson(doc, bufferedFile);

                if(error) {
                    Serial.print("deserializeJson() failed: ");
                    Serial.println(error.c_str());
                    client.stop();
                    return;
                }

                int status_code = doc["status_code"];
                const char* message = doc["message"];

                JsonObject data_sumatra_utara_mobile_tapanuliTengah = doc["data"]["sumatra_utara"]["mobile_tapanuliTengah"];
                bool data_sumatra_utara_mobile_tapanuliTengah_test = data_sumatra_utara_mobile_tapanuliTengah["test"];
                bool data_sumatra_utara_mobile_tapanuliTengah_real = data_sumatra_utara_mobile_tapanuliTengah["real"];
                bool data_sumatra_utara_mobile_tapanuliTengah_voice = data_sumatra_utara_mobile_tapanuliTengah["voice"];
                const char* data_sumatra_utara_mobile_tapanuliTengah_date_time = data_sumatra_utara_mobile_tapanuliTengah["date_time"];
                bool data_sumatra_utara_mobile_tapanuliTengah_on = data_sumatra_utara_mobile_tapanuliTengah["on"];
                
                Serial.println("Siren Activator");
                Serial.print("test  : ");
                Serial.println(data_sumatra_utara_mobile_tapanuliTengah_test);
                Serial.print("real  : ");
                Serial.println(data_sumatra_utara_mobile_tapanuliTengah_real);
                Serial.print("voice : ");
                Serial.println(data_sumatra_utara_mobile_tapanuliTengah_voice);
                Serial.print("date and time : ");
                Serial.println(data_sumatra_utara_mobile_tapanuliTengah_date_time);
                Serial.print("status : ");
                Serial.println(data_sumatra_utara_mobile_tapanuliTengah_on);
                Serial.println();

                client.stop();
                delay(1);
                client.connect(server, 443);
                i = true;
            } else {
                if(reset==true && test_value=="true" && real_value=="false") {
                    digitalWrite(inet,HIGH);
                    client.println("POST /api/v1/esp32/change-state?province=sumatra_utara&site=mobile_tapanuliTengah&test=true&real=false HTTP/1.1");
                    client.println("Host: semarsiren.id");
                    client.println("Connection: close");
                    client.println();
                    reset = false;
                } else if(reset==true && test_value=="false" && real_value=="true") {
                    digitalWrite(inet,HIGH);
                    client.println("POST /api/v1/esp32/change-state?province=sumatra_utara&site=mobile_tapanuliTengah&test=false&real=true HTTP/1.1");
                    client.println("Host: semarsiren.id");
                    client.println("Connection: close");
                    client.println();
                    reset = false;
                } else if(reset==false && test_value=="false" && real_value=="false") {
                    digitalWrite(inet,HIGH);
                    client.println("POST /api/v1/esp32/change-state?province=sumatra_utara&site=mobile_tapanuliTengah&test=false&real=false HTTP/1.1");
                    client.println("Host: semarsiren.id");
                    client.println("Connection: close");
                    client.println();
                    reset = true;
                } else {
                    digitalWrite(inet,LOW);
                }
                i = false;
            }
        }
    } else {
        digitalWrite(inet,LOW);
        endMicros = micros();
        Serial.println();
        Serial.println("disconnecting.");
        client.stop();
        Serial.print("Received ");
        Serial.print(byteCount);
        Serial.print(" bytes in ");
        seconds = (float)(endMicros - beginMicros) / 1000000.0;
        Serial.print(seconds, 4);
        rate = (float)byteCount / seconds / 1000.0;
        Serial.print(", rate = ");
        Serial.print(rate);
        Serial.println(" kbytes/second");
        Serial.println();
        delay(1);
        client.connect(server,443);
    }
}