///////////////////
#include "vive510.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#define SIGNALPIN1 5 // pin receiving signal from Vive circuit
#define UDPPORT 2510 // For GTA 2022C game 
#define STUDENTIP 197 // choose a teammembers assigned IP number
#define teamNumber 16
#define FREQ 1 // in Hz
const char* ssid     = "TP-Link_E0C8";
const char* password = "52665134";
Vive510 vive1(SIGNALPIN1);
WiFiUDP UDPTestServer;
IPAddress ipTarget(192, 168, 1, 255); // 255 => broadcast
////////////////////

#define RGBLED 2 // for ESP32S2 Devkit pin 18, for M5 stamp=2
#define IR_INPUT1 1
#define IR_INPUT2 4
#define IR_OUTPUT1 6
#define IR_OUTPUT2 7
int highTheshold = 2400;
int lowTheshold = 2500;
double measureDuration = 0.1; // 0.1s

//////////////

void UdpSend(int x, int y)
{
    char udpBuffer[20];
    sprintf(udpBuffer, "%02d:%4d,%4d",teamNumber,x,y);   
                                                
    UDPTestServer.beginPacket(ipTarget, UDPPORT);
    UDPTestServer.println(udpBuffer);
    UDPTestServer.endPacket();
    // Serial.println(udpBuffer);
}
/////////////////

double readBeaconFreq(int pin){
    int voltage;
    // int highVoltage = highTheshold;
    int lowVoltage = lowTheshold;
    unsigned long int startTime = micros();
    unsigned long int endTime = startTime + (unsigned long int)1000000*measureDuration; // 0.1秒 = 100,000微秒
    int risingEdges = 0;
    bool isLow = true; // flag to indicate if the current voltage is low

    while (micros() < endTime) {
        voltage = analogRead(pin);
        // detect rising edge
        if (isLow && voltage > lowVoltage) {
        risingEdges++;  // calculate rising edges
        isLow = false;  // update status to high voltage
        }
        // detect falling edge
        if (!isLow && voltage < lowVoltage) {
        isLow = true;   // update status to low voltage
        }
    }

    // calculate frequency
    double frequency = (double)risingEdges / measureDuration;

    return frequency;

}

bool detectTrophy(int pin){
    double freq = readBeaconFreq(pin);
    Serial.println(freq);
    if (freq > 400 && freq < 650) {
        return true;
    }else{
        return false;
    }
}

bool detectFake(int pin){
    double freq = readBeaconFreq(pin);
    if (freq > 10 && freq < 100) {
        return true;
    }else{
        return false;
    }
}

void setup() {
    Serial.begin(115200);

    int i=0;
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.config(IPAddress(192, 168, 1, STUDENTIP), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
    WiFi.begin(ssid, password);
    Serial.printf("team  #%d ", teamNumber); 
    Serial.print("Connecting to ");  Serial.println(ssid);
    while(WiFi.status()!=WL_CONNECTED && i++ < 20){
        delay(500);   Serial.print(".");
    }
    if (i<19) {
        Serial.println("WiFi connected as "); Serial.print(WiFi.localIP());
    } else {
        Serial.printf("Could not connect err: %d ",i); 
    }
    UDPTestServer.begin(UDPPORT);
    vive1.begin();
    Serial.println("  Vive trackers started");


    pinMode(IR_INPUT1, INPUT);
    pinMode(IR_INPUT2, INPUT);
    pinMode(IR_OUTPUT1, OUTPUT);
    pinMode(IR_OUTPUT2, OUTPUT);
}

void loop() { 
    ///////////////// Vive send 
    static long int ms = millis();
    static uint16_t x,y;

    if (millis()-ms>1000/FREQ) {
        ms = millis();
        // if (WiFi.status()==WL_CONNECTED)
            // neopixelWrite(RGBLED,255,255,255);  // full white
        UdpSend(x,y);
    }
    
    if (vive1.status() == VIVE_RECEIVING) {
        x = vive1.xCoord();
        y = vive1.yCoord();
        // neopixelWrite(RGBLED,0,x/200,y/200);  // blue to greenish
    }
    else {
        x=0;
        y=0; 
        switch (vive1.sync(5)) {
        break;
        case VIVE_SYNC_ONLY: // missing sweep pulses (signal weak)
            // neopixelWrite(RGBLED,64,32,0);  // yellowish
        break;
        default:
        case VIVE_NO_SIGNAL: // nothing detected     
            // neopixelWrite(RGBLED,128,0,0);  // red
        break;
        }
    }
    
    ////////////// IR send
    if (Serial.available() > 0) {
    
        String input = Serial.readStringUntil('\n'); // read the incoming data as string
        input.trim(); 

        // for convenience, we allow the user to set the thresholds from the serial monitor
        if (input.startsWith("h")) {
            highTheshold = input.substring(1).toInt();
            Serial.print("High threshold set to: ");
            Serial.println(highTheshold);
        } else if (input.startsWith("l")) {
            lowTheshold = input.substring(1).toInt();
            Serial.print("Low threshold set to: ");
            Serial.println(lowTheshold);
        } else {
            Serial.println("Invalid input. Please use 'h<value>' or 'l<value>'");
        }
    }

    // Serial.print("High threshold: ");
    // Serial.println(highTheshold);
    // Serial.print("Low threshold: ");
    // Serial.println(lowTheshold);

    bool isTrophy1 = detectTrophy(IR_INPUT1);
    bool isTrophy2 = detectTrophy(IR_INPUT2); 

    if (isTrophy1 && isTrophy2) {
        neopixelWrite(RGBLED,0,255,0);  // green
        digitalWrite(IR_OUTPUT1, HIGH);
        digitalWrite(IR_OUTPUT2, HIGH);
    }else if (isTrophy1 && !isTrophy2) {
        neopixelWrite(RGBLED,255,0,0);  // red
        digitalWrite(IR_OUTPUT1, HIGH);
        digitalWrite(IR_OUTPUT2, LOW);
    }else if (!isTrophy1 && isTrophy2) {
        neopixelWrite(RGBLED,255,255,0);  // yellow
        digitalWrite(IR_OUTPUT1, LOW);
        digitalWrite(IR_OUTPUT2, HIGH);
    }else{
        neopixelWrite(RGBLED,0,0,0);  // black
        digitalWrite(IR_OUTPUT1, LOW);
        digitalWrite(IR_OUTPUT2, LOW);
    }

    delay(10);
}
