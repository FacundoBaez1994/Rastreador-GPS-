//=====[#include guards - begin]===============================================

#ifndef _GSM_GPRS_COM_
#define _GSM_GPRS_COM_

#include "arm_book_lib.h"
#include "mbed.h"
#include "non_Blocking_Delay.h"
#include <string>

//=====[Declaration of public defines]=========================================


//=====[Declaration of public data types]======================================
typedef enum {
    GSM_GPRS_STATE_INIT,
    GSM_GPRS_STATE_IDLE,
    GSM_GPRS_STATE_AT_TO_BE_SEND,
    GSM_GPRS_STATE_AT_WAIT_FOR_RESPONSE,
    GSM_GPRS_STATE_ATPLUSCSQ_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCSQ_WAIT_FOR_RESPONSE,
    GSM_GPRS_STATE_ATPLUSCCID_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCCID_WAIT_FOR_RESPONSE,
    GSM_GPRS_STATE_ATPLUSCGREG_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCGREG_WAIT_FOR_RESPONSE, 
    GSM_GPRS_STATE_ATPLUSCGATT_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCGATT_WAIT_FOR_RESPONSE, 
    GSM_GPRS_STATE_ATPLUSCIPSHUT_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCIPSHUT_WAIT_FOR_RESPONSE,
    GSM_GPRS_STATE_ATPLUSCIPMUX_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCIPMUX_WAIT_FOR_RESPONSE,
    GSM_GPRS_STATE_ATPLUSCSTT_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCSTT_WAIT_FOR_RESPONSE,
    GSM_GPRS_STATE_ATPLUSCIICR_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCIICR_WAIT_FOR_RESPONSE,
    
    GSM_GPRS_STATE_ATPLUSCIFSR_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCIFSR_WAIT_FOR_RESPONSE,

    GSM_GPRS_STATE_ATPLUSCIPSTART_TO_BE_SEND,
    GSM_GPRS_STATE_ATPLUSCIPSTART_WAIT_FOR_RESPONSE,
    GSM_GPRS_STATE_CONNECTION_ESTABLISHED,

    GSM_GPRS_STATE_NO_SIM_CARD,
    GSM_GPRS_STATE_NO_SIGNAL,
    GSM_GPRS_STATE_DISCONNECTED,
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
    BufferedSerial* uartGsmGprs;
    nonBlockingDelay* refreshDelay;
    std::string localIP;
    float signalLevel;

// private methods
    void checkATPLUSCIPSTARTcommand ();
    void sendATPLUSCIPSTARTcommand ();
    void checkATPLUSCIFSRcommand ();
    void sendATPLUSCIFSRcommand ();
    void checkATPLUSCIICRcommand ();
    void sendATPLUSCIICRcommand ();
    void checkATPLUSCSTTcommand ();
    void sendATPLUSCSTTcommand ();
    void checkATPLUSCIPMUXcommand ();
    void sendATPLUSCIPMUXcommand ();
    void checkATPLUSCIPSHUTcommand ();
    void sendATPLUSCIPSHUTcommand ();
    void checkATPLUSCGATTcommand ();
    void sendATPLUSCGATTcommand ();
    void checkATPLUSCGREGcommand ();
    void sendATPLUSCGREGcommand ();
    void checkATPLUSCCIDcommand ();
    void sendATPLUSCCIDcommand ();
    void checkATPLUSCSQResponse ();
    void sendATPLUSCSQcommand ();
    void sendATCommand (); 
    void checkATCommandResponse ();
    bool charRead(char* receivedChar);
    void write(const char* str);
    bool checkUARTResponse (const char* stringToCheck);    
};

//=====[#include guards - end]=================================================

#endif /* _GSM_GPRS_COM_ */
