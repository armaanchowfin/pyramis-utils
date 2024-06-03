/*********************************************************************
*
* IITB CONFIDENTIAL
* __________________
*
* All Rights Reserved © 2018-19 IITB, India
* All Rights Reserved © 2019-20 IITB, India
*
* NOTICE: All information contained in this document/file is, and remains the 
* property of Indian Institute of Technology, Bombay (IITB).The intellectual
* and technical concepts contained herein may be proprietary to IITB and are
* protected by trade secret or copyright laws. Unauthorized use, dissemination, 
* reproduction, duplication, reverse engineering, any form of redistribution, 
* or use in part or in whole, by any means is strictly forbidden unless prior, 
* express, written and signed consent is obtained from IITB.
*
* Institute   : Indian Institute of Technology Bombay (IITB)
*/

#ifndef __PFCP_IE_SDF_FILTER
#define __PFCP_IE_SDF_FILTER

#include "pfcp/include/pfcpDefs.h"
#include "pfcp/include/pfcpLogging.h"


#define PFCP_IE_SDF_FILTER_FD_MAX_NO 128
#define PFCP_IE_SDF_FILTER_MIN_LEN 6
#define PFCP_IE_SDF_FILTER_MASK_LEN 1
#define PFCP_SDF_FILTER_FD_LEN 2
#define PFCP_SDF_FILTER_TTC_CONTENTS_LEN 2
#define PFCP_SDF_FILTER_SPI_CONTENTS_LEN 4
#define PFCP_SDF_FILTER_FL_CONTENTS_LEN 3
#define PFCP_SDF_FILTER_BID_CONTENTS_LEN 4
#define PFCP_SDF_FILTER_FD_CONTENTS_MANDATORY_LEN 5
#define PFCP_SDF_FILTER_FD_CONTENTS_OPTIONAL_LEN 6
#define PFCP_SDF_FILTER_FD_CONTENT_SOURCE_PORT_PRESENT (1<<0)
#define PFCP_SDF_FILTER_FD_CONTENT_DESTINATION_PORT_PRESENT (1<<1)
#define PFCP_SDF_FILTER_FD_PRESENT  (1<<2)
#define PFCP_SDF_FILTER_TTC_PRESENT (1<<3)
#define PFCP_SDF_FILTER_SPI_PRESENT (1<<4)
#define PFCP_SDF_FILTER_FL_PRESENT  (1<<5)
#define PFCP_SDF_FILTER_BID_PRESENT (1<<6)

typedef struct sdfFilter {
    uint16_t IEI;
    

    uint8_t FD:1;
    uint8_t TTC:1;
    uint8_t SPI:1;
    uint8_t FL:1;
    uint8_t BID:1;
    uint16_t spare:11;
    uint16_t lengthOfFlowDescription;
    uint8_t flowDescription[PFCP_IE_SDF_FILTER_FD_MAX_NO];
    
    uint8_t tosTrafficClass[PFCP_SDF_FILTER_TTC_CONTENTS_LEN];
    
    uint8_t securityParameterIndex[PFCP_SDF_FILTER_SPI_CONTENTS_LEN];
    uint8_t flowLabel[PFCP_SDF_FILTER_FL_CONTENTS_LEN];
    uint8_t sdfFilterId[PFCP_SDF_FILTER_BID_CONTENTS_LEN];

    uint32_t presenceMask;

} sdfFilter_t;

int sdfFilterEncode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *encodedLen);
int sdfFilterDecode(uint8_t *buffer, uint16_t bufLen, void* ptr,
                 uint16_t *decodedLen);

#endif
