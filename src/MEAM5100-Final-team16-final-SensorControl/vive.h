/*
 * broadcasts data packets to port 2510 
 * Demonstrates high precision vive localization and displays on RGB LED
 * 
 */
#include "vive510.h"

#include <WiFi.h>
#include <WiFiUdp.h>

#define VIVEPIN1 17
#define VIVEPIN2 16
#define UDPPORT 2510 // For GTA 2022C game 
#define STUDENTIP 197 // choose a teammembers assigned IP number
#define teamNumber 16
#define FREQ 1 // in Hz

#define X_MIN 2000
#define X_MAX 5400
#define Y_MIN 3400
#define Y_MAX 4800

const int UDP_PACKET_SIZE = 100;  
byte packetBuffer[UDP_PACKET_SIZE]; 

// const char* ssid     = "TP-Link_05AF";
// const char* password = "47543454";

const char* ssid     = "TP-Link_E0C8";
const char* password = "52665134";

//const char* ssid     = "TP-Link_FD24";
// const char* password = "65512111";

Vive510 vive1(VIVEPIN1, "Vive1");
Vive510 vive2(VIVEPIN2, "Vive2");
WiFiUDP UDPTestServer;
IPAddress ipTarget(192, 168, 1, 255); // 255 => broadcast

void UdpSend(int x, int y, Vive510& vive)
{
  char udpBuffer[20];
  String name = vive.getName();
  sprintf(udpBuffer, "%02d:%s:%4d,%4d", teamNumber, name.c_str(), x, y);                                            
  UDPTestServer.beginPacket(ipTarget, UDPPORT);
  UDPTestServer.println(udpBuffer);
  UDPTestServer.endPacket();
  Serial.println(udpBuffer);
}


int UdpReceive() {
  int cb = UDPTestServer.parsePacket();                          // Check if there is a UDP packet available
  if (cb) {                                                      
    int len = UDPTestServer.read(packetBuffer, UDP_PACKET_SIZE-1);               // Read the UDP packet into packetBuffer
    packetBuffer[len] = 0;                                       // Null-terminate the packetBuffer string
    Serial.printf("Receive message: %s\n",packetBuffer);         // Print the received message
    int receivedValue = atoi((char*)packetBuffer);               // Convert the received message to an integer
    return receivedValue;                                        // Return the received value
  }
  return 0;                                                      // Return 0 if no packet received
}


void sendVive(Vive510& vive) {
  static long int ms = millis();
  static uint16_t x, y;

  if (millis() - ms > 1000 / FREQ) {
    ms = millis();
    UdpSend(x, y, vive);
  }

  if (vive.status() == VIVE_RECEIVING) {
    // x = vive.xCoord_filtered(5);
    // y = vive.yCoord_filtered(5);
    x = vive.xCoord();
    y = vive.yCoord();

  } else {
    x = 0;
    y = 0;
    switch (vive.sync(5)) {
      case VIVE_SYNC_ONLY:  // missing sweep pulses (signal weak)
        // Serial.println("missing sweep pulses");
        break;
      default:
      case VIVE_NO_SIGNAL:  // nothing detected
        // Serial.println("nothing detected");
        break;
    }
  }
  delay(20);
}

void getViveData(uint16_t& x, uint16_t& y, Vive510& vive) {
  if (vive.status() == VIVE_RECEIVING) {
    // x = vive.xCoord_filtered(5);
    // y = vive.yCoord_filtered(5);
    x = vive.xCoord();
    y = vive.yCoord();

  } else {
    x = 0;
    y = 0;
    switch (vive.sync(5)) {
      case VIVE_SYNC_ONLY:  // missing sweep pulses (signal weak)
        // Serial.println("missing sweep pulses");
        break;
      default:
      case VIVE_NO_SIGNAL:  // nothing detected
        // Serial.println("nothing detected");
        break;
    }
  }
  delay(20);
  
}

float calcAngle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
    float deltaX = x1 - x2;
    float deltaY = y1 - y2;
    float angle = atan2(deltaY, deltaX)/3.1415926*180.0;
    return angle;
}

void setupVive(){
  vive1.begin();
  vive2.begin();
  Serial.println("  Vive trackers started");
}     

void setupUDP() {
  int i=0;
  // WiFi.mode(WIFI_AP_STA);
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
  
}
            
