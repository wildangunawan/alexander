// Helper functions
#include "actuators.h"
#include "sensors.h"
#include "alexander.h"

void setup()
{
  setupSensors();
  setupActuators();
}

void loop()
{
  // If distance to object is less than 10 cm OR
  // IRs don't detect anything then STOP
  if (readUltrasonicData() < 30)
  {
    controlSpeed(0, 0);
    return;
  }

  // Uncomment to run task as requested
  runTask();
}