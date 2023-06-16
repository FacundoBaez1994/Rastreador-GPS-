
//=====[Libraries]=============================================================
#include "Gsm_Gprs_Com.h"
#include "arm_book_lib.h"
#include "wifi_com.h"
#include "mbed.h"
#include "string.h"
#include "non_Blocking_Delay.h"
//#include "pc_serial_com.h"

//=====[Declaration of private defines]========================================
#define REFRESH_TIME_10MS         10
#define REFRESH_TIME_1000MS       1000
#define APN_USER_PASS "AT+CSTT=\"wap.gprs.unifon.com.ar \",\"wap\",\"wap\"" //APN / username / password (CAMBIAR SI SE CAMBIA LA SIM!)
#define IP_PORT "AT+CIPSTART=\"TCP\",\"181.229.29.221\",\"123\"" //PROTOCOL / EXTERNAL IP / PORT
#define DEBUG

//=====[Declaration of private functions]=====================================
static void pcSerialComCharWrite( char chr );

//=====[Declaration and initialization of public global objects]===============


//=====[Declaration of external public global variables]=======================


//=====[Declaration and initialization of public global variables]=============
UnbufferedSerial uartUsb(USBTX, USBRX, 115200 ); // debug only

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public methods]===================================

gsmGprsCom::gsmGprsCom() {
    this->refreshDelay =  new nonBlockingDelay ( REFRESH_TIME_1000MS  ); 
    this->uartGsmGprs = new BufferedSerial ( PE_8, PE_7, 9600 ); // TX RX
    this->gsmGprsComState = GSM_GPRS_STATE_INIT;
    
}

gsmGprsCom::gsmGprsCom(BufferedSerial * serialCom) {
    this->uartGsmGprs = serialCom;
    this->gsmGprsComState = GSM_GPRS_STATE_INIT;
}

void gsmGprsCom::connect () {
    switch (this->gsmGprsComState) { // Se puede cambiar a un arreglo de punteros a array o por un patron de diseño
        case GSM_GPRS_STATE_INIT:
            this->gsmGprsComState = GSM_GPRS_STATE_AT_TO_BE_SEND;
            break;

        case GSM_GPRS_STATE_AT_TO_BE_SEND:
            this->sendATCommand ();
            break;
            
        case GSM_GPRS_STATE_AT_WAIT_FOR_RESPONSE:
            this->checkATCommandResponse ();
            if (this->refreshDelay->read ()) {
                this->gsmGprsComState = GSM_GPRS_STATE_ERROR;
            }
            break;

        case  GSM_GPRS_STATE_ATPLUSCSQ_TO_BE_SEND:
            break;

        case GSM_GPRS_STATE_ERROR:
            #ifdef DEBUG
            uartUsb.write( "Error\r\n",  6 );  // debug only
            #endif
            this->gsmGprsComState = GSM_GPRS_STATE_AT_TO_BE_SEND; // Vuelve al primer estado
            break;
    }
    
 }

 void gsmGprsCom::sendATCommand (  ) {
    this->write( "AT\r\n");
    this->refreshDelay->write( REFRESH_TIME_1000MS );
    this->gsmGprsComState =  GSM_GPRS_STATE_AT_WAIT_FOR_RESPONSE;
}

void gsmGprsCom::checkATCommandResponse (  ) {
    char expectedResponse [] = "OK";
    if (checkUARTResponse (expectedResponse )) {
        this->gsmGprsComState = GSM_GPRS_STATE_ATPLUSCSQ_TO_BE_SEND;
        #ifdef DEBUG
        uartUsb.write( "Se leyo correctamente OK \r\n",  26 );  // debug only
        #endif
    } 
}

void gsmGprsCom::write( const char* str ) {
    this->uartGsmGprs->write( str, strlen(str) );
}

// Compara caracter pasado como parametro contra caracter recibido por UART
bool gsmGprsCom::charRead( char* receivedChar )
{
    char receivedCharLocal = '\0';
    if( this->uartGsmGprs->readable() ) {
        this->uartGsmGprs->read(&receivedCharLocal,1);
        pcSerialComCharWrite (receivedCharLocal); // debug only
        *receivedChar = receivedCharLocal;       
        return true;
    }
    return false;
}


 bool gsmGprsCom::checkUARTResponse (const char* stringToCheck)
{
   static int responseStringPositionIndex = 0;
   char charReceived;
   bool moduleResponse = false;

   if( this->charRead(&charReceived) ){
      if (charReceived == stringToCheck [responseStringPositionIndex]) {
         responseStringPositionIndex++;
         if (charReceived ==stringToCheck [responseStringPositionIndex] == '\0') {
            responseStringPositionIndex = 0;
            moduleResponse = true;
         }
      } else {
         responseStringPositionIndex = 0;
      }
   }
   return moduleResponse;
}

// debug only
static void pcSerialComCharWrite( char chr )  {
    char str[2] = "";
    sprintf (str, "%c", chr);
    uartUsb.write( str, strlen(str) );
}