#ifndef __GPRS_TIMER_H
#define __GPRS_TIMER_H
#include <bits/stdc++.h>
#include "common/include/datatypes.h"
#include "nas/nasUtil/nasLogging.h"

#define GPRS_TIMER_LEN	1


#define GPRS_TIMER_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_2_SECONDS	0b000
#define GPRS_TIMER_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_MINUTE	0b001
#define GPRS_TIMER_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_DECIHOURS	0b010
#define GPRS_TIMER_VALUE_INDICATES_THAT_THE_TIMER_IS_DEACTIVATED	0b111

typedef struct gprsTimer {
	uint8_t	unit:3;
	uint8_t	timerValue:5;
}GprsTimer_t;

int decodeGprsTimer(uint8_t *buffer, uint32_t bufLen, GprsTimer_t *gprsTimer, uint32_t* decodedLen);

int encodeGprsTimer(uint8_t *buffer, uint32_t bufLen, GprsTimer_t *gprsTimer, uint32_t* encodedLen);

#endif 

