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

#include "nas/NasDatatypes/rejectedNssai.h"

int decodeRejectedNssai(uint8_t *buffer, uint32_t bufLen,
                       rejectedNssai_t* _rejectedNssai, uint32_t* decodedLen)
{
    nasLogENTRY;
    uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || _rejectedNssai == NULL || 
        decodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    int _nssaiLen = *(buffer + len);
    len++;
    nasLog("rejected nssai len in buffer:%d",_nssaiLen);
    if(len + _nssaiLen > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }
    else if(_nssaiLen < NSSAI_LEN_MIN ||
            _nssaiLen > NSSAI_LEN_MAX) {
        higLog("%s", "RejectedNssai NSSAI with incorrect length");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    int _nssaiLen2 = _nssaiLen;
    /* Total buff remaining that has to be extracted */
    int pos = 0;
    while(_nssaiLen2 > 0)
    {
        if(len + sizeof(uint8_t) > bufLen) {
            higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
            errno = E_NAS_ERROR_BUFF_INSUFF;
            nasLogEXIT;
            return FAILURE;
        }

        uint8_t temp = *(buffer + len);
        _rejectedNssai->Nssai[pos].len_s_nssai = (temp & 0xF0) >> 4;
        _rejectedNssai->Nssai[pos].reject_cause = temp & 0x0F;
        len++;

        switch(_rejectedNssai->Nssai[pos].len_s_nssai)
        {
            //only supported mode as of now
            case LEN_SST_AND_SD:
            {
                if((len + SIZE_S_NSSAI - sizeof(temp)) > bufLen)
                {
                    higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
                    errno = E_NAS_ERROR_BUFF_INSUFF;
                    nasLogEXIT;
                    return FAILURE;
                }

                _rejectedNssai->Nssai[pos].sST = buffer[len++];
                _rejectedNssai->Nssai[pos].sD  = ((uint32_t)buffer[len++])<< 16;
                _rejectedNssai->Nssai[pos].sD |= ((uint32_t)buffer[len++])<< 8;
                _rejectedNssai->Nssai[pos].sD |= buffer[len++];
                
                _nssaiLen2 -= SIZE_S_NSSAI;
                nasLog("S-NSSAI decoded | currentLen = %d | encoded %d bytes", 
                    len, SIZE_S_NSSAI);
            }
            break;
            default:
                higLog("Unsupported S-NSSAI length!");
                errno = E_NAS_ERROR_INVALID_INPUT;
                nasLogEXIT;
                return FAILURE;

        }
        pos++;
        if(pos >= MAX_S_NSSAI) {
            higLog("nssai Len incorrect");
            errno = E_NAS_ERROR_INVALID_INPUT;
            nasLogEXIT;
            return FAILURE;    
        }
    }

    if(_nssaiLen2 != 0) {
        higLog("nssai Len incorrect");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    nasLog("NSSAI decoded | currentLen = %d | decoded %d bytes", 
                                            _nssaiLen-NSSAI_IEI_SIZE, len);

   _rejectedNssai->no_of_slices = pos;
    for(int i=0; i< pos; i++)
    {
        if(_rejectedNssai->Nssai[i].reject_cause == CAUSE_S_NSSAI_NA_FOR_REGAREA)
            nasLog("Reject Cause: S-NSSAI Not Available in the current registration area");
        else if(_rejectedNssai->Nssai[i].reject_cause == CAUSE_S_NSSAI_NA_FOR_PLMN)
            nasLog("Reject Cause: S-NSSAI Not Available in the current PLMN");

        nasLog("S-NSSAI type:%d", _rejectedNssai->Nssai[i].len_s_nssai);
        nasLog("S-NSSAI %d: SST= %d, SD=%lu", i, 
                    _rejectedNssai->Nssai[i].sST, _rejectedNssai->Nssai[i].sD);
    }

    *decodedLen = len;
    nasLogEXIT;
    return SUCCESS;

}

int encodeRejectedNssai(uint8_t *buffer, uint32_t bufLen,
                       rejectedNssai_t* _rejectedNssai, uint32_t *encodedLen)
{
    nasLogENTRY;
    uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || _rejectedNssai == NULL || 
        encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if( _rejectedNssai->no_of_slices < 1 ||
        _rejectedNssai->no_of_slices > MAX_S_NSSAI ) {
        errno = E_NAS_ERROR_INVALID_INPUT;
        higLog("Wrong S-NSSAI/s present in Rejected NSSAI: %d",
                                                _rejectedNssai->no_of_slices);
        nasLogEXIT;
        return FAILURE;
    }

    //first octet will contain total NSSAI length,we fill it after each S-NSSAI
    uint8_t * rejNssaiLen = buffer;
    *rejNssaiLen = 0;

    len += 1;
    nasLog("NSSAI len encoded | currentLen = %d | encoded 1 bytes", len);
    /* TODO: use offset, rm len*/
    for(int i=0; i<_rejectedNssai->no_of_slices; i++)
    {
        switch(_rejectedNssai->Nssai[i].len_s_nssai)
        {
            //only supported mode as of now
            case LEN_SST_AND_SD:
            {
                if(len + SIZE_S_NSSAI > bufLen)
                {
                    higLog("%s","Buffer capacity insufficient!");
                    errno = E_NAS_ERROR_BUFF_INSUFF;
                    nasLogEXIT;
                    return FAILURE;
                }

                //1st octet combined from len_s_nssai and reject cause
                buffer[len++] = (_rejectedNssai->Nssai[i].len_s_nssai << 4) |
                                (_rejectedNssai->Nssai[i].reject_cause);
                buffer[len++] = _rejectedNssai->Nssai[i].sST;
                buffer[len++] = _rejectedNssai->Nssai[i].sD >> 16;
                buffer[len++] = (_rejectedNssai->Nssai[i].sD >> 8) % 256;
                buffer[len++] = (_rejectedNssai->Nssai[i].sD) % 256;
                *rejNssaiLen += SIZE_S_NSSAI;
                nasLog(" S-NSSAI encoded | currentLen = %d | encoded %d bytes",
                    len, SIZE_S_NSSAI);
            }
            break;

            default:
                higLog("Unsupported S-NSSAI length!");
                errno = E_NAS_ERROR_INVALID_INPUT;
                nasLogEXIT;
                return FAILURE;
        }
    }
    nasLog("NSSAI length %d ", len);

    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;
}
