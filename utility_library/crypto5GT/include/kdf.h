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

#ifndef __UTILS_KDF_H
#define __UTILS_KDF_H

#include <stdint.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

struct kdf_parameter {
	uint8_t		*param;
	unsigned int	len;
};


int kdf(uint8_t FC, struct kdf_parameter *data_input, int input_data_len,
	 struct kdf_parameter *key_input, int input_key_len,
	 struct kdf_parameter *result);

#endif 

