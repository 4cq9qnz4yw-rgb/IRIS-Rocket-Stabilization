#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// IRIS Rocket Stabilization System
// Hardware configuration and control parameters
// ============================================================

// -------------------- I2C --------------------

#define MPU6050_ADDRESS 0x68

// -------------------- Servo Pins --------------------
// Change these pins if your physical wiring is different.

#define SERVO_1_PIN 3
#define SERVO_2_PIN 5
#define SERVO_3_PIN 6
#define SERVO_4_PIN 9

// -------------------- Servo Center --------------------

#define SERVO_CENTER 90

// Conservative mechanical limits
#define SERVO_MIN 60
#define SERVO_MAX 120

// -------------------- PID PARAMETERS --------------------

// Start conservatively.
// These values MUST be tuned experimentally on the test rig.

#define KP_ROLL  1.20
#define KI_ROLL  0.02
#define KD_ROLL  0.35

#define KP_PITCH 1.20
#define KI_PITCH 0.02
#define KD_PITCH 0.35

// Maximum PID correction in degrees
#define MAX_PID_OUTPUT 25.0

// Integral windup protection
#define MAX_INTEGRAL 50.0

// -------------------- CONTROL --------------------

#define CONTROL_FREQUENCY_HZ 100
#define CONTROL_PERIOD_MS 10

// Deadband prevents very small corrections
#define ANGLE_DEADBAND 0.5

// -------------------- SAFETY --------------------

// Stabilization starts disabled.
// Set true only after confirming wiring and test rig behavior.

#define START_STABILIZATION false

// Maximum allowed estimated tilt before safety shutdown
#define MAX_SAFE_ANGLE 45.0

// Serial communication
#define SERIAL_BAUD 115200

#endif
