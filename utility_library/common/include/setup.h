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

/* 
 * This file specifies set up of system under test(SUT)
 * It enumerates NFV components which form part of SUT.
 * It specifies the ip addresses of components.
 *
 * setup.h being a compile time entity, the SUT parameters
 * need to be specified at compile time. The rationale
 * behind this to speed up core development & avoid getting 
 * into secondary issues of reading configuration files
 * 
 * At a later stage set up can be made run time
*/

#ifndef __SETUP_H
#define __SETUP_H

#include "common/include/datatypes.h"

typedef enum nfv_component {
	E_LOAD_GENERATOR_INST = 0,
	E_AMF_INST, 
	E_SMF_INST,
    E_UPF_INST,
    E_DPDK_UPF_INST,
	E_MAX_NFV_COMPONENTS
} _e_nfv_component;

extern char nfvCompIpAddr[E_MAX_NFV_COMPONENTS][IP_ADDR_MAX_STRLEN];


#endif 
