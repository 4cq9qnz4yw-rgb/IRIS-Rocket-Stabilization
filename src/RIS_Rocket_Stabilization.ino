/*
 * ============================================================
 * IRIS Rocket Stabilization System
 * ============================================================
 *
 * Controller:
 *     Arduino UNO R4 WiFi
 *
 * IMU:
 *     MPU6050
 *
 * Actuators:
 *     4 x SG90 Servo Motors
 *
 * Control:
 *     Complementary Filter
 *     PID Stabilization
 *     Four-servo control mixing
 *
 * IMPORTANT:
 * This project is intended for laboratory/ground testing.
 * It is NOT flight-certified rocket guidance software.
 * ============================================================
 */

#include <Arduino.h>

#include "config.h"
#include "IMU.h"
#include "PIDController.h"
#include "ServoControl.h"


// ------------------------------------------------------------
// Objects
// ------------------------------------------------------------

IMU imu;

ServoControl servos;

PIDController rollPID(
    KP_ROLL,
    KI_ROLL,
    KD_ROLL,
    MAX_PID_OUTPUT
);

PIDController pitchPID(
    KP_PITCH,
    KI_PITCH,
    KD_PITCH,
    MAX_PID_OUTPUT
);


// ------------------------------------------------------------
// System state
// ------------------------------------------------------------

bool stabilizationEnabled = START_STABILIZATION;

unsigned long lastControlTime = 0;

unsigned long lastTelemetryTime = 0;


// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------

void setup() {

    Serial.begin(SERIAL_BAUD);

    delay(1000);

    Serial.println();
    Serial.println("=================================");
    Serial.println("IRIS ROCKET STABILIZATION SYSTEM");
    Serial.println("=================================");

    Serial.println("Initializing servos...");

    servos.begin();

    Serial.println("Initializing MPU6050...");

    if (!imu.begin()) {

        Serial.println("ERROR: MPU6050 NOT FOUND.");

        while (true) {

            servos.center();

            delay(500);
        }
    }

    Serial.println("MPU6050 connected.");

    // Calibration
    imu.calibrate();

    servos.center();

    lastControlTime = millis();

    Serial.println();
    Serial.println("System ready.");

    if (stabilizationEnabled) {
        Serial.println("STABILIZATION: ENABLED");
    }
    else {
        Serial.println("STABILIZATION: DISABLED");
        Serial.println("Send 'E' through Serial Monitor to enable.");
    }

    Serial.println();
}


// ------------------------------------------------------------
// Main loop
// ------------------------------------------------------------

void loop() {

    unsigned long currentTime = millis();

    // --------------------------------------------------------
    // Serial commands
    // --------------------------------------------------------

    if (Serial.available()) {

        char command = Serial.read();

        // Enable
        if (command == 'E' || command == 'e') {

            stabilizationEnabled = true;

            rollPID.reset();
            pitchPID.reset();

            Serial.println("STABILIZATION ENABLED");
        }


        // Disable
        else if (command == 'D' || command == 'd') {

            stabilizationEnabled = false;

            rollPID.reset();
            pitchPID.reset();

            servos.center();

            Serial.println("STABILIZATION DISABLED");
        }


        // Center servos
        else if (command == 'C' || command == 'c') {

            servos.center();

            Serial.println("SERVOS CENTERED");
        }


        // Print status
        else if (command == 'S' || command == 's') {

            Serial.println("---- IRIS STATUS ----");

            Serial.print("Roll: ");
            Serial.println(imu.getRoll());

            Serial.print("Pitch: ");
            Serial.println(imu.getPitch());

            Serial.print("Stabilization: ");

            if (stabilizationEnabled) {
                Serial.println("ON");
            }
            else {
                Serial.println("OFF");
            }
        }
    }


    // --------------------------------------------------------
    // Sensor update
    // --------------------------------------------------------

    imu.update();


    // --------------------------------------------------------
    // Safety check
    // --------------------------------------------------------

    float roll = imu.getRoll();
    float pitch = imu.getPitch();

    if (
        abs(roll) > MAX_SAFE_ANGLE ||
        abs(pitch) > MAX_SAFE_ANGLE
    ) {

        stabilizationEnabled = false;

        rollPID.reset();
        pitchPID.reset();

        servos.center();

        Serial.println("SAFETY LIMIT EXCEEDED");
        Serial.println("STABILIZATION DISABLED");
    }


    // --------------------------------------------------------
    // Control loop
    // --------------------------------------------------------

    if (
        stabilizationEnabled &&
        currentTime - lastControlTime >= CONTROL_PERIOD_MS
    ) {

        lastControlTime = currentTime;

        float dt =
            CONTROL_PERIOD_MS / 1000.0;


        // Target orientation
        // The goal is level = 0 degrees.

        float rollTarget = 0.0;
        float pitchTarget = 0.0;


        // ----------------------------------------------------
        // Deadband
        // ----------------------------------------------------

        if (abs(roll) < ANGLE_DEADBAND) {
            roll = 0;
        }

        if (abs(pitch) < ANGLE_DEADBAND) {
            pitch = 0;
        }


        // ----------------------------------------------------
        // PID calculations
        // ----------------------------------------------------

        float rollCorrection =
            rollPID.compute(
                rollTarget,
                roll,
                dt
            );

        float pitchCorrection =
            pitchPID.compute(
                pitchTarget,
                pitch,
                dt
            );


        // ----------------------------------------------------
        // Servo mixing
        // ----------------------------------------------------

        servos.stabilize(
            rollCorrection,
            pitchCorrection
        );
    }


    // --------------------------------------------------------
    // Telemetry
    // --------------------------------------------------------

    if (
        currentTime - lastTelemetryTime >= 100
    ) {

        lastTelemetryTime = currentTime;

        Serial.print("ROLL:");
        Serial.print(roll, 2);

        Serial.print(",PITCH:");
        Serial.print(pitch, 2);

        Serial.print(",GX:");
        Serial.print(imu.getGyroX(), 2);

        Serial.print(",GY:");
        Serial.print(imu.getGyroY(), 2);

        Serial.print(",GZ:");
        Serial.print(imu.getGyroZ(), 2);

        Serial.print(",STATE:");

        if (stabilizationEnabled) {
            Serial.println("ON");
        }
        else {
            Serial.println("OFF");
        }
    }
}
