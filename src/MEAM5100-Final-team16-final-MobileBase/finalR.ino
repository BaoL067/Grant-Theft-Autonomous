#include <PID_v1.h>
// This library is used to control the PID loop
#include <ESP32Encoder.h> 
// This library is used to read the encoder values
// #include <ESP32Servo.h>
// This library is used to control the servo motor
#include "L298N.h"
// This library is used to control the DC motor
#include <esp_now.h>
#include <WiFi.h>


// Define the pins for the servo motor
#define FRONT_SERVO_PIN 23
#define REAR_SERVO_PIN2 22

// Define the pins for the DC motors and encoders
#define LEFT_MOTOR_PWM_PIN 32
#define LEFT_MOTOR_DIR_PIN1 33
#define LEFT_MOTOR_DIR_PIN2 26

// pin 25 not work, we tested two ESP32 board
// unknown reason
#define RIGHT_MOTOR_DIR_PIN1 27
#define RIGHT_MOTOR_DIR_PIN2 14
#define RIGHT_MOTOR_PWM_PIN 12

#define LEFT_ENCODER_PIN1 36
#define LEFT_ENCODER_PIN2 39
#define RIGHT_ENCODER_PIN1 34
#define RIGHT_ENCODER_PIN2 35



// Motor Control ///////////////////////////////////////////////////////////////////
const int gearRatio = 210;
const int encoderCPR = 64;
const float wheelCircumference = 0.276;

L298N leftMotor(LEFT_MOTOR_PWM_PIN, LEFT_MOTOR_DIR_PIN1, LEFT_MOTOR_DIR_PIN2);
L298N rightMotor(RIGHT_MOTOR_PWM_PIN, RIGHT_MOTOR_DIR_PIN1, RIGHT_MOTOR_DIR_PIN2);

struct MotorState {
    ESP32Encoder encoder;
    int64_t lastEncoderCount = 0;
    unsigned long lastTime = 0;    
};

MotorState leftMotorState, rightMotorState;

double calculateSpeed(MotorState& motorState) {
    unsigned long currentTime = millis();
    double timeInterval = (currentTime - motorState.lastTime) / 1000.0; // in seconds

    int64_t currentEncoderCount = motorState.encoder.getCount();
    int64_t countDelta = currentEncoderCount - motorState.lastEncoderCount;

    double speed = (countDelta / (double)(encoderCPR * gearRatio)) * wheelCircumference / timeInterval;

    // update state
    motorState.lastEncoderCount = currentEncoderCount;
    motorState.lastTime = currentTime;

    return speed;
}

// Car Control ////////////////////////////////////////////////////////
volatile char direction;


// PID Control ////////////////////////////////////////////////////////

double Kp = 1370.0, Ki = 170.0, Kd = 10.0;
double leftSpeedInput, rightSpeedInput;
double leftPWMOutput, rightPWMOutput;
double setLeftSpeed = 0.0; 
double setRightSpeed = 0.0; 
double referenceSpeed = 0.0; // for debug only

PID leftMotorPID(&leftSpeedInput, &leftPWMOutput, &setLeftSpeed, Kp, Ki, Kd, DIRECT);
PID rightMotorPID(&rightSpeedInput, &rightPWMOutput, &setRightSpeed, Kp, Ki, Kd, DIRECT);

// ESPNOW //////////////////////////////////////////////////////

// R mac address: {0xD4,0xD4,0xDA,0x42,0x75,0x98}
// C mac address: {0xD8,0xBC,0x38,0xE5,0xC1,0xF4}
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    Serial.printf(" Recv from: %02x:%02x:%02x:%02x:%02x:%02x ",mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]); 
    Serial.print(" Data: "); 
    // if the first byte is a char, then it is a command
    if (isascii(data[0])) {
        Serial.println((char *)data);
        direction = data[0];
    } else {
        Serial.println("Not a command");
    }
}
///////////////////////////////////////////////////////////////

void setupESPNOW() {
    WiFi.mode(WIFI_STA);
    Serial.print("ESPNow Receiving MAC: ");  Serial.println(WiFi.macAddress());

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESPNow Init Failed");
        ESP.restart();
    }
    esp_now_register_recv_cb(OnDataRecv);   
    Serial.println("ESP32 MAC Address: ");
    Serial.println(WiFi.macAddress());  // find my mac adress 
    delay(50);
}

void setupEncoder() {
    // attach the pins of the encoder to the ESP32
    leftMotorState.encoder.attachFullQuad(LEFT_ENCODER_PIN1, LEFT_ENCODER_PIN2);
    rightMotorState.encoder.attachFullQuad(RIGHT_ENCODER_PIN1, RIGHT_ENCODER_PIN2);
    // clear the encoder count
    leftMotorState.encoder.clearCount();
    rightMotorState.encoder.clearCount();
    Serial.println("Setup Encoder Done!");
    delay(50);
}

void setupMotor(){
    leftMotor.setVelocity(0); // 0-255
    rightMotor.setVelocity(0); // 0-255
    Serial.println("Setup Motor Done!");
    delay(50);
}

void setupPID(){
    leftMotorPID.SetMode(AUTOMATIC);
    rightMotorPID.SetMode(AUTOMATIC);
    leftMotorPID.SetOutputLimits(-255, 255);
    rightMotorPID.SetOutputLimits(-255, 255);
    leftMotorPID.SetSampleTime(50);
    rightMotorPID.SetSampleTime(50);
    Serial.println("Setup PID Done!");
    delay(50);
}

void setup() {
    Serial.begin(115200);
    setupPID();
    setupEncoder(); 
    setupMotor();
    setupESPNOW();
    Serial.println("Setup All Done!");
}

void loop() {

    switch (direction) {
    case 'w':          // forward
        setLeftSpeed = 0.24;
        setRightSpeed = 0.24;
        break;
    case 's':          // backward
        setLeftSpeed = -0.24;
        setRightSpeed = -0.24;
        break;

    case 'a':          // turn left
        setLeftSpeed = -0.2;
        setRightSpeed = 0.2;
        break;

    case 'd':          // turn right
        setLeftSpeed = 0.2;
        setRightSpeed = -0.2;
        break;

    case 'q':          // fine turn left
        setLeftSpeed = 0.15;
        setRightSpeed = 0.23;
        break;

    case 'e':          // fine turn right
        setLeftSpeed = 0.23;
        setRightSpeed = 0.15;
        break;

    case 'p':          // stop
        setLeftSpeed = 0;
        setRightSpeed = 0;
        break;
}


    leftSpeedInput = calculateSpeed(leftMotorState);
    rightSpeedInput = calculateSpeed(rightMotorState);
    
    Serial.printf("leftSpeedInput:%lf , rightSpeedInput:%lf \n", leftSpeedInput, rightSpeedInput);
    Serial.printf("referenceSpeed:%lf \n", referenceSpeed);
    // update PID
    leftMotorPID.Compute();
    rightMotorPID.Compute();

    // Serial.printf("leftPWMOutput:%lf , rightPWMOutput:%lf \n", leftPWMOutput, rightPWMOutput);

    // output PWM
    leftMotor.setVelocity(leftPWMOutput);
    rightMotor.setVelocity(rightPWMOutput);


    delay(30);
}
