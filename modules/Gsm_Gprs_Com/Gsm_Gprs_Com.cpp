
//=====[Libraries]=============================================================

#include "Gsm_Gprs_Com.h"
#include "arm_book_lib.h"
#include "wifi_com.h"
#include "mbed.h"
#include "non_Blocking_Delay.h"
//#include "pc_serial_com.h"


//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================


//=====[Declaration and initialization of public global objects]===============


//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============



//=====[Declarations (prototypes) of private functions]========================


//=====[Implementations of public methods]===================================
gsmGprsCom::gsmGprsCom ()
{
    UnbufferedSerial this->uartGsmGprs( PE_8, PE_7, 115200 );
 
}
