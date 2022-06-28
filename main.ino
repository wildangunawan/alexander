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

    // Selanjutnya, kita akan ketemu Right Junction dan belok ke kanan
    find_line(RIGHT_JUNCTION, 0);
    controlSpeed(0, 0);
    delay(300);
    turnRightUntilCenter();
    dari_home = 0;
  }

  // Sudah masuk ke daerah perempatan, kita cari T-Junction sebanyak 5x
  for (int i = 0; i < 5; i++)
  {
    find_line(T_JUNCTION);
    controlSpeed(0, 0);
    delay(100);
  }

  // Find last T-Junction, kanan ke trolley, kiri ke jalanan biasa
  find_line(T_JUNCTION, 0);
  controlSpeed(0, 0);
  delay(300);
  turnLeftUntilCenter();

  // If not pulled then we grab the trolley first
  if (!trolley_pulled)
  {
    int start = millis();

    while (millis() - start < 2500)
    {
      basicLineFollower();
    }

    turn360UntilCenter();

    // Go find trolley
    find_line(RIGHT_JUNCTION);
    controlSpeed(50, 50);
    delay(750);

    // Putar balik
    turn360UntilCenter();
    turn360UntilCenter(100);

    // Bernafas dulu
    controlSpeed(0, 0);
    delay(300);

    // Mundur
    controlSpeed(-100, -99);
    delay(300);
    controlSpeed(0, 0);

    // Grab trolley
    grabTrolley();
    trolley_pulled = 1;

    // Jalan biasa lagi. Akan ada satu left junction ke home, kita skip itu ya
    find_line(LEFT_JUNCTION);
  }

  // Di jalanan biasa, akan ada 2x Left Junction dan 1x Right Junction
  find_line(LEFT_JUNCTION);
  find_line(RIGHT_JUNCTION);
  find_line(LEFT_JUNCTION);

  // Kita kembali ke awal lagi, find Left Junction dekat home
  find_line(LEFT_JUNCTION, 0);
  turnLeftUntilCenter();
}