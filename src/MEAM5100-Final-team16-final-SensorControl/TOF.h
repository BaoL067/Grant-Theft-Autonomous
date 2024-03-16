#ifndef TOF_H
#define TOF_H

#include "Adafruit_VL53L0X.h"

#define SENSOR1_ADDRESS 0x30
#define SENSOR2_ADDRESS 0x31
#define SENSOR3_ADDRESS 0x32
// #define SENSOR4_ADDRESS 0x33

#define XSHUT_1 25
#define XSHUT_2 26
#define XSHUT_3 27
// #define XSHUT_4 26

class TOFSensorManager {
public:
    TOFSensorManager();
    void initializeSensors();
    int readSensor(int sensorNumber);

    void initialize();
    void startMeasurement();
    void stopMeasurement();
    void processMeasurement();

private:
    Adafruit_VL53L0X sensor1;
    Adafruit_VL53L0X sensor2;
    Adafruit_VL53L0X sensor3;
    // Adafruit_VL53L0X sensor4;

    VL53L0X_RangingMeasurementData_t measure1;
    VL53L0X_RangingMeasurementData_t measure2;
    VL53L0X_RangingMeasurementData_t measure3;
    // VL53L0X_RangingMeasurementData_t measure4;

    void initializeSensor(Adafruit_VL53L0X& sensor, uint8_t address);
    void setID();

};

#endif // TOF_H

