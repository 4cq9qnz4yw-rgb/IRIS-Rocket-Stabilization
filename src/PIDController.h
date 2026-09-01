#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

class PIDController {

public:

    PIDController(
        float kp,
        float ki,
        float kd,
        float outputLimit
    );

    float compute(
        float setpoint,
        float measurement,
        float dt
    );

    void reset();

private:

    float Kp;
    float Ki;
    float Kd;

    float outputLimit;

    float integral;
    float previousError;

    bool firstRun;
};

#endif
