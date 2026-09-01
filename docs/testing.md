# IRIS Testing Plan

## Test 1 — IMU Connectivity

### Objective

Verify communication between the Arduino and MPU6050.

### Procedure

1. Connect the MPU6050.
2. Upload the firmware.
3. Open Serial Monitor at 115200 baud.
4. Confirm that the MPU6050 is detected.

### Expected Result

```text
MPU6050 connected.
```

---

## Test 2 — IMU Calibration

### Objective

Determine gyroscope bias.

### Procedure

Keep the sensor completely stationary while the system performs calibration.

### Expected Result

The system should complete calibration without significant movement.

---

## Test 3 — Attitude Estimation

### Objective

Verify roll and pitch estimation.

### Procedure

Rotate the sensor manually around the relevant axes.

### Expected Result

The serial telemetry should change in response to sensor movement.

---

## Test 4 — Servo Centering

### Objective

Verify actuator operation.

### Procedure

Send:

```text
C
```

### Expected Result

All four servos move to the defined center position.

---

## Test 5 — Control Direction

### Objective

Verify that servo corrections oppose disturbances.

### Procedure

Use the mechanical test rig and introduce small angular disturbances.

### Expected Result

The servo system should attempt to return the platform toward the level position.

If the system increases the disturbance instead, disable stabilization immediately and reverse the relevant control direction in the servo mixing.

---

## Test 6 — Closed-Loop Stabilization

### Objective

Evaluate PID stabilization.

### Procedure

1. Secure the test rig.
2. Enable stabilization.
3. Introduce small disturbances.
4. Record roll, pitch and gyro telemetry.
5. Evaluate settling time and oscillation.

### Parameters to Record

* Maximum angular deviation
* Settling time
* Overshoot
* Oscillation frequency
* Servo movement
* Steady-state error

## Safety

Testing must be performed on a secure ground-test rig.

Never begin testing with unrestricted actuator movement or an unsecured flight vehicle.
