#include <EthernetUdp.h>
#include <TimeLib.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xF9, 0xD9, 0x04 };
 
#define MYIPADDR 192,168,1,28
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,1
#define MYGW 192,168,1,1

unsigned int localPort = 8888;
const char timeServer[] = "ntp.bmkg.go.id";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
int size;

const int timeZone = 7;

EthernetUDP Udp;

time_t prevDisplay = 0;
uint32_t beginWait;
unsigned long secsSince1900;

void sendNTPpacket(const char * address) {
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;
    packetBuffer[1] = 0;
    packetBuffer[2] = 6;
    packetBuffer[3] = 0xEC;

    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;                
    Udp.beginPacket(address, 123);
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

time_t getNtpTime() {
    while(Udp.parsePacket()>0);
    Serial.println("Transmit NTP Request");
    sendNTPpacket(timeServer);
    beginWait = millis();
    while(millis()-beginWait<1000) {
        size = Udp.parsePacket();
        if(size>=NTP_PACKET_SIZE) {
            Serial.println("Receive NTP Response");
            Udp.read(packetBuffer, NTP_PACKET_SIZE);
            secsSince1900 =  word(packetBuffer[40]) << 24;
            secsSince1900 |= word(packetBuffer[41]) << 16;
            secsSince1900 |= word(packetBuffer[42]) << 8;
            secsSince1900 |= word(packetBuffer[43]);
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    Serial.println("No NTP Response");
    return 0; 
}

void bmkg_time_setup() {
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
 
    Serial.print("Local IP : ");
    Serial.println(Ethernet.localIP());
    Serial.print("Subnet Mask : ");
    Serial.println(Ethernet.subnetMask());
    Serial.print("Gateway IP : ");
    Serial.println(Ethernet.gatewayIP());
    Serial.print("DNS Server : ");
    Serial.println(Ethernet.dnsServerIP());
 
    Serial.println("Ethernet Successfully Initialized");

    Udp.begin(localPort);
    Serial.println("waiting for sync");
    setSyncProvider(getNtpTime);
}

void printDigits(int digits) {
    if(digits < 10) Serial.print('0');
    Serial.print(digits);
}

void digitalClockDisplay() {
    printDigits(day());
    Serial.print("/");
    printDigits(month());
    Serial.print("/");
    Serial.print(year()); 
    Serial.print(" ");
    printDigits(hour());
    Serial.print(":");
    printDigits(minute());
    Serial.print(":");
    printDigits(second());
    Serial.println(); 
}

void bmkg_time_loop() {
    if(timeStatus()!=timeNotSet) {
        if(now()!=prevDisplay) {
            prevDisplay = now();
            // digitalClockDisplay();  
        }
    }
}