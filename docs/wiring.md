# IRIS Wiring

## MPU6050

Connect the MPU6050 using I²C.

| MPU6050 | Arduino UNO R4 WiFi                       |
| ------- | ----------------------------------------- |
| SDA     | SDA                                       |
| SCL     | SCL                                       |
| GND     | GND                                       |
| VCC     | According to breakout-board specification |

## Servos

| Servo   | Arduino Pin |
| ------- | ----------- |
| Servo 1 | D3          |
| Servo 2 | D5          |
| Servo 3 | D6          |
| Servo 4 | D9          |

## Important Power Note

Do **not** assume that the Arduino's 5V rail can safely supply four SG90 servos under load.

Use an appropriate external regulated servo supply and connect the grounds together:

```text
External Servo Supply
       │
       ├──── Servo 1
       ├──── Servo 2
       ├──── Servo 3
       └──── Servo 4

External Supply GND
       │
       └──────── Arduino GND
```

Keep the sensor supply clean and physically separated from noisy servo power where practical.
