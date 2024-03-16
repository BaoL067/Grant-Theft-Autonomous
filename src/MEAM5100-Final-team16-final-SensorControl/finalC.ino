#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "TOF.h"

#define IR_PIN1 18
#define IR_PIN2 19
#define C_MAC  {0xD8,0xBC,0x38,0xE5,0xC1,0xF4} 
#define R_MAC  {0xD4,0xD4,0xDA,0x42,0x75,0x98} 
#define TA_MAC {0xC8,0xF0,0x9E,0xF6,0xE0,0xC8}  
#define M_MAC {0xA0,0x76,0x4E,0x19,0xC0,0x14}  
// C mac address is D8:BC:38:E5:C1:F4
// C is this borad
// R mac address is D4:D4:DA:42:75:98
// R is the mobile base
// M mac address is A0:76:4E:19:C0:14
// M is the main control board on computer
// TA mac address is C8:F0:9E:F6:E0:C8

#include "vive.h"
#define CHANNEL 1

#include <ESP32Servo.h>
#define SERVO_PIN 4
Servo myservo;
int pos = 0;

TOFSensorManager tofManager;
volatile char ModeStatus = '0';
volatile char GripperStatus = 'k';

esp_now_peer_info_t peerR = 
{
    .peer_addr = R_MAC, 
    .channel = CHANNEL,
    .encrypt = false,
};
esp_now_peer_info_t peerTA = 
{
    .peer_addr = TA_MAC, 
    .channel = CHANNEL,
    .encrypt = false,
};
esp_now_peer_info_t peerM = 
{
    .peer_addr = M_MAC, 
    .channel = CHANNEL,
    .encrypt = false,
};


// Usage: send_message('R', "w");
// To R board, send w
void send_message(char where, uint8_t key) {
    uint8_t message[] = {key};
    if (where == 'R'){
        esp_now_send(peerR.peer_addr, message, sizeof(message));
    }
    else if (where == 'T'){
        esp_now_send(peerTA.peer_addr, message, sizeof(message));
    }
    else if (where == 'M'){
        esp_now_send(peerM.peer_addr, message, sizeof(message));
    }
    delay(30); 
}
void send_message(char where, const char* message) {
    if (where == 'R'){
        esp_now_send(peerR.peer_addr, reinterpret_cast<const uint8_t*>(message), strlen(message));
    }
    else if (where == 'T'){
        esp_now_send(peerTA.peer_addr, reinterpret_cast<const uint8_t*>(message), strlen(message));
    }
    else if (where == 'M'){
        esp_now_send(peerM.peer_addr, reinterpret_cast<const uint8_t*>(message), strlen(message));
    }
    delay(30); 
}

// everytime we receive a message, we send it to M and TA board
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) { 

    String message = "Team16: I received a message from Main board: ";
    message += String(reinterpret_cast<const char*>(data), data_len);
    send_message('T', message.c_str());
    send_message('M', message.c_str());
    if(isValidGripper(data[0])){
        GripperStatus = data[0];
    }else{
        ModeStatus = data[0];
    }
}

void closeGripper(){
  for (pos = myservo.read(); pos <= 60; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  GripperStatus = 'k'; // keep
}

void openGripper(){
    for (pos = myservo.read(); pos >= 0; pos -= 1) {
        myservo.write(pos);
        delay(15);
    }
    GripperStatus = 'k'; // keep
}

void setupESPNOW() {
    WiFi.mode(WIFI_STA);  
    Serial.print("MAC address: ");  Serial.println(WiFi.macAddress());

    if (esp_now_init() != ESP_OK) {
        Serial.println("init failed");
        ESP.restart();
    }
    esp_now_register_recv_cb(OnDataRecv);  
    if (esp_now_add_peer(&peerR) != ESP_OK) {
        Serial.println("Pair failed with R");     // ERROR  should not happen
    }
    if (esp_now_add_peer(&peerTA) != ESP_OK) {
        Serial.println("Pair failed with TA");     // ERROR  should not happen
    }
    if (esp_now_add_peer(&peerM) != ESP_OK) {
        Serial.println("Pair failed with M");     // ERROR  should not happen
    }
}



/////////////////////////////////////////////////////////
// wasdqe for movement, zc for rotation, p for stop
const char validKeys[] = {'w', 'a', 's', 'd', 'q', 'e', 'p', 'z', 'c'};
// different modes
const char validMode[] = {'1', '2', '3'};
// o for open, l for close
const char validGripper[] = {'o', 'l'};

bool isValidKey(char key) {
    for (int i = 0; i < sizeof(validKeys) / sizeof(validKeys[0]); i++) {
        if (key == validKeys[i]) {
            return true;
        }
    }
    return false;
}

bool isValidGripper(char key) {
    return (key == 'o' || key == 'l' || key=='k');
}

/////////////////////////////////////////////////////////
int trophyCount = 0;

void setup() {
    Serial.begin(115200);
    // setupUDP(); // wifi channel problem
    setupVive(); 
    setupESPNOW();
    tofManager.initializeSensors();
    pinMode(IR_PIN1, INPUT);
    pinMode(IR_PIN2, INPUT);
    myservo.attach(SERVO_PIN, 500, 2500);
}

void loop() {
    static uint32_t lastPrintTime = millis();
    static uint32_t lastSendTime = millis();
    
    // get vive data
    uint16_t frontX = 0;
    uint16_t frontY = 0;
    uint16_t rearX = 0;
    uint16_t rearY = 0;
    getViveData(frontX, frontY, vive1);
    getViveData(rearX, rearY, vive2);
    uint16_t centerX = frontX;
    uint16_t centerY = frontY;
    float angle = calcAngle(frontX, frontY, rearX, rearY);
    // map to percentage
    int centerPercX = (centerX - X_MIN)/((float)X_MAX - X_MIN)*100.0;
    int centerPercY = (centerY - Y_MIN)/((float)Y_MAX - Y_MIN)*100.0;
    // get tof data
    int d1 = tofManager.readSensor(1);
    int d2 = tofManager.readSensor(2);
    int d3 = tofManager.readSensor(3);
    // get IR data
    // no need to get IR data, just use digitalRead(IR_PIN1) and digitalRead(IR_PIN2)
    


    // every second, send 5 times vive, tof, IR data to M
    if (millis() - lastPrintTime > 200) {
        lastPrintTime = millis();
        Serial.println("I am running");

        String ViveMessage = "x: " + String(centerPercX) + " y: " + String(centerPercY) + " angle: " + String(angle) + "\n";
        String TOFMessage = "d1: " + String(d1) + " d2: " + String(d2) + " d3: " + String(d3) + "\n";
        String IRMessage = "IR1: " + String(digitalRead(IR_PIN1)) + " IR2: " + String(digitalRead(IR_PIN2)) + "\n";
        String SensorMessage = ViveMessage  + TOFMessage + IRMessage;
        send_message('M', SensorMessage.c_str());
    }
    
    delay(20);


    // gripper control
    if(isValidGripper(GripperStatus)){
        if (GripperStatus == 'l') {            
            Serial.println("Increasing servo position");
            closeGripper();
        } else if (GripperStatus == 'o') {
            Serial.println("Decreasing servo position");
            openGripper();
        }else {
            myservo.write(pos);
        }
    }

    // movement control
    if (isValidKey(ModeStatus)) {
        send_message('R', ModeStatus);
        if (millis() - lastSendTime > 1000) {
            lastSendTime = millis();
            String CommandMessage = "Send to R : ";
            CommandMessage += ModeStatus;
            send_message('M', CommandMessage.c_str());
        }
    }

    else if (ModeStatus == '1') {  // auto mode
        // wall following

        // keep the distance between 200mm and 300mm
        if (d1 < 400){
            send_message('R', 'a');
        }
        else if(d2 > 200 && d3 > 200){
            send_message('R', 'e');
        }
        else if(d2 < 150 && d3 < 150){
            send_message('R', 'q');
        }
        else if(d2 - d3 > 20){ // 160mm * sin(5)
            send_message('R', 'e');
        }
        else if(d3 - d2 > 20){
            send_message('R', 'q');
        }
        else{
            send_message('R', 'w');
        }
    }
    else if (ModeStatus == '2') {    
        // search for the trophy in front

        if(digitalRead(IR_PIN1) && digitalRead(IR_PIN2)){
            send_message('R', 'p');
            trophyCount++;
            if (trophyCount>10){
                String SearchMessage = "###########\n###########\n###########\n";
                send_message('M', SearchMessage.c_str());
                trophyCount = 0;
            }
        }else if(digitalRead(IR_PIN1) && !digitalRead(IR_PIN2)){
            send_message('R', 'z');
            trophyCount = 0;
        }else if(!digitalRead(IR_PIN1) && digitalRead(IR_PIN2)){
            send_message('R', 'c');
            trophyCount = 0;
        }else{
            send_message('R', 'z');
            trophyCount =0;
        }
    }


    // delay(30);
}
