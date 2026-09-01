# IRIS — Rocket Stabilization System

> **IMU-based attitude stabilization demonstrator using Arduino UNO R4 WiFi, MPU6050 and four servo actuators.**

## Overview

IRIS is an experimental rocket stabilization system developed to demonstrate the fundamentals of closed-loop attitude control.

The system measures the orientation of a test platform using an MPU6050 inertial measurement unit and applies corrective servo movements through a PID controller.

The project is designed as a **ground-based engineering demonstrator** and is intended for testing the principles of:

* Inertial measurement
* Attitude estimation
* PID control
* Servo actuation
* Closed-loop feedback
* Embedded systems
* Aerospace control systems

## Hardware

| Component                |    Quantity |
| ------------------------ | ----------: |
| Arduino UNO R4 WiFi      |           1 |
| MPU6050 IMU              |           1 |
| SG90 Servo Motor         |           4 |
| Mini Breadboard          |          1+ |
| Jumper Wires             | As required |
| External 5V Servo Supply | Recommended |

## System Architecture

```text
             ┌─────────────────┐
             │    MPU6050      │
             │   IMU Sensor    │
             └────────┬────────┘
                      │
                      │ I²C
                      ▼
             ┌─────────────────┐
             │ Arduino UNO R4  │
             │      WiFi       │
             └────────┬────────┘
                      │
              ┌───────┴────────┐
              │                │
              ▼                ▼
       ┌─────────────┐  ┌──────────────┐
       │   Attitude  │  │     PID      │
       │  Estimation │─►│  Controller  │
       └─────────────┘  └──────┬───────┘
                               │
                               ▼
                       ┌───────────────┐
                       │ Servo Mixing  │
                       └───────┬───────┘
                               │
                 ┌─────────────┼─────────────┐
                 ▼             ▼             ▼
              Servo 1       Servo 2       Servo 3/4
```

## Control Concept

The desired attitude is:

```text
Roll  = 0°
Pitch = 0°
```

The MPU6050 measures acceleration and angular velocity.

A complementary filter combines the accelerometer and gyroscope measurements to estimate the platform's roll and pitch.

The estimated orientation is then compared against the desired orientation.

The resulting error is processed by PID controllers:

```text
Error = Desired Angle - Measured Angle

PID Output =
Kp × Error
+
Ki × Integral(Error)
+
Kd × Derivative(Error)
```

The PID outputs are converted into opposing servo commands.

## Servo Mixing

The four servo outputs are mixed to produce pitch and roll corrections.

```text
              FRONT

        S1             S2

             ROCKET
              BODY

        S3             S4

               REAR
```

Conceptually:

```text
S1 = Center + Pitch + Roll
S2 = Center + Pitch - Roll

S3 = Center - Pitch + Roll
S4 = Center - Pitch - Roll
```

The exact sign convention depends on the physical orientation of the servos and control surfaces.

## Pin Configuration

Default configuration:

| Component   | Pin                            |
| ----------- | ------------------------------ |
| Servo 1     | D3                             |
| Servo 2     | D5                             |
| Servo 3     | D6                             |
| Servo 4     | D9                             |
| MPU6050 SDA | SDA                            |
| MPU6050 SCL | SCL                            |
| MPU6050 VCC | 3.3V/appropriate module supply |
| MPU6050 GND | GND                            |

Check the specific MPU6050 breakout board before connecting its power supply.

## Software Structure

```text
src/
│
├── IRIS_Rocket_Stabilization.ino
│
├── config.h
│   └── Hardware and PID configuration
│
├── IMU.h / IMU.cpp
│   └── MPU6050 communication
│   └── Sensor calibration
│   └── Attitude estimation
│
├── PIDController.h / PIDController.cpp
│   └── PID control algorithm
│   └── Integral windup protection
│
└── ServoControl.h / ServoControl.cpp
    └── Servo initialization
    └── Servo mixing
    └── Servo safety limits
```

## Serial Commands

Open the Arduino Serial Monitor at:

```text
115200 baud
```

Available commands:

| Command | Function              |
| ------- | --------------------- |
| `E`     | Enable stabilization  |
| `D`     | Disable stabilization |
| `C`     | Center all servos     |
| `S`     | Print system status   |

Telemetry is continuously printed in the following format:

```text
ROLL:1.23,PITCH:-0.42,GX:0.10,GY:-0.22,GZ:0.03,STATE:ON
```

This format can later be used for plotting and data analysis.

## Testing Procedure

### Stage 1 — Sensor Test

Do not connect or move the servos initially.

1. Upload the program.
2. Open Serial Monitor.
3. Keep the MPU6050 stationary during calibration.
4. Confirm that roll and pitch values remain reasonably stable.
5. Slowly rotate the sensor by hand.
6. Confirm that the reported orientation changes correctly.

### Stage 2 — Servo Test

1. Keep stabilization disabled.
2. Send:

```text
C
```

3. Confirm that all servos move to approximately their center position.
4. Verify that the mechanical system cannot bind.
5. Confirm the servo directions.

### Stage 3 — Closed-Loop Test

Use a **mechanical test rig/pivot**, not a free-flying vehicle.

1. Keep the stabilization system disabled.
2. Center the servos.
3. Secure the test platform.
4. Verify all mechanical connections.
5. Enable stabilization using:

```text
E
```

6. Apply small manual disturbances.
7. Observe the direction of servo correction.
8. Disable the system immediately if the response becomes unstable.

## PID Tuning

The default PID values are starting values only.

The following parameters are located in `config.h`:

```cpp
#define KP_ROLL  1.20
#define KI_ROLL  0.02
#define KD_ROLL  0.35

#define KP_PITCH 1.20
#define KI_PITCH 0.02
#define KD_PITCH 0.35
```

PID tuning should be performed experimentally on the ground-test rig.

A typical approach is:

1. Start with `Ki = 0`.
2. Increase `Kp` gradually.
3. Introduce `Kd` to reduce oscillation.
4. Add a small `Ki` only if persistent steady-state error exists.
5. Record sensor and servo response.
6. Retest after every change.

## Safety Features

IRIS currently includes:

* Servo angle limits
* Integral windup protection
* Stabilization disabled at startup
* Manual enable/disable
* Automatic shutdown at excessive measured tilt
* Servo centering
* IMU connection check

## Limitations

This project is an educational prototype.

It currently does **not** provide:

* Flight certification
* Redundant sensors
* Fault-tolerant avionics
* High-performance navigation
* GPS/INS navigation
* Flight-qualified actuators
* Structural flight validation
* Certified control software
* Propulsion control
* Autonomous launch or flight control

The system should therefore remain a **ground-based stabilization demonstrator**.

## Future Development

Possible future versions of IRIS could include:

* Better IMU sensor fusion
* Kalman filtering
* Higher-quality IMU
* Real-time data logging
* Wireless telemetry
* Web-based telemetry dashboard
* Servo position feedback
* Automated PID tuning
* 3-axis attitude estimation
* Test-rig data visualization
* Hardware-in-the-loop simulation
* CAD-designed gimbal/control mechanism

## Project Goals

The primary objective of IRIS is to understand and demonstrate the engineering principles behind aerospace attitude stabilization.

The project combines:

```text
Mechanical Engineering
        +
Electronics
        +
Embedded Programming
        +
Control Systems
        +
Aerospace Engineering
```

## Author

**Yug Gajjar**

Aerospace Engineering Student

Project: **IRIS Rocket Stabilization System**

## License

This project is provided for educational and experimental purposes.

Do not use this software as the sole control system for an actual flight vehicle.
