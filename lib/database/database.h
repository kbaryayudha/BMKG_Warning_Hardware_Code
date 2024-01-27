#include <ArduinoJson.h>
#include <StreamUtils.h>

char server[] = "semarsiren.id";
// char server[] = "httpbin.org"; 

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
        if(currentMillis - previousMillis >= 10000) {
            previousMillis = currentMillis;
            if(i==false) {
                digitalWrite(inet,HIGH);
                client.println("GET /api/v1/esp32/siren-activator?province=daerah_istimewa_yogyakarta&site=mobile_kulonProgo HTTP/1.0");
                client.println("Host: semarsiren.id");
                // client.println("GET /get HTTP/1.1");
                // client.println("Host: httpbin.org");
                client.println("Connection: close");
                client.println();
                
                char status[32] = {0};
                client.readBytesUntil('\r', status, sizeof(status));
                if(strcmp(status, "HTTP/1.1 200 OK") != 0) {
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

                StaticJsonDocument<384> doc;

                ReadBufferingStream bufferedFile(client, 64);
                DeserializationError error = deserializeJson(doc, bufferedFile);

                if(error) {
                    Serial.print("deserializeJson() failed: ");
                    Serial.println(error.c_str());
                    return;
                }

                int status_code = doc["status_code"];
                const char* message = doc["message"];

                JsonObject data_daerah_istimewa_yogyakarta_mobile_kulonProgo = doc["data"]["daerah_istimewa_yogyakarta"]["mobile_kulonProgo"];
                bool data_daerah_istimewa_yogyakarta_mobile_kulonProgo_real = data_daerah_istimewa_yogyakarta_mobile_kulonProgo["real"];
                bool data_daerah_istimewa_yogyakarta_mobile_kulonProgo_voice = data_daerah_istimewa_yogyakarta_mobile_kulonProgo["voice"];
                bool data_daerah_istimewa_yogyakarta_mobile_kulonProgo_test = data_daerah_istimewa_yogyakarta_mobile_kulonProgo["test"];
                bool data_daerah_istimewa_yogyakarta_mobile_kulonProgo_on = data_daerah_istimewa_yogyakarta_mobile_kulonProgo["on"];
                
                Serial.println("Siren Activator");
                Serial.print("real  : ");
                Serial.println(data_daerah_istimewa_yogyakarta_mobile_kulonProgo_real);
                Serial.print("test  : ");
                Serial.println(data_daerah_istimewa_yogyakarta_mobile_kulonProgo_test);
                Serial.print("voice : ");
                Serial.println(data_daerah_istimewa_yogyakarta_mobile_kulonProgo_voice);
                Serial.print("status : ");
                Serial.println(data_daerah_istimewa_yogyakarta_mobile_kulonProgo_on);
                Serial.println();

                client.stop();
                delay(1);
                client.connect(server, 443);
                bufferedFile.flush();
                i = true;
            } else {
                digitalWrite(inet,HIGH);

                if(reset==true && test_value=="true" && real_value=="false") {
                    client.println("POST /api/v1/esp32/change-state?province=daerah_istimewa_yogyakarta&site=mobile_kulonProgo&real=false&test="+test_value+" HTTP/1.1");
                    client.println("Host: semarsiren.id");
                    client.println("Connection: close");
                    client.println();
                    reset = false;
                } else if(reset==true && test_value=="false" && real_value=="true") {
                    client.println("POST /api/v1/esp32/change-state?province=daerah_istimewa_yogyakarta&site=mobile_kulonProgo&real="+real_value+"&test=false HTTP/1.1");
                    client.println("Host: semarsiren.id");
                    client.println("Connection: close");
                    client.println();
                    reset = false;
                } else if(reset==false && test_value=="false" && real_value=="false") {
                    client.println("POST /api/v1/esp32/change-state?province=daerah_istimewa_yogyakarta&site=mobile_kulonProgo&real="+real_value+"&test="+test_value+" HTTP/1.1");
                    client.println("Host: semarsiren.id");
                    client.println("Connection: close");
                    client.println();
                    reset = true;
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