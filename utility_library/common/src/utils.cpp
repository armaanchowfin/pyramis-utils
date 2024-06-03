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

#include <iostream>
#include <bitset>
#include <string>
#include "common/include/utils.h"

void getPlmnString(uint16_t mcc, uint16_t mnc, uint8_t mnc_digit_length,
                   std::string &str)
{
    str = std::to_string(MCC_HUNDREDS(mcc)) +
           std::to_string(MCC_MNC_TENS(mcc)) +
           std::to_string(MCC_MNC_DIGIT(mcc)) +
           ((MNC_HUNDREDS(mnc,mnc_digit_length) == 15) ?
                    std::string("") :
                    std::to_string(MNC_HUNDREDS(mnc, mnc_digit_length))) +
           std::to_string(MCC_MNC_TENS(mnc)) +
           std::to_string(MCC_MNC_DIGIT(mnc));
}

#define NAMEBUFF_LEN    100
#define PRINTBUFF_LEN   5000

void __printArray(const char *name, uint8_t* inputArray, size_t size,
                  const char *filename, const int line, const char *func)
{
    int counter = 0, len = PRINTBUFF_LEN;
    char printBuff[PRINTBUFF_LEN] = {};
    if(strlen(name) >= NAMEBUFF_LEN) {
        higLog("Likely Error: huge name");
        return;
    }
    if(size >= PRINTBUFF_LEN) {
        higLog("Internal Error: resource: unable to get array contents");
        return;
    }
    for(uint i=0;i<size;i++)
    {
        //print each byte as hex
        counter += snprintf(printBuff + counter, len - counter, "%02X ",
                            inputArray[i]);
    }
    utilLog(filename, line, func, "printArray: %s : %s", name, printBuff);
}

#undef NAMEBUFF_LEN
#undef PRINTBUFF_LEN

void bytecopy(uint8_t *dest, uint8_t *src, int len)
{
    for(int i=0;i<len;i++)
        dest[i]=src[i];
}


void __binPrint(const char *name, uint8_t val,
                const char *filename, const int line, const char *func)
{
    __attribute__((unused)) char bitstr[9] = {};
    for(int i=0; i<8;i++)
        bitstr[7-i] = val & (1<<i) ? '1' : '0';
    bitstr[8] = '\0';
    utilLog(filename, line, func, "binPrint %s :%s", name, bitstr);
}

string binaryPrint(uint16_t val)
{
    string bitStr = "";
    for(int i=15; i>=0; i--)
        bitStr += val & (1<<i) ? '1' : '0';
    return bitStr;
}

void __print_HEAV(_5gHeAv_t *udmHeAv,
                const char *filename, const int line, const char *func)
{
    __printArray("print 5G HE AV: RAND", udmHeAv->RAND, RAND_SIZE,
                 filename, line, func);
    __printArray("print 5G HE AV: AUTN", udmHeAv->AUTN, AUTN_SIZE,
                 filename, line, func);
    __printArray("print 5G HE AV: XRES", udmHeAv->XRES, RES_SIZE,
                 filename, line, func);
    __printArray("print 5G HE AV: Kausf",udmHeAv->Kausf, KAUSF_SIZE,
                 filename, line, func);
}

void __print_AV(_5gAv_t* ausfAv,
              const char *filename, const int line, const char *func)
{
    __printArray("print 5G AV: RAND", ausfAv->RAND, RAND_SIZE,
                 filename, line, func);
    __printArray("print 5G AV: AUTN", ausfAv->AUTN, AUTN_SIZE,
                 filename, line, func);
    __printArray("print 5G AV: HXRES", ausfAv->HXRES, XRES_SIZE,
                 filename, line, func);
    __printArray("print 5G AV: Kseaf", ausfAv->Kseaf, KSEAF_SIZE,
                 filename, line, func);
}

//TODO: IMSI format assumed, can be modified later
void __print_SUPI(supi_t supi,
                  const char *filename, const int line, const char *func)
{
    //Below Print Decimal,uncomment if needed
    //unary operator since these values are less than uint8_t i.e. byte
    //cout<<"mnc:"<<+supi.mcc<<endl;
    __binPrint("SUPI:: mcc",+supi.imsi.mcc, filename, line, func);
    __binPrint("SUPI:: mnc",+supi.imsi.mnc, filename, line, func);
    __binPrint("SUPI:: msin_high",supi.imsi.msin_high, filename, line, func);
    __binPrint("SUPI:: msin low",+supi.imsi.msin_low, filename, line, func);
}
void initialize_SUCI(suci_t &suci)
{
    suci.mcc_1 = 0x0;
    suci.mcc_2 = 0x0;
    suci.mcc_3 = 0x0;
    suci.mnc_1 = 0x0;
    suci.mnc_2 = 0x0;
    suci.mnc_3 = 0x0;
    suci.routingIndicator = 0x0000;
    suci.protectionSchemeId = 0x0;
    suci.homeNtwrkPKI = 0x00;
    suci.schemeOutput[5] = {};
}

void print_SUCI(suci_t suci)
{
    lowLog("SUCI:: mcc 0x%01x%01x%01x",suci.mcc_1, suci.mcc_2, suci.mcc_3);
    lowLog("SUCI:: mnc 0x%01x%01x%01x",suci.mnc_1, suci.mnc_2, suci.mnc_3);

    lowLog("SUCI:: routingIndicator 0x%02x%02x",
			(suci.routingIndicator & 0xFF00) >> 8,
			(suci.routingIndicator & 0x00FF));

    lowLog("SUCI:: protectionSchemeId 0x%02x", suci.protectionSchemeId);

    lowLog("SUCI:: homeNtwrkPKI 0x%02x", suci.homeNtwrkPKI);
#if 0
    lowLog("SUCI:: scheme output 0x%02x%02x%02x%02x%02x%02x%02x%01x",
            suci.schemeOutput[0], suci.schemeOutput[1],
            suci.schemeOutput[2], suci.schemeOutput[3],
            suci.schemeOutput[4], suci.schemeOutput[5],
            suci.schemeOutput[6], (suci.schemeOutput[7] & 0xF0)>>4);
#endif
    lowLog("SUCI:: scheme output 0x%02x%02x%02x%02x%02x",
            suci.schemeOutput[0], suci.schemeOutput[1],
            suci.schemeOutput[2], suci.schemeOutput[3],
            suci.schemeOutput[4]);
}

//Convert 2 decimal numbers from string to 2 octet at a time
void hexCopyFromString(uint8_t *arr, uint size, std::string hexS)
{
    for(uint i=0; i < size; i++) {
        string temp = hexS.substr(2*i, 2);
        arr[i] = (uint8_t) strtol(temp.c_str(), NULL, 16);
    }
}

void hexCopyToString(std::string &hexS, uint8_t *arr, uint size)
{
    char temp[size*2] = {}; /* each octet will be two */
    for(uint i=0; i< size ; i++) {
        sprintf(temp+ 2*i,"%02X", *(arr+i));
    }
    hexS = temp;
}

//Handling Case when username in NAI is IMSI, other case(NON-IMSI) not clear yet
//This method is not used as of now
std::string build_nai_string(imsi_t userName_imsi, std::string domainName){
    std::string imsi_nai_str = "";
    std::string nai_5gc = "@nai.5gc"; //can be defined as macro
    //using variable separator since it might be customized on different m/c
    std::string separator = ".";
    imsi_nai_str+= std::to_string(userName_imsi.mcc);
    imsi_nai_str+= std::to_string(userName_imsi.mnc);
    imsi_nai_str+= std::to_string(userName_imsi.msin_high);
    imsi_nai_str+= std::to_string(userName_imsi.msin_low);
    imsi_nai_str+= nai_5gc + separator;
    imsi_nai_str+= "mnc"+ std::to_string(userName_imsi.mnc) + separator;
    imsi_nai_str+= "mcc"+ std::to_string(userName_imsi.mcc) + separator;
    imsi_nai_str+= domainName; //e.g. "3gppnetwork.org"
    return imsi_nai_str;
}

/*TODO: this is temporary function, to be replaced by the one which fetched
 * deconcealed supi from UDM->AUSF->AMF */
supi_t getSupi() {
    supi_t supi = {};
    supi.supi_format = SUPI_IMSI & 0x1;
    supi.imsi.mcc = 0x7 & 0x0F;
    supi.imsi.mnc = 0x7 & 0x0F;
    supi.imsi.msin_high = 0x1 & 0x0F;
    supi.imsi.msin_low = 0xBB;//8 bit as expected

    return supi;
}

/*Helper Method Added manually:
Required for compatibility in case of ue Security Capabilities*/
uint16_t mirrorOctet(uint8_t input)
{
	uint16_t output=0;
	output|=(input>>7)&0b00000001;
	output|=(input>>5)&0b00000010;
	output|=(input>>3)&0b00000100;
	output|=(input>>1)&0b00001000;
	output|=(input<<1)&0b00010000;
	output|=(input<<3)&0b00100000;
	output|=(input<<5)&0b01000000;
	output|=(input<<7)&0b10000000;
	return output;
}

void copyNSSAI(nssai_t srcNSSAI, nssai_t destNSSAI)
{
    int no_of_slices = srcNSSAI.no_of_slices;
    //copy regRequest NSSAI into amf::requestedNSSAI
    destNSSAI.no_of_slices = no_of_slices;
    lowLog("Required NSSAI has %d slice/s!",no_of_slices);

    //NOTE: NSSAI is already checked in NAS decode for correctness
    for(int i=0; i<no_of_slices; i++)
    {
        if(srcNSSAI.Nssai[i].len_s_nssai == LEN_SST_AND_SD)
        {
            destNSSAI.Nssai[i].len_s_nssai =
                            srcNSSAI.Nssai[i].len_s_nssai;
            destNSSAI.Nssai[i].sST =
                                    srcNSSAI.Nssai[i].sST;
            destNSSAI.Nssai[i].sD =
                                    srcNSSAI.Nssai[i].sD;
            lowLog("S-NSSAI type:%d", srcNSSAI.Nssai[i].len_s_nssai);
            lowLog("S-NSSAI %d: SST= %d, SD=%lu", i,
                    srcNSSAI.Nssai[i].sST, srcNSSAI.Nssai[i].sD);
        }
    }
}


