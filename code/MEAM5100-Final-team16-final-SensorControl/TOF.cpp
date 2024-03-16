#include "tof.h"

TOFSensorManager::TOFSensorManager() {
    // do nothing
}

void TOFSensorManager::initializeSensors() {

    pinMode(XSHUT_1, OUTPUT);
    pinMode(XSHUT_2, OUTPUT);
    pinMode(XSHUT_3, OUTPUT);
    // pinMode(XSHUT_4, OUTPUT);

    Serial.println(F("Shutdown pins inited..."));

    digitalWrite(XSHUT_1, LOW);
    digitalWrite(XSHUT_2, LOW);
    digitalWrite(XSHUT_3, LOW);
    // digitalWrite(XSHUT_4, LOW);

    Serial.println(F("All in reset mode...(pins are low)"));
    Serial.println(F("Starting..."));

    setID();
}

int TOFSensorManager::readSensor(int sensorNumber) {
    switch (sensorNumber) {
        case 1:
            sensor1.rangingTest(&measure1, false); 
            if (measure1.RangeStatus != 4) {
                return measure1.RangeMilliMeter;
            } else {
                return 8192; 
            }
            break;
        case 2:
            sensor2.rangingTest(&measure2, false); 
            if (measure2.RangeStatus != 4) {
                return measure2.RangeMilliMeter;
            } else {
                return 8192; 
            }
            break;

        case 3:
            sensor3.rangingTest(&measure3, false); 
            if (measure3.RangeStatus != 4) {
                return measure3.RangeMilliMeter;
            } else {
                return 8192; // 8192 is the max value of uint16_t
            }
            break;

        // case 4:
        //     sensor4.rangingTest(&measure4, false); 
        //     if (measure4.RangeStatus != 4) {
        //         return measure4.RangeMilliMeter;
        //     } else {
        //         return 8192; 
        //     }
        //     break;

        default:
            return -1; // error
    }
}


// private //////////////////////////////
void TOFSensorManager::initializeSensor(Adafruit_VL53L0X& sensor, uint8_t address){
    if (!sensor.begin(address)) {
        Serial.println(F("Failed to boot Sensor"));
        // while (1);
    }else{
        Serial.println(F("Good!"));
    }
    delay(10);
}

void TOFSensorManager::setID(){
    // all reset
    digitalWrite(XSHUT_1, LOW);    
    digitalWrite(XSHUT_2, LOW);
    digitalWrite(XSHUT_3, LOW);
    // digitalWrite(XSHUT_4, LOW);
    delay(10);
    // all unreset
    digitalWrite(XSHUT_1, HIGH);
    digitalWrite(XSHUT_2, HIGH);
    digitalWrite(XSHUT_3, HIGH);
    // digitalWrite(XSHUT_4, HIGH);
    delay(10);

    // activating Sensor1 and resetting Sensor2 and Sensor3
    digitalWrite(XSHUT_1, HIGH);
    digitalWrite(XSHUT_2, LOW);
    digitalWrite(XSHUT_3, LOW);
    // digitalWrite(XSHUT_4, LOW);

    // initing sensor1
    initializeSensor(sensor1, SENSOR1_ADDRESS);

    // activating sensor2
    digitalWrite(XSHUT_2, HIGH);
    delay(10);

    // initing sensor2
    initializeSensor(sensor2, SENSOR2_ADDRESS);

    // activating sensor3
    digitalWrite(XSHUT_3, HIGH);
    delay(10);

    // initing sensor3
    initializeSensor(sensor3, SENSOR3_ADDRESS);

    // // activating sensor4
    // digitalWrite(XSHUT_4, HIGH);
    // delay(10);

    // //initing sensor4
    // initializeSensor(sensor4, SENSOR4_ADDRESS);
}