/*  
 *  Copyright Droids Corporation, Microb Technology, Eirbot (2007)
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Revision : $Id: aversive.h,v 1.1.2.6 2009/05/18 12:19:51 zer0 Exp $
 *
 */

/**
 * here are some cute little macros, and other stuff, microcontroller
 * related !
 */


#ifndef _AVERSIVE_H_
#define _AVERSIVE_H_

//#include <autoconf.h>
#include <math.h>
#include "types.h"

/* Aversive target configuration */
#include "aversive_config.h"

/* Some useful constants */

#define Hz  1l
#define KHz 1000l
#define MHz 1000000l

#ifndef M_PI
#  define M_PI 3.141592653589
#endif

/*
 *  a few "mathematical" macros : maximums and minimums
 */

/**
 *  signed maxmimum : both signs are tested
 */
#define S_MAX(to_saturate, value_max)    \
do {                                     \
   if (to_saturate > value_max)          \
     to_saturate = value_max;            \
   else if (to_saturate < -value_max)    \
     to_saturate = -value_max;           \
 } while(0)

/**
 *  unsigned maximum : result >0 is forced
 */
#define U_MAX(to_saturate, value_max)    \
do {                                     \
   if (to_saturate > value_max)          \
     to_saturate = value_max;            \
   else if (to_saturate < 0)             \
     to_saturate = 0;                    \
 } while(0)

/**
 *   simple maximum
 */
#define MAX_SAT(to_saturate, value_max)      \
do {                                     \
   if (to_saturate > value_max)          \
     to_saturate = value_max;            \
} while(0)

/**
 *  simple minimum
 */
#define MIN_SAT(to_saturate, value_min)      \
do {                                     \
   if (to_saturate < value_min)          \
     to_saturate = value_min;            \
} while(0)


/** absolute
 *  while the abs() function in the libc works only with int type
 *  this macro works with every numerical type including floats
 */
#define ABS(val) ({					\
			__typeof(val) __val = (val);	\
			if (__val < 0)			\
				__val = - __val;	\
			__val;				\
		})

/* 
 * Extract bytes and u16 from larger integer
 */

#if __BYTE_ORDER != __LITTLE_ENDIAN && __BYTE_ORDER != __BIG_ENDIAN
# error	"Endianness not defined"
#endif

struct extract32 {
	union {
		struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
			uint8_t u8_0;
			uint8_t u8_1;
			uint8_t u8_2;
			uint8_t u8_3;
#elif __BYTE_ORDER == __BIG_ENDIAN
			uint8_t u8_3;
			uint8_t u8_2;
			uint8_t u8_1;
			uint8_t u8_0;
#endif
		} __attribute__ ((packed)) u8;
		struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
			uint16_t u16_0;
			uint16_t u16_1;
#elif __BYTE_ORDER == __BIG_ENDIAN
			uint16_t u16_1;
			uint16_t u16_0;
#endif
		} __attribute__ ((packed)) u16;
		struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
			uint8_t u8_0;
			uint16_t u16_mid;
			uint8_t u8_3;
#elif __BYTE_ORDER == __BIG_ENDIAN
			uint8_t u8_3;
			uint16_t u16_mid;
			uint8_t u8_0;
#endif
		} __attribute__ ((packed)) u16_b;
		uint32_t u32;
	} __attribute__ ((packed)) u;
} __attribute__ ((packed));

#define extr32_08_0(i) ({ struct extract32 __x; __x.u.u32 = i; __x.u.u8.u8_0; })
#define extr32_08_1(i) ({ struct extract32 __x; __x.u.u32 = i; __x.u.u8.u8_1; })
#define extr32_08_2(i) ({ struct extract32 __x; __x.u.u32 = i; __x.u.u8.u8_2; })
#define extr32_08_3(i) ({ struct extract32 __x; __x.u.u32 = i; __x.u.u8.u8_3; })

#define extr32_16_0(i) ({ struct extract32 __x; __x.u.u32 = i; __x.u.u16.u16_0; })
#define extr32_16_1(i) ({ struct extract32 __x; __x.u.u32 = i; __x.u.u16.u16_1; })
#define extr32_16_mid(i) ({ struct extract32 __x; __x.u.u32 = i; __x.u.u16_b.u16_mid; })


struct extract16 {
	union {
		struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
			uint8_t u8_0;
			uint8_t u8_1;
#elif __BYTE_ORDER == __BIG_ENDIAN
			uint8_t u8_1;
			uint8_t u8_0;
#endif
		} __attribute__ ((packed)) u8;
		uint16_t u16;
	} __attribute__ ((packed)) u;
} __attribute__ ((packed));

#define extr16_08_0(i) ({ struct extract16 __x; __x.u.u16 = i; __x.u.u8.u8_0; })
#define extr16_08_1(i) ({ struct extract16 __x; __x.u.u16 = i; __x.u.u8.u8_1; })


/**
 *   little bit toggeling macro 
 *  
 *  change pin state
 *  usage :
 *  BIT_TOGGLE(PORTB,2) to make the pin 2 of PORTB toggle
 */
#define BIT_TOGGLE(port,bit) do {\
      if(bit_is_set(PIN(port),bit)) \
	cbi(port,bit); \
      else \
	sbi(port,bit); \
      } while(0)


/** booleans */


/** DDR and PINS from port adress */
#define DDR(port) (*(&(port) -1))
#define PIN(port) (*(&(port) -2))

/** open collector simulation macros */
#define OPEN_CO_INIT(port, bit) sbi(port,bit)
#define OPEN_CO_HIGH(port, bit) cbi(DDR(port),bit)
#define OPEN_CO_LOW(port, bit)  cbi(DDR(port),bit)

/** deprecated macros in libc, but they're almost used, so we implement them again ;) */
#ifndef cbi
#define cbi(sfr, bit) ( sfr &= ~ _BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) ( sfr |= _BV(bit))
#endif


#endif /* ifndef _AVERSIVE_H_ */

