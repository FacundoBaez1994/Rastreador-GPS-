
//=====[Libraries]=============================================================
#include "Gsm_Gprs_Com.h"
#include "arm_book_lib.h"
#include "wifi_com.h"
#include "mbed.h"
#include "string.h"
#include "non_Blocking_Delay.h"
#include <string>


//=====[Declaration of private defines]========================================
#define REFRESH_TIME_10MS         10
#define REFRESH_TIME_1000MS       1000
#define APN_USER_PASS "AT+CSTT=\"wap.gprs.unifon.com.ar \",\"wap\",\"wap\"" //APN / username / password (CAMBIAR SI SE CAMBIA LA SIM!)
#define IP_PORT "AT+CIPSTART=\"TCP\",\"181.229.29.221\",\"123\"" //PROTOCOL / EXTERNAL IP / PORT
#define DEBUG
#define LOW_LEVEL_SIGNAL 6
#define CCID_VERIFICATION "8954078100795517486f" // (CAMBIAR SI SE CAMBIA LA SIM!)

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
    this->signalLevel = 0;
    this->gsmGprsComState = GSM_GPRS_STATE_INIT;
}

gsmGprsCom::gsmGprsCom(BufferedSerial * serialCom) {
    this->uartGsmGprs = serialCom;
    this->gsmGprsComState = GSM_GPRS_STATE_INIT;
}

void gsmGprsCom::connect () {
    switch (this->gsmGprsComState) { // Se puede cambiar a un arreglo de punteros a array o por un patron de diseño
        case GSM_GPRS_STATE_INIT: {
            this->gsmGprsComState = GSM_GPRS_STATE_AT_TO_BE_SEND;
        } break;

        case GSM_GPRS_STATE_AT_TO_BE_SEND: {
            this->sendATCommand ();
        } break;
            
        case GSM_GPRS_STATE_AT_WAIT_FOR_RESPONSE: {
            this->checkATCommandResponse ();
            if (this->refreshDelay->read ()) {
                this->gsmGprsComState = GSM_GPRS_STATE_DISCONNECTED;
            }
        } break;

        case  GSM_GPRS_STATE_ATPLUSCSQ_TO_BE_SEND:  {
            this->sendATPLUSCSQcommand ();
            break;
        } 

        case  GSM_GPRS_STATE_ATPLUSCSQ_WAIT_FOR_RESPONSE:  {
            this->checkATPLUSCSQResponse  ();
            if (this->refreshDelay->read ()) {
                this->gsmGprsComState = GSM_GPRS_STATE_NO_SIGNAL;
            }
        } break;

        case  GSM_GPRS_STATE_ATPLUSCCID_TO_BE_SEND: {
            this->sendATPLUSCCIDcommand ();
            break;
        } break;

        case  GSM_GPRS_STATE_ATPLUSCCID_WAIT_FOR_RESPONSE:  {
            this->checkATPLUSCCIDcommand  ();
            if (this->refreshDelay->read ()) {
                this->gsmGprsComState = GSM_GPRS_STATE_NO_SIM_CARD;
            }
        } break;

        case  GSM_GPRS_STATE_ATPLUSCGREG_TO_BE_SEND:  { //CREG SEND
            this->sendATPLUSCGREGcommand ();
        } break;

        case  GSM_GPRS_STATE_ATPLUSCGREG_WAIT_FOR_RESPONSE:  {  //CREG WAIT
            this->checkATPLUSCGREGcommand  ();
            if (this->refreshDelay->read ()) {
               this->gsmGprsComState = GSM_GPRS_STATE_ATPLUSCGREG_TO_BE_SEND;
                uartUsb.write ( "\r\n",  3 );  // debug only
                char msg []  = "ATPLUSCREG? NOT command responded correctly \r\n";
                uartUsb.write( msg, strlen (msg) );  // debug only
                uartUsb.write ( "\r\n",  3 );  // debug only
            }
        } break;

       case  GSM_GPRS_STATE_ATPLUSCGATT_TO_BE_SEND:  { //CGATT SEND
            this->sendATPLUSCGATTcommand ();
        } break;

        case  GSM_GPRS_STATE_ATPLUSCGATT_WAIT_FOR_RESPONSE:  {  //CGATT WAIT
            this->checkATPLUSCGATTcommand  ();
            if (this->refreshDelay->read ()) {
               this->gsmGprsComState = GSM_GPRS_STATE_ATPLUSCGREG_TO_BE_SEND;
                uartUsb.write ( "\r\n",  3 );  // debug only
                char msg []  = "AT+GATT=1 NOT command responded correctly \r\n";
                uartUsb.write( msg, strlen (msg) );  // debug only
                uartUsb.write ( "\r\n",  3 );  // debug only
            }
        } break;
        

        case  GSM_GPRS_STATE_NO_SIM_CARD:  {
            #ifdef DEBUG
            uartUsb.write ( "\r\n",  3 );  // debug only
            char messageModuleWithoutSIMCard []  = "The Tracker cannot read the SIM CARD - If the error persist Please check if the Sim Card it's installed correctly\r\n";
            uartUsb.write (messageModuleWithoutSIMCard ,  strlen (messageModuleWithoutSIMCard) );  // debug only
            uartUsb.write ( "\r\n",  3 );  // debug only
            #endif
            this->gsmGprsComState = GSM_GPRS_STATE_ATPLUSCCID_TO_BE_SEND;
        } break;

        case GSM_GPRS_STATE_DISCONNECTED: {
            #ifdef DEBUG
            uartUsb.write ( "\r\n",  3 );  // debug only
            char messageModuleDisconected []  = "GSM GPRS Module disconnected or plug wrongly\r\n";
            uartUsb.write (messageModuleDisconected,  strlen (messageModuleDisconected) );  // debug only
            uartUsb.write ( "\r\n",  3 );  // debug only
            #endif
            this->gsmGprsComState = GSM_GPRS_STATE_AT_TO_BE_SEND; // Vuelve al primer estado
        } break;

        case GSM_GPRS_STATE_NO_SIGNAL: {
            #ifdef DEBUG
            uartUsb.write ( "\r\n",  3 );  // debug only
            char messageModuleWithNoSignal []  = "GSM GPRS Module without signal\r\n";
            uartUsb.write (messageModuleWithNoSignal,  strlen (messageModuleWithNoSignal) );  // debug only
            uartUsb.write ( "\r\n",  3 );  // debug only
            #endif
            this->gsmGprsComState = GSM_GPRS_STATE_ATPLUSCSQ_TO_BE_SEND; 
        } break;

        case  GSM_GPRS_STATE_IDLE: {
        } break;
    }
}

void gsmGprsCom::checkATPLUSCGATTcommand ()  {
    char expectedResponse [] = "OK";
    if (checkUARTResponse (expectedResponse )) {
        this->gsmGprsComState = GSM_GPRS_STATE_IDLE ;
        #ifdef DEBUG
        uartUsb.write ( "\r\n",  3 );  // debug only
        char msg []  = "Device attach to the GPRS Network \r\n";
        uartUsb.write( msg, strlen (msg) );  // debug only
        uartUsb.write ( "\r\n",  3 );  // debug only
        #endif
    }
}

void gsmGprsCom::sendATPLUSCGATTcommand ()  {
    uartUsb.write ("\r\n ", 3 );  // debug on
    this->write( "AT+CGATT=1\r\n");
    uartUsb.write ("\r\n ", 3 );  // debug on
    this->refreshDelay->write( REFRESH_TIME_1000MS );
    this->gsmGprsComState = GSM_GPRS_STATE_ATPLUSCGATT_WAIT_FOR_RESPONSE;
}

void gsmGprsCom::checkATPLUSCGREGcommand ()  {
    char expectedResponse [] = "OK";
    if (checkUARTResponse (expectedResponse )) {
        this->gsmGprsComState = GSM_GPRS_STATE_ATPLUSCGATT_TO_BE_SEND ;
        #ifdef DEBUG
        uartUsb.write ( "\r\n",  3 );  // debug only
        char msg []  = "Device Registed and operational \r\n";
        uartUsb.write( msg, strlen (msg) );  // debug only
        uartUsb.write ( "\r\n",  3 );  // debug only
        #endif
    }
}

void gsmGprsCom::sendATPLUSCGREGcommand ()  {
    uartUsb.write ("\r\n ", 3 );  // debug on
    this->write( "AT+CREG?\r\n");
    uartUsb.write ("\r\n ", 3 );  // debug on
    this->refreshDelay->write( REFRESH_TIME_1000MS );
    this->gsmGprsComState =  GSM_GPRS_STATE_ATPLUSCGREG_WAIT_FOR_RESPONSE;
}


void gsmGprsCom::checkATPLUSCCIDcommand ()  {
    if (checkUARTResponse (CCID_VERIFICATION)) {
        #ifdef DEBUG
        uartUsb.write ( "\r\n",  3 );  // debug only
        char msg []  = "CCID command responded correctly \r\n";
        uartUsb.write( msg, strlen (msg) );  // debug only
        uartUsb.write ( "\r\n",  3 );  // debug only
        #endif
        this->gsmGprsComState =  GSM_GPRS_STATE_ATPLUSCGREG_TO_BE_SEND;
    } 
}

void gsmGprsCom::sendATPLUSCCIDcommand ()  {
    uartUsb.write ("\r\n ", 3 );  // debug on
    this->write( "AT+CCID\r\n");
    uartUsb.write ("\r\n ", 3 );  // debug on
    this->refreshDelay->write( REFRESH_TIME_1000MS );
    this->gsmGprsComState =  GSM_GPRS_STATE_ATPLUSCCID_WAIT_FOR_RESPONSE;
}

 /*
 La respuesta es el nivel de perdida de la señal
si ok respuesta tipo +CSQ: 24
es un numero de 2 digitos (fijo)
 */
void gsmGprsCom::checkATPLUSCSQResponse (  ) {
    char stringToCheck [7] = "+CSQ: ";
    char msgStringSignalQuality [9]= "";
    static std::string strSignalQuality = "";
    int stringIndexSignalQuality = 5;
    static int responseStringPositionIndex = 0;
    char charReceived;
    char charSignalLevel;

    if( this->charRead(&charReceived)) {
        if (charReceived != stringToCheck [responseStringPositionIndex] && responseStringPositionIndex < (strlen (stringToCheck) )   )  { 
             responseStringPositionIndex = 0;
             std::string strSignalQuality = "";
        }
        if ( (responseStringPositionIndex >= (strlen (stringToCheck) - 1 ))   ) {
            if (isdigit(charReceived)) {
                strSignalQuality += charReceived;
            } 
            responseStringPositionIndex++;
            if ( (responseStringPositionIndex == (strlen (stringToCheck) + 2 ))   ) {       
                this->signalLevel = std::stof(strSignalQuality);

                #ifdef DEBUG     
                sprintf (msgStringSignalQuality, "%f", this->signalLevel );  
                 uartUsb.write ( "\r\n",  3 );  // debug only  
                char msg []  = "signal level: ";
                uartUsb.write (msg,  strlen (msg) );  // debug only
                uartUsb.write (msgStringSignalQuality,  strlen (msgStringSignalQuality) );  // debug only
                uartUsb.write ( "\r\n",  3 );  // debug only
                #endif
                if (this->signalLevel < LOW_LEVEL_SIGNAL) {
                    this->gsmGprsComState = GSM_GPRS_STATE_NO_SIGNAL;
                    responseStringPositionIndex = 0;
                    std::string strSignalQuality = "";
                    return;
                } else {
                    this->gsmGprsComState = GSM_GPRS_STATE_ATPLUSCCID_TO_BE_SEND;
                    responseStringPositionIndex = 0;
                    std::string strSignalQuality = "";
                    return;
                }
            }  
        } 
        if (charReceived == stringToCheck [responseStringPositionIndex] && responseStringPositionIndex < (strlen (stringToCheck) )   )  { //&& responseStringPositionIndex <= strlen (stringToCheck) -1
            responseStringPositionIndex++;
        }
 
    }
}

 void gsmGprsCom::sendATPLUSCSQcommand (  ) {
    uartUsb.write ("\r\n ", 3 );  // debug on
    this->write( "AT+CSQ\r\n");
    uartUsb.write ("\r\n ", 3 );  // debug on
    this->refreshDelay->write( REFRESH_TIME_1000MS );
    this->gsmGprsComState =   GSM_GPRS_STATE_ATPLUSCSQ_WAIT_FOR_RESPONSE;
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
        uartUsb.write ( "\r\n",  3 );  // debug only
        char msg []  = "AT command responded correctly \r\n";
        uartUsb.write( msg, strlen (msg) );  // debug only
        uartUsb.write ( "\r\n",  3 );  // debug only
        #endif
    } 
}

void gsmGprsCom::write( const char* str ) {
    this->uartGsmGprs->write( str, strlen(str) );
}

// inidica si pudo leer algo por UART y lo forwardea por UARTUSB
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