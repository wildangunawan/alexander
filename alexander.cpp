#include "alexander.h"

int dari_home = 1;
int left_home = 0;
int right_home = 0;

int perempatan = 0;
int pertigaan = 0;

int garis_kanan = 0;
int garis_kiri = 0;

int masih_detect_kiri = 0;
int masih_detect_kanan = 0;

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
    else if (!readLeftIRData() && readRightIRData())
    {
        lastFound = 2;
        controlSpeed(60, -60);
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
        // Forget about all first detected left and right
        if (readLeftIRData())
            left_home = 1;
        if (readRightIRData())
            right_home = 1;

        // Detect first pertigaan, belok kanan
        if (left_home && right_home && !readLeftIRData() && readRightIRData())
        {
            turnRightUntilCenter();
            dari_home = 0;
        }
    }
    else
    {
        if (perempatan >= 5)
        {
            perempatan = 0;

            turnLeftUntilCenter();
            lastFound = -2;
        }

        if (pertigaan >= 3)
        {
            pertigaan = 0;

            turnLeftUntilCenter();
            lastFound = -2;
        }

        // Count pertigaan
        if ((readLeftIRData() || readRightIRData()) && readCenterIRData())
        {
            if (readLeftIRData() && !masih_detect_kiri)
            {
                garis_kiri++;
                masih_detect_kiri = 1;
            }
            else
                masih_detect_kiri = 0;

            if (readRightIRData() && !masih_detect_kanan)
            {
                garis_kanan++;
                masih_detect_kanan = 1;
            }
            else
                masih_detect_kanan = 0;

            if (garis_kiri == garis_kanan && garis_kiri != 0 && garis_kanan != 0)
            {
                perempatan++;
                garis_kiri = 0;
                garis_kanan = 0;
            }

            if (garis_kiri == 2 && garis_kanan == 1)
                pertigaan = 3;
        }
    }

    if (runLineFollower)
        basicLineFollower();
}
