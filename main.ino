// Helper functions
#include "actuators.h"
#include "sensors.h"

int dari_home = 1;
int perempatan = 0;
int pertigaan = 0;

void setup() {
  setupSensors();
  setupActuators();
}

void loop() {
  // If distance to object is less than 10 cm OR
  // IRs don't detect anything then STOP
  if (readUltrasonicData() < 30) {
    controlSpeed(0, 0);
    return;
  }

  // Uncomment to run only the line follower
  basicLineFollower();

  // Uncomment to run task as requested
  // runTask();
}

void basicLineFollower() {
  /**
    * Last found line:
    * -2 - full left
    * -1 - left but so-so
    * 0 - center
    * 1 - right but so-so
    * 2 - full right
    */
  int lastFound = 0;

  if (
    readLeftIRData() == 0 
    && readMiddleLeftIRData() == 0 
    && readCenterIRData() == 0 
    && readMiddleRightIRData() == 0 
    && readRightIRData() == 0
  ) {
    if (lastFound == -2) {
      controlSpeed(-20, 20);
    } else if (lastFound == -1) {
      controlSpeed(0, 10);
    } else if (lastFound == 0) {
      controlSpeed(20, 20);
    } else if (lastFound == 1) {
      controlSpeed(10, 0);
    } else if (lastFound == 2) {
      controlSpeed(20, -20);
    }
  }

  // Belok kiri 90 degree
  else if (readLeftIRData() && !readRightIRData()) {
    lastFound = -2;
    controlSpeed(-20, 20);
  }

  // Belok kiri setengah-setengah
  else if (readMiddleLeftIRData() && readRightIRData()) {
    lastFound = -1;
    controlSpeed(0, 10);
  }

  // Belok kanan setengah-setengah
  else if (!readLeftIRData() && readMiddleRightIRData()) {
    lastFound = 1;
    controlSpeed(10, 0);
  }

  // Belok kanan 90 degree
  else if (!readLeftIRData() && readRightIRData()) {
    lastFound = 2;
    controlSpeed(20, -20);
  }

  // Lurus
  else if (readCenterIRData()) {
    lastFound = 0;
    if (readCenterIRData() == 1 || readCenterIRData() == 3 || readCenterIRData() == 5) {
      controlSpeed(20, 20);
    } else if (readCenterIRData() == 2) {
      controlSpeed(0, 10);
    } else if (readCenterIRData() == 4) {
      controlSpeed(10, 0);
    }
  }
}

void toTry1() {
  controlSpeed(100, 100);
  delay(1000 * 5);
  controlSpeed(0, 0);
}

void toTry2() {
  controlSpeed(20, 20);
  delay(1000 * 5);
  controlSpeed(0, 0);
}

void runTask() {
  // If from home and right IR detect a line, then turn right
  if (dari_home && readRightIRData()) {
    controlSpeed(20, -20);
    dari_home = 0;
  }

  // Count perempatan
  else if (readLeftIRData() && readRightIRData()) {
    if (perempatan < 5) {
      perempatan++;
    } else {
      controlSpeed(-20, 20);
      perempatan = 0;
    } 
  }
  
  // Count pertigaan or see belokan
  else if (readLeftIRData() || readRightIRData()) {
    // Check if it's a pertigaan
    // Shown by center IR and left/right sensor
    if (readCenterIRData() > 0) {
      if (pertigaan < 3) {
        pertigaan++;
      } else {
        controlSpeed(-20, 20);
        pertigaan = 0;
      }
    } 

    // Is not a pertigaan, but we still need to turn
    // to keep on the line
    else {
      if (readLeftIRData()) controlSpeed(-20, 20);
      else if (readRightIRData()) controlSpeed(20, -20);
    }
  }
  
  // Robot doesn't detect perempatan or pertigaan
  // Therefore we can assume that it's lurus
  else {
    // We need to check if robot is on the line or deviated
    // If it's deviated, then we need to turn back
    if (readCenterIRData() == 1) {
      controlSpeed(10, 0);
    } else if (readCenterIRData() == 2) {
      controlSpeed(0, 10);
    }

    // If it's not then we can proceed as usual
    else {
      controlSpeed(20, 20);
    }
  }
}
