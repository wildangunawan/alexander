// Helper functions
#include "actuators.h"
#include "sensors.h"
#include "alexander.h"
#include "constants.cpp"

int dari_home = 1;

void setup()
{
  setupSensors();
  setupActuators();
}

void loop()
{
  // Dari home, kita akan find T-Junction di border home
  if (dari_home == 1)
  {
    find_line(T_JUNCTION);
    dari_home = 0;

    // Selanjutnya, kita akan ketemu Right Junction dan belok ke kanan
    find_line(RIGHT_JUNCTION);
    controlSpeed(0, 0);
    delay(300);
    turnRightUntilCenter();
  }

  // Sudah masuk ke daerah perempatan, kita cari T-Junction sebanyak 5x
  for (int i = 0; i < 5; i++)
  {
    find_line(T_JUNCTION);
  }
  delay(3000);

  // Find last T-Junction, kanan ke trolley, kiri ke jalanan biasa
  find_line(T_JUNCTION);
  controlSpeed(0, 0);
  turnLeftUntilCenter();

  // Di jalanan biasa, akan ada 2x Left Junction dan 1x Right Junction
  find_line(LEFT_JUNCTION);
  find_line(RIGHT_JUNCTION);
  find_line(LEFT_JUNCTION);

  delay(3000);

  // Kita kembali ke awal lagi, find Left Junction dekat home
  find_line(LEFT_JUNCTION);
  controlSpeed(0, 0);
  delay(1000);
  turnLeftCOGUntilCenter();
}