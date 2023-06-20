//=====[Libraries]=============================================================

#include "tracker_GPS.h"


//=====[Declaration of private defines]========================================
#define LATENCY        20000

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut LED (LED2);
DigitalOut LED_2 (LED3);
UnbufferedSerial uartComUSB (USBTX, USBRX, 115200 );
BufferedSerial  uartGPSCom ( PG_14, PG_9, 9600 ); // debug only



//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public methods]===================================

trackerGPS::trackerGPS ()
{
    this->gsmGprs = new gsmGprsCom ( );
    this->latency = new nonBlockingDelay (LATENCY );
    this->gsmGprs->transmitionStop();
    this->numberOfDevice = 1;
    LED = ON;
    LED_2 = ON;
    TinyGPS ();
}

void trackerGPS::update ()
{
    char c  = '\0';
    static bool readyToReadNewGeo = true;
    static bool timeToSend = false;
    static char str[100] = "";
    float flat, flon;
    unsigned long age;
    if( this->latency->read() || (timeToSend == true) ) {
        timeToSend = true;

        while ( uartGPSCom.readable() ) {
            uartGPSCom.read(&c, 1); 
            if (encode(c) && (readyToReadNewGeo == true)) {
                f_get_position(&flat, &flon, &age);
                sprintf ( str, "%d|%.7f|%.7f\r\n", this->numberOfDevice ,flat, flon);
                uartComUSB.write( str, strlen(str) );
                this->gsmGprs->transmitionStart();
                readyToReadNewGeo = false;
            }
        }
        this->gsmGprs->connect ();
        this->gsmGprs->send (str);
        if (this->gsmGprs->transmitionHasEnded ()) {
            this->gsmGprs->disconnect ();
            this->gsmGprs->transmitionStop();
            if (this->gsmGprs->disconnectionProcessHasEnded()) {
                readyToReadNewGeo = true;
                timeToSend = false;
                this->latency->write (LATENCY );
            }
        }
    } 
}



//=====[Implementations of private methods]==================================
