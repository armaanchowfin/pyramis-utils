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

#include "../include/kdf.h"
#include <iostream>
using namespace std;

#define TEST_DATA_LEN	100
#define TEST_KEY_LEN	100

void octetcpy_internal_test(uint8_t *dest, uint8_t *src, int len)
{
	int i=0;
	for(i=0;i<len;i++)
		dest[i]=src[i];
}

void printArray(uint8_t* inputArray, size_t size)
{
    for(uint i=0;i<size;i++)
    {
        //print each byte as hex
        printf("%02X ",inputArray[i]);
    }
    printf("\n");
}

void test1() {
	int i;
	uint8_t FC = 0xaa;
	int data_in_len = 1;
	struct kdf_parameter data_in;
	char str_data[] = "112233445566778899";
	int key_in_len = 1;
	struct kdf_parameter key_in;
	char str_key[] = "123456789";
	struct kdf_parameter res_out;
	int result;

	//initialize everything
	data_in.param = (uint8_t*) malloc(TEST_DATA_LEN*sizeof(uint8_t));
	octetcpy_internal_test(data_in.param, (uint8_t*)str_data, sizeof(str_data));
	data_in.len = sizeof(str_data);

	key_in.param = (uint8_t*) malloc(TEST_KEY_LEN*sizeof(uint8_t));
	octetcpy_internal_test(key_in.param, (uint8_t*)str_data, sizeof(str_key));
	key_in.len = sizeof(str_data);

	res_out.param = (uint8_t*) malloc(TEST_DATA_LEN*sizeof(uint8_t));
	res_out.len = TEST_DATA_LEN;

	result = kdf(FC, &data_in, data_in_len, &key_in, key_in_len, &res_out);
	if(result == 0) {
		cout << "test1: kdf call success." << endl;
	} else {
		cout << "test1: kdf call failed with" << result;
		return;
	}

	cout << "response key:";
	for(i=0; i<res_out.len; i++) {
		if(i%16==0)
			cout << endl;
		cout << std::hex << (int)res_out.param[i] << ".";
	}
	cout<<endl;
}

void main() {
	//test1: one data struct, one key struct. check output.
	test1();
}
