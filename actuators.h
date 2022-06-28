#include "Arduino.h"
#include "sensors.h"

void setupActuators();
void controlSpeed(int speedL = 0, int speedR = 0);
void turnLeftUntilCenter();
void turnRightUntilCenter();
void turn360UntilCenter(int time_needed = 1000);
void controlServo(int leftAngle = 0, int rightAngle = 0);