#include "Arduino.h"
#include "sensors.h"

void setupActuators();
void controlSpeed(int speedL = 0, int speedR = 0);
void turnLeftUntilCenter();
void turnLeftCOGUntilCenter();
void turnRightUntilCenter();
void turnRightCOGUntilCenter();