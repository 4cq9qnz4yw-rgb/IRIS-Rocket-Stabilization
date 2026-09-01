#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Servo.h>

class ServoControl {

public:

    void begin();

    void stabilize(
        float rollCorrection,
        float pitchCorrection
    );

    void center();

private:

    Servo servo1;
    Servo servo2;
    Servo servo3;
    Servo servo4;

    int constrainServo(float angle);
};

#endif
