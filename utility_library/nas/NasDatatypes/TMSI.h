#ifndef __TMSI_H
#define __TMSI_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"

#define  _5G_TMSI_LEN 4



typedef struct _5gTmsi {
    uint32_t         value;
}_5gTmsi_t;


int decodeTmsi(uint8_t *buffer, uint32_t bufLen,
			           _5gTmsi_t *_5gTmsi, uint32_t* decodedLen);
int encodeTmsi(uint8_t *buffer, uint32_t bufLen,
                       _5gTmsi_t *_5gTmsi, uint32_t *encodedLen);

#endif 

