#include "GNSSComModel.h"


/**
  * @brief  Crea una instancia de GPS en caso de que se use de forma dinamica
  * @param
  * @retval puntero a instancia GPSdata
  */
/*
GPSdata * CreateGPS()
{
	GPSdata * newGPS = malloc(sizeof(GPSdata));

	GPS_init(newGPS);

    return newGPS;
}*/

/**
  * @brief  Init GPS attributes.
  * @retval -
  */
void initGPSmodel(GPSdata * self)
{
	  self->latitude = 0;
	  self->longitude = 0;
	  self->satellites_in_view = 0;
	  self->altitude = 0;
}

/**
  * @brief  Saves data in GPSdata object from array of fields
  * @param self pointer to GPSdata object
  * @param fields array of uint8_t strings
  * @param talker NMEAtalker 
  * @retval -
  */
void setGPSdata( GPSdata * self, uint8_t fields[][FIELD_BUFF], enum NMEAtalker talker )
{
    switch(talker)
    {
    case GPGGA:
    	updateGPGGA(self, fields);
    	break;
    case GPGLL:

    	break;
    default:
      break;
    }
}
/**
  * @brief  Saves data in GPSdata from the fields of GNGGA talker
  * @param self pointer to GPSdata object
  * @param fields array of uint8_t strings
  * @retval -
  */
void updateGPGGA(GPSdata * self, uint8_t fields[][FIELD_BUFF])
{
  enum GNGGAfields {
  TIME, LATITUDE, NoS, LONGITUDE, EoW,
  FIX, SIV, HDOP, ALTTUDE_GEOID, ALTTUDE_METER,
  GEO_SEP, GEO_SEP_METER, DIFF, DIFF_REF};

  float aux = 0;
  /* converts latitude field to float */
	ascii_to_float(fields[LATITUDE], &(aux) );

  /* if the latitude is 0, it means the fields are empty, return */
  if ( 0 == aux)
	  return ;

  self->latitude = aux;

  /* converts degrees to decimal */
	NMEA_deg2dec(&(self->latitude));
	if(*(fields[NoS]) == 'S') // changes the sign if needed
			self->latitude = -self->latitude;

  /* converts longitude field to float */
	ascii_to_float(fields[LONGITUDE], &(self->longitude) );
	NMEA_deg2dec(&(self->longitude));
	if(*(fields[EoW])== 'W')  // changes the sign if needed
			self->longitude = -self->longitude;

	ascii_to_int(fields[SIV], &(self->satellites_in_view));

	ascii_to_int(fields[ALTTUDE_METER], &(self->altitude));

}

void NMEA_deg2dec(float* number)
{
	if(!*number) return;
	*number/=100;
	int integer = (int) *number;//grados
	*number = (*number - (float)integer)*100;// quedan solo los minutos y sus decimales
	*number /=60;
	*number = (float) integer + (*number); //a los grados le agrega los minutos /60 para que sean grados
}

#ifdef DYNAMIC
satellite * CreateSatellite(   uint8_t number, uint8_t elevation,
		unsigned int azimut, uint8_t SNR )
{
	satellite * newSatellite = malloc(sizeof(satellite));

    Satellite_init(newSatellite, number, elevation, azimut, SNR );

    return newSatellite;
}

void Satellite_init(satellite * new_sat, uint8_t number, uint8_t elevation,
		unsigned int azimut, uint8_t SNR )
{
	new_sat->number = number;
	new_sat->elevation = elevation;
	new_sat->azimut = azimut;
	new_sat->SNR = SNR;
	new_sat->next = NULL;
}

void freeSatellites(GPSdata * self)
{
	if(self->sat_list)
		free_all_satellite(&(self->sat_list));
}

void free_all_satellite(satellite ** chain)
{
	if((*chain)->next)
		free_last_satellite(&((*chain)->next));
	free(*chain);
	(*chain) = NULL;

}

void free_last_satellite(satellite ** currentnode)
{
	if((*currentnode)->next)
		free_last_satellite(& ((*currentnode)->next));
	else
	{
		free((*currentnode));
		(*currentnode) = NULL;
	}
}

void FreeGPS(GPSdata * self)
{
    // Do any other freeing required here.
	freeSatellites(self);
    free(self);
}
#endif