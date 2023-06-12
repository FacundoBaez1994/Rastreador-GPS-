#include "GPS/parser.h"

//#define WINDOWS 

#ifdef algo
#ifndef strsep

char *strsep (char **stringp, const char *delim)
{
  char *begin, *end;
  begin = *stringp;
  if (begin == NULL)
    return NULL;
  /* Find the end of the token.  */
  end = begin + strcspn (begin, delim);
  if (*end)
    {
      /* Terminate the token and set *STRINGP past NUL character.  */
      *end++ = END_OF_STRING;
      *stringp = end;
    }
  else
    /* No more delimiters; this is the last token.  */
    *stringp = NULL;
  return begin;
}
#endif



uint8_t checksum(const char *s) {
    int c = 0;

    while (*s)
        c ^= *s++;

    return c;
}

uint8_t CRC_check( unsigned char* mystring)
{
    unsigned char *tok = mystring;
    char str_crc[3];

    tok = (unsigned char *)strtok( (char*) tok,END_STAR);

    //printf("CRC_check tok %s\r\n",tok);

    uint8_t crc_calc = checksum((char*) tok);

    sprintf(str_crc, "%x",crc_calc );
    sscanf(str_crc, "%u",&crc_calc );

    //printf("CRC_calc %d\r\n",crc_calc);

    if (! (tok = (unsigned char *) strtok( NULL,END_STAR)) )
        return 0;
    int crc = atoi((char*) tok);
    //printf("CRC %d\r\n",crc);

    if(crc == crc_calc)
    {
        //printf("CRC correcto\r\n");
        return 1;

    }
    else
    {
        //printf("CRC incorrecto\r\n");
        return 0;
    }
    return 0;
}
#endif

/** @brief Returns the message fields in a given matrix.
 *
 *  @param fields array of pointers to each comma
 *  @param field_count count o fields to copy
 *  @param dest matrix to copy the values
 *  @return -
 */
void copyValues(uint8_t** fields, uint8_t field_count , uint8_t dest[FIELD_BUFF][FIELD_BUFF])
{
	for(uint8_t index = 0; index<field_count;index++)
	{
		if( fields[index] == NULL) return;

		/* if the next char in fields[index] string is a comma
		 * it means the field is empty */
		if (COMA == *(fields[index] + NEXT) ||
			END_STAR == *(fields[index] + NEXT)) 
		{
			;
		}
		else
		{
			/* checks if it's the last pointer with data*/
			if (fields[index + NEXT])
			{
				memcpy( dest[index], fields[index] + NEXT, fields[NEXT + index] - fields[index] - 1);
			}
			/* if it's the last pointer, copies char by char until * */
			else
			{
				uint8_t subIndex = 0;
				do
				{
					/* copy until * */
					dest[index][subIndex] = fields[index][subIndex + NEXT];
				} while ( fields[index][++subIndex + NEXT] != END_STAR);
			}
		}		
	}
}

/** @brief Returns the message fields in a given matrix.
 *
 *  @param ptrMessage pointer to the start of the message.
 *  @param ptrTalker pointer to the talker from wich get the message.
 *  @param fieldsArray matrix to copy the values
 *  @return -
 */
void  getMessageFields(const uint8_t* ptrMessage, uint16_t* msgSize, const uint8_t* ptrTalker, uint8_t fields_array[FIELD_BUFF][FIELD_BUFF] )
{

	uint8_t i;
	/* define an array of pointer to reference where each value 
	* is allocated in the message */
	uint8_t* fields[FIELD_BUFF];
	const uint8_t* auxPtrMessage;

	for(i = 0; i<FIELD_BUFF; i++) // initializes all the pinters
		fields[i] = NULL;

	auxPtrMessage = getMessageptr( ptrMessage, msgSize, ptrTalker, NULL );
	if (NULL == auxPtrMessage) return; // if the talkers is not found, return

	if ( ! isSentenceComplete( ptrMessage, msgSize,auxPtrMessage ) ) return;
	

	uint8_t field_count = coma_count(auxPtrMessage);

	for(i = 0; i< field_count; i++)
	{
		for(auxPtrMessage; !( (*auxPtrMessage) == COMA ||
				(*auxPtrMessage) == END_STAR); auxPtrMessage++);
		fields[i] = auxPtrMessage;
		auxPtrMessage++;
	}
	fields[i] = NULL;

	copyValues(fields, field_count , fields_array);
}

/** @brief Checks if the message is complete.
 *
 *   Uses ptrStart to checks if there is al \r o \0 in the message.
 *
 *  @param ptrStart pointer to look where to start from
 *  @param ptrMessage pointer to the start of the message.
 *  @return True or False.
 */
uint8_t isSentenceComplete(const uint8_t *ptrMessage, uint16_t* msgSize,const uint8_t *ptrStart)
{
	const uint8_t* ptrAux;
	//checkea que la frase este completa
	for(ptrAux = ptrStart;
			*ptrAux != '\r' && *ptrAux != END_OF_STRING && (ptrAux-ptrMessage) < msgSize;
			ptrAux++);
	//si llego al final, devuelve NULL
	if((ptrAux-ptrMessage) == (*msgSize) || *ptrAux == END_OF_STRING)
		return FALSE;
	else return TRUE;

}

/** @brief Returns the number of commas in a sentence.
 *
 *  Starts where the pointer "string" is pointing and stops when an "*" is found.
 *
 *  @param string pointer that refers to where is expected tu start looking for.
 *  @return Number of commas.
 */
uint8_t coma_count(const uint8_t* ptrMessage)
{
	uint8_t count=0U;
	//aumenta el puntero hasta que encuentra una coma. Cuando la encuetra avanza
	//count y se va separando del inicio del puntero. Cuando no encuentra la coma
	//sigue aumentando el puntero hasta llegar al final.
	for (count=0U; ptrMessage[count] != END_STAR; 
		 ptrMessage[count]==COMA ? count++ : *ptrMessage++);
	return count;
}

/** @brief Returns a pointer to the first aparition of "$" in prtSartBuff.
 *
 *  Starts at prtSartBuff address and runs through the string until the end or until
 *  it fins a match. Whatever comes first.
 *
 *  @param ptrStartBuff pointer that refers to where is expected tu start looking for.
 *  @return ptrAux. NULL if there is no match.
 */
uint8_t* findStartChar(const uint8_t* ptrMessage, uint16_t* msgSize, const uint8_t *ptrStart )
{
	const uint8_t* ptrAux ;	// aux pointer for moving through the string
	for( ptrAux = ptrStart ;
			( START_PESOS != *ptrAux ) && ( (*msgSize) > (ptrAux - ptrMessage) );
		ptrAux++);	//starts at the begining. Ends if it matches "$" or end of buffer
	
	if( (*msgSize) == ( ptrAux - ptrMessage ) )
		ptrAux= NULL;	// returns NULL if it didn't find a match
	
	return ptrAux;
}

#ifdef WINDOWS
void printTalker(const char *message)
{
	char talker[6];
	memcpy(talker, message , 5);
	printf("Talker %s\r\n", talker);
}
#endif

/** @brief Returns a pointer to the first aparition of type string.
 *
 *  Starts at message address and runs through the string until the end or until
 *  it fins a match. Whatever comes first.
 *
 *  @param message pointer that refers to where is expected tu start looking for.
 *  @param type string to find. NMEA Talker
 *  @return token to start of message. NULL if there is no match.
 */
uint8_t* getMessageptr(const uint8_t *message, uint16_t* msgSize, const uint8_t *type, const uint8_t *init_ptr)
{
	const uint8_t* tok;
	const uint8_t * type_offset;
	const uint8_t* tok_offset;

	/* assign aux pointers */
	if ( NULL == init_ptr ) tok = message;
	else tok = init_ptr;


	do
	{
		if( !(tok = findStartChar(message, msgSize, tok) ) ) return NULL; //looks for next $ ocurrence
		tok++;	// findStartChar return a pointer to $. moves one more 
		#ifdef WINDOWS
		printTalker(tok);
		#endif

		type_offset= type;
		tok_offset = tok;

		/* compares the chars in type string
		 * until the end or a failed match
		 */
		while( ( *tok_offset ) == ( *type_offset )
				&& END_OF_STRING != ( *type_offset ) )
		{
			tok_offset++;	// moves the pointer
			type_offset++;
		}
	/* loop until reach the end of the buffer or 
	 * type_offset point to '\0', which means the type matched
	 * in the message
	 */ 
	}while ( END_OF_STRING != (*type_offset) &&
			(*msgSize) > (tok-message) );

	//checkea que haya cortado por coincidencia y no por fin del buffer
	if((*type_offset) != END_OF_STRING) return NULL;

	return tok;
}