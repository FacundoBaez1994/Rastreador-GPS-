//=====[Libraries]=============================================================

#include "arm_book_lib.h"
#include "tracker_GPS.h"
#include "non_Blocking_Delay.h"


//=====[Declaration of private defines]========================================
#define DELAY_2_SECONDS         2000

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

nonBlockingDelay latency ( DELAY_2_SECONDS);
DigitalOut LED (LED2);
DigitalOut LED_2 (LED3);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public methods]===================================

trackerGPS::trackerGPS ()
{
    this->gsmGprs = new gsmGprsCom ( );
    LED = ON;
    LED_2 = ON;
}

void trackerGPS::update ()
{
    gsmGprs->connect ();
    
    if( latency.read() ) {
        LED = !LED;
    }    
   //wifiComUpdate();
}

//=====[Implementations of private methods]==================================
