#ifndef INC_GNSS_COM_CONTROLLER_H_
#define INC_GNSS_COM_CONTROLLER_H_

#include "GNSSComModel.h"
#include "parser.h"

#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#undef DEBUG


#ifndef MAX_ARGUMENTS
#define MAX_ARGUMENTS 19
#endif

#define ZDA_PAYLOAD 11
#define GGA_PAYLOAD 11
#define GBS_PAYLOAD 11
#define RMC_PAYLOAD 11
#define GLL_PAYLOAD 11
#define RXM_PAYLOAD 10

	
void initGPS(GPSdata * _gps);
void updateGPS(GPSdata* _gps, const uint8_t bufferDMA [], uint16_t* msgSize);
void updateDateTime( RTC_HandleTypeDef* hrtc, const uint8_t bufferDMA[], uint16_t* msgSize);
uint8_t setDate(RTC_HandleTypeDef* hrtc, uint8_t fields[FIELD_BUFF][FIELD_BUFF]);
uint8_t setTime(RTC_HandleTypeDef* hrtc, uint8_t fields[FIELD_BUFF]);
void configGPS();
void GPS_1min_sleep();

#endif /* INC_GPSCONTROLLER_H_ */