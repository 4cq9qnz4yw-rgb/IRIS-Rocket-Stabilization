#include "ServoControl.h"
#include "config.h"


void ServoControl::begin() {

    servo1.attach(SERVO_1_PIN);
    servo2.attach(SERVO_2_PIN);
    servo3.attach(SERVO_3_PIN);
    servo4.attach(SERVO_4_PIN);

    center();
}


// ------------------------------------------------------------

int ServoControl::constrainServo(float angle) {

    if (angle < SERVO_MIN) {
        angle = SERVO_MIN;
    }

    if (angle > SERVO_MAX) {
        angle = SERVO_MAX;
    }

    return (int)angle;
}


// ------------------------------------------------------------
// Four-servo control mixing
//
// Servo arrangement:
//
//          FRONT
//      S1          S2
//
//      S3          S4
//
//          REAR
//
// Roll and pitch corrections are mixed into
// opposing servo movements.
// ------------------------------------------------------------

void ServoControl::stabilize(
    float rollCorrection,
    float pitchCorrection
) {

    float s1 =
        SERVO_CENTER
        + pitchCorrection
        + rollCorrection;

    float s2 =
        SERVO_CENTER
        + pitchCorrection
        - rollCorrection;

    float s3 =
        SERVO_CENTER
        - pitchCorrection
        + rollCorrection;

    float s4 =
        SERVO_CENTER
        - pitchCorrection
        - rollCorrection;

    servo1.write(constrainServo(s1));
    servo2.write(constrainServo(s2));
    servo3.write(constrainServo(s3));
    servo4.write(constrainServo(s4));
}


// ------------------------------------------------------------

void ServoControl::center() {

    servo1.write(SERVO_CENTER);
    servo2.write(SERVO_CENTER);
    servo3.write(SERVO_CENTER);
    servo4.write(SERVO_CENTER);
}
