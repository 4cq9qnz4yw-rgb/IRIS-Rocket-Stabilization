#include "IMU.h"
#include "config.h"
#include <math.h>

bool IMU::begin() {

    Wire.begin();

    // Wake MPU6050
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x6B);
    Wire.write(0x00);

    if (Wire.endTransmission() != 0) {
        return false;
    }

    // Accelerometer ±2g
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1C);
    Wire.write(0x00);
    Wire.endTransmission();

    // Gyroscope ±250 deg/s
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1B);
    Wire.write(0x00);
    Wire.endTransmission();

    roll = 0;
    pitch = 0;

    gyroXOffset = 0;
    gyroYOffset = 0;
    gyroZOffset = 0;

    lastUpdate = micros();

    return true;
}


// ------------------------------------------------------------
// Read raw MPU6050 data
// ------------------------------------------------------------

void IMU::readRawData() {

    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x3B);
    Wire.endTransmission(false);

    Wire.requestFrom(MPU6050_ADDRESS, 14);

    if (Wire.available() < 14) {
        return;
    }

    accelX = Wire.read() << 8 | Wire.read();
    accelY = Wire.read() << 8 | Wire.read();
    accelZ = Wire.read() << 8 | Wire.read();

    // Temperature
    Wire.read();
    Wire.read();

    gyroX = Wire.read() << 8 | Wire.read();
    gyroY = Wire.read() << 8 | Wire.read();
    gyroZ = Wire.read() << 8 | Wire.read();
}


// ------------------------------------------------------------
// Gyroscope calibration
// ------------------------------------------------------------

void IMU::calibrate() {

    Serial.println("IMU calibration starting...");
    Serial.println("Keep the sensor completely still.");

    const int samples = 1000;

    long sumX = 0;
    long sumY = 0;
    long sumZ = 0;

    for (int i = 0; i < samples; i++) {

        readRawData();

        sumX += gyroX;
        sumY += gyroY;
        sumZ += gyroZ;

        delay(2);
    }

    gyroXOffset = (float)sumX / samples;
    gyroYOffset = (float)sumY / samples;
    gyroZOffset = (float)sumZ / samples;

    roll = 0;
    pitch = 0;

    lastUpdate = micros();

    Serial.println("IMU calibration complete.");
}


// ------------------------------------------------------------
// Sensor fusion
// Complementary filter
// ------------------------------------------------------------

void IMU::update() {

    readRawData();

    unsigned long currentTime = micros();

    float dt = (currentTime - lastUpdate) / 1000000.0;

    lastUpdate = currentTime;

    // Prevent unrealistic timestep
    if (dt <= 0 || dt > 0.1) {
        return;
    }

    // Convert accelerometer to g
    float ax = accelX / 16384.0;
    float ay = accelY / 16384.0;
    float az = accelZ / 16384.0;

    // Convert gyro to degrees/sec
    float gx = (gyroX - gyroXOffset) / 131.0;
    float gy = (gyroY - gyroYOffset) / 131.0;

    // Accelerometer angle
    float accelRoll =
        atan2(ay, az) * 180.0 / PI;

    float accelPitch =
        atan2(-ax, sqrt(ay * ay + az * az))
        * 180.0 / PI;

    // Gyroscope integration
    roll += gx * dt;
    pitch += gy * dt;

    // Complementary filter
    roll =
        0.98 * roll +
        0.02 * accelRoll;

    pitch =
        0.98 * pitch +
        0.02 * accelPitch;
}


// ------------------------------------------------------------

float IMU::getRoll() {
    return roll;
}

float IMU::getPitch() {
    return pitch;
}

float IMU::getGyroX() {

    return (gyroX - gyroXOffset) / 131.0;
}

float IMU::getGyroY() {

    return (gyroY - gyroYOffset) / 131.0;
}

float IMU::getGyroZ() {

    return (gyroZ - gyroZOffset) / 131.0;
}


// ------------------------------------------------------------

bool IMU::isConnected() {

    Wire.beginTransmission(MPU6050_ADDRESS);

    return Wire.endTransmission() == 0;
}
