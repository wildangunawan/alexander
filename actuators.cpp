#include "actuators.h"
#include "constants.cpp"
#include <Servo.h>

Servo leftServo;
Servo rightServo;

void setupActuators()
{
    // Motor DC
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Servo
    leftServo.attach(LEFT_SERVO);
    rightServo.attach(RIGHT_SERVO);

    // Reset servo
    controlServo(180, 0);
}

int convertPercentageToPWM(int value)
{
    /**
     * Convert the percentage to PWM value.
     *
     * @param int percentage
     * @return int PWM value
     */

    if (value > 75)
    {
        value = 75;
    }
    else if (value < -75)
    {
        value = -75;
    }

    return value * 255 / 100;
}

// MOTOR DC FUNCTIONS
void controlSpeed(int speedL = 0, int speedR = 0)
{
    /**
     * Control the speed of the motors.
     *
     * @param int speedL
     * @param int speedR
     */

    int pwmL = convertPercentageToPWM(speedL > 0 ? speedL : -speedL);
    int pwmR = convertPercentageToPWM(speedR > 0 ? speedR : -speedR);

    // Somehow A is RIGHT and B is LEFT
    // Motor A - Kanan
    if (speedR > 0)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    }
    else
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }

    // Motor B - Kiri
    if (speedL > 0)
    {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    }
    else
    {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }

    // Write speed
    analogWrite(ENA, pwmR);
    analogWrite(ENB, pwmL);
}

void turnRightUntilCenter()
{
    uint32_t time_start = millis();
    while (millis() - time_start < 300)
    {
        controlSpeed(75, 0);
    }

    while (readCenterIRData() != 3 && readCenterIRData() != 4)
    {
        controlSpeed(60, 0);
    }
}

void turnLeftUntilCenter()
{
    uint32_t time_start = millis();
    while (millis() - time_start < 300)
    {
        controlSpeed(0, 60);
    }

    while (readCenterIRData() != 1 && readCenterIRData() != 2)
    {
        controlSpeed(0, 45);
    }
}

void turn360UntilCenter(int time_needed = 1000)
{
    uint32_t time_start = millis();
    while (millis() - time_start < time_needed)
    {
        controlSpeed(60, -60);
    }

    while (readCenterIRData() != 1 && readCenterIRData() != 2)
    {
        controlSpeed(50, -50);
    }
}

// SERVO MOTOR FUNCTIONS
void controlServo(int leftAngle = 0, int rightAngle = 0)
{
    /**
     * Control the servo motor.
     *
     * @param int left angle
     * @param int right angle
     */

    if (leftAngle > 180)
        leftAngle = 180;
    else if (leftAngle < 0)
        leftAngle = 0;

    if (rightAngle > 180)
        rightAngle = 180;
    else if (rightAngle < 0)
        rightAngle = 0;

    // Push data
    leftServo.write(leftAngle);
    rightServo.write(rightAngle);
}