
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
#define APN_USER_PASS "AT+CSTT=\"wap.gprs.unifon.com.ar \",\"wap\",\"wap\"" //APN / username / password (CAMBIAR SI SE CAMBIA LA SIM!)
#define IP_PORT "AT+CIPSTART=\"TCP\",\"181.229.29.221\",\"123\"" //PROTOCOL / EXTERNAL IP / PORT


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
    this->refreshDelay =  new nonBlockingDelay ( REFRESH_TIME_10MS  ); 
    this->uartGsmGprs = new BufferedSerial ( PE_8, PE_7, 9600 ); // TX RX
    this->gsmGprsComState = GSM_GPRS_STATE_INIT;
    
}

gsmGprsCom::gsmGprsCom(BufferedSerial * serialCom) {
    this->uartGsmGprs = serialCom;
    this->gsmGprsComState = GSM_GPRS_STATE_INIT;
}

void gsmGprsCom::connect () {
    /*
    switch (this->gsmGprsComState) {
    cases
    }
    */
     //if (this->gsmGprsComState == GSM_GPRS_STATE_INIT) {

        //this->gsmGprsComState = GSM_GPRS_STATE_AT;

    this->write( "AT\r\n");
    delay (100); // DELAY BLOQUEANTE DE PRUEBA CAMBIAR
    if (isTheExpectedResponse()) {
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


 bool gsmGprsCom::isTheExpectedResponse ()
{
   static int responseStringPositionIndex = 0;
   char charReceived;
   bool moduleResponse = false;

   if( this->charRead(&charReceived) ){
      if (charReceived == GsmGprsComExpectedResponse [this->gsmGprsComState][responseStringPositionIndex]) {
         responseStringPositionIndex++;
         if (charReceived == GsmGprsComExpectedResponse [this->gsmGprsComState][responseStringPositionIndex] == '\0') {
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