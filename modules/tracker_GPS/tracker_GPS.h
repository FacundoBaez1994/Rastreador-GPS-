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
class trackerGPS {
public:
    trackerGPS ();
    void update();
private:
    gsmGprsCom * gsmGprs;
    nonBlockingDelay * latency;
};


//=====[#include guards - end]=================================================

#endif // _TRACKER_GPS_H_
