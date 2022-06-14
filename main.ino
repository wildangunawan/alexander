// Helper functions
#include "actuators.h"
#include "sensors.h"

int dari_home = 1;
int perempatan = 0;
int pertigaan = 0;
int masih_diperempatan = 0;
int masih_dipertigaan = 0;
int force = 0;

/**
 * Last found line:
 * -2 - full left
 * -1 - left but so-so
 * 0 - center
 * 1 - right but so-so
 * 2 - full right
 */
int lastFound = 0;

void setup()
{
  setupSensors();
  setupActuators();

  Serial.begin(9600);
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

  // Uncomment to run only the line follower
  // basicLineFollower();

  // Uncomment to run task as requested
  runTask();
}

void basicLineFollower()
{
  if (
      readLeftIRData() == 0 && readMiddleLeftIRData() == 0 && readCenterIRData() == 0 && readMiddleRightIRData() == 0 && readRightIRData() == 0)
  {
    if (lastFound == -2)
    {
      controlSpeed(-60, 60);
    }
    else if (lastFound == -1)
    {
      controlSpeed(0, 65);
    }
    else if (lastFound == 0)
    {
      controlSpeed(60, 60);
    }
    else if (lastFound == 1)
    {
      controlSpeed(65, 0);
    }
    else if (lastFound == 2)
    {
      controlSpeed(60, -60);
    }
  }

  // Belok kiri 90 degree
  else if (readLeftIRData() && !readRightIRData())
  {
    lastFound = -2;
    controlSpeed(-60, 60);
  }

  // Belok kiri setengah-setengah
  else if (readMiddleLeftIRData() && !readRightIRData())
  {
    lastFound = -1;
    controlSpeed(0, 65);
  }

  // Belok kanan setengah-setengah
  else if (!readLeftIRData() && readMiddleRightIRData())
  {
    lastFound = 1;
    controlSpeed(65, 0);
  }

  // Belok kanan 90 degree
  else if (!readLeftIRData() && readRightIRData())
  {
    lastFound = 2;
    controlSpeed(60, -60);
    // turnRightUntilCenter();
  }

  // Lurus
  else if (readCenterIRData())
  {
    lastFound = 0;
    if (readCenterIRData() == 1 || readCenterIRData() == 3 || readCenterIRData() == 5)
    {
      controlSpeed(60, 60);
    }
    else if (readCenterIRData() == 2)
    {
      controlSpeed(0, 65);
    }
    else if (readCenterIRData() == 4)
    {
      controlSpeed(65, 0);
    }
  }
}

void runTask()
{
  int runLineFollower = 1;

  // If from home and right IR detect a line, then turn right
  if (dari_home)
  {
    // Detect first pertigaan, belok kanan
    if (!readLeftIRData() && readRightIRData())
    {
      controlSpeed(60, -60);
      delay(350);
      lastFound = 2;

      dari_home = 0;
      return;
    }
  }
  else
  {
    // Count perempatan
    if (readLeftIRData() && readRightIRData())
    {
      if (perempatan >= 5)
      {
        perempatan = 0;

        runLineFollower = 0;
        controlSpeed(-60, 60);
        lastFound = -2;
      }

      if (!masih_diperempatan)
      {
        perempatan++;
        masih_diperempatan = 1;

        runLineFollower = 0;
        controlSpeed(60, 60);
        lastFound = 0;
      }
    }

    // Count pertigaan
    else if ((readLeftIRData() || readRightIRData()) && readCenterIRData())
    {
      if (perempatan >= 5)
      {
        // Dia udah sampai ujung tapi cuman detect salah satu aja. Jadi kita bisa skip dulu ya
        perempatan = 0;

        controlSpeed(-60, 60);
        delay(350);

        lastFound = -2;
      }

      if (pertigaan >= 3)
      {
        pertigaan = 0;

        runLineFollower = 0;
        controlSpeed(-60, 60);
        lastFound = -2;
      }

      if (!masih_dipertigaan)
      {
        pertigaan++;
        masih_dipertigaan = 1;

        runLineFollower = 0;
        controlSpeed(60, 60);
        lastFound = 0;
      }
    }

    else if (!readLeftIRData() || !readRightIRData())
    {
      // Reset state
      masih_diperempatan = 0;
      masih_dipertigaan = 0;
    }
  }

  if (runLineFollower)
    basicLineFollower();
}
