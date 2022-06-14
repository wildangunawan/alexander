#include "actuators.h"
#include "constants.cpp"

void setupActuators() {
    // Motor DC
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

int convertPercentageToPWM(int value) {
    /**
     * Convert the percentage to PWM value.
     *
     * @param int percentage
     * @return int PWM value
     */

    return value * 255 / 100;
}

// MOTOR DC FUNCTIONS
void controlSpeed(int speedL = 0, int speedR = 0) {
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
    if (speedR > 0) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    } else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }

    // Motor B - Kiri
    if (speedL > 0) {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    } else {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }

    // Write speed
    analogWrite(ENA, pwmR);
    analogWrite(ENB, pwmL);
}
