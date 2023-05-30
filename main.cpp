//=====[Libraries]=============================================================

#include "tracker_GPS.h"

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    trackerGPS tracker;
    while (true) {
        tracker.update();
    }
}