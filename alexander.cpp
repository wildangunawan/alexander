#include "alexander.h"
#include "constants.cpp"

int lastError = 0;
const float Kp = 1.4;
const float Kd = 1.1;

void basicLineFollower(int baseSpeed = 45)
{
    int error;

    if (readUltrasonicData() < 20)
    {
        controlSpeed(0, 0);
        return;
    }

    if (
        !readMiddleLeftIRData() &&
        !readCenterIRData() &&
        !readMiddleRightIRData())
    {
        error = lastError;
    }

    // Lurus
    else if (readCenterIRData())
    {
        if (readCenterIRData() == 1 || readCenterIRData() == 3 || readCenterIRData() == 5)
            error = 0;
        else if (readCenterIRData() == 2)
            error = -10;
        else if (readCenterIRData() == 4)
            error = 10;
    }

    // Kiri tengah
    else if (readMiddleLeftIRData() && !readRightIRData())
    {
        error = -30;
    }

    // Kanan tengah
    else if (!readLeftIRData() && readMiddleRightIRData())
    {
        error = 30;
    }

    // Calculate derivative of the error
    int DError = error - lastError;

    // Calculate additional speed needed
    float additionalSpeed = Kp * error + Kd * DError;

    if (error == 0)
        controlSpeed(55, 55);
    else
        controlSpeed(baseSpeed + additionalSpeed, baseSpeed - additionalSpeed);

    // Save current error as last error
    lastError = error;
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