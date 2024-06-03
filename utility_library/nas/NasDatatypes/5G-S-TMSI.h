#ifndef __5G_S_TMSI_H
#define __5G_S_TMSI_H

#include <cstdint>
#include <cstdlib>
#include <bits/stdc++.h>
#include "nas/nasUtil/nasLogging.h"
#include "common/include/datatypes.h"
#include "ran/intf/include/ue.h"
#include "TMSI.h"

#define _5G_S_TMSI_LEN 7
#define _5G_S_TMSI_ID_TYPE 4



typedef struct _5gsTmsi {
    uint16_t         len;
    uint8_t          idType:3;
    uint8_t          spare:1;
    uint8_t          setBits:4;
    uint16_t         amfSetID:10;
    uint16_t         amfPointer:6;
    _5gTmsi_t        _5gTmsi;
}_5gsTmsi_t;


int decode5gsTmsi(uint8_t *buffer, uint32_t bufLen,
			           _5gsTmsi_t *_5gsTmsi, uint32_t* decodedLen);
int encode5gsTmsi(uint8_t *buffer, uint32_t bufLen,
                       _5gsTmsi_t *_5gsTmsi, uint32_t *encodedLen);

#endif 

