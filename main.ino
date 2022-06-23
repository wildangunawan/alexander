// Helper functions
#include "actuators.h"
#include "sensors.h"
#include "alexander.h"
#include "constants.cpp"

int dari_home = 1;
int trolley_pulled = 0;

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
    controlSpeed(0, 0);
    delay(300);
    dari_home = 0;

    // Selanjutnya, kita akan ketemu Right Junction dan belok ke kanan
    find_line(RIGHT_JUNCTION, 0);
    controlSpeed(0, 0);
    delay(300);
    turnRightUntilCenter();
  }

  // Sudah masuk ke daerah perempatan, kita cari T-Junction sebanyak 5x
  for (int i = 0; i < 5; i++)
  {
    find_line(T_JUNCTION);
  }

  // Find last T-Junction, kanan ke trolley, kiri ke jalanan biasa
  find_line(T_JUNCTION, 0);

  // If not pulled then we grab the trolley first
  if (!trolley_pulled)
  {
    turnLeftUntilCenter();
    int start = millis();

    while (millis() - start < 1500)
    {
      basicLineFollower();
    }

    turn360LeftUntilCenter();

    // Go find trolley
    find_line(RIGHT_JUNCTION);
    find_line(T_JUNCTION, 0);
    controlSpeed(0, 0);
    delay(300);
    turn360RightUntilCenter();

    // Grab trolley, somehow
    // grabTrolley();
    controlSpeed(0, 0);
    delay(3000);
    trolley_pulled = 1;

    // Jalan biasa lagi. Akan ada satu left junction ke home, kita skip itu ya
    find_line(LEFT_JUNCTION);
  }

  // Else we run as usual in the arena
  else
  {
    turnLeftUntilCenter();
  }

  // Di jalanan biasa, akan ada 2x Left Junction dan 1x Right Junction
  find_line(LEFT_JUNCTION);
  find_line(RIGHT_JUNCTION);
  find_line(LEFT_JUNCTION);

  // Kita kembali ke awal lagi, find Left Junction dekat home
  find_line(LEFT_JUNCTION, 0);
  turnLeftUntilCenter();
}