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

/*
    TODO unify the log statements
*/
#include "nas/NasMessage.h"
#include "nas/mobility/_5gmmMsgApi.h"
#include "aka/include/securityContext.h"
#include "common/include/utils.h"
#include "aka/include/securityKeysUtil.h"

#define IS_ENCODING 1
#define IS_DECODING 2


/* internal functions used by nasMessageEncode and nasMessageDecode */
int nasMessageEncrypt(const uint8_t *plainMsg, int msglength,
    uint8_t direction,void *security, uint8_t * cipherMsg);
int nasMessageDecrypt(const uint8_t *cipherMsg, int length,
    uint8_t direction,void *security, uint8_t* origMsg);
int nasMessagePlainDecode(uint8_t * buffer, nasMessagePlain_t *msgStruct,
    uint32_t decodedLen);
int nasMessagePlainEncode(uint8_t *buffer,uint32_t bufLen,
                          nasMessagePlain_t *msgStruct,uint32_t *encodedLen);
int nasMessageGetMac(const uint8_t *nasMsg,size_t msgLen,int direction,
                     void *security,uint8_t *nasMac);
void fillNasInputStream(nasInputStream_t *nasStream,uint8_t NAS_Key[NAS_KEY_SIZE],
                        uint8_t countArray[SIZE_NASCOUNT], uint8_t direction,
                        uint8_t bearer, const uint8_t *nasMessage, size_t msgLength);
int checkForSecCapMismatch(secContext_t *secContext ,
    SecurityModeCommand_t * secModeCmdMsg);

int matchNasMac(uint8_t *buffer, uint32_t decodedLen, uint backupCount,
    secContext_t *secContext, secContext_t *tempSecCtxt);

/* Peep at the plain part of nasMsg */
/* Main use only for Integrity Protected NAS msg */
int nasMessagePeepAtPlain(uint8_t *buffer, uint32_t bufLen,
                            nasMessagePlain_t *plainMsg) {
    nasLogENTRY;
    errno = E_NAS_ERROR_UNKNOWN; /* entry into the library */
    int rc = FAILURE;

    if (buffer == NULL || plainMsg == NULL ||
            bufLen < SIZE_EPD_AND_SECU_HEADER) {
        higLog("Invalid Input, Failure peeping");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    uint8_t epd = buffer[0];
    if(epd != _5GS_MOBILITY_MANAGEMENT_MESSAGE) {
        higLog("Makes sense to call this function only for 5GMM msg."
                " 5GSM are always plain msgs anyways.");
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
        nasLogEXIT;
        return FAILURE;
    }

    uint8_t secuHeader = buffer[1];
    if((secuHeader == SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED) ||
        (secuHeader == SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED_NEW)) {
    }

    switch(secuHeader) {
        case SECURITY_HEADER_TYPE_NOT_PROTECTED: {
            lowLog("Input msg itself is plain nas msg");
        }
        break;
        case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED: {
            uint8_t plain[MAX_MSG_LEN] = {};
            memcpy(plain , buffer + SIZE_MM_SECU_HEADER,
                    bufLen - SIZE_MM_SECU_HEADER);
            rc = nasMessagePlainDecode(plain, plainMsg,
                                        bufLen - SIZE_MM_SECU_HEADER);
            if(rc == FAILURE) {
                higLog("nasMessagePlainDecode failed");
                nasLogEXIT;
                return FAILURE;
            }
            lowLog("peep successful");
        }
        break;
        case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_NEW: {
            lowLog("Input msg should certainly be a SEC MODE CMD msg");
        }
        break;
        case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED:
        case SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED_NEW: {
            higLog("Can't peep for Plain part in Ciphered msg w/o decryption");
            errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
            nasLogEXIT;
            return FAILURE;
        }
        break;
        default: {
            higLog("Invalid Sec Header");
            errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;
            nasLogEXIT;
            return FAILURE;
        }
    }

    nasLogEXIT;
    return SUCCESS;
}

/* Encode a plain nas message into buffer. */
int nasMessagePlainEncode(uint8_t *buffer,uint32_t bufLen,
                          nasMessagePlain_t *msgStruct,
                          uint32_t *encodedLen)
{
    nasLogENTRY;

    /* encode from epd till the end */
    int rc = 0;

    if(msgStruct->_5gmmMsg.mmheader.epd == _5GS_MOBILITY_MANAGEMENT_MESSAGE) {
        rc = _5gmmMsgEncode(buffer,bufLen,&(msgStruct->_5gmmMsg),encodedLen);
            /*
                this _5gmmMsgEncode() function will further call the
                actual nasMessage encode function calls
                For example:
                    1. RegistrationRequest
                    2. AuthenticationRequest
                    3. SecurityModeCommand
                     and many other MM nas messages
            */
        if(rc == FAILURE) {
            higLog("%s","_5gmmMsgEncode encode function failed");
            nasLogEXIT;
            return FAILURE;
        }

    /*FIXME: the check should be based on smheader, not mmheader */
    }else if(msgStruct->_5gsmMsg.smheader.epd == _5GS_SESSION_MANAGEMENT_MESSAGE) {
        rc = _5gsmMsgEncode(buffer,bufLen,&(msgStruct->_5gsmMsg),encodedLen);   /*arguments TODO:(ddeka)*/
        if(rc == FAILURE) {
            higLog("%s","_5gsmMsgEncode encode function failed");
            nasLogEXIT;
            return FAILURE;
        }
    }else {
        higLog("%s","Unsupported ExtendedProtocolDiscriminator");
        errno = E_NAS_ERROR_INVALID_PDU;
        nasLogEXIT;
        return FAILURE;
    }

    // TODO:ddeka is there anythign else to do inside this function ??
    nasLog("encoded plainMsg currentLen = %d ", *encodedLen);

    nasLogEXIT;
    return SUCCESS;
}

int nasMessagePlainDecode(uint8_t * buffer, nasMessagePlain_t *msgStruct,
                          uint32_t decodedLen)
{
    nasLogENTRY;

    int epd = buffer[0];
    int rc;

    if(epd == _5GS_MOBILITY_MANAGEMENT_MESSAGE) {
        rc = _5gmmMsgDecode(buffer, &(msgStruct->_5gmmMsg),decodedLen);
        if(rc == FAILURE) {
            higLog("%s","_5gmmMsgDecode decode function failed");
            nasLogEXIT;
            return FAILURE;
        }
    }else if(epd == _5GS_SESSION_MANAGEMENT_MESSAGE) {
        rc = _5gsmMsgDecode(buffer, &(msgStruct->_5gsmMsg),decodedLen);   /*arguments TODO:(ddeka)*/
        if(rc == FAILURE) {
            higLog("%s","_5gsmMsgDecode decode function failed");
            nasLogEXIT;
            return FAILURE;
        }
    }else {
        higLog("%s","Unsupported ExtendedProtocolDiscriminator");
        errno = E_NAS_ERROR_INVALID_PDU;
        nasLogEXIT;
        return FAILURE;
    }

    nasLog("decoded plainMsg Len = %d", decodedLen);
    nasLogEXIT;
    return SUCCESS;
}

/*
    Encode a nasMessage_t type struct into a linear buffer
    input :
        1. buffer pointer
        2. buffer length allocated before
        3. nasMessage_t msgStruct pointer
        4. int * encodedLen : which will indicate how much bytes has been encoded
        5. security context from the caller : accessed for security purpose
*/

/*  nasMessageEncode(nasMessage_t) -> nasBuf
 *  |
 *  L__ if msg is security protected:
 *  |   L__ encode the complete message in a plain Msg.
 *  |   |   nasMessagePlainEncode(nasMessage_t->plain) -> tempBuf
 *  |   L__ encrypt buff in tempBuf using securityContext, add it to nasBuf
 *  |   |   nasMessageEncrypt(tempBuf, secContext, securityHeaderType) -> nasBuf
 *  |   L__ update ul and dl count in securityContext
 *  |       (revert if nasMac calculation fails)
 *  |   L__ calc nasMac : nasMessageGetMac(nasBuf) -> nasMac
 *  |   L__ encode epd and securityHeaderType
 *  |   L__ encode macCode into nasBuf
 *  |
 *  L__ if msg is a plain message
 *      L__ encode message into nasBuf
 *          nasMessagePlainEncode(nasMessage_t->plain) -> nasBuf
 */
int nasMessageEncode(uint8_t *buffer, uint32_t bufLen, nasMessage_t *msgStruct,
                     uint32_t *encodedLen, void *security)
{
    nasLogENTRY;
    errno = E_NAS_ERROR_UNKNOWN; /* entry into the library */

    int rc = 0;
    if (buffer == NULL || bufLen < SIZE_EPD_AND_SECU_HEADER
        || msgStruct == NULL || encodedLen ==NULL) {
        higLog("%s","Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }
    // peek the securityHeader of the nasMessage only : nasMessage is an union
    nasMessageSecurityHeader_t *header = &msgStruct->header;

    /* Create a new variable len that will keep track of the total length
     * encoded till a point */
    uint32_t len = 0;

    /* it is NOT a plain nas Msg */
    if(header->epd == _5GS_MOBILITY_MANAGEMENT_MESSAGE &&
            header->secuHeader != SECURITY_HEADER_TYPE_NOT_PROTECTED) {

        /* First encode the plain message */
        uint32_t plainMsgEncodedLen = 0;
        uint8_t plainMsg[MAX_MSG_LEN] = {}; /* FIXME : MAX_MESSAGE_SIZE ? */
        rc = nasMessagePlainEncode(plainMsg, MAX_MSG_LEN, &(msgStruct->plain),
                                   &plainMsgEncodedLen);
        if(rc == FAILURE) {
            higLog("%s","nasMessagePlainEncode() failed");
            nasLogEXIT;
            return FAILURE;
        }
        /* buffer must be able to contain:
         * epd(1) + secHeader(1) + macCode(4) + sqnNum(1) + encrypted msg len */
        if(SIZE_MM_SECU_HEADER + plainMsgEncodedLen > bufLen) {
            higLog("%s", "Buffer capacity insufficient, FAILURE encoding");
            errno = E_NAS_ERROR_BUFF_INSUFF;
            nasLogEXIT;
            return FAILURE;
        }
        /* ok, the buffer has sufficient space. Now encrypt plainMsg and store
         * into buffer.
         * IMPORTANT: plainmsg len does not change on encryption.*/

        if(security == NULL) {
            higLog("%s","ERROR: security context NULL");
            errno = E_NAS_ERROR_SECCTX_NULL;
            nasLogEXIT;
            return FAILURE;
        }
        secContext_t *secContext = static_cast<secContext_t *>(security);

        /*save a backup of seqNum. revert to old value if a failure occurs. */
        uint32_t backupCount = 0;   /* prev value */
        if(SECU_DIRECTION_DOWNLINK == secContext->direction_encode) { /*AMF*/
            backupCount = *((uint32_t*) &secContext->dl_count); /*backup */
            secContext->dl_count.seq_num++; /*update */
            if(!secContext->dl_count.seq_num) {
                secContext->dl_count.overflow += 1;
            }
            header->sqnNum = secContext->dl_count.seq_num;
            nasLog("dl_count.seq_num  : %2X | dl_count.overflow : %2X",
            secContext->dl_count.seq_num, secContext->dl_count.overflow);
        } else {
            backupCount = *((uint32_t*) &secContext->ul_count);
            secContext->ul_count.seq_num++;
            if(!secContext->ul_count.seq_num) {
                secContext->ul_count.overflow += 1;
            }
            header->sqnNum = secContext->ul_count.seq_num;
            nasLog("ul_count.seq_num : %2X | ul_count.overflow : %2X",
            secContext->ul_count.seq_num, secContext->ul_count.overflow);
        }

        nasLog("dl_count.seq_num : %2X | dl_count.overflow : %2X",
            secContext->dl_count.seq_num, secContext->dl_count.overflow);
        nasLog("ul_count.seq_num : %2X | ul_count.overflow : %2X",
            secContext->ul_count.seq_num, secContext->ul_count.overflow);

        /*  call the encryption function
            key inputs :
                1. secuHeader
                2. plainMsgEncodedLen
                3. direction_encode
            write into the buffer at offset of sizeof(SecurityHeader)
        */
        bool nullciphering = (secContext->secAlgo.nasEncAlgo ==
                                                      NAS_ENCRYPTION_ALG_NEA0);
        if( (!nullciphering) &&
          (header->secuHeader == SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED ||
           header->secuHeader == SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED_NEW ))
        {
            rc = nasMessageEncrypt(plainMsg, plainMsgEncodedLen,
                                   secContext->direction_encode,secContext,
                                   buffer + SIZE_MM_SECU_HEADER);
            if(rc == FAILURE) {
                higLog("%s"," nasMessageEncrypt() failed");
                nasLogEXIT;
                return FAILURE;
            }
            nasLog("Encrypted msg encoded | sizeof : %d", plainMsgEncodedLen);
        } else{
            /* integrity protected but not encrypted. */
            if(nullciphering)
                nasLog("%s","Null Ciphered Message. Encryption not needed");

            memcpy(buffer + SIZE_MM_SECU_HEADER, plainMsg, plainMsgEncodedLen);
        }

        memcpy(buffer + SIZE_MM_SECU_HEADER - sizeof(header->sqnNum),
               &(header->sqnNum), sizeof(header->sqnNum));

        /* wonderful, now get nasMac */
        /* seqNumber + encrypted msg is integrity protected */
        uint32_t bytesToIntegrityProtect = sizeof(uint8_t) + plainMsgEncodedLen;
        uint32_t nasMac = 0;
          //Null Integrity supported for emergency/test mode (Ref.33.501 D.1)
        if(secContext->secAlgo.nasIntAlgo == NAS_INTEGRITY_ALG_NIA0)
        {
            nasLog("Null Integrity Protected Message. MAC is empty");
            nasLog("WARNING: There is no replay protection with NIA0");
        }
        else{
            rc = nasMessageGetMac(buffer + SIZE_MM_SECU_HEADER
                                         - sizeof(header->sqnNum),
                                  bytesToIntegrityProtect,
                                  IS_ENCODING,
                                  secContext,(uint8_t*) &nasMac);
            if(rc == FAILURE) {
                higLog("%s"," nasMessageGetMac() failed");
                if(SECU_DIRECTION_DOWNLINK == secContext->direction_encode){ /*AMF*/
                    *((uint32_t*) &secContext->dl_count) = backupCount; /*revert */
                }else { /*UE*/
                    *((uint32_t*) &secContext->ul_count) = backupCount; /*revert */
                }
                nasLogEXIT;
                return FAILURE;
            }
        }

        /* we now have all necessary pieces, encode them */
        /* 1. Encode the epd value */
        memcpy(buffer + len,&(header->epd),sizeof(header->epd));
        len += sizeof(header->epd);
        nasLog("epd value : 0x%x | currentLen = %d | sizeof : %d", header->epd,
                len, sizeof(header->epd));

        /* 2. Encode the SecurityHeaderType */
        memcpy(buffer + len, &(header->secuHeader),sizeof(header->secuHeader));
        len += sizeof(header->secuHeader);
        nasLog("secuHeader value : %d | currentLen = %d | sizeof : %d",
                header->secuHeader,len,sizeof(header->secuHeader));

        /*3. encode nasMac
         *   epd + secuHeader (2 octets) == 2*sizeof (uint8_t) */
        *(uint32_t *) (buffer + 2 * sizeof (uint8_t)) = nasMac;
        len += sizeof(nasMac);
        nasLog("nasMac value : %2X | currentLen = %d | sizeof : %d",
                htonl(nasMac), len, sizeof(nasMac));

        /*4. sqnNum was encoded, just increment len */
        len += sizeof(header->sqnNum);
        nasLog("sqnNum value : %d | currentLen = %d | sizeof : %d",
                header->sqnNum, len, sizeof(header->sqnNum));

        /*dont forget to add the encrypted message len*/
        len += plainMsgEncodedLen;

        /*  update the secContext variables
            24.501 + 33.501
        */


    }   /* end of non-plain nas msg encoding */

    /* it is a plain nas message */
    else {
        /*  len = 0 , we have just started */
        int rc;
        uint32_t plainMsgEncodedLen = 0;

        /* just plain encode directly into buff */
        rc = nasMessagePlainEncode(buffer,bufLen,&(msgStruct->plain),
                &plainMsgEncodedLen); /*    msgStruct is an union
                                            it has plain nas message also
                                        */
        if(rc == FAILURE) {
            higLog("%s","nasMessagePlainEncode  failed");
            nasLogEXIT;
            return FAILURE;
        }
        len = plainMsgEncodedLen;
        // update the len : do not add to the previous len value
    }

    *encodedLen = len;         /* update the encodedLen pointer data */
    nasLog("nas encode successful, encoded len %d", len);
    nasLogEXIT;
    return SUCCESS;
}

/*  nasMessageDecode(nasBuf) -> nasMessage_t
 *  L__ read securityHeaderType
 *  |
 *  L__ if msg is security protected
 *  |   L__ update ul and dl count in security Context
 *  |   L__ compute nasMac and compare with header.
 *  |   L__ decrypt message into a buf
 *  |       nasMessageDecrypt(nasBuf) -> plain
 *  |   L__ decode as a plain message
 *  |       nasMessagePlainDecode(plain) -> nasMessage_t
 *  |
 *  L__ if msg is a plain msg, decode it
 *      nasMessagePlainDecode(nasBuf) -> nasMessage_t
 *
 */
int nasMessageDecode(uint8_t *buffer,nasMessage_t *msgStruct,
    uint32_t decodedLen, void *security)
{
    nasLogENTRY;
    errno = E_NAS_ERROR_UNKNOWN; /* entry into the library */

    int rc = 0;
    if (buffer == NULL || msgStruct == NULL ||
            decodedLen < SIZE_EPD_AND_SECU_HEADER) {
        higLog("%s","Invalid Input, Failure encoding");
        errno = E_NAS_ERROR_INVALID_INPUT;
        nasLogEXIT;
        return FAILURE;
    }

    //TODO:Teja to clean this dummy mess for Secuheader
    nasMessageSecurityHeader_t *header = &msgStruct->header;
    uint32_t len = 0;
    uint8_t secuHeader = buffer[1];
    uint8_t header_epd = buffer[0];

    if(header_epd == _5GS_MOBILITY_MANAGEMENT_MESSAGE) {
        /* If nas message is plain then security context will be null.
         * Directly decode and return. */
        if(secuHeader == SECURITY_HEADER_TYPE_NOT_PROTECTED) {

            if (nasMessagePlainDecode(buffer, &(msgStruct->plain), decodedLen)
            == FAILURE) {
                higLog("%s"," nasMessagePlainDecode failed");
                nasLogEXIT;
                return FAILURE;
            }
            nasLogEXIT;
            return SUCCESS;
        }

        /* If code has reached this point then nas message is either ciphered 
         * or integrity protected.

         * For security protected messages decodeLen should be able to contain:
         * epd(1) + secHeader(1) + macCode(4) + sqnNum(1) = 7
         */

        if (decodedLen < SIZE_MM_SECU_HEADER) {
            higLog("%s", "Incomplete input buffer ");
            errno = E_NAS_ERROR_BUFF_INCOMPLETE;
            nasLogEXIT;
            return FAILURE;
        }

        /* retrieve security context */
        if (security == NULL) {
            higLog("%s","ERROR: security context NULL");
            errno = E_NAS_ERROR_SECCTX_NULL;
            nasLogEXIT;
            return FAILURE;
        }
        secContext_t *secContext = static_cast<secContext_t *>(security);

        /*  get the offset of the SQN num   */
        int seqNumOffset = SIZE_MM_SECU_HEADER - sizeof(uint8_t);

        /* decode sqn num. consists of 8 bits : 1 byte */
        header->sqnNum = *(buffer + seqNumOffset);
        len += sizeof(header->sqnNum);
        nasLog("sqnNum value : %d | currentLen = %d | sizeof : %d",
                header->sqnNum, len, sizeof(header->sqnNum));

        /* the incoming sqnNumber should be:
         *      either more than the current sqn Number
         *          i.e. ideal case new msg
         *      or atleast 127 (max uint8 /2) less than the current sqn Number
         *          i.e. sqn number has overflowed on the other end
         *          TODO: this seems very liberal.
         */

        uint tempSeqN = ((SECU_DIRECTION_UPLINK == secContext->direction_decode)
            ? secContext->ul_count.seq_num : secContext->dl_count.seq_num);
        uint backupCount = *(uint32_t*)((SECU_DIRECTION_UPLINK ==
            secContext->direction_decode) ? &secContext->ul_count
                                          : &secContext->dl_count);
        if(header->sqnNum < tempSeqN &&
           (tempSeqN - header->sqnNum < SCHAR_MAX)){
            higLog("%s","Incorrect Nas Seq Number, received a very old msg");
            errno = E_NAS_ERROR_OLD_SEQ_NUM;
            nasLogEXIT;
            return FAILURE;
        }
        if(SECU_DIRECTION_UPLINK == secContext->direction_decode) { /*AMF*/
            if (secContext->ul_count.seq_num > header->sqnNum) {
                secContext->ul_count.overflow += 1;
            }
            secContext->ul_count.seq_num = header->sqnNum;
        }else {                                                     /*UE*/
            if (secContext->dl_count.seq_num > header->sqnNum) {
                secContext->dl_count.overflow += 1;
            }
            secContext->dl_count.seq_num = header->sqnNum;
        }

        nasLog("dl_count.seq_num value : %2X | dl_count.overflow value : %2X",
            secContext->dl_count.seq_num, secContext->dl_count.overflow);
        nasLog("ul_count.seq_num : %2X | ul_count.overflow : %2X",
            secContext->ul_count.seq_num, secContext->ul_count.overflow);

        /* SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_NEW is ONLY for
         * Security mode command but Security mode command can be IP as well.
         * SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED can be for other NAS msgs
         * like Service Request. Accordingly checkForSecCapMismatch API will
         * change. checkForSecCapMismatch should take in two SecCapabilities
         * and compare 
         */
        if(secuHeader == SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_NEW) {

            uint8_t plainMsg[MAX_MSG_LEN] = {};
            memcpy(plainMsg , buffer + SIZE_MM_SECU_HEADER,
                    decodedLen - SIZE_MM_SECU_HEADER);
            rc = nasMessagePlainDecode(plainMsg, &(msgStruct->plain),
                                       decodedLen - SIZE_MM_SECU_HEADER);
            if(rc == FAILURE) {
                higLog("%s"," nasMessagePlainDecode failed");
                nasLogEXIT;
                return FAILURE;
            } else if ( msgStruct->plain._5gmmMsg.mmheader.msgType != SECURITY_MODE_COMMAND) {
                errno = E_NAS_ERROR_NOT_SEC_MODE_CMD;
                higLog("Incorrect NAS Message type ");
                nasLogEXIT;
                return FAILURE; //To send Sec mode Reject in this case
            }

            SecurityModeCommand_t *secModeCmdMsg =
                                &(msgStruct->plain._5gmmMsg.secModeCmdMsg);
            /* Compare Replayed UE security capabilities (sent by AMF) in
             * security mode command msg with the one which UE has in
             * secContext
             */
            rc = checkForSecCapMismatch(secContext, secModeCmdMsg);
            if(rc == FAILURE)
            {
                errno = E_NAS_ERROR_SEC_CAP_MISMATCH;
                higLog("%s","Security capabilities mismatch between UE and AMF!!");
                nasLogEXIT;
                return FAILURE;
            }
            /* Read security algorithms chosen by AMF from security mode
             * command into a temporary struct. Use it to compute NASint and
             * NASenc, and then check integrity of the recvd message. Add
             * these changes back into secCtxt only if integrity is verified.
             */
            secContext_t tempSecCtxt = *secContext;
            tempSecCtxt.secAlgo.nasEncAlgo =
                        secModeCmdMsg->nasSecurityAlgorithm.encryptionAlgo;
            tempSecCtxt.secAlgo.nasIntAlgo =
                        secModeCmdMsg->nasSecurityAlgorithm.integrityAlgo;

            /* algo type distinguisher for N-NAS-int-algo */
            uint8_t algo_distinguisher = N_NAS_INTEGRITY_ALG;

            /* Calculate KNASint */
            rc = computeNasSecurityKey(tempSecCtxt.Kamf, algo_distinguisher,
                    tempSecCtxt.secAlgo.nasIntAlgo, tempSecCtxt.K_NASint);
            if(rc == FAILURE)
            {
                higLog("%s","computation of NAS integrity key failed");
                nasLogEXIT;
                return rc;
            }
            printArray("k_NASint ", tempSecCtxt.K_NASint, NAS_KEY_SIZE);

            /* algo type distinguisher for N-NAS-enc-algo */
            algo_distinguisher = N_NAS_ENCRYPTION_ALG;

            /* Calculate KNASenc */
            rc = computeNasSecurityKey(tempSecCtxt.Kamf, algo_distinguisher,
                    tempSecCtxt.secAlgo.nasEncAlgo, tempSecCtxt.K_NASenc);
            if(rc == FAILURE)
            {
                higLog("Generation of K_NASint key failed!");
                nasLogEXIT;
                return rc;
            }
            printArray("k_NASenc ", tempSecCtxt.K_NASenc, NAS_KEY_SIZE);

            if(matchNasMac(buffer, decodedLen, backupCount, secContext,
            &tempSecCtxt) == FAILURE) {
                higLog("matchNasMac failed");
                nasLogEXIT;
                return FAILURE;
            } 
            *secContext = tempSecCtxt;
            nasLog("%s","Message integrity verified. nasMAC matched");

        }

        /* Note: Even if msg is just Integrity Protected and not ciphered.
         *       it comes under the category of Security Protected.
         *       Accordingly SeqNum and Sec Context are needed for successful
         *       decoding of Integrity Protected NAS msgs.
         *       Refer 4.4.3.3 Integrity protection and verification for ref.
         */
        else if (secuHeader == SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED) {

            uint8_t plainMsg[MAX_MSG_LEN] = {};
            memcpy(plainMsg , buffer + SIZE_MM_SECU_HEADER,
                    decodedLen - SIZE_MM_SECU_HEADER);
            rc = nasMessagePlainDecode(plainMsg, &(msgStruct->plain),
                                       decodedLen - SIZE_MM_SECU_HEADER);
            if(rc == FAILURE) {
                higLog("%s"," nasMessagePlainDecode failed");
                nasLogEXIT;
                return FAILURE;
            }

            secContext_t tempSecCtxt = *secContext;

            if(matchNasMac(buffer, decodedLen, backupCount, secContext,
            &tempSecCtxt) == FAILURE) {
                higLog("matchNasMac failed");
                nasLogEXIT;
                return FAILURE;
            } 
            *secContext = tempSecCtxt;
            nasLog("%s","Message integrity verified. nasMAC matched");

        }

        /* TODO: SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED_NEW is ONLY
         * for SECURITY MODE COMPLETE. Thus need to add a check for same.
         * No restriciton for SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED 
         */
        else if(secuHeader == SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED
        ||secuHeader == SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED_NEW) 
        {
            secContext_t tempSecCtxt = *secContext;

            if(matchNasMac(buffer, decodedLen, backupCount, secContext,
            &tempSecCtxt) == FAILURE) {
                higLog("matchNasMac failed");
                nasLogEXIT;
                return FAILURE;
            } 
            *secContext = tempSecCtxt;
            nasLog("%s","Message integrity verified. nasMAC matched");

            /* 33.501 */
            /* ul/dl count will be incremented irrespective of success or 
             * failure while further decoding the message. This is because the
             * primary purpose is to estimate the other end's encoding seq Num.
             * The NAS message contents might be wrong, but seqNum's integrity 
             * and validity have been verified.
             */

            /*
                We need to decode the protected mesage now
                The remaing part of the buffer is in encrypted form
                1. decrypt it first (we are using security context,
                therefore this step will be bypassed anyway for now)
                2. then use plain decoder
            */

            uint8_t plainMsg[MAX_MSG_LEN] = {};
            bool nullciphering = (secContext->secAlgo.nasEncAlgo ==
                                                    NAS_ENCRYPTION_ALG_NEA0);

            if (!nullciphering)
            {
                int rc = nasMessageDecrypt(buffer + SIZE_MM_SECU_HEADER,
                                           decodedLen - SIZE_MM_SECU_HEADER,
                                           secContext->direction_decode,
                                           secContext, plainMsg);
                if(rc == FAILURE) {
                    higLog("%s"," nasMessageDecrypt() failed");
                    nasLogEXIT;
                    return FAILURE;
                }

            } else {
                nasLog("%s","Null Ciphered Message, Decryption is not needed");
                memcpy(plainMsg , buffer + SIZE_MM_SECU_HEADER,
                                            decodedLen - SIZE_MM_SECU_HEADER);
            }
            /* In the current function only nasMac and SeqNum are decoded.
             * all other elements will be decoded in plain decode. */
            rc = nasMessagePlainDecode(plainMsg, &(msgStruct->plain),
                                       decodedLen - SIZE_MM_SECU_HEADER);
            if(rc == FAILURE) {
                higLog("%s"," nasMessagePlainDecode failed");
                nasLogEXIT;
                return FAILURE;
            }
        }
    }
    else if (header_epd == _5GS_SESSION_MANAGEMENT_MESSAGE) {

        if (nasMessagePlainDecode(buffer, &(msgStruct->plain), decodedLen)
        == FAILURE) {
            higLog("%s"," nasMessagePlainDecode failed");
            nasLogEXIT;
            return FAILURE;
        }
    }
    nasLogEXIT;
    return SUCCESS;
}

/* Function: fillNasInputStream
    Input :
        1. NAS Key (integrity or ciphering)
        2. NAS Count
        3. direction
        4. bearer
        5. input NAS msg
        6. msg length
    Output
        1.Input parameters put into structure nasInputStream
*/
void fillNasInputStream(nasInputStream_t *nasStream,uint8_t NAS_Key[NAS_KEY_SIZE],
                        uint8_t countArray[SIZE_NASCOUNT], uint8_t direction,
                        uint8_t bearer, const uint8_t *nasMessage,  size_t msgLength)
{
    nasStream->key = NAS_Key;
    nasStream->key_length = NAS_KEY_SIZE;
    nasStream->count= countArray;
    nasStream->bearer = bearer;      /* 33.401-8.1.1, 33.501-6.4.3.1 &  6.4.2.2*/
    nasStream->direction = direction;
    nasStream->message = nasMessage;
    nasStream->msglength = msgLength;
}

/* Function: nasMessageEncrypt
    Input :
        1. Plain NAS msg
        2. msg length
        3. direction
        4. security context
    Output
        1.ciphered NAS msg
*/
int nasMessageEncrypt(const uint8_t *plainMsg, int msglength,
     uint8_t direction, void *security, uint8_t * cipherMsg)
{
    nasLogENTRY;
    int rc = FAILURE;
    uint8_t countArray[SIZE_NASCOUNT]={};

    /* check the input pointers validity */
    if( plainMsg == NULL or security == NULL or cipherMsg == NULL) {
        errno = E_NAS_ERROR_INVALID_INPUT;
        higLog("%s"," Invalid input");
        return FAILURE;
    }
    /* convert the security pointer to secContext_t */
    secContext_t *secContext = static_cast<secContext_t *>(security);
    /* count parameter for the encryption algorithm */
    uint32_t    count = 0;

    if (direction == SECU_DIRECTION_UPLINK) {
        count = 0x00000000 | ((secContext->ul_count.overflow && 0x0000FFFF)
                << 8) | (secContext->ul_count.seq_num & 0x000000FF);
    } else {
        count = 0x00000000 | ((secContext->dl_count.overflow && 0x0000FFFF)
                << 8) | (secContext->dl_count.seq_num & 0x000000FF);
    }

    memcpy(countArray, (uint8_t*)&count, SIZE_NASCOUNT);

    nasLog("NAS COUNT value::%2X",count);
    printArray("NAS COUNT Array", countArray, SIZE_NASCOUNT);
    nasLog("Direction:%2X",direction);

    nasInputStream_t nasInput={};
    fillNasInputStream(&nasInput,secContext->K_NASenc, countArray,
                    direction, secContext->bearer, plainMsg, msglength);

    rc = NASCipheringAlgo(&nasInput, secContext->secAlgo.nasEncAlgo, AES_ENCRYPT, cipherMsg);

    if(rc == FAILURE){
        errno = E_NAS_ERROR_ENCRYPT_FAIL;
        higLog("%s"," encryption failed");
        nasLogEXIT;
        return rc;
    }

    nasLogEXIT;
    return SUCCESS;
}

/* Function: nasMessageDecrypt
    Input :
        1. Ciphered NAS msg
        2. msg length
        3. direction
        4. security context
    Output
        1.De-ciphered original NAS msg
*/

int nasMessageDecrypt(const uint8_t *cipherMsg, int length,
    uint8_t direction,void *security, uint8_t* origMsg)
{

    nasLogENTRY;
    int rc = FAILURE;
    uint8_t countArray[SIZE_NASCOUNT]={};
    uint32_t count = 0;
    /* First check for the invalid input */
    if(cipherMsg == NULL or security == NULL) {
        errno = E_NAS_ERROR_INVALID_INPUT;
        higLog("%s"," Invalid input");
        return FAILURE;
    }

    /*  count parameter for the decryption algorithm
        8.1.1 NAS input parameters and mechanism : 33.401
    */
    secContext_t *secContext = static_cast<secContext_t *>(security);

    if (direction == SECU_DIRECTION_UPLINK) {
        count = 0x00000000 | ((secContext->ul_count.overflow && 0x0000FFFF)
         << 8) | (secContext->ul_count.seq_num & 0x000000FF);
    } else {
        count = 0x00000000 | ((secContext->dl_count.overflow && 0x0000FFFF)
         << 8) | (secContext->dl_count.seq_num & 0x000000FF);
    }

    memcpy(countArray, (uint8_t*)&count, SIZE_NASCOUNT);

    nasLog("NAS COUNT value::%2X",count);
    printArray("NAS COUNT Array", countArray, SIZE_NASCOUNT);
    nasLog("Direction:%2X",direction);

    nasInputStream_t nasInput={};
    fillNasInputStream(&nasInput,secContext->K_NASenc, countArray,
                    direction, secContext->bearer, cipherMsg, length);

    rc = NASCipheringAlgo(&nasInput, secContext->secAlgo.nasEncAlgo, AES_DECRYPT, origMsg);
    if(rc == FAILURE){
        errno = E_NAS_ERROR_DECRYPT_FAIL;
        higLog("%s"," decryption routine failed");
        nasLogEXIT;
        return rc;
    }

    nasLogEXIT;
    return SUCCESS;
}

/* Note: This function is used in nasMessageDecode to verify nas message 
 *       integrity by calculating nasMac and matching the received nasMac
 *       with the expected nasMac. So direction required for nasMessageGetMac
 *       function invoked from this function is hardcoded to IS_DECODING. 
 *       This function should never be used in nasMessageEncode. If at all
 *       this function is to be used in nasMessageEncode then direction
 *       should be IS_ENCODING.
 */
int matchNasMac(uint8_t *buffer, uint32_t decodedLen, uint backupCount,
    secContext_t *secContext, secContext_t *tempSecCtxt) {

    nasLogENTRY;
    int rc = FAILURE;
    /*  get the offset of the SQN num   */
    int seqNumOffset = SIZE_MM_SECU_HEADER - sizeof(uint8_t);
    uint32_t nasMac = *(uint32_t *) (buffer + 2 * sizeof (uint8_t));
    uint32_t xNasMac = 0;
    if(secContext->secAlgo.nasIntAlgo != NAS_INTEGRITY_ALG_NIA0)
    {
        rc = nasMessageGetMac(buffer + seqNumOffset, decodedLen - seqNumOffset,
                              IS_DECODING, tempSecCtxt, (uint8_t *)&xNasMac);
        if(rc == FAILURE) {
            higLog("%s"," nasMessageGetMac() failed");
            nasLogEXIT;
            return FAILURE;
        }
        if(nasMac != xNasMac) {
            errno = E_NAS_ERROR_MAC_MISMATCH;
            higLog("%s","Message Integrity check failed, mismatch nasMac");
            /* dl and ul count values are unverified. Revert them */
            if(SECU_DIRECTION_UPLINK == secContext->direction_decode){ /*AMF*/
                *((uint32_t*) &secContext->ul_count) = backupCount; /*revert */
            }else { /*UE*/
                *((uint32_t*) &secContext->dl_count) = backupCount; /*revert */
            }
            /* return choice FIXME */
            nasLogEXIT;
            return FAILURE;
        }

        /* decode macCode */
        //header->macCode = nasMac;   //FIXME
        nasLog("nas mac value : %2X ",  nasMac);
    }
    else {       
        nasLog("NAS Mac matching skipped because of Null Integrity");
    }

    nasLogEXIT;
    return SUCCESS;
}

/* Function:nasMessageGetMac
    Input :
        1. Plain NAS msg
        2. msg length
        3. direction
        4. security context
    Output
        1.NAS MAC (32-bits)
*/

int nasMessageGetMac(const uint8_t *nasMsg,size_t msgLen,int operation,
    void *security,uint8_t *nasMac)
{
    nasLogENTRY;
    uint8_t countArray[SIZE_NASCOUNT]={};
    uint32_t count=0;
    int direction=-1;

    if(security == NULL or nasMac == NULL) {
       errno = E_NAS_ERROR_INVALID_INPUT;
       higLog("%s"," invalid input");
       return FAILURE;
    }
    secContext_t *secContext = static_cast<secContext_t *>(security);

    memset(nasMac, 0, SIZE_NASMAC);

    /* calculate the count variable for the nasEncryptNIA1() */

    if(SECU_DIRECTION_UPLINK == secContext->direction_decode) {  /*AMF*/
        if (operation == IS_ENCODING) {
            count = 0x00000000 |
                    ((secContext->dl_count.overflow & 0x0000FFFF) << 8) |
                    (secContext->dl_count.seq_num & 0x000000FF);
            direction = SECU_DIRECTION_DOWNLINK;
            nasLog("%s","AMF encoding before sending DOWNLINK message");

        } else {
            count = 0x00000000 |
                    ((secContext->ul_count.overflow & 0x0000FFFF) << 8) |
                    (secContext->ul_count.seq_num & 0x000000FF);
            direction = SECU_DIRECTION_UPLINK;
            nasLog("%s","AMF decoding received UPLINK message");
        }

    }else {                                                      /*UE*/
        if (operation == IS_ENCODING) {
            count = 0x00000000 |
                    ((secContext->ul_count.overflow & 0x0000FFFF) << 8) |
                    (secContext->ul_count.seq_num & 0x000000FF);
            direction = SECU_DIRECTION_UPLINK;
            nasLog("%s","UE encoding before sending UPLINK message");
        } else {
            count = 0x00000000 |
                    ((secContext->dl_count.overflow & 0x0000FFFF) << 8) |
                    (secContext->dl_count.seq_num & 0x000000FF);
            direction = SECU_DIRECTION_DOWNLINK;
            nasLog("%s","UE decoding received DOWNLINK message");
        }
    }

    memcpy(countArray, (uint8_t*)&count, SIZE_NASCOUNT);
    nasLog("NAS COUNT value::%2X",count);
    //printArray("NAS COUNT Array", countArray, SIZE_NASCOUNT);
    /*  Integrity Key K_NASint must exist before this step in security context*/

    nasInputStream_t nasInput={};
    fillNasInputStream(&nasInput,secContext->K_NASint, countArray,
                    direction, secContext->bearer, nasMsg, msgLen);

    nasLog("secContext->secAlgo.nasIntAlgo:0x%2X",
       secContext->secAlgo.nasIntAlgo);
    if(NASIntegrityAlgo(&nasInput, secContext->secAlgo.nasIntAlgo, nasMac) == FAILURE) {
        errno = E_NAS_ERROR_INTEG_ALGO_FAIL;
        higLog("%s", "NASIntegrityAlgo failed");
        nasLogEXIT;
        return FAILURE;
    }

    nasLogEXIT;
    return SUCCESS;
}

int checkForSecCapMismatch(secContext_t *secContext ,
    SecurityModeCommand_t * secModeCmdMsg)
{
    nasLogENTRY;
    int rc = FAILURE;
    UeSecurityCapability_t *ueSecCapAMF = &(secModeCmdMsg->ueSecuCapability);
    UeSecurityCapability_t *ueSecCapUE = &(secContext->ueSecurityCapability);
    uint8_t amfSelectedEncAlgo = secModeCmdMsg->nasSecurityAlgorithm.encryptionAlgo;
    uint8_t amfSelectedIntAlgo = secModeCmdMsg->nasSecurityAlgorithm.integrityAlgo;
    bool bEncAlgo = false;
    bool bIntAlgo = false;
    /*1.Checking if security capabilities sent by UE are same when AMF received
    them i.e. not modified by attacker. Note that this is being checked
    by MAC matching in next steps. But this step as done per 33.501 6.7.2 2a,
    will avoid unnecessary MAC calculation if sec capabilities are different */

    //1.matching ciphering capabilities between UE and AMF
    if((ueSecCapAMF->_5gNASEncAlgo) == (ueSecCapUE->_5gNASEncAlgo))
    {
        //2 Check:AMF choose Algo that is supported by UE sec capabilities or not?
        if(amfSelectedEncAlgo == NAS_ENCRYPTION_ALG_NEA0){
            if(ueSecCapAMF->_5gNASEncAlgo & NAS_SEC_ALGO_NEA0)
                bEncAlgo = true;
        }
        else if(amfSelectedEncAlgo == NAS_ENCRYPTION_ALG_NEA1){
            if(ueSecCapAMF->_5gNASEncAlgo & NAS_SEC_ALGO_NEA1)
                bEncAlgo = true;
        }
        else if(amfSelectedEncAlgo == NAS_ENCRYPTION_ALG_NEA2){
            if(ueSecCapAMF->_5gNASEncAlgo & NAS_SEC_ALGO_NEA2)
                bEncAlgo = true;
        }
        else if(amfSelectedEncAlgo == NAS_ENCRYPTION_ALG_NEA3){
            if(ueSecCapAMF->_5gNASEncAlgo & NAS_SEC_ALGO_NEA3)
                bEncAlgo = true;
        }
        else if(amfSelectedEncAlgo == NAS_ENCRYPTION_ALG_NEA4){
            if(ueSecCapAMF->_5gNASEncAlgo & NAS_SEC_ALGO_NEA4)
                bEncAlgo = true;
        }
        else if(amfSelectedEncAlgo == NAS_ENCRYPTION_ALG_NEA5){
            if(ueSecCapAMF->_5gNASEncAlgo & NAS_SEC_ALGO_NEA5)
                bEncAlgo = true;
        }
        else if(amfSelectedEncAlgo == NAS_ENCRYPTION_ALG_NEA6){
            if(ueSecCapAMF->_5gNASEncAlgo & NAS_SEC_ALGO_NEA6)
                bEncAlgo = true;
        }
        else if(amfSelectedEncAlgo == NAS_ENCRYPTION_ALG_NEA7){
            if(ueSecCapAMF->_5gNASEncAlgo & NAS_SEC_ALGO_NEA7)
                bEncAlgo = true;
        }
    }
    if( !bEncAlgo) {
        higLog("ERROR! AMF choosen ciphering Algo not supported by UE!!");
        nasLogEXIT;
        return rc;
    }

    if((ueSecCapAMF->_5gNASIntAlgo) == (ueSecCapUE->_5gNASIntAlgo))
    {
        //2 Check:AMF choose Algo that is supported by UE sec capabilities or not?
        if(amfSelectedIntAlgo == NAS_INTEGRITY_ALG_NIA0){
            if(ueSecCapAMF->_5gNASIntAlgo & NAS_SEC_ALGO_NIA0)
                bIntAlgo = true;
        }
        else if(amfSelectedIntAlgo == NAS_INTEGRITY_ALG_NIA1){
            if(ueSecCapAMF->_5gNASIntAlgo & NAS_SEC_ALGO_NIA1)
                bIntAlgo = true;
        }
        else if(amfSelectedIntAlgo == NAS_INTEGRITY_ALG_NIA2){
            if(ueSecCapAMF->_5gNASIntAlgo & NAS_SEC_ALGO_NIA2)
                bIntAlgo = true;
        }
        else if(amfSelectedIntAlgo == NAS_INTEGRITY_ALG_NIA3){
            if(ueSecCapAMF->_5gNASIntAlgo & NAS_SEC_ALGO_NIA3)
                bIntAlgo = true;
        }
        else if(amfSelectedIntAlgo == NAS_INTEGRITY_ALG_NIA4){
            if(ueSecCapAMF->_5gNASIntAlgo & NAS_SEC_ALGO_NIA4)
                bIntAlgo = true;
        }
        else if(amfSelectedIntAlgo == NAS_INTEGRITY_ALG_NIA5){
            if(ueSecCapAMF->_5gNASIntAlgo & NAS_SEC_ALGO_NIA5)
                bIntAlgo = true;
        }
        else if(amfSelectedIntAlgo == NAS_INTEGRITY_ALG_NIA6){
            if(ueSecCapAMF->_5gNASIntAlgo & NAS_SEC_ALGO_NIA6)
                bIntAlgo = true;
        }
        else if(amfSelectedIntAlgo == NAS_INTEGRITY_ALG_NIA7){
            if(ueSecCapAMF->_5gNASIntAlgo & NAS_SEC_ALGO_NIA7)
                bIntAlgo = true;
        }
    }

    if( !bIntAlgo){
        higLog("ERROR! AMF choosen Integrity Algo not supported by UE!!");
        nasLogEXIT;
        return FAILURE;
    }

    nasLogEXIT;
    return SUCCESS;
}
