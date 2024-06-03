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

#include "nas/NasDatatypes/TAIList.h"

int addTAIUint(uint16_t mcc, uint16_t mnc, uint8_t mncLen, uint32_t tac,
                                            partialTAIList_t* pTAI) {

    nasLogENTRY;

    tAI_t tAI = {};

    tAI.mcc1 = mcc%10;
    mcc = mcc/10;
    tAI.mcc2 = mcc%10;
    mcc = mcc/10;
    tAI.mcc3 = mcc%10;

    tAI.mnc1 = mnc %10;
    mnc = mnc/10;
    tAI.mnc2 = mnc%10;
    mnc = mnc/10;
    if((mncLen == 2) ? tAI.mnc3 = 0xF : tAI.mnc3 = mnc%10);
    nasLog("mccDigit1 0x%x | mccDigit2 0x%x | mccDigit3 0x%x",
        tAI.mcc1, tAI.mcc2, tAI.mcc3);
    nasLog("mncDigit1 0x%x | mncDigit2 0x%x | mncDigit3 0x%x",
        tAI.mnc1, tAI.mnc2, tAI.mnc3);

    tAI.tac[0] = (tac & 0x00FF0000) >> 16;
    tAI.tac[1] = (tac & 0x0000FF00) >> 8;
    tAI.tac[2] = (tac & 0x000000FF);
    printArray("TAC", tAI.tac, TAC_SIZE);

    for(int i = 0; i < MAX_NUM_OF_TAI; i++) {
        /* TODO: Library maintains a variable to track tAI index ? */
        if(pTAI->tAI[i].mcc1 | pTAI->tAI[i].mcc2 | pTAI->tAI[i].mcc3) {
            continue;
        }
        pTAI->tAI[i].mcc1 = tAI.mcc1;
        pTAI->tAI[i].mcc2 = tAI.mcc2;
        pTAI->tAI[i].mcc3 = tAI.mcc3;
        pTAI->tAI[i].mnc1 = tAI.mnc1;
        pTAI->tAI[i].mnc2 = tAI.mnc2;
        pTAI->tAI[i].mnc3 = tAI.mnc3;
        memcpy(pTAI->tAI[i].tac, tAI.tac, TAC_SIZE);
        nasLogEXIT;
        return SUCCESS;
    }
    errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
    nasLogEXIT;
    return FAILURE;
}

int addPartialTAIList(partialTAIList_t pTAI, tAIList_t *taiList) {

    nasLogENTRY;

    if(taiList->filled >= (MAX_NUM_OF_TAI-1)) {
        higLog("TAIList filled up to brim. Can not accomodate more. Sorry");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    int i = taiList->filled;
    switch(pTAI.listType) {

        /* Note - Based on type of Partial TAI List(P-TAI), index of P-TAI
         * in TAI List and index of TAI in P-TAI will be different.
         * For example - P-TAI Type 1 has only one PLMN and one TAC.
         * P-TAI Type 0 has 1 PLMN and multiple TACs. Whereas P-TAI Type 2
         * has multiple PLMNs and multiple TACs.
         * So the variable used to track index of P-TAI in TAI list
         * cannot be used to track TAI in P-TAI (especially for P-TAI
         * type 0 and P-TAI type 2).
         */

        case PARTIAL_LIST_TYPE_1: {
            taiList->pTAI[i].numOfElmnts = pTAI.numOfElmnts;
            taiList->pTAI[i].listType = pTAI.listType;
            taiList->pTAI[i].tAI[i].mcc1 = pTAI.tAI[i].mcc1;
            taiList->pTAI[i].tAI[i].mcc2 = pTAI.tAI[i].mcc2;
            taiList->pTAI[i].tAI[i].mcc3 = pTAI.tAI[i].mcc3;
            taiList->pTAI[i].tAI[i].mnc1 = pTAI.tAI[i].mnc1;
            taiList->pTAI[i].tAI[i].mnc2 = pTAI.tAI[i].mnc2;
            taiList->pTAI[i].tAI[i].mnc3 = pTAI.tAI[i].mnc3;
            memcpy(taiList->pTAI[i].tAI[i].tac,pTAI.tAI[i].tac,TAC_SIZE);
            taiList->len += TAI_SIZE ;
            nasLog("Current TAI List len %d", taiList->len);
            taiList->filled ++;
            nasLogEXIT;
            return SUCCESS;
        }
        break;

        default: {
            higLog("Partial list type %d not supported", pTAI.listType);
            nasLogEXIT;
            return FAILURE;
        }
    }

    /* Control will not reach here */
}

void addPlmn(uint8_t* buffer, tAI_t* tai, uint8_t* consumedLen) {

    nasLogENTRY;

    uint8_t mask = 0;
    uint8_t len = 0;
    mask |= (tai->mcc1 & 0x0F);
    mask |= (tai->mcc2 & 0X0F) << 4;
    *(buffer + len) = mask;
    len++;
    mask = 0;
    mask |= (tai->mcc3 & 0x0F);
    mask |= (tai->mnc3 & 0X0F) << 4;
    *(buffer + len) = mask;
    len++;
    mask = 0;
    mask |= (tai->mnc1 & 0x0F);
    mask |= (tai->mnc2 & 0X0F) << 4;
    *(buffer + len) = mask;
    len++;
    *consumedLen = len;

    nasLogEXIT;
}

inline void addTac(uint8_t *buffer, tAI_t* tai, uint8_t* consumedLen) {

    memcpy(buffer, tai->tac, TAC_SIZE);
    *consumedLen = TAC_SIZE;
}

void retrievePlmn(uint8_t *buffer, tAI_t* tai, uint8_t* consumedLen) {

    nasLogENTRY;

    uint8_t mask = 0;
    uint8_t len = 0;
    mask = *(buffer + len);
    tai->mcc1 = (mask & 0x0F);
    tai->mcc2 = (mask & 0xF0) >> 4;
    len++;
    mask = *(buffer + len);
    tai->mcc3 = (mask & 0x0F);
    tai->mnc3 = (mask & 0xF0) >> 4;
    len++;
    mask = *(buffer + len);
    tai->mnc1 = (mask & 0x0F);
    tai->mnc2 = (mask & 0xF0) >> 4;
    len++;
    *consumedLen = len;

    nasLog("mccDigit1 0x%x | mccDigit2 0x%x | mccDigit3 0x%x",
        tai->mcc1, tai->mcc2, tai->mcc3);
    nasLog("mncDigit1 0x%x | mncDigit2 0x%x | mncDigit3 0x%x",
        tai->mnc1, tai->mnc2, tai->mnc3);
    nasLog("PLMN decoded from TAI | decoded %d bytes", (*consumedLen));

    nasLogEXIT;
}

inline void retrieveTac(uint8_t *buffer, tAI_t* tai, uint8_t* consumedLen) {

    memcpy(tai->tac, buffer, TAC_SIZE);
    *consumedLen = TAC_SIZE;
}

int decodeTAIList(uint8_t *buffer, uint32_t bufLen,
                       tAIList_t *taiList, uint32_t* decodedLen) {
    nasLogENTRY;

    uint32_t len = 0;
    uint8_t mask = 0;
    uint8_t ithLen = 0;
    uint8_t contentsLen = 0;
    uint8_t consumedLen = 0;
    partialTAIList_t pTai = {};
    tAI_t tai = {};

    if(buffer == NULL || bufLen == 0 ||
        taiList == NULL || decodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if(len + (TAI_LIST_LEN_MIN - TAI_LIST_OFFSET) > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }

    taiList->len = *(buffer + len);
    contentsLen = taiList->len;
    len++;
    nasLog("TAI List contents len %d", taiList->len);

    for(int i = 0; i < MAX_NUM_OF_TAI; i++) {
        contentsLen -= ithLen;
        if(!contentsLen) {
            nasLog("Decode complete for %d bytes specified in len field",
                                                            taiList->len);
            break;
        }
        mask = 0;
        ithLen = 0;
        pTai = {};
        mask = *(buffer + len);
        pTai.numOfElmnts = (mask & 0x1F);
        pTai.listType = (mask & 0x60) >> 5;
        ithLen++;
        len++;
        switch(pTai.listType) {

            case PARTIAL_LIST_TYPE_1: {

                tai = {};
                consumedLen = 0;
                retrievePlmn(buffer + len, &tai, &consumedLen);
                len += consumedLen;
                ithLen += consumedLen;
                consumedLen = 0;
                retrieveTac(buffer + len, &tai, &consumedLen);
                len += consumedLen;
                ithLen += consumedLen;
                pTai.tAI[0] = tai;
            }
            break;

            default: {
                higLog("Unsupported partial list type");
                continue;
            }
        }
        taiList->pTAI[i] = pTai;
    }
    *decodedLen = len;
    nasLog("Decoded %d bytes", *decodedLen);

    printArray("TAI List buffer", buffer, *decodedLen);

    nasLogEXIT;
    return SUCCESS;
}

int encodeTAIList(uint8_t *buffer, uint32_t bufLen,
                       tAIList_t *taiList, uint32_t *encodedLen) {
    nasLogENTRY;

    uint32_t len = 0;
    uint8_t mask = 0;
    uint8_t consumedLen = 0;
    uint8_t contentsLen = 0;
    partialTAIList_t pTai = {};
    tAI_t tai = {};

    if(buffer == NULL || bufLen == 0 || taiList == NULL || encodedLen == NULL)
    {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if (len + TAI_LIST_LEN_MIN > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    *(buffer + len) = taiList->len;
    len++;
    nasLog("TAI List contents len %d", taiList->len);

    for(int i = 0; i < MAX_NUM_OF_TAI; i++) {
        if(contentsLen == taiList->len) {
            break;
        }
        mask = 0;
        pTai = taiList->pTAI[i];
        mask |= pTai.numOfElmnts;
        mask |= ((pTai.listType) << 5);
        *(buffer + len) = mask;
        len++;
        contentsLen++;
        switch(pTai.listType) {

            case PARTIAL_LIST_TYPE_1: {

                tai = pTai.tAI[0];
                consumedLen = 0;
                addPlmn(buffer + len, &tai, &consumedLen);
                len += consumedLen;
                contentsLen += consumedLen;
                consumedLen = 0;
                addTac(buffer + len, &tai, &consumedLen);
                len += consumedLen;
                contentsLen += consumedLen;
            }
            break;

            default: {
                higLog("Unsupported partial list type");
                continue;
            }
        }
    }
    *encodedLen = len;
    nasLog("Encoded %d bytes", *encodedLen);

    printArray("TAI List buffer", buffer, *encodedLen);

    nasLogEXIT;
    return SUCCESS;
}
