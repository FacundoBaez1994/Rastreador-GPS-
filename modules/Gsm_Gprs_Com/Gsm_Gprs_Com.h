//=====[#include guards - begin]===============================================

#ifndef _GSM_GPRS_COM_
#define _GSM_GPRS_COM_

#include "arm_book_lib.h"
#include "mbed.h"
#include "non_Blocking_Delay.h"

//=====[Declaration of public defines]=========================================
typedef enum {
    GSM_GPRS_STATE_INIT,
    GSM_GPRS_STATE_AT,
} GsmGprsComState_t;
//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================

//=====[Declaration of public classes]=========================
class gsmGprsCom {
public:
    gsmGprsCom ();
    void conect ();
    void send (char * mensaje);
    // char* recv (char * mensaje);
    void disconect ();
private:
    GsmGprsComState_t GsmGprsCom;
    UnbufferedSerial  uartGsmGprs;
};

//=====[#include guards - end]=================================================

#endif /* _GSM_GPRS_COM_ */
