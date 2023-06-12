#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdint.h>
#include <string.h>
//#include "main.h"


#define __8BITCAT(x,y) ( (x << 8 ) + y )
/*
Set single bit at pos to '1' by generating a mask
in the proper bit location and ORing (|) x with the mask.
*/
#define MY_SET_BIT(x, pos) (x |= (1U << pos))
/*
Set single bit at pos to '0' by generating a mask
in the proper bit location and Anding x with the mask.
*/
#define MY_CLEAR_BIT(x, pos) (x &= (~(1U<< pos)))
/*
Set single bit at pos to '1' by generating a mask
in the proper bit location and ex-ORing x with the mask.
*/
#define MY_TOGGLE_BIT(x, pos) x ^= (1U<< pos)
/*
Set single bit at pos to '1' by generating a mask
in the proper bit location and Anding x with the mask.
It evaluates 1 if a bit is set otherwise 0.
*/
#define MY_CHECK_BIT(x, pos) (x & (1UL << pos) )
//Macro to swap nibbles
#define MY_SWAP_NIBBLES(x) ((x & 0x0F)<<4 | (x & 0xF0)>>4)
//Macro to swap byte of 32-bit +ve integer variable
#define MY_SWAP_BYTES(u32Value) ((u32Value & 0x000000FF) << 24)\
|((u32Value & 0x0000FF00) << 8) \
|((u32Value & 0x00FF0000) >> 8) \
|((u32Value & 0xFF000000) >> 24)
//macro to swap odd-even bits
#define MY_SWAP_ODD_EVEN_BIT(x) ((x & 0xAAAAAAAA)>>1 | (x & 0x55555555)<<1);
//macro to swap two numbers
#define MY_SWAP(x, y) (x ^= y ^= x)
//macro to get low and high bytes
#define MY_LOWBYTE(v)   ((uint8_t) (x))
#define MY_HIGHBYTE(v)  ((uint8_t) (((uint8_t) (x)) >> 8))

#define ASCII_OFFSET 48

#ifndef END_OF_STRING
#define END_OF_STRING '\0'
#endif

int pow_10(uint8_t n);
void ascii_to_int(uint8_t *str, int *result);
void ascii_to_float(const uint8_t* ptrStr, float* ptrFloat);
uint8_t myStrlen(const uint8_t *ptrStr);
void reverse(char* str, int len);
__uint8_t int_to_ascii(int x, char dest[], __uint8_t d);
void float_to_ascii(float n, uint8_t* res, uint8_t afterpoint);
void insert_chars(uint8_t *dest, uint8_t *src, uint8_t d);
void get_time_string(RTC_TimeTypeDef *Time, uint8_t *time_string);
void get_date_string(RTC_DateTypeDef *Date, uint8_t *date_string);

#endif /* TOOLS_H_ */