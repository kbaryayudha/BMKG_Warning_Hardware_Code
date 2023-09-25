#include <Ethernet.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>

IPAddress server(54,252,196,141);

#define inet 2

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 
#define MYIPADDR 192,168,1,28
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,1
#define MYGW 192,168,1,1

EthernetClient client;

unsigned long beginMicros, endMicros, byteCount = 0, currentMillis, previousMillis = 0;
bool printWebData = true, i = false;
float seconds, rate;
int len;

void database_setup() {
    pinMode(inet,OUTPUT);
    Serial.println("Begin Ethernet");
 
    Ethernet.init(5);
 
    if(Ethernet.begin(mac)) {
        Serial.println("DHCP OK!");
    } else {
        Serial.println("Failed to configure Ethernet using DHCP");
        if(Ethernet.hardwareStatus() == EthernetNoHardware) {
            Serial.println("Ethernet shield was not found");
            while(true) {
                delay(1);
            }
        }
        if(Ethernet.linkStatus() == LinkOFF) {
            Serial.println("Ethernet cable is not connected");
        }
        
        IPAddress ip(MYIPADDR);
        IPAddress dns(MYDNS);
        IPAddress gw(MYGW);
        IPAddress sn(MYIPMASK);
        Ethernet.begin(mac, ip, dns, gw, sn);
        Serial.println("STATIC OK!");
    }
    delay(5000);
 
    Serial.print("Local IP: ");
    Serial.println(Ethernet.localIP());
    Serial.print("Subnet Mask : ");
    Serial.println(Ethernet.subnetMask());
    Serial.print("Gateway IP : ");
    Serial.println(Ethernet.gatewayIP());
    Serial.print("DNS Server : ");
    Serial.println(Ethernet.dnsServerIP());
 
    Serial.println("Ethernet Successfully Initialized");

    client.connect(server, 80);

    if(client.connected()) {
        Serial.println("Connected to Server");
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
            if(i==false) {
                digitalWrite(inet,HIGH);
                client.println("GET /api/v1/esp32/siren-activator?province=daerah_istimewa_yogyakarta&site=tower_glagah HTTP/1.1");
                client.println("Host: 54.252.196.141");
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

                JsonObject data_daerah_istimewa_yogyakarta_tower_glagah = doc["data"]["daerah_istimewa_yogyakarta"]["tower_glagah"];
                bool data_daerah_istimewa_yogyakarta_tower_glagah_real = data_daerah_istimewa_yogyakarta_tower_glagah["real"];
                bool data_daerah_istimewa_yogyakarta_tower_glagah_spare = data_daerah_istimewa_yogyakarta_tower_glagah["spare"];
                bool data_daerah_istimewa_yogyakarta_tower_glagah_test = data_daerah_istimewa_yogyakarta_tower_glagah["test"];
                bool data_daerah_istimewa_yogyakarta_tower_glagah_voice = data_daerah_istimewa_yogyakarta_tower_glagah["voice"];
                
                Serial.println("Siren Activator");
                Serial.print("real  : ");
                Serial.println(data_daerah_istimewa_yogyakarta_tower_glagah_real);
                Serial.print("test  : ");
                Serial.println(data_daerah_istimewa_yogyakarta_tower_glagah_test);
                Serial.print("voice : ");
                Serial.println(data_daerah_istimewa_yogyakarta_tower_glagah_voice);
                Serial.print("spare : ");
                Serial.println(data_daerah_istimewa_yogyakarta_tower_glagah_spare);
                Serial.println();
                client.stop();
                delay(1);
                client.connect(server, 80);
                bufferedFile.flush();
                i = true;
            } else {
                digitalWrite(inet,HIGH);
                client.println("POST /api/v1/esp32/send-state-kulon-progo?&real="+d1_value+"&test="+d0_value+"&spare="+a1_value+"&voice="+a0_value+" HTTP/1.1");
                client.println("Host: 54.252.196.141");
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
        client.connect(server, 80);
    }
}