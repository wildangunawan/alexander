#include "alexander.h"

int dari_home = 1;

/**
 * Last found line:
 * -2 - full left
 * -1 - left but so-so
 * 0 - center
 * 1 - right but so-so
 * 2 - full right
 */
int lastFound = 0;

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
    //    else if (!readLeftIRData() && readRightIRData())
    //    {
    //        lastFound = 2;
    //        controlSpeed(60, -60);
    //    }

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

int T_JUNCTION = 1;
int LEFT_JUNCTION = 2;
int RIGHT_JUNCTION = 3;

void find_line(int type, int pass_through = 1)
{
    int left_found = 0;
    int right_found = 0;

    if (type == T_JUNCTION)
    {
        while (left_found == 0 || right_found == 0)
        {
            if (readLeftIRData() == 1)
            {
                left_found = 1;
            }
            if (readRightIRData() == 1)
            {
                right_found = 1;
            }

            basicLineFollower();
        }

        if (pass_through)
        {
            while (readLeftIRData() == 1 && readRightIRData() == 1)
            {
                basicLineFollower();
            }
        }
    }
    else if (type == LEFT_JUNCTION)
    {
        while (left_found == 0)
        {
            if (readLeftIRData() == 1)
            {
                left_found = 1;
            }

            basicLineFollower();
        }

        if (pass_through)
        {
            while (readLeftIRData() == 1)
            {
                basicLineFollower();
            }
        }
    }
    else if (type == RIGHT_JUNCTION)
    {
        while (right_found == 0)
        {
            if (readRightIRData() == 1)
            {
                right_found = 1;
            }

            basicLineFollower();
        }

        if (pass_through)
        {
            while (readRightIRData() == 1)
            {
                basicLineFollower();
            }
        }
    }
}

void runTask()
{
    // Dari home, kita akan find T-Junction di border home
    if (dari_home == 1)
    {
        find_line(T_JUNCTION);
        controlSpeed(0, 0);
        delay(300);
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
        controlSpeed(0, 0);
        delay(300);
    }

    // Find last T-Junction, kanan ke trolley, kiri ke jalanan biasa
    find_line(T_JUNCTION);
    controlSpeed(0, 0);
    turnLeftUntilCenter();

    // Di jalanan biasa, akan ada 2x Left Junction dan 1x Right Junction
    find_line(LEFT_JUNCTION);
    controlSpeed(0, 0);

    find_line(RIGHT_JUNCTION);
    controlSpeed(0, 0);

    find_line(LEFT_JUNCTION);
    controlSpeed(0, 0);

    // Kita kembali ke awal lagi, find Left Junction dekat home
    find_line(LEFT_JUNCTION);
    controlSpeed(0, 0);
    delay(500);
    turnLeftUntilCenter();
}