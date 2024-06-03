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

#ifndef __UE_H
#define __UE_H

#include <iostream>
#include <string.h>

#include "platform/include/logging.h"

#define SUPI_IMSI 0
#define SUPI_NAI  1
#define MAX_LEN  256

typedef struct _suci {
	uint8_t mcc_1:4;
	uint8_t mcc_2:4;
	uint8_t mcc_3:4;
	uint8_t mnc_1:4;
	uint8_t mnc_2:4;
	uint8_t mnc_3:4;
	uint16_t routingIndicator; 
	uint8_t protectionSchemeId:4;
    uint8_t homeNtwrkPKI;
    uint8_t schemeOutput[5];

    _suci() {
        memset(this, 0, sizeof(*this));
    }
	
	
	bool operator<(const _suci& input) const
	{
		uint8_t schemeOutput1 = (this->schemeOutput[0] & 0x0F);
		uint8_t schemeOutput2 = (this->schemeOutput[0] & 0xF0) >> 4;
		uint8_t schemeOutput3 = (this->schemeOutput[1] & 0x0F);
		uint8_t schemeOutput4 = (this->schemeOutput[1] & 0xF0) >> 4;
		uint8_t schemeOutput5 = (this->schemeOutput[2] & 0x0F);
		uint8_t schemeOutput6 = (this->schemeOutput[2] & 0xF0) >> 4;
		uint8_t schemeOutput7 = (this->schemeOutput[3] & 0x0F);
		uint8_t schemeOutput8 = (this->schemeOutput[3] & 0xF0) >> 4;
		uint8_t schemeOutput9 = (this->schemeOutput[4] & 0x0F);
		uint8_t schemeOutput10 = (this->schemeOutput[4] & 0xF0) >> 4;
#if 0
		uint8_t schemeOutput11 = (this->schemeOutput[5] & 0xF0) >> 4;
		uint8_t schemeOutput12 = (this->schemeOutput[5] & 0x0F);
		uint8_t schemeOutput13 = (this->schemeOutput[6] & 0xF0) >> 4;
		uint8_t schemeOutput14 = (this->schemeOutput[6] & 0x0F);
		uint8_t schemeOutput15 = (this->schemeOutput[7] & 0xF0) >> 4;
		uint8_t schemeOutput16 = (this->schemeOutput[7] & 0x0F);
#endif
		uint8_t inpSchemeOutput1 = (input.schemeOutput[0] & 0x0F);
		uint8_t inpSchemeOutput2 = (input.schemeOutput[0] & 0xF0) >> 4;
		uint8_t inpSchemeOutput3 = (input.schemeOutput[1] & 0x0F);
		uint8_t inpSchemeOutput4 = (input.schemeOutput[1] & 0xF0) >> 4;
		uint8_t inpSchemeOutput5 = (input.schemeOutput[2] & 0x0F);
		uint8_t inpSchemeOutput6 = (input.schemeOutput[2] & 0xF0) >> 4;
		uint8_t inpSchemeOutput7 = (input.schemeOutput[3] & 0x0F);
		uint8_t inpSchemeOutput8 = (input.schemeOutput[3] & 0xF0) >> 4;
		uint8_t inpSchemeOutput9 = (input.schemeOutput[4] & 0x0F);
		uint8_t inpSchemeOutput10 = (input.schemeOutput[4] & 0xF0) >> 4;
#if 0
		uint8_t inpSchemeOutput11 = (input.schemeOutput[5] & 0xF0) >> 4;
		uint8_t inpSchemeOutput12 = (input.schemeOutput[5] & 0x0F);
		uint8_t inpSchemeOutput13 = (input.schemeOutput[6] & 0xF0) >> 4;
		uint8_t inpSchemeOutput14 = (input.schemeOutput[6] & 0x0F);
		uint8_t inpSchemeOutput15 = (input.schemeOutput[7] & 0xF0) >> 4;
		uint8_t inpSchemeOutput16 = (input.schemeOutput[7] & 0x0F);
#endif
		if(this->mcc_1 != input.mcc_1)
			return (this->mcc_1 < input.mcc_1);
		else if(this->mcc_2 != input.mcc_2)
			return (this->mcc_2 < input.mcc_2);
		else if(this->mcc_3 != input.mcc_3)
			return (this->mcc_3 < input.mcc_3);
		else if(this->mnc_1 != input.mnc_1)
			return (this->mnc_1 < input.mnc_1);
		else if(this->mnc_2 != input.mnc_2)
			return (this->mnc_2 < input.mnc_2);
		else if(this->mnc_3 != input.mnc_3)
			return (this->mnc_3 < input.mnc_3);
		else if(this->routingIndicator != input.routingIndicator)
			return (this->routingIndicator < input.routingIndicator);
		else if(this->protectionSchemeId != input.protectionSchemeId)
			return (this->protectionSchemeId < input.protectionSchemeId);
		else if(this->homeNtwrkPKI != input.homeNtwrkPKI)
			return (this->homeNtwrkPKI < input.homeNtwrkPKI);
		else if(schemeOutput1 != inpSchemeOutput1)
			return (schemeOutput1 < inpSchemeOutput1);
		else if(schemeOutput2 != inpSchemeOutput2)
			return (schemeOutput2 < inpSchemeOutput2);
		else if(schemeOutput3 != inpSchemeOutput3)
			return (schemeOutput3 < inpSchemeOutput3);
		else if(schemeOutput4 != inpSchemeOutput4)
			return (schemeOutput4 < inpSchemeOutput4);
		else if(schemeOutput5 != inpSchemeOutput5)
			return (schemeOutput5 < inpSchemeOutput5);
		else if(schemeOutput6 != inpSchemeOutput6)
			return (schemeOutput6 < inpSchemeOutput6);
		else if(schemeOutput7 != inpSchemeOutput7)
			return (schemeOutput7 < inpSchemeOutput7);
		else if(schemeOutput8 != inpSchemeOutput8)
			return (schemeOutput8 < inpSchemeOutput8);
		else if(schemeOutput9 != inpSchemeOutput9)
			return (schemeOutput9 < inpSchemeOutput9);
		else
			return (schemeOutput10 < inpSchemeOutput10);
#if 0
		else if(schemeOutput11 != inpSchemeOutput11)
			return (schemeOutput11 < inpSchemeOutput11);
		else if(schemeOutput12 != inpSchemeOutput12)
			return (schemeOutput12 < inpSchemeOutput12);
		else if(schemeOutput13 != inpSchemeOutput13)
			return (schemeOutput13 < inpSchemeOutput13);
		else if(schemeOutput14 != inpSchemeOutput14)
			return (schemeOutput14 < inpSchemeOutput14);
		else if(schemeOutput15 != inpSchemeOutput15)
			return (schemeOutput15 < inpSchemeOutput15);
		else
			return (schemeOutput16 < inpSchemeOutput16);
#endif
	}
    std::string to_string()
    {
        std::string suci_str = "";
        suci_str += std::to_string(mcc_1);
        suci_str += std::to_string(mcc_2);
        suci_str += std::to_string(mcc_3);
        suci_str += std::to_string(mnc_1);
        suci_str += std::to_string(mnc_2);
        suci_str += std::to_string(mnc_3);
        suci_str += std::to_string((routingIndicator & 0xF000) >> 12);
        suci_str += std::to_string((routingIndicator & 0x0F00) >> 8);
        suci_str += std::to_string((routingIndicator & 0x00F0) >> 4);
        suci_str += std::to_string((routingIndicator & 0x000F));
        suci_str += std::to_string(protectionSchemeId);
        suci_str += std::to_string((homeNtwrkPKI & 0xF0) >> 4);
        suci_str += std::to_string((homeNtwrkPKI & 0x0F));
        for (int i=0; i<5; i++) {
			suci_str += std::to_string((schemeOutput[i] & 0x0F));
#if 0
			if(i == 7)
				continue;
#endif
	        suci_str += std::to_string(((schemeOutput[i] & 0xF0)>>4));
        }
        return suci_str;
    }
    std::string scheme_to_string()
    {
        std::string scheme_suci_str = "";
        for (int i=0; i<8; i++) {
			scheme_suci_str += std::to_string(((schemeOutput[i] & 0xF0)>>4));
			if(i == 7)
				continue;
	        scheme_suci_str += std::to_string((schemeOutput[i] & 0x0F));
        }
        return scheme_suci_str;
    }
    std::string scheme_to_imsi()
    {

        std::string scheme_imsi_str = "";
        scheme_imsi_str += std::to_string(mcc_1);
        scheme_imsi_str += std::to_string(mcc_2);
        scheme_imsi_str += std::to_string(mcc_3);
        scheme_imsi_str += std::to_string(mnc_1);
        scheme_imsi_str += std::to_string(mnc_2);
        mnc_3 &= 0xF;
        if(mnc_3 != 0xF){
            
            scheme_imsi_str += std::to_string(mnc_3);
            for (int i=0; i<5; i++) {
                scheme_imsi_str += std::to_string((schemeOutput[i] & 0x0F));
                if(i == 4)
                    continue;
                scheme_imsi_str += std::to_string(((schemeOutput[i] & 0xF0)>>4));
            }
        }
        else{

            for (int i=0; i<5; i++) {
                scheme_imsi_str += std::to_string((schemeOutput[i] & 0x0F));
                scheme_imsi_str += std::to_string(((schemeOutput[i] & 0xF0)>>4));
            }
        }
        return scheme_imsi_str;
    }
} suci_t;



typedef struct _imsi {
		uint8_t mcc:3;
		uint8_t mnc:3;
		uint8_t msin_high:1;
		uint8_t msin_low;
} imsi_t;

/* SUPI may take form of NAI which is either:-
1 IMSI based (always used for interworking with EPC or roaming)  OR
2 non-IMSI based (non-3GPP access or private network)
Ref. 23.501 5.9.2*/
typedef struct _nai
{
	unsigned char userName[MAX_LEN]; 
	unsigned char domainName[MAX_LEN]; 
}nai_t;

typedef struct _supi{
	uint8_t supi_format:1;
	union {
		imsi_t imsi;
		nai_t nai;
	};
}supi_t;


#endif 
