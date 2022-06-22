#include "alexander.h"
#include "constants.cpp"

/**
 * Last found line:
 * -2 - left
 * -1 - left but so-so
 * 0 - center
 * 1 - right but so-so
 * 2 - right
 */
int lastFound = 0;
int lastError = 0;
const int Kp = 2;
const int Kd = 0;

void basicLineFollower(int baseSpeed = 50)
{
    int error;

    if (readUltrasonicData() < 20)
    {
        controlSpeed(0, 0);
        return;
    }

    if (!readLeftIRData() && !readMiddleLeftIRData() && !readCenterIRData() && !readMiddleRightIRData() && !readRightIRData())
    {
        if (lastFound == 2)
        {
            controlSpeed(-50, 50);
        }
        else if (lastFound == -1)
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
        else if (lastFound == 2)
        {
            controlSpeed(50, -50);
        }
    }
    else
    {
        // Lurus
        if (readCenterIRData())
        {
            lastFound = 0;

            if (readCenterIRData() == 1 || readCenterIRData() == 3 || readCenterIRData() == 5)
                error = 0;
            else if (readCenterIRData() == 2)
                error = -10;
            else if (readCenterIRData() == 4)
                error = 10;
        }

        // Kiri pojok
        else if (readLeftIRData() && !readRightIRData())
        {
            lastFound = -2;
            error = -50;
        }

        // Kiri tengah
        else if (readMiddleLeftIRData() && !readRightIRData())
        {
            lastFound = -1;
            error = -25;
        }

        // Kanan tengah
        else if (!readLeftIRData() && readMiddleRightIRData())
        {
            lastFound = 1;
            error = 25;
        }

        // Kanan pojok
        else if (readLeftIRData() && !readRightIRData())
        {
            lastFound = 2;
            error = 50;
        }

        // Calculate derivative of the error
        int DError = error - lastError;

        // Calculate additional speed needed
        int additionalSpeed = Kp * error + Kd * DError;
        controlSpeed(baseSpeed + additionalSpeed, baseSpeed - additionalSpeed);

        // Save current error as last error
        int lastError = error;
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