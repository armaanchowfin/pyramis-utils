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
#include <string>
#include "ran/intf/include/ue.h"
#include "common/include/datatypes.h"
#include "common/include/utils.h"
//#include "ausf/include/ausf.h"
//#include "udm/include/udm.h"
#include "platform/include/logging.h" //For traces
#include "aka/include/securityKeysUtil.h"

using namespace std;

supi_t getSupi() {
    supi_t supi = {};
    supi.mcc = 0x7 & 0x0F;
    supi.mnc = 0x7 & 0x0F;
    supi.msin_high = 0x1 & 0x0F;
    supi.msin_low = 0xBB;//8 bit as expected

    return supi;
}


int test()
{
#if 0
    int rc=FAILURE;
    ausf* ausfInstance = ausf::getInstance();
    suci_t suci={};
    suci.mcc_1=0x4 & 0x0F;
    suci.mcc_2=0x5 & 0x0F ;
    suci.mcc_3=0x3 & 0x0F;
    suci.mnc_1=0x2 & 0x0F;
    suci.mnc_2=0x7 & 0x0F;
    suci.mnc_3=0x6 & 0x0F;
    suci.routingIndicator=0x55aa;
    suci.protectionSchemeId=0xc & 0x0F;
    const char* snName = "5G:SNId";

    //Authentication request to AUSF with Dummy suci, which calls udm->uEAuthenticateRequest in turn
    rc = ausfInstance->Nausf_UEAuthenticate_Request(suci, (char*)snName);
    if(rc == FAILURE) {
        return rc;
    }

    _5gAv_t ausfAv;
    rc = ausfInstance->Nausf_UEAuthenticate_Response(suci, &ausfAv);
    if(rc == FAILURE) {
        return rc;
    }

    print_AV(&ausfAv);
    
    //Verifying HE AV at AUSF
    ausf::ueContext_t ueContext = ausfInstance->ueMap.at(suci);
    _5gHeAv_t *tmpHeAv = &ueContext.secCtxt._5gHeAv;
    print_HEAV(tmpHeAv);
#endif
    return 0;
}

int test1()
{
    int rc=-1;

    /*uint8_t Kseaf[KSEAF_SIZE]={0x7e, 0x03, 0x3c, 0x21, 0xe3, 0x49, 0x3c, 0x7e,
                           0x03, 0x5d, 0x49, 0x21, 0x03, 0x7d, 0xaa, 0xff,
                           0x5e, 0x33, 0xc5, 0x16, 0xdd, 0x44, 0xf3, 0xe9,
                           0x08, 0x4c, 0xcc, 0x61, 0x01, 0x77, 0x1a, 0xef};*/


    uint8_t Kamf[KAMF_SIZE] = {};
    printArray("Kamf",Kamf, KAMF_SIZE);
    uint8_t Kseaf2[KSEAF_SIZE] = {};
    printArray("Kseaf2", Kseaf2, KSEAF_SIZE);
    supi_t supi2 = {};//getSupi();
    uint8_t ABBA = 0;
    print_SUPI(supi2);

    //rc = computeKamf( Kseaf, supi2, ABBA, Kamf);
    rc = computeKamf( Kseaf2, supi2, ABBA, Kamf);
    if(rc == FAILURE)
    {
        higLog("%s","computeKamf failed");
        LOG_EXIT;
        return rc;
    }
    printArray("Kamf", Kamf, KAMF_SIZE);
    return 0;
}
int main()
{
    int rc = test1();
    string status = rc? "KO":"OK";
    cout<<"========= AUSF/UDM Test status::"<<status<<"========"<<endl; 

    return 0;
}





