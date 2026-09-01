#include "PIDController.h"
#include "config.h"

PIDController::PIDController(
    float kp,
    float ki,
    float kd,
    float limit
) {

    Kp = kp;
    Ki = ki;
    Kd = kd;

    outputLimit = limit;

    integral = 0;
    previousError = 0;

    firstRun = true;
}


// ------------------------------------------------------------

float PIDController::compute(
    float setpoint,
    float measurement,
    float dt
) {

    if (dt <= 0) {
        return 0;
    }

    float error = setpoint - measurement;

    // Integral
    integral += error * dt;

    // Anti-windup
    if (integral > MAX_INTEGRAL) {
        integral = MAX_INTEGRAL;
    }

    if (integral < -MAX_INTEGRAL) {
        integral = -MAX_INTEGRAL;
    }

    // Derivative
    float derivative = 0;

    if (!firstRun) {
        derivative =
            (error - previousError) / dt;
    }

    firstRun = false;

    previousError = error;

    float output =
        Kp * error +
        Ki * integral +
        Kd * derivative;

    // Output limiting
    if (output > outputLimit) {
        output = outputLimit;
    }

    if (output < -outputLimit) {
        output = -outputLimit;
    }

    return output;
}


// ------------------------------------------------------------

void PIDController::reset() {

    integral = 0;

    previousError = 0;

    firstRun = true;
}
