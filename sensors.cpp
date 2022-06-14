#include "sensors.h"
#include "constants.cpp"

// SETUP
void setupSensors()
{
    // Ultrasonic
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // IR sensor
    pinMode(IR1, INPUT);
    pinMode(IR2, INPUT);
    pinMode(IR3, INPUT);
    pinMode(IR4, INPUT);
    pinMode(IR5, INPUT);
    pinMode(IR6, INPUT);
    pinMode(IR7, INPUT);
}

// READ DATA
// Infrared sensor
int readLeftIRData()
{
    /**
     * Read the left IR sensor and return the value.
     *
     * @return 1 if line detected, 0 otherwise
     */

    int ir = digitalRead(IR1);
    return ir;
}

int readMiddleLeftIRData()
{
    /**
     * Read the middle left IR sensor and return the value.
     *
     * @return 1 if line detected, 0 otherwise
     */

    int ir = digitalRead(IR2);
    return ir;
}

int readCenterIRData()
{
    /**
     * Read the center IR sensor and return the value.
     *
     * @return 0 if no line detected,
     *         1 if left and center IR detect a like, 2 if ONLY left IR detect a line,
     *         3 if right and center IR detect a line, 4 if ONLY right IR detect a line,
     *         5 if left and right IR detect a line
     */

    int center_left_ir = digitalRead(IR3);
    int center_center_ir = digitalRead(IR4);
    int center_right_ir = digitalRead(IR5);

    if (center_left_ir == 0 && center_right_ir == 0 && center_center_ir == 0)
    {
        return 0;
    }
    else if (center_left_ir == 1 && center_right_ir == 0)
    {
        if (center_center_ir == 1)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    else if (center_left_ir == 0 && center_right_ir == 1)
    {
        if (center_center_ir == 1)
        {
            return 3;
        }
        else
        {
            return 4;
        }
    }
    else
    {
        return 5;
    }
}

int readMiddleRightIRData()
{
    /**
     * Read the middle right IR sensor and return the value.
     *
     * @return 1 if line detected, 0 otherwise
     */

    int ir = digitalRead(IR6);
    return ir;
}

int readRightIRData()
{
    /**
     * Read the right IR sensor and return the value.
     *
     * @return 1 if line detected, 0 otherwise
     */

    int ir = digitalRead(IR7);
    return ir;
}

// Ultrasonic sensor
int readUltrasonicData()
{
    /**
     * Read the distance from the ultrasonic sensor
     *
     * @return int distance in cm
     */

    // Trigger the sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Get duration of the pulse
    long duration = pulseIn(echoPin, HIGH);

    // Calculate distance in cm
    int distanceCm = duration * 0.034 / 2;

    // Return distance in cm
    return distanceCm;
}
