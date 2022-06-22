#include "alexander.h"
#include "constants.cpp"

/**
 * Last found line:
 * -1 - left but so-so
 * 0 - center
 * 1 - right but so-so
 */
int lastFound = 0;

void basicLineFollower()
{
    if (readUltrasonicData() < 20)
    {
        controlSpeed(0, 0);
        return;
    }

    if (!readMiddleLeftIRData() && !readCenterIRData() && !readMiddleRightIRData())
    {
        if (lastFound == -1)
        {
            controlSpeed(0, 50);
        }
        else if (lastFound == 0)
        {
            controlSpeed(50, 50);
        }
        else if (lastFound == 1)
        {
            controlSpeed(50, 0);
        }
    }

    // Belok kiri setengah-setengah
    else if (readMiddleLeftIRData() && !readRightIRData())
    {
        lastFound = -1;
        controlSpeed(0, 50);
    }

    // Belok kanan setengah-setengah
    else if (!readLeftIRData() && readMiddleRightIRData())
    {
        lastFound = 1;
        controlSpeed(50, 0);
    }

    // Lurus
    else if (readCenterIRData())
    {
        lastFound = 0;
        if (readCenterIRData() == 1 || readCenterIRData() == 3 || readCenterIRData() == 5)
        {
            controlSpeed(50, 50);
        }
        else if (readCenterIRData() == 2)
        {
            controlSpeed(0, 50);
        }
        else if (readCenterIRData() == 4)
        {
            controlSpeed(50, 0);
        }
    }
}

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
        while (left_found == 0 || (left_found == 1 && !readMiddleLeftIRData()))
        {
            if (readLeftIRData() && readCenterIRData())
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
        while (right_found == 0 || (right_found == 1 && !readMiddleRightIRData()))
        {
            if (readRightIRData() && readCenterIRData())
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