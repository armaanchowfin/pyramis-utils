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

#include "nas/NasDatatypes/nssai.h"

/*decoder for a single NSSAI IE */
int decodeSnssai(uint8_t *buffer, uint32_t bufLen,
                       s_nssai_t* _snssai, uint32_t* decodedLen)
{
    nasLogENTRY;
    uint32_t len = 0;
    if(buffer == NULL || bufLen == 0 || _snssai == NULL || decodedLen == NULL){
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    /* check if buffer has enough content of not */
    if(len + sizeof(_snssai->len_s_nssai) > bufLen) {
        higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
        errno = E_NAS_ERROR_BUFF_INSUFF;
        nasLogEXIT;
        return FAILURE;
    }

    /*decode snssai len part, this len does not represent the entire len of 
    snssai IE */
    int _snssaiLen = *(buffer + len);
    len++;
    nasLog("S-NSSAI len type:%d", _snssaiLen);

    /* refer to Table 9.11.2.8.1 for different values of _snssaiLen */
    switch(_snssaiLen)
    {
        case LEN_SST_AND_SD: /* 0b00000100 SST and SD */
        {   
            /* check if buffer has enough content of not */
            if(len + SIZE_S_NSSAI - sizeof(_snssai->len_s_nssai) > bufLen)
            {
                higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
                errno = E_NAS_ERROR_BUFF_INSUFF;
                nasLogEXIT;
                return FAILURE;
            }
            /*if yes, decode all of the snssai elements */
            _snssai->sST = buffer[len++];
            _snssai->sD = ((uint32_t)buffer[len++])<< 16;
            _snssai->sD |= ((uint32_t)buffer[len++])<< 8;
            _snssai->sD |= buffer[len++];
            _snssai->len_s_nssai = LEN_SST_AND_SD;
            nasLog("S-NSSAI decoded sST %d, sD %lu | currentLen = %d | "
                    "decoded %d bytes", _snssai->sST, _snssai->sD,
                    len, SIZE_S_NSSAI);
        }
    }
    /*assign len value to decodedLen*/
    *decodedLen = len;
    nasLogEXIT;
    return SUCCESS;       
}

/*Enoder for a single NSSAI IE*/
int encodeSnssai(uint8_t *buffer, uint32_t bufLen,
                       s_nssai_t* _snssai, uint32_t *encodedLen)
{
    nasLogENTRY;
    uint32_t len = 0;
    if(buffer == NULL || bufLen == 0 || _snssai == NULL || encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }
    /* check the special len param, it does not include the whole length of
    nssai IE */
    /* no need to encode the total length */
    /* refer to Table 9.11.2.8.1 for different values of _snssaiLen */
    switch(_snssai->len_s_nssai) 
    {
        case LEN_SST_AND_SD: /* 0b00000100 SST and SD */
        {
            if(len + SIZE_S_NSSAI > bufLen)
            {
                higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
                errno = E_NAS_ERROR_BUFF_INSUFF;
                nasLogEXIT;
                return FAILURE;
            }
            buffer[len++] = _snssai->len_s_nssai;/*len of snssai IE is encoded*/
            buffer[len++] = _snssai->sST;
            buffer[len++] = _snssai->sD >> 16;
            buffer[len++] = (_snssai->sD >> 8)%256;
            buffer[len++] = (_snssai->sD)%256;
            nasLog("S-NSSAI encoded sST %d, sD %lu | currentLen = %d | encoded %d bytes",
                    _snssai->sST, _snssai->sD,
                    len, SIZE_S_NSSAI);                                
        }
        break;
        default:
            higLog("Unsupported S-NSSAI length!");
            errno = E_NAS_ERROR_INVALID_INPUT;
            nasLogEXIT;
            return FAILURE;        
    }

    nasLog("S-NSSAI length %d ", len);
    *encodedLen = len;
    nasLogEXIT;
    return SUCCESS;            
}


int decodeNssai(uint8_t *buffer, uint32_t bufLen,
                       nssai_t* _nssai, uint32_t* decodedLen)
{
    nasLogENTRY;
    uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || _nssai == NULL || decodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure decoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    int _nssaiLen = *(buffer + len);
    len++;
    nasLog("nssai len in buffer:%d",_nssaiLen);
    int _nssaiOctets = _nssaiLen + NSSAI_IEI_SIZE + NSSAI_LEN_SIZE;
    if(len + _nssaiLen > bufLen) {
        higLog("%s", "FAILURE, Incomplete buffer");
        errno = E_NAS_ERROR_BUFF_INCOMPLETE;
        nasLogEXIT;
        return FAILURE;
    }
    else if(_nssaiOctets < NSSAI_LEN_MIN ||
            _nssaiOctets > NSSAI_LEN_MAX) {
        higLog("%s", "Requested NSSAI with incorrect length");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    int _nssaiLen2 = _nssaiLen;
    /* Total buff remaining that has to be extracted */
    int pos = 0;
    while(_nssaiLen2 > 0) {
        if (pos >= MAX_S_NSSAI) {
            higLog("The number of S-NSSAI values in a requested NSSAI or "
                             "allowed NSSAI cannot exceed %d", MAX_S_NSSAI);
            errno = E_NAS_ERROR_INVALID_INPUT;
            nasLogEXIT;
            return FAILURE;    
        }

        if(len + sizeof(_nssai->Nssai[pos].len_s_nssai) > bufLen) {
            higLog("%s", "FAILURE, Incomplete buffer");
            errno = E_NAS_ERROR_BUFF_INCOMPLETE;
            nasLogEXIT;
            return FAILURE;
        }

        _nssai->Nssai[pos].len_s_nssai = *(buffer + len);
        len++;
        nasLog("S-NSSAI len type:%d", _nssai->Nssai[pos].len_s_nssai);
        switch(_nssai->Nssai[pos].len_s_nssai)
        {
            //only supported mode as of now
            case LEN_SST_AND_SD:
            {
                if(len + SIZE_S_NSSAI - sizeof(_nssai->Nssai[pos].len_s_nssai)
                    > bufLen)
                {
                    higLog("%s", "FAILURE, Incomplete buffer");
                    errno = E_NAS_ERROR_BUFF_INCOMPLETE;
                    nasLogEXIT;
                    return FAILURE;
                }

                _nssai->Nssai[pos].sST = buffer[len++];
                _nssai->Nssai[pos].sD = ((uint32_t)buffer[len++])<< 16;
                _nssai->Nssai[pos].sD |= ((uint32_t)buffer[len++])<< 8;
                _nssai->Nssai[pos].sD |= buffer[len++];
                
                _nssaiLen2 -= SIZE_S_NSSAI;
                nasLog("S-NSSAI decoded sST %d, sD %lu | currentLen = %d | decoded %d bytes",
                        _nssai->Nssai[pos].sST, _nssai->Nssai[pos].sD,
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
    }

    if(_nssaiLen2 != 0) {
        higLog("nssai Len incorrect");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    nasLog("NSSAI decoded | currentLen = %d | decoded %d bytes", 
                                            _nssaiLen, len);

   _nssai->no_of_slices = pos;
    for(int i=0; i< pos; i++)
    {
        nasLog("S-NSSAI type:%d", _nssai->Nssai[i].len_s_nssai);
        nasLog("S-NSSAI %d: SST= %d, SD=%lu", i, 
                        _nssai->Nssai[i].sST, _nssai->Nssai[i].sD);
    }

    *decodedLen = len;
    nasLogEXIT;
    return SUCCESS;

}

int encodeNssai(uint8_t *buffer, uint32_t bufLen,
                       nssai_t* _nssai, uint32_t *encodedLen)
{
    nasLogENTRY;
    uint32_t len = 0;
    
    if(buffer == NULL || bufLen == 0 || _nssai == NULL || encodedLen == NULL) {
        higLog("%s", "Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    if( _nssai->no_of_slices < 1 ||
        _nssai->no_of_slices > MAX_S_NSSAI ) {
        errno = E_NAS_ERROR_INVALID_INPUT;
        higLog("Invalid/insufficient S-NSSAI's: %d",  _nssai->no_of_slices);
        nasLogEXIT;
        return FAILURE;
    }

    //first octet will contain total NSSAI length, we fill it after each S-NSSAI
    uint8_t * nssaiLen = buffer;
    *nssaiLen = 0;

    len += 1;
    nasLog("NSSAI len encoded | currentLen = %d | encoded 1 bytes", len);
    /* TODO: use offset, rm len*/
    for(int i=0; i<_nssai->no_of_slices; i++)
    {
        switch(_nssai->Nssai[i].len_s_nssai)
        {
            //only supported mode as of now
            case LEN_SST_AND_SD:
            {
                if(len + SIZE_S_NSSAI > bufLen)
                {
                    higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
                    errno = E_NAS_ERROR_BUFF_INSUFF;
                    nasLogEXIT;
                    return FAILURE;
                }

                if (len + SIZE_S_NSSAI + NSSAI_IEI_SIZE > NSSAI_LEN_MAX) {
                    errno = E_NAS_ERROR_INVALID_INPUT;
                    higLog("crossed maximum Nssai octect: %d",
                            len + SIZE_S_NSSAI + NSSAI_IEI_SIZE);
                    nasLogEXIT;
                    return FAILURE;
                    
                }

                buffer[len++] = _nssai->Nssai[i].len_s_nssai;
                buffer[len++] = _nssai->Nssai[i].sST;
                buffer[len++] = _nssai->Nssai[i].sD >> 16;
                buffer[len++] = (_nssai->Nssai[i].sD >> 8)%256;
                buffer[len++] = (_nssai->Nssai[i].sD)%256;
                *nssaiLen += SIZE_S_NSSAI;
                nasLog("S-NSSAI encoded sST %d, sD %lu | currentLen = %d | encoded %d bytes",
                        _nssai->Nssai[i].sST, _nssai->Nssai[i].sD,
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
