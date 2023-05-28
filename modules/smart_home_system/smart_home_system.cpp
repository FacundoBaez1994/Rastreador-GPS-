//=====[Libraries]=============================================================

#include "arm_book_lib.h"

#include "smart_home_system.h"

/*
#include "alarm.h"
#include "user_interface.h"
#include "fire_alarm.h"
#include "intruder_alarm.h"
#include "pc_serial_com.h"
#include "event_log.h"
#include "motion_sensor.h"
#include "motor.h"
#include "gate.h"
#include "light_system.h"
#include "audio.h"
#include "sd_card.h"
#include "ble_com.h"

#include "wifi_com.h"
*/
#include "non_blocking_delay.h"


//=====[Declaration of private defines]========================================
#define DELAY_2_SECONDS         2000
//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

nonBlockingDelay_t smartHomeSystemDelay;
DigitalOut LED (LED2);
DigitalOut LED_2 (LED3);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void smartHomeSystemInit()
{
    LED = ON;
    LED_2 = ON;
    tickInit();
    /*
    //audioInit();
    userInterfaceInit();
    alarmInit();
    //fireAlarmInit();
    intruderAlarmInit();
    pcSerialComInit();
    motorControlInit();
    gateInit();
    // lightSystemInit();
    sdCardInit();
    wifiComInit();
    */
    nonBlockingDelayInit( &smartHomeSystemDelay, DELAY_2_SECONDS  );
}

void smartHomeSystemUpdate()
{
    if( nonBlockingDelayRead(&smartHomeSystemDelay) ) {
     /*
        userInterfaceUpdate();
      //  fireAlarmUpdate();
        intruderAlarmUpdate();
       // alarmUpdate();
        eventLogUpdate();
        pcSerialComUpdate();
        // motorControlUpdate();
        lightSystemUpdate();
        bleComUpdate();
       */ 
        LED = !LED;
    }    
   //wifiComUpdate();
}

//=====[Implementations of private functions]==================================
