//=====[#include guards - begin]===============================================

#ifndef _TRACKER_GPS_H_
#define _TRACKER_GPS_H_

#include "Gsm_Gprs_Com.h"
#include "TinyGPS.h"
#include "non_Blocking_Delay.h"
#include "arm_book_lib.h"



//=====[Declaration of public defines]=========================================


//=====[Declaration of public data types]======================================

//=====[Declaration of public classes]=========================
/*
 * Class implementation for a GPS tracker
 * High hierarchy class
 * it will be instantiated and used from the main function
 * handles a GPS module, a GSM GPRS module and a delay in order to send the geolocation of the device
 * to a TCP IP server
 */
class trackerGPS {
public:
    trackerGPS ();
    void update();
private:
    gsmGprsCom * gsmGprs;
    nonBlockingDelay * latency;
    int numberOfDevice;
};


//=====[#include guards - end]=================================================

#endif // _TRACKER_GPS_H_
