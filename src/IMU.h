#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>

class IMU {

public:

    bool begin();

    void calibrate();

    void update();

    float getRoll();
    float getPitch();

    float getGyroX();
    float getGyroY();
    float getGyroZ();

    bool isConnected();

private:

    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;

    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;

    float roll;
    float pitch;

    float gyroXOffset;
    float gyroYOffset;
    float gyroZOffset;

    unsigned long lastUpdate;

    void readRawData();
};

#endif
