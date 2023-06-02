//=====[#include guards - begin]===============================================

#ifndef _GSM_GPRS_COM_
#define _GSM_GPRS_COM_

#include "arm_book_lib.h"
#include "mbed.h"
#include "non_Blocking_Delay.h"

//=====[Declaration of public defines]=========================================


//=====[Declaration of public data types]======================================
typedef enum {
    GSM_GPRS_STATE_INIT,
    GSM_GPRS_STATE_AT,
} gsmGprsComState_t;

//=====[Declarations (prototypes) of public functions]=========================



//=====[Declaration of public classes]=========================
class gsmGprsCom {

public:
// public methods
    gsmGprsCom ();
    gsmGprsCom (UnbufferedSerial * uartGsmGprs);
    void connect ();
    void send (char * mensaje);
    // char* recv (char * mensaje);
    void disconnect ();

private:
// private attributtes
    gsmGprsComState_t gsmGprsComState;
    UnbufferedSerial * uartGsmGprs;
    char GsmGprsComExpectedResponse [20][20] = {"OK", "??"}; //Chequear largos
    nonBlockingDelay * refreshDelay;

// private methods
    bool charRead( char* receivedChar);
    void write( const char* str );
    bool isTheExpectedResponse ();
};

//=====[#include guards - end]=================================================

#endif /* _GSM_GPRS_COM_ */
