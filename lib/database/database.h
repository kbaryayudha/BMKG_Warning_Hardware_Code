#include <ArduinoJson.h>
#include <StreamUtils.h>

char server[] = "semarsiren.id";  

#define inet 2
#define spare 17

const int rand_pin = 36;

EthernetClient base_client;
SSLClient client(base_client, TAs, (size_t)TAs_NUM, rand_pin);
 
unsigned long beginMicros, endMicros, byteCount = 0, previousMillis = 0;
bool printWebData = true, i = false;
float seconds, rate;
int len;
String DFPlayer_status = "LOW";

void database_setup() {
    pinMode(inet,OUTPUT);
    pinMode(spare,OUTPUT);

    client.connect(server, 443);

    if(client.connected()) {
        Serial.println("Connected to Server");
        DFPlayer.play(1);
        delay(2000);
        DFPlayer.pause();
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
        byte buffer[80];
        if(len > 80) {
            len = 80;
        }
        client.read(buffer, len);
        if(printWebData) {
            Serial.write(buffer, len);
        }
        byteCount = byteCount + len;
    }

    if(client.connected()) {
        if(millis() - previousMillis >= 10000) {
            previousMillis = millis();
            if(i == false) {
                digitalWrite(inet,HIGH);
                client.println("GET /api/v1/esp32/siren-activator?province=demonstration&site=tower_demo HTTP/1.0");
                client.println("Host: semarsiren.id");
                client.println("Connection: close");
                client.println();

                if(client.println() == 0) {
                    Serial.println("Failed to send request");
                    client.stop();
                    return;
                }
                
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

                JsonObject data_demonstration_tower_demo = doc["data"]["demonstration"]["tower_demo"];
                bool data_demonstration_tower_demo_test = data_demonstration_tower_demo["test"];
                bool data_demonstration_tower_demo_real = data_demonstration_tower_demo["real"];
                bool data_demonstration_tower_demo_spare = data_demonstration_tower_demo["spare"];
                const char* data_demonstration_tower_demo_date_time = data_demonstration_tower_demo["date_time"];
                bool data_demonstration_tower_demo_on = data_demonstration_tower_demo["on"];
                
                Serial.println("Siren Activator");
                Serial.print("test  : ");
                Serial.println(data_demonstration_tower_demo_test);
                Serial.print("real  : ");
                Serial.println(data_demonstration_tower_demo_real);
                Serial.print("spare : ");
                Serial.println(data_demonstration_tower_demo_spare);
                Serial.print("date_time : ");
                Serial.println(data_demonstration_tower_demo_date_time);
                Serial.print("status : ");
                Serial.println(data_demonstration_tower_demo_on);
                Serial.println();

                if(data_demonstration_tower_demo_spare==1) {
                    digitalWrite(spare,HIGH);
                } else {
                    digitalWrite(spare,LOW);
                }

                if(DFPlayer_status=="LOW") {
                    if(data_demonstration_tower_demo_test==1 && data_demonstration_tower_demo_real==0) {
                        DFPlayer.play(2);
                        DFPlayer_status = "HIGH";
                    } else if(data_demonstration_tower_demo_test==0 && data_demonstration_tower_demo_real==1)  {
                        DFPlayer.play(3);
                        DFPlayer_status = "HIGH";
                    } 
                }
                if(data_demonstration_tower_demo_test==0 && data_demonstration_tower_demo_real==0) {
                    DFPlayer.pause();
                    DFPlayer_status = "LOW";
                }
                client.stop();
                delay(1);
                client.connect(server, 443);
                i = true;
            } else {
                digitalWrite(inet,HIGH);
                client.println("POST /api/v1/esp32/send-data?province=demonstration&site=tower_demo&primary_voltage="+(String)primary_voltage_value+"&secondary_voltage="+(String)secondary_voltage_value+"&accu_voltage="+(String)accu_voltage_value+"&temp="+(String)temp_value+"&data_rate="+(String)rate+"&date="+(String)rtcday+"/"+(String)rtcmonth+"/"+(String)rtcyear+"&time="+(String)rtchour+":"+(String)rtcminute+":"+(String)rtcsecond+" HTTP/1.0");
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
    }
}