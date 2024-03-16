#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1                  // channel can be 1 to 14, channel 0 means current channel.  
#define C_MAC {0xD8,0xBC,0x38,0xE5,0xC1,0xF4} 
#define R_MAC {0xD4,0xD4,0xDA,0x42,0x75,0x98} 
#define M_MAC {0xA0,0x76,0x4E,0x19,0xC0,0x14}  

// C mac address is D8:BC:38:E5:C1:F4
// R mac address is D4:D4:DA:42:75:98
// M mac address is A0:76:4E:19:C0:14

// ESP-NOW Send //////////////////////////////////

esp_now_peer_info_t peerR = 
{
    .peer_addr = R_MAC, 
    .channel = CHANNEL,
    .encrypt = false,
};

esp_now_peer_info_t peerC = 
{
    .peer_addr = C_MAC, 
    .channel = CHANNEL,
    .encrypt = false,
};

// Usage: send_message_R("w");
void send_message_R(uint8_t key) {
    uint8_t message[] = {key};
    esp_now_send(peerR.peer_addr, message, sizeof(message));
    delay(30); 
}
void send_message_R(const char* message) {
    esp_now_send(peerR.peer_addr, reinterpret_cast<const uint8_t*>(message), strlen(message));
    delay(30); 
}

// Usage: send_message_M("This is a message");
void send_message_C(uint8_t key) {
    uint8_t message[] = {key};
    esp_now_send(peerC.peer_addr, message, sizeof(message));
    delay(30); 
}
void send_message_C(const char* message) {
    esp_now_send(peerC.peer_addr, reinterpret_cast<const uint8_t*>(message), strlen(message));
    delay(30); 
}


void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) { 
    // create a new array with the data
    char message[data_len + 1];

    // copy the data to the array
    memcpy(message, data, data_len);
    message[data_len] = '\0'; // make sure there is a null terminator

    // print the data
    if (isascii(message[0])) {
        Serial.println(message);
    }
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

    if (esp_now_add_peer(&peerC) != ESP_OK) {
        Serial.println("Pair failed with C");     // ERROR  should not happen
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    setupESPNOW();
}

void loop() {

    // for input from Serial Monitor
    static String inputString = ""; 
    inputString.reserve(200); 

    while (Serial.available()) {
        char c = Serial.read(); 
        if (c == '\r') {
            send_message_C(inputString.c_str()); 
            Serial.print("send to C: ");
            Serial.println(inputString); 
            inputString = ""; 
        } else {
            inputString += c;
        }
    }


    delay(100);
}