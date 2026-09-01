# IRIS System Overview

## Objective

IRIS demonstrates closed-loop attitude stabilization using an inertial sensor, microcontroller and servo actuators.

## Signal Flow

```text
MPU6050
   │
   ▼
Raw Accelerometer + Gyroscope Data
   │
   ▼
Sensor Calibration
   │
   ▼
Complementary Filter
   │
   ▼
Roll / Pitch Estimation
   │
   ▼
PID Controller
   │
   ▼
Control Correction
   │
   ▼
Four-Servo Mixing
   │
   ▼
Servo Actuators
   │
   ▼
Mechanical Test Platform
   │
   └────────────── Feedback ──────────────► MPU6050
```

## Closed-Loop Control

The system continuously measures its orientation and compares it against the desired level orientation.

```text
Desired Angle
      │
      ▼
   [ Error ] ◄──── Measured Angle
      │
      ▼
     PID
      │
      ▼
 Servo Commands
      │
      ▼
 Mechanical Response
      │
      ▼
    MPU6050
```

This feedback loop is the fundamental principle behind the stabilization demonstrator.
