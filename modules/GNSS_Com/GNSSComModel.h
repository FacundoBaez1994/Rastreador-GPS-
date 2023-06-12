#ifndef GNSS_COM_MODEL_H_
#define GNSS_COM_MODEL_H_

#include <stdint.h>
#include <stdlib.h>
#include "tools.h"
#define FIELD_BUFF 20

#ifdef DYNAMIC
typedef struct satellite_in_view {
   uint8_t index;
   uint8_t number;
   uint8_t elevation;
   unsigned int azimut;
   uint8_t SNR;
   struct satellite_in_view* next;
} satellite;
#endif

typedef struct GPS {
   float latitude;
   float longitude;
   float altitude;
   uint8_t satellites_in_view;
} GPSdata;

typedef struct time {
   uint8_t hour;
   uint8_t minute;
   uint8_t seconds;
   uint8_t year;
   uint8_t month;
   uint8_t day;
   uint8_t UTC;
} GPStime;

enum NMEAtalker {
	GPGGA, GPGLL, GPGSA, BDGSA, GPGSV,
	BDGSV, GPRMC,GPVTG, GPZDA, GPTXT};

enum GPGSVfields {
	TOTAL_MESSAGE, MESSAGE, SIV, SAT_NUMBER,
	ELEVATION, AZIMUTH, SNR};

enum GNZDAfields {
	TIME, DAY, MONTH, YEAR};
    
#ifdef DYNAMIC
GPSdata * CreateGPS();
satellite * CreateSatellite(   uint8_t number, uint8_t elevation,
		unsigned int azimut, uint8_t SNR );
void Satellite_init(satellite * new_sat, uint8_t number, uint8_t elevation,
		unsigned int azimut, uint8_t SNR );
void freeSatellites(GPSdata * self);
void free_all_satellite(satellite ** chain);
void free_last_satellite(satellite ** currentnode);
void FreeGPS(GPSdata * self);
#endif

void initGPSmodel(GPSdata * self);
void setGPSdata( GPSdata * self, uint8_t fields[][FIELD_BUFF], enum NMEAtalker talker );
void updateGPGGA(GPSdata * self, uint8_t fields[][FIELD_BUFF]);
void NMEA_deg2dec(float* number);

#endif /* GNSS_COM_MODEL_H_ */