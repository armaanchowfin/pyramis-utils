#ifndef __GPRS_TIMER3_H
#define __GPRS_TIMER3_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define GPRS_TIMER3_LEN	1


#define GPRS_TIMER3_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_10_MINUTES	0b000
#define GPRS_TIMER3_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_HOUR	0b001
#define GPRS_TIMER3_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_10_HOURS	0b010
#define GPRS_TIMER3_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_2_SECONDS	0b011
#define GPRS_TIMER3_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_30_SECONDS	0b100
#define GPRS_TIMER3_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_MINUTE	0b101
#define GPRS_TIMER3_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_320_HOURS	0b110
#define GPRS_TIMER3_VALUE_INDICATES_THAT_THE_TIMER_IS_DEACTIVATED	0b111

typedef struct gprsTimer3 {
	uint8_t	lengthOfGprsTimer3Contents;
	uint8_t	unit:3;
	uint8_t	timerValue:5;
}GprsTimer3_t;

int decodeGprsTimer3(uint8_t *buffer, uint32_t bufLen, GprsTimer3_t *gprsTimer3, uint32_t* decodedLen);

int encodeGprsTimer3(uint8_t *buffer, uint32_t bufLen, GprsTimer3_t *gprsTimer3, uint32_t* encodedLen);

#endif 

