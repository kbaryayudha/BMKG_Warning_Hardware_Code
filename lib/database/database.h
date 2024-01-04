#include <ArduinoJson.h>
#include <StreamUtils.h>

char server[] = "semarsiren.id";
// char server[] = "httpbin.org";  

#define inet 2
#define spare 17

const int rand_pin = 36;

EthernetClient base_client;
// EthernetClient client;
SSLClient client(base_client, TAs, (size_t)TAs_NUM, rand_pin);
 
unsigned long beginMicros, endMicros, byteCount = 0, currentMillis, previousMillis = 0;
bool printWebData = true, i = false;
float seconds, rate;
int len;
String DFPlayer_status = "LOW";

void database_setup() {
    pinMode(inet,OUTPUT);
    pinMode(spare,OUTPUT);

    client.connect(server, 443);
    // client.connect(server, 80);

    if(client.connected()) {
        Serial.println("Connected to Server");
        DFPlayer.play(1);
        delay(2000);
        DFPlayer.pause();
    } else {
        Serial.println("Not Connected to Server!");
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
            if(i == false) {
                digitalWrite(inet,HIGH);
                client.println("GET /api/v1/esp32/siren-activator?province=daerah_istimewa_yogyakarta&site=tower_underpassYIA HTTP/1.0");
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

                JsonObject data_daerah_istimewa_yogyakarta_tower_underpassYIA = doc["data"]["daerah_istimewa_yogyakarta"]["tower_underpassYIA"];
                bool data_daerah_istimewa_yogyakarta_tower_underpassYIA_real = data_daerah_istimewa_yogyakarta_tower_underpassYIA["real"];
                bool data_daerah_istimewa_yogyakarta_tower_underpassYIA_spare = data_daerah_istimewa_yogyakarta_tower_underpassYIA["spare"];
                bool data_daerah_istimewa_yogyakarta_tower_underpassYIA_test = data_daerah_istimewa_yogyakarta_tower_underpassYIA["test"];
                bool data_daerah_istimewa_yogyakarta_tower_underpassYIA_on = data_daerah_istimewa_yogyakarta_tower_underpassYIA["on"];
                
                Serial.println("Siren Activator");
                Serial.print("real  : ");
                Serial.println(data_daerah_istimewa_yogyakarta_tower_underpassYIA_real);
                Serial.print("test  : ");
                Serial.println(data_daerah_istimewa_yogyakarta_tower_underpassYIA_test);
                Serial.print("spare : ");
                Serial.println(data_daerah_istimewa_yogyakarta_tower_underpassYIA_spare);
                Serial.print("status : ");
                Serial.println(data_daerah_istimewa_yogyakarta_tower_underpassYIA_on);
                Serial.println();

                if(data_daerah_istimewa_yogyakarta_tower_underpassYIA_spare==1) {
                    digitalWrite(spare,HIGH);
                } else {
                    digitalWrite(spare,LOW);
                }

                if(DFPlayer_status=="LOW") {
                    if(data_daerah_istimewa_yogyakarta_tower_underpassYIA_test==1 && data_daerah_istimewa_yogyakarta_tower_underpassYIA_real==0) {
                        DFPlayer.play(2);
                        DFPlayer_status = "HIGH";
                    } else if(data_daerah_istimewa_yogyakarta_tower_underpassYIA_test==0 && data_daerah_istimewa_yogyakarta_tower_underpassYIA_real==1)  {
                        DFPlayer.play(3);
                        DFPlayer_status = "HIGH";
                    } 
                }
                if(data_daerah_istimewa_yogyakarta_tower_underpassYIA_test==0 && data_daerah_istimewa_yogyakarta_tower_underpassYIA_real==0) {
                    DFPlayer.pause();
                    DFPlayer_status = "LOW";
                }
                client.stop();
                delay(1);
                client.connect(server, 443);
                bufferedFile.flush();
                i = true;
            } else {
                digitalWrite(inet,HIGH);
                client.println("POST /api/v1/esp32/send-data?province=daerah_istimewa_yogyakarta&site=tower_underpassYIA&primary_voltage="+(String)primary_voltage_value+"&secondary_voltage="+(String)secondary_voltage_value+"&accu_voltage="+(String)accu_voltage_value+"&temp="+(String)temp_value+"&data_rate="+(String)rate+"&date="+(String)rtcday+"/"+(String)rtcmonth+"/"+(String)rtcyear+"&time="+(String)rtchour+":"+(String)rtcminute+":"+(String)rtcsecond+" HTTP/1.0");
                client.println("Host: semarsiren.id");
                client.println("Connection: close");
                client.println();
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
        client.connect(server, 443);
        // client.connect(server, 80);
    }
}