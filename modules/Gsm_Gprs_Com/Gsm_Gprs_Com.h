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
    GSM_GPRS_STATE_AT_TO_BE_SEND,
    GSM_GPRS_STATE_AT_SENT,
    GSM_GPRS_STATE_ATPLUSCSQ_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCSQ_SENT,
    GSM_GPRS_STATE_NO_SIGNAL,
} gsmGprsComState_t;

/*
 AT+CSQ //Signal quality test, value range is 0-31 , 31 is the best
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  mySerial.println("AT+COPS?");
*/
//=====[Declarations (prototypes) of public functions]=========================



//=====[Declaration of public classes]=========================
class gsmGprsCom {

public:
// public methods
    gsmGprsCom ();
    gsmGprsCom (BufferedSerial * uartGsmGprs);
    void connect ();
    void send (char * mensaje);
    // char* recv (char * mensaje);
    void disconnect ();

private:
// private attributtes
    gsmGprsComState_t gsmGprsComState;
    BufferedSerial * uartGsmGprs;
    char GsmGprsComExpectedResponse [20][20] = {"OK", "??"}; //Chequear largos
    nonBlockingDelay * refreshDelay;

// private methods
    bool charRead( char* receivedChar);
    void write( const char* str );
    bool isTheExpectedResponse ();
};

//=====[#include guards - end]=================================================

#endif /* _GSM_GPRS_COM_ */
