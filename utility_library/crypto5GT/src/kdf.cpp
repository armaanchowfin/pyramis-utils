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

#include "crypto5GT/include/kdf.h"

#define HMAC_DATA_LENGTH		1000
#define HMAC_KEY_LENGTH			1000

void octetcpy_internal(uint8_t *dest, uint8_t *src, int len)
{
	int i=0;
	for(i=0;i<len;i++)
		dest[i]=src[i];
}

unsigned char* hmac_sha256(const void *key, int keylen,
                           const unsigned char *data, int datalen,
                           unsigned char *result, unsigned int* resultlen)
{
    return HMAC(EVP_sha256(), key, keylen, data, datalen, result, resultlen);
}

/**
 * generic kdf function, based on 33.220, Annex B.2
 * provide FC and three initialized kdf_param arrays.
 * result is a single array pointer.
 * return negative on failure, 0 on success.
 * TODO: assign proper returns on failures
 */

int kdf(uint8_t FC, struct kdf_parameter *data_input, int input_data_len,
	 struct kdf_parameter *key_input, int input_key_len,
	 struct kdf_parameter *result)
{
	int p, copy_head=1;
	uint8_t data[HMAC_DATA_LENGTH]={};
	int data_len = 0;
	uint8_t key[HMAC_KEY_LENGTH]={};
	int key_len;
	uint8_t res[HMAC_DATA_LENGTH]={};
	unsigned int res_len;

	data[0]=FC;
    /* FC part of input data S, thus copy_head starts at 1 */

	for(p=0; p<input_data_len; p++) {
		/* copy input params*/
		if(HMAC_DATA_LENGTH < copy_head + data_input[p].len + 2) {
			//ERROR. insufficient len.
			return -1;  //TODO
		}
		octetcpy_internal(data+copy_head, data_input[p].param,
					data_input[p].len);
		copy_head += data_input[p].len;

		/*copy len, 2 octets*/
		data[copy_head]   = (uint8_t) ((data_input[p].len & 0xFF00)>>8);
		data[copy_head+1] = (uint8_t) (data_input[p].len & 0x00FF);
		copy_head += 2;
	}
	data_len = copy_head;

	copy_head = 0;
	for(p=0; p<input_key_len; p++) {
		/* copy key params*/
		if(HMAC_KEY_LENGTH < copy_head + key_input[p].len) {
			//ERROR. insufficient key len.
			return -2;  //TODO
		}
		octetcpy_internal(key+copy_head, key_input[p].param,
					key_input[p].len);
		copy_head += key_input[p].len;
	}
	key_len = copy_head;

	if(hmac_sha256(key, key_len, data, data_len, res, &res_len) == NULL)
		return -3;//TODO

	if(res_len > result->len)
		return -4; //TODO

	octetcpy_internal(result->param, res, res_len);
	result->len = res_len;
	return 0;
}

