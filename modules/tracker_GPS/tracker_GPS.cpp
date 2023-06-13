//=====[Libraries]=============================================================

#include "tracker_GPS.h"


//=====[Declaration of private defines]========================================
#define DELAY_2_SECONDS         2000

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut LED (LED2);
DigitalOut LED_2 (LED3);
UnbufferedSerial uartComUSB (USBTX, USBRX, 115200 );
UnbufferedSerial  uartGPSCom ( PG_14, PG_9, 9600 ); // debug only



//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public methods]===================================

trackerGPS::trackerGPS ()
{
    this->gsmGprs = new gsmGprsCom ( );
    this->latency = new nonBlockingDelay ( DELAY_2_SECONDS);
    LED = ON;
    LED_2 = ON;
    TinyGPS ();
}

void trackerGPS::update ()
{  
    char c  = '\0';
    char str[100] = "";
    float flat, flon;
    unsigned long age;

    while ( uartGPSCom.readable() ) {
        uartGPSCom.read(&c, 1); 
        encode(c);
        f_get_position(&flat, &flon, &age);
        sprintf ( str, "lat: %.6f  long: %.6f ", flat, flon);
        uartComUSB.write( str, strlen(str) );
    }
     
//    this->gsmGprs->connect ();

}

//=====[Implementations of private methods]==================================
