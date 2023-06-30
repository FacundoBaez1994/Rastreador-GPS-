//=====[Libraries]=============================================================

#include "non_Blocking_Delay.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============


//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static tick_t tickCounter = 0;


//=====[Declarations (prototypes) of private functions]========================

void tickerCallback();
tick_t tickRead();

//=====[Implementations of private methods]===================================
/** 
* @brief attachs the callback function to the ticker
*/
void nonBlockingDelay::tickInit() {
    this->ticker.attach( tickerCallback, 1ms );
}

//=====[Implementations of public methods]===================================
/** 
* @brief Contructor method
* creates a new nonBlockingDelay instance ready to be used
* @param durationValue the duration of the delay
*/
nonBlockingDelay::nonBlockingDelay (tick_t durationValue ) {
    this->tickInit ();
    this->duration = durationValue;
    this->isRunning = false;
}

/** 
* @brief reads if the delay time has ended
* if the delay wasn't running it will set it on
* when the duration of the delay has reach or surpasses the attribute duration the delay will stop and the method returns true
* @returns true or false if the delay has reach the duration
*/
bool nonBlockingDelay::read (void) {
   bool timeArrived = false;
   tick_t elapsedTime;

   if( !this->isRunning ) {
        this->startTime = tickCounter;
        this->isRunning = true;
   } else {
      elapsedTime = tickCounter - this->startTime;
      if ( elapsedTime >= this->duration ) {
         timeArrived = true;
         this->isRunning = false;
      }
   }
   return timeArrived;
}

/** 
* @brief sets a new duration value for the delay
* @note durationValue the new duration to be set into the delay
*/
void nonBlockingDelay::write( tick_t durationValue ) {
   this->duration = durationValue;
}

//=====[Implementations of private functions]==================================
/** 
* @brief callback function adds 1 to a counter
* @note ISR cannot form part of the class
*/
void tickerCallback( void )  {
    tickCounter++;
}

/** 
* @brief returns counter
* @return tickCounter a counter value
*/
tick_t tickRead() {
    return tickCounter;
}