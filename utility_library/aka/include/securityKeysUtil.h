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

#ifndef __SECKEYSUTILS_H
#define __SECKEYSUTILS_H

#include <iostream>
#include <string>

using namespace std;

int computeKausf(uint8_t CK[], uint8_t IK[], uint8_t SQN[], uint8_t AK[], const char snName[], uint8_t (&Kausf)[KAUSF_SIZE]);
int computeXRES(uint8_t rand[], uint8_t CK[], uint8_t IK[], uint8_t RES[], const char snName[], uint8_t (&XRES)[XRES_SIZE]);
int computeKseaf(uint8_t Kausf[], char snName[], uint8_t (&Kseaf)[KSEAF_SIZE]);
int computeHXRES(uint8_t rand[], uint8_t XRES[], uint8_t (&HXRES)[XRES_SIZE]);
int computeKamf(uint8_t Kseaf[], std::string supi_str, uint8_t ABBA, uint8_t (&Kamf)[KAMF_SIZE]);
int computeKgnb(uint8_t Kamf[], uint8_t *uplinkNAScount, uint8_t (&Kgnb)[KGNB_SIZE]);
int computeKn3iwf(uint8_t Kamf[], uint8_t *uplinkNAScount, uint8_t (&Kn3iwf)[KN3IWF_SIZE]);
int computeNasSecurityKey(uint8_t Kamf[], uint8_t algo_type, uint8_t algo_id, uint8_t (&NAS_key)[NAS_KEY_SIZE]);

#endif
