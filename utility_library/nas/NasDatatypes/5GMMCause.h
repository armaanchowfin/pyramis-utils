#ifndef __5GMM_CAUSE_H
#define __5GMM_CAUSE_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "nas/nasUtil/nasCommon.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define SIZE_OF_5GMM_CAUSE  1



typedef struct  _5gmmCause{
	uint8_t cause;
}_5gmmCause_t;



int decode5GMMCause(uint8_t *buffer, uint32_t bufLen,
	_5gmmCause_t *_5gmmCause, uint32_t* decodedLen);
int encode5GMMCause(uint8_t *buffer, uint32_t bufLen,
    _5gmmCause_t *_5gmmCause, uint32_t *encodedLen);

#endif

