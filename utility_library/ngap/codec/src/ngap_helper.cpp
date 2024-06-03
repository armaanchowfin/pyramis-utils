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

#include "ngap/codec/include/ngap_helper.h"
#include "common/include/datatypes.h"

/* There is no order constraint. SEQUENCE items can be in any possible order.
 * These functions search and provide the pointer to the same.
 *
 * Note: A comment was written here previously, which stated the opposite.
 *       It has been removed.
 *         -TT
 */

#define MAX_MESSAGE_SIZE 3000

#define FETCH_IE_PTR(ieType, ieVar, var)                                        \
    void *temp;                                                             \
    /* got the seqHead pointer. find RAN_UE_NGAP_ID */                      \
    for(seqItr=0; seqItr<seqCount; seqItr++) {                           \
        if(ASN_SEQUENCE_GET(seqHead, seqItr, &temp) < 0) {                 \
            higLog("ASN_SEQUENCE_GET failed itr %d, count %d",              \
                        seqItr, seqCount);                                 \
            return NULL;                                                    \
        }                                                                   \
        if(((ieType##_t*)temp)->value.present !=                            \
            ieType##__value_PR_##ieVar) {                                   \
            continue;                                                       \
        }                                                                   \
        var = &(((ieType##_t*)temp)->value.choice.ieVar);                   \
        break;                                                              \
    }                                                                       \
    if(seqItr == seqCount) {                                               \
        higLog("%s",#ieVar " Not Found");                                    \
        return NULL;                                                        \
    }

/* Will allocate and decode the contents into the struct.
 * RETURN: SUCCESS/FAILURE.
 * on success the extracted struct MUST be freed using ngapFree() NOT free()
 * on failure, the decoded struct is freed in decodeNgapPdu()
 */
int decodeNgapPdu(NGAP_PDU_t **ngapPdu_p, void * blob, int blobLen)
{
    NGAP_PDU_t  *ngapPdu = 0;
    asn_dec_rval_t decRet;
    char errBuf[MAX_MESSAGE_SIZE] = {};
    size_t errlen = MAX_MESSAGE_SIZE;

    /*decode msg into NGAP PDU*/
    decRet = aper_decode_complete(0, &asn_DEF_NGAP_PDU,
                (void**) &ngapPdu, (char*) blob, blobLen);

    if(asn_check_constraints(&asn_DEF_NGAP_PDU, ngapPdu,
                    errBuf, &errlen) < 0)
        higLog("check constraints failed: %s", errBuf);

    if(decRet.code == RC_OK) {
        lowLog("%s", "Decode succeeded");
        asnLog(&asn_DEF_NGAP_PDU, ngapPdu);
        *ngapPdu_p = ngapPdu;
        return SUCCESS;
    } else {
        higLog("Decode failed, Err %s", decRet.code == RC_FAIL ?
                        "RC_FAIL" : "RC_WMORE");
        ngapFree(ngapPdu);
        *ngapPdu_p = NULL;
        return FAILURE;
    }
}

#if (LOGGING_BITMASK & 0b001)
void __asnLog(const asn_TYPE_descriptor_t *td, const void *struct_ptr,
            const char* file, int line, const char* func) {

    char *printBuf = NULL;
    size_t ptr = 0;
    FILE *filep = open_memstream(&printBuf, &ptr);
    if(filep==NULL) {
        printf("ERROR:open_memstream failed");
    }
    asn_fprint(filep, td, struct_ptr);

    fclose(filep);
    platformLog(0xF1, file, line, func,
                "\tASN_LOG: start:\n%sASN_LOG:end", printBuf);
    free(printBuf);
}
#endif

/* get the procedure code from NGAP_PDU*/
int ngapGetProcedureCode(NGAP_PDU_t* ngapPdu)
{
    switch (ngapPdu->present) {
    case NGAP_PDU_PR_initiatingMessage:
        return ngapPdu->choice.initiatingMessage->procedureCode;
        break;
    case NGAP_PDU_PR_successfulOutcome:
        return ngapPdu->choice.successfulOutcome->procedureCode;
        break;
    case NGAP_PDU_PR_unsuccessfulOutcome:
        return ngapPdu->choice.unsuccessfulOutcome->procedureCode;
        break;
    default:
        higLog("Invalid NGAP PDU\n");
        return FAILURE;
    }
}

void * ngapGetProtocolIeListPtr(NGAP_PDU_t *ngapPdu)
{
    switch(ngapPdu->present) {
    case NGAP_PDU_PR_initiatingMessage: /*initiatingMessage*/
        switch(ngapPdu->choice.initiatingMessage->value.present) {
        case InitiatingMessage__value_PR_InitialUEMessage:
        {   /*brackets to prevent declarations within a case from
             * being visible in the next case */
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                    ProcedureCode_id_InitialUEMessage){
                higLog("Mismatching procedureCode %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            InitialUEMessage_t *initUEmsg =
                &ngapPdu->choice.initiatingMessage->value.choice
                        .InitialUEMessage;
            return (void*)(&initUEmsg->protocolIEs.list);
        }

        case InitiatingMessage__value_PR_Paging:
        {
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                    ProcedureCode_id_Paging){
                higLog("Mismatching procedureCode %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            Paging_t *paging =
                &ngapPdu->choice.initiatingMessage->value.choice.Paging;
            return (void*)(&paging->protocolIEs.list);

        }

        case InitiatingMessage__value_PR_PDUSessionResourceSetupRequest:
        {
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                        ProcedureCode_id_PDUSessionResourceSetup){
                higLog("Mismatching procedureCode %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            PDUSessionResourceSetupRequest_t *PDUSessionResSetupReq =
                &ngapPdu->choice.initiatingMessage->value.choice
                                .PDUSessionResourceSetupRequest;
            return (void*) (&PDUSessionResSetupReq->protocolIEs.list);
        }
        case InitiatingMessage__value_PR_UplinkNASTransport:
        {
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                    ProcedureCode_id_UplinkNASTransport){
                /* asn_VAL_44_id_UplinkNASTransport */
                higLog("Mismatching procedureCode %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            UplinkNASTransport_t *uplinkNasTransport =
                &ngapPdu->choice.initiatingMessage->value.choice
                        .UplinkNASTransport;
            return (void*)(&uplinkNasTransport->protocolIEs.list);
        }

        case InitiatingMessage__value_PR_DownlinkNASTransport:
        {
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                    ProcedureCode_id_DownlinkNASTransport){
                /* asn_VAL_22_id_DownlinkNASTransport */
                higLog("Mismatching procedureCode %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            DownlinkNASTransport_t *downlinkNasTransport =
                &ngapPdu->choice.initiatingMessage->value.choice
                        .DownlinkNASTransport;
            return (void*)(&downlinkNasTransport->protocolIEs.list);
        }


        case InitiatingMessage__value_PR_NGSetupRequest:
        {
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                    ProcedureCode_id_NGSetup){
                /* asn_VAL_7_id_NGSetup */
                higLog("Mismatching procedureCode %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            NGSetupRequest_t *ngSetupReq =
                &ngapPdu->choice.initiatingMessage->value.choice.NGSetupRequest;
            return (void*)(&ngSetupReq->protocolIEs.list);
        }

        case InitiatingMessage__value_PR_InitialContextSetupRequest: {
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                    ProcedureCode_id_InitialContextSetup){
                /*asn_VAL_5_id_InitialContextSetup*/
                higLog("Mismatching Procedure Code : %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            InitialContextSetupRequest_t *initialContextSetupReq =
                &ngapPdu->choice.initiatingMessage->value.choice.
                InitialContextSetupRequest;
            return (void*)(&initialContextSetupReq->protocolIEs.list);
        }
        case InitiatingMessage__value_PR_UEContextReleaseRequest: {
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                    ProcedureCode_id_UEContextReleaseRequest){
                /*asn_VAL_16_id_UEContextRelease */
                higLog("Mismatching Procedure Code : %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            UEContextReleaseRequest_t *ueCntxtReleaseRequest =
                    &ngapPdu->choice.initiatingMessage->value.choice.UEContextReleaseRequest;
            return (void*)(&ueCntxtReleaseRequest->protocolIEs.list);
        }
        case InitiatingMessage__value_PR_UEContextReleaseCommand: {
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                    ProcedureCode_id_UEContextRelease){
                /*asn_VAL_16_id_UEContextRelease */
                higLog("Mismatching Procedure Code : %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            UEContextReleaseCommand_t *UEContextReleaseCommand =
                    &ngapPdu->choice.initiatingMessage->value.choice.
                    UEContextReleaseCommand;
            return (void*)(&UEContextReleaseCommand->protocolIEs.list);
        }
        case InitiatingMessage__value_PR_PDUSessionResourceReleaseCommand: {
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                    ProcedureCode_id_PDUSessionResourceRelease){
                /*asn_VAL_11_id_PDUSessionResourceRelease */
                higLog("Mismatching Procedure Code : %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            PDUSessionResourceReleaseCommand_t  *PDUSessResRelCmd =
                        &ngapPdu->choice.initiatingMessage->value.
                        choice.PDUSessionResourceReleaseCommand;
            return (void*)(&PDUSessResRelCmd->protocolIEs.list);
        }
        break;

        case  InitiatingMessage__value_PR_PDUSessionResourceModifyRequest: {
            if(ngapPdu->choice.initiatingMessage->procedureCode !=
                    ProcedureCode_id_PDUSessionResourceModify){
                /*asn_VAL_11_id_PDUSessionResourceRelease */
                higLog("Mismatching Procedure Code : %d",
                        ngapPdu->choice.initiatingMessage->procedureCode);
                return NULL;
            }
            PDUSessionResourceModifyRequest_t *PDUSessResModReq =
                        &ngapPdu->choice.initiatingMessage->value.
                        choice.PDUSessionResourceModifyRequest;
            return (void*)(&PDUSessResModReq->protocolIEs.list);
        }

        default:
            higLog("Invalid Initiating Message data\n");
            return NULL;
        }
    break; /* end of NGAP_PDU_PR_initiatingMessage */

    case NGAP_PDU_PR_successfulOutcome:
        switch(ngapPdu->choice.successfulOutcome->value.present) {
        case SuccessfulOutcome__value_PR_InitialContextSetupResponse: {
            if(ngapPdu->choice.successfulOutcome->procedureCode !=
                    ProcedureCode_id_InitialContextSetup){
                /*asn_VAL_5_id_InitialContextSetup*/
                higLog("Mismatching Procedure Code : %d",
                        ngapPdu->choice.successfulOutcome->procedureCode);
                return NULL;
            }
            InitialContextSetupResponse_t *initialContextSetupResponse =
                &ngapPdu->choice.successfulOutcome->value.choice.
                InitialContextSetupResponse;
            return (void*)(&initialContextSetupResponse->protocolIEs.list);
        }
        break;

        case SuccessfulOutcome__value_PR_UEContextReleaseComplete: {
            if(ngapPdu->choice.successfulOutcome->procedureCode !=
                    ProcedureCode_id_UEContextRelease){
                /*asn_VAL_16_id_UEContextRelease */
                higLog("Mismatching Procedure Code : %d",
                        ngapPdu->choice.successfulOutcome->procedureCode);
                return NULL;
            }
            UEContextReleaseComplete_t *UEContextReleaseComplete=
                    &ngapPdu->choice.successfulOutcome->value.choice.
                    UEContextReleaseComplete;
            return (void*)(&UEContextReleaseComplete->protocolIEs.list);
        }
        break;

        case SuccessfulOutcome__value_PR_PDUSessionResourceReleaseResponse: {
            if(ngapPdu->choice.successfulOutcome->procedureCode !=
                    ProcedureCode_id_PDUSessionResourceRelease){
                higLog("Mismatching Procedure Code : %d",
                        ngapPdu->choice.successfulOutcome->procedureCode);
                return NULL;
            }
            PDUSessionResourceReleaseResponse_t  *PDUSessResRelResp =
                        &ngapPdu->choice.successfulOutcome->value.
                        choice.PDUSessionResourceReleaseResponse;
            return (void*)(&PDUSessResRelResp->protocolIEs.list);
        }
        break;
        case SuccessfulOutcome__value_PR_PDUSessionResourceSetupResponse: {
            if(ngapPdu->choice.successfulOutcome->procedureCode !=
                    ProcedureCode_id_PDUSessionResourceSetup){
                higLog("Mismatching Procedure Code : %d",
                        ngapPdu->choice.successfulOutcome->procedureCode);
                return NULL;
            }
            PDUSessionResourceSetupResponse_t  *PDUSessResSetupResp =
                        &ngapPdu->choice.successfulOutcome->value.
                        choice.PDUSessionResourceSetupResponse;
            return (void*)(&PDUSessResSetupResp->protocolIEs.list);
        }
        break;
        case SuccessfulOutcome__value_PR_PDUSessionResourceModifyResponse: {
            if(ngapPdu->choice.successfulOutcome->procedureCode !=
                   ProcedureCode_id_PDUSessionResourceModify){
                higLog("Mismatching Procedure Code : %d",
                        ngapPdu->choice.successfulOutcome->procedureCode);
                return NULL;
            }
            PDUSessionResourceModifyResponse_t *PDUSessResModifyResp =
                        &ngapPdu->choice.successfulOutcome->value.
                        choice.PDUSessionResourceModifyResponse;
            return (void*)(&PDUSessResModifyResp->protocolIEs.list);
        }
        break;

        default:
            higLog("Invalid SuccessMessage Data");
            return NULL;
        }
    break; /* end of NGAP_PDU_PR_successfulOutcome */

    case NGAP_PDU_PR_unsuccessfulOutcome:
        switch(ngapPdu->choice.unsuccessfulOutcome->value.present)
        {
        case UnsuccessfulOutcome__value_PR_InitialContextSetupFailure:{
            if(ngapPdu->choice.unsuccessfulOutcome->procedureCode !=
                ProcedureCode_id_InitialContextSetup){
            /*asn_VAL_5_id_InitialContextSetup*/
            higLog("Mismatching Procedure Code : %d",
                    ngapPdu->choice.unsuccessfulOutcome->procedureCode);
            return NULL;
            }
            InitialContextSetupFailure_t *initialContextSetupFailure =
                &ngapPdu->choice.unsuccessfulOutcome->value.choice.
                InitialContextSetupFailure;
            return (void*)&initialContextSetupFailure->protocolIEs.list;
        }
        break;

        default:
            higLog("Invalid UnsuccessMessage Data");
            return NULL;
        }
    break; /* end of NGAP_PDU_PR_unsuccessfulOutcome */

    default:
        higLog("Invalid NGAP PDU\n");
        return NULL;
    }
}

/* returns SUCCESS or FAILURE */
RAN_UE_NGAP_ID_t* ngapGetRanUeNgapId(NGAP_PDU_t *ngapPdu)
{
    RAN_UE_NGAP_ID_t* ranUeNgapId = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }

    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
    case ProcedureCode_id_InitialUEMessage: {
        FETCH_IE_PTR(InitialUEMessage_IEs, RAN_UE_NGAP_ID, ranUeNgapId);
    }
    break;

    case ProcedureCode_id_UplinkNASTransport: {
        FETCH_IE_PTR(UplinkNASTransport_IEs, RAN_UE_NGAP_ID, ranUeNgapId);
    }
    break;

    case ProcedureCode_id_DownlinkNASTransport: {
        FETCH_IE_PTR(DownlinkNASTransport_IEs, RAN_UE_NGAP_ID, ranUeNgapId);
    }
    break;

    case ProcedureCode_id_InitialContextSetup: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR(InitialContextSetupRequestIEs, RAN_UE_NGAP_ID,
                            ranUeNgapId);
        }
        break;
        case NGAP_PDU_PR_successfulOutcome: {
            FETCH_IE_PTR(InitialContextSetupResponseIEs, RAN_UE_NGAP_ID,
                            ranUeNgapId);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    case ProcedureCode_id_UEContextReleaseRequest: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR(UEContextReleaseRequest_IEs, RAN_UE_NGAP_ID,
                            ranUeNgapId);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;
    case ProcedureCode_id_UEContextRelease: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_successfulOutcome: {
            FETCH_IE_PTR(UEContextReleaseComplete_IEs, RAN_UE_NGAP_ID,
                            ranUeNgapId);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    case ProcedureCode_id_PDUSessionResourceSetup: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR(PDUSessionResourceSetupRequestIEs , RAN_UE_NGAP_ID,
                            ranUeNgapId);
        }
        break;
        case NGAP_PDU_PR_successfulOutcome: {
            FETCH_IE_PTR(PDUSessionResourceSetupResponseIEs , RAN_UE_NGAP_ID,
                            ranUeNgapId);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    case ProcedureCode_id_PDUSessionResourceRelease: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR(PDUSessionResourceReleaseCommandIEs , RAN_UE_NGAP_ID,
                            ranUeNgapId);
        }
        break;
        case NGAP_PDU_PR_successfulOutcome: {
            FETCH_IE_PTR(PDUSessionResourceReleaseResponseIEs, RAN_UE_NGAP_ID,
                            ranUeNgapId);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    case ProcedureCode_id_PDUSessionResourceModify: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR(PDUSessionResourceModifyRequestIEs , RAN_UE_NGAP_ID,
                            ranUeNgapId);
        }
        break;
        case NGAP_PDU_PR_successfulOutcome: {
            FETCH_IE_PTR(PDUSessionResourceModifyResponseIEs, RAN_UE_NGAP_ID,
                            ranUeNgapId);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    default:
        higLog("Invalid procedureCode %d", procedureCode);
        return NULL;
    }
    return ranUeNgapId;
}

FiveG_S_TMSI_t* ngapGetFiveGSTMSI(NGAP_PDU_t *ngapPdu)
{
    FiveG_S_TMSI_t* _5GSTmsi = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }

    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_InitialUEMessage: {
            FETCH_IE_PTR(InitialUEMessage_IEs, FiveG_S_TMSI, _5GSTmsi);
        }
        break;

        case ProcedureCode_id_Paging: {

            UEPagingIdentity_t* uePagingId = 0;
            FETCH_IE_PTR(PagingIEs, UEPagingIdentity, uePagingId);
            _5GSTmsi = uePagingId->choice.fiveG_S_TMSI;
        }
        break;

        default: {
            higLog("Invalid or unsupported procedureCode %d", procedureCode);
            return NULL;
        }
    }

    return _5GSTmsi;
}

RRCEstablishmentCause_t* ngapGetRRCEstbCause(NGAP_PDU_t *ngapPdu)
{
    RRCEstablishmentCause_t* RRCEstbCause = NULL;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }

    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_InitialUEMessage: {
            FETCH_IE_PTR(InitialUEMessage_IEs, RRCEstablishmentCause, RRCEstbCause);
        }
        break;

        /* Since RRC Estb Cause is part of only Initial UE Msg
           according to NGAP spec */
        default: {
            higLog("Invalid or unsupported procedureCode %d", procedureCode);
            return NULL;
        }
    }

    return RRCEstbCause;
}



/* returns NULL in case of FAILURE and valid ptr in case of SUCCESS */
AMF_UE_NGAP_ID_t* ngapGetAmfUeNgapId(NGAP_PDU_t *ngapPdu)
{
    AMF_UE_NGAP_ID_t* amfUeNgapId = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }

    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_UplinkNASTransport: {
            FETCH_IE_PTR(UplinkNASTransport_IEs, AMF_UE_NGAP_ID, amfUeNgapId);
            break;
        }

        case ProcedureCode_id_DownlinkNASTransport: {
            FETCH_IE_PTR(DownlinkNASTransport_IEs, AMF_UE_NGAP_ID, amfUeNgapId);
        }
        break;

        case ProcedureCode_id_InitialContextSetup: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(InitialContextSetupRequestIEs, AMF_UE_NGAP_ID,
                            amfUeNgapId);
            }
            break;
            case NGAP_PDU_PR_successfulOutcome: {
                FETCH_IE_PTR(InitialContextSetupResponseIEs, AMF_UE_NGAP_ID,
                            amfUeNgapId);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        case ProcedureCode_id_UEContextReleaseRequest: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(UEContextReleaseRequest_IEs, AMF_UE_NGAP_ID,
                            amfUeNgapId);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        case ProcedureCode_id_UEContextRelease: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_successfulOutcome: {
                FETCH_IE_PTR(UEContextReleaseComplete_IEs, AMF_UE_NGAP_ID,
                            amfUeNgapId);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        case ProcedureCode_id_PDUSessionResourceSetup: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(PDUSessionResourceSetupRequestIEs , AMF_UE_NGAP_ID,
                                amfUeNgapId);
            }
            break;
            case NGAP_PDU_PR_successfulOutcome: {
                FETCH_IE_PTR(PDUSessionResourceSetupResponseIEs , AMF_UE_NGAP_ID,
                                amfUeNgapId);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        case ProcedureCode_id_PDUSessionResourceRelease: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(PDUSessionResourceReleaseCommandIEs , AMF_UE_NGAP_ID,
                                amfUeNgapId);
            }
            break;
            case NGAP_PDU_PR_successfulOutcome: {
                FETCH_IE_PTR(PDUSessionResourceReleaseResponseIEs, AMF_UE_NGAP_ID,
                                amfUeNgapId);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        case ProcedureCode_id_PDUSessionResourceModify: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(PDUSessionResourceModifyRequestIEs , AMF_UE_NGAP_ID,
                                amfUeNgapId);
            }
            break;
            case NGAP_PDU_PR_successfulOutcome: {
                FETCH_IE_PTR(PDUSessionResourceModifyResponseIEs, AMF_UE_NGAP_ID,
                                amfUeNgapId);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
    }
    return amfUeNgapId;
}

/* returns SUCCESS or FAILURE */
NAS_PDU_t* ngapGetNasPdu(NGAP_PDU_t *ngapPdu)
{
    NAS_PDU_t* nasPdu = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }

    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_InitialUEMessage: {
            FETCH_IE_PTR(InitialUEMessage_IEs, NAS_PDU, nasPdu);
        }
        break;

        case ProcedureCode_id_UplinkNASTransport: {
            FETCH_IE_PTR(UplinkNASTransport_IEs, NAS_PDU, nasPdu);
        }
        break;

        case ProcedureCode_id_DownlinkNASTransport: {
            FETCH_IE_PTR(DownlinkNASTransport_IEs, NAS_PDU, nasPdu);
        }
        break;

        case ProcedureCode_id_InitialContextSetup: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(InitialContextSetupRequestIEs, NAS_PDU, nasPdu);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        case ProcedureCode_id_PDUSessionResourceRelease: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(PDUSessionResourceReleaseCommandIEs , NAS_PDU,
                                nasPdu);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
    }
    return nasPdu;
}

/* returns SUCCESS or FAILURE */
GlobalRANNodeID_t* ngapGetGlobalRANNodeId(NGAP_PDU_t *ngapPdu)
{
    GlobalRANNodeID_t *gRANNodeId = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }

    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_NGSetup: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(NGSetupRequestIEs, GlobalRANNodeID, gRANNodeId);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
    }
    return gRANNodeId;
}

/* returns SUCCESS or FAILURE */
SupportedTAList_t* ngapGetSupportedTAList(NGAP_PDU_t *ngapPdu)
{
    SupportedTAList_t *supportedTAList = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }

    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_NGSetup: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(NGSetupRequestIEs, SupportedTAList,
                                supportedTAList);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        {
        default:
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
    }
    return supportedTAList;
}

/* returns SUCCESS or FAILURE */
RANNodeName_t* ngapGetRANNodeName(NGAP_PDU_t *ngapPdu)
{
    RANNodeName_t* ranNodeName = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }

    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_NGSetup: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(NGSetupRequestIEs, RANNodeName, ranNodeName);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        {
        default:
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
    }
    return ranNodeName;
}

/* returns SUCCESS or FAILURE */
PagingDRX_t* ngapGetPagingDRX(NGAP_PDU_t *ngapPdu)
{
    PagingDRX_t* pagingDRX = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }

    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_NGSetup: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(NGSetupRequestIEs, PagingDRX, pagingDRX);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        {
        default:
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
    }
    return pagingDRX;
}

GUAMI_t* ngapGetGuami(NGAP_PDU_t* ngapPdu)
{
    GUAMI_t* gUAMI = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_InitialContextSetup: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(InitialContextSetupRequestIEs, GUAMI, gUAMI);
            }
            break;
            default: {
                higLog("Invalid procedureCode %d", procedureCode);
                return NULL;
            }
            }
        }
        break;

        {
        default:
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
    }
    return gUAMI;
}

UESecurityCapabilities_t* ngapGetUeSecurityCapabilities(NGAP_PDU_t* ngapPdu)
{
    UESecurityCapabilities_t* uESecurityCapabilities = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
    case ProcedureCode_id_InitialContextSetup: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
        FETCH_IE_PTR(InitialContextSetupRequestIEs, UESecurityCapabilities,
                        uESecurityCapabilities);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }

    return uESecurityCapabilities;
}



PDUSessionResourceSetupListSURes_t*
ngapGetpduSessionResourceSetupListSURes(NGAP_PDU_t *ngapPdu)
{
    PDUSessionResourceSetupListSURes_t* pduSessRSRList = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
    case ProcedureCode_id_PDUSessionResourceSetup:
    {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_successfulOutcome: {
            FETCH_IE_PTR(PDUSessionResourceSetupResponseIEs,
                     PDUSessionResourceSetupListSURes, pduSessRSRList);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }
    return pduSessRSRList;
}

PDUSessionResourceListCxtRelReq_t*
ngapGetPDUSessionResourceListCxtRelReq(NGAP_PDU_t *ngapPdu)
{
    PDUSessionResourceListCxtRelReq_t* pduSessResList = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_UEContextReleaseRequest:
        {
            switch(ngapPdu->present) {
                case NGAP_PDU_PR_initiatingMessage: {
                    FETCH_IE_PTR(UEContextReleaseRequest_IEs,
                             PDUSessionResourceListCxtRelReq, pduSessResList);
                }
                break;
                default: {
                    higLog("Invalid procedureCode %d", procedureCode);
                    return NULL;
                }
            }
        }
        break;

        default:
        higLog("Invalid procedureCode %d", procedureCode);
        return NULL;
    }
    return pduSessResList;
}

/* TODO - This function may not be required. Remove if unnecessary */
UEPagingIdentity_t* ngapGetUePagingIdentity(NGAP_PDU_t *ngapPdu) {

    UEPagingIdentity_t* uePagingId = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_Paging:
        {
            FETCH_IE_PTR(PagingIEs, UEPagingIdentity, uePagingId);
        }
        break;

        default:
        {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
    }
    return uePagingId;

}

TAIListForPaging_t* ngapGetTAIListForPaging(NGAP_PDU_t *ngapPdu) {

    TAIListForPaging_t* tAIList = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_Paging:
        {
            FETCH_IE_PTR(PagingIEs, TAIListForPaging, tAIList);
        }
        break;

        default:
        {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
    }
    return tAIList;
}


Cause_t* ngapGetCause(NGAP_PDU_t *ngapPdu)
{
    Cause_t* cause = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
    case ProcedureCode_id_InitialContextSetup: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_unsuccessfulOutcome: {
            FETCH_IE_PTR(InitialContextSetupFailureIEs, Cause, cause);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    case ProcedureCode_id_UEContextReleaseRequest: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR(UEContextReleaseRequest_IEs, Cause, cause);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;
    case ProcedureCode_id_UEContextRelease: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR(UEContextReleaseCommand_IEs, Cause, cause);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }
    return cause;
}

UE_NGAP_IDs_t* ngapGetUeNgapIDs(NGAP_PDU_t *ngapPdu)
{
    UE_NGAP_IDs_t* ueNgapID = 0;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);

    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
    case ProcedureCode_id_UEContextRelease: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR(UEContextReleaseCommand_IEs, UE_NGAP_IDs, ueNgapID);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }
    return ueNgapID;
}

PDUSessionResourceToReleaseListRelCmd_t*
        PDUSessResToRelListRelCmd(NGAP_PDU_t *ngapPdu)
{
    PDUSessionResourceToReleaseListRelCmd_t* resToRelList;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);
    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
    case ProcedureCode_id_PDUSessionResourceRelease: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR(PDUSessionResourceReleaseCommandIEs,
                    PDUSessionResourceToReleaseListRelCmd, resToRelList);
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
        }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }
    return resToRelList;
}

PDUSessionResourceSetupListSUReq_t*
        ngapGetPDUSessionResourceSetupListSUReq(NGAP_PDU_t *ngapPdu)
{
    PDUSessionResourceSetupListSUReq_t* setupList;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);
    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
    case ProcedureCode_id_PDUSessionResourceSetup: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR(PDUSessionResourceSetupRequestIEs ,
                    PDUSessionResourceSetupListSUReq, setupList);
        }
        break;
        default: {
            higLog("Invalid ngapPdu->present val %d", ngapPdu->present);
            return NULL;
        }
        }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }
    return setupList;
}

PDUSessionResourceSetupListCxtReq_t*
        ngapGetPDUSessionResourceSetupListCxtReq(NGAP_PDU_t *ngapPdu)
{
    PDUSessionResourceSetupListCxtReq_t* setupList;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);
    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_InitialContextSetup: {
            switch(ngapPdu->present) {
                case NGAP_PDU_PR_initiatingMessage: {
                    FETCH_IE_PTR(InitialContextSetupRequestIEs ,
                            PDUSessionResourceSetupListCxtReq, setupList);
                }
                break;
                default: {
                    higLog("Invalid ngapPdu->present val %d", ngapPdu->present);
                    return NULL;
                }
            }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }

    return setupList;
}

PDUSessionResourceSetupListCxtRes_t*
        ngapGetPDUSessionResourceSetupListCxtRes(NGAP_PDU_t *ngapPdu)
{
    PDUSessionResourceSetupListCxtRes_t* setupList;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);
    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case ProcedureCode_id_InitialContextSetup: {
            switch(ngapPdu->present) {
                case NGAP_PDU_PR_successfulOutcome: {
                    FETCH_IE_PTR(InitialContextSetupResponseIEs ,
                            PDUSessionResourceSetupListCxtRes, setupList);
                }
                break;
                default: {
                    higLog("Invalid ngapPdu->present val %d", ngapPdu->present);
                    return NULL;
                }
            }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }

    return setupList;
}

PDUSessionResourceSetupListSURes_t*
        ngapGetPDUSessionResourceSetupListSURes(NGAP_PDU_t *ngapPdu)
{
    PDUSessionResourceSetupListSURes_t* setupList;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);
    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
    case ProcedureCode_id_PDUSessionResourceSetup: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_successfulOutcome: {
            FETCH_IE_PTR(PDUSessionResourceSetupResponseIEs ,
                    PDUSessionResourceSetupListSURes, setupList);
        }
        break;
        default: {
            higLog("Invalid ngapPdu->present val %d", ngapPdu->present);
            return NULL;
        }
        }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }

    return setupList;
}



PDUSessionResourceReleasedListRelRes_t *
        ngapGetPDUSessionResourceReleasedListRelRes(NGAP_PDU_t *ngapPdu)
{
    PDUSessionResourceReleasedListRelRes* setupList;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);
    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
    case ProcedureCode_id_PDUSessionResourceRelease: {
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_successfulOutcome: {
            FETCH_IE_PTR(PDUSessionResourceReleaseResponseIEs ,
                    PDUSessionResourceReleasedListRelRes, setupList);
        }
        break;
        default: {
            higLog("Invalid ngapPdu->present val %d", ngapPdu->present);
            return NULL;
        }
        }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }

    return setupList;
}

UserLocationInformation_t* ngapGetUserLocationInfo(NGAP_PDU_t *ngapPdu){

    UserLocationInformation_t* userLocInfo;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);
    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode) {
        case ProcedureCode_id_InitialUEMessage: {
            switch(ngapPdu->present) {
            case NGAP_PDU_PR_initiatingMessage: {
                FETCH_IE_PTR(InitialUEMessage_IEs ,
                        UserLocationInformation, userLocInfo);
            }
            break;
            default: {
                higLog("Invalid ngapPdu->present val %d", ngapPdu->present);
                return NULL;
            }
            }
        }
        break;
        case ProcedureCode_id_UplinkNASTransport: {
            switch(ngapPdu->present) {
                case NGAP_PDU_PR_initiatingMessage: {
                    FETCH_IE_PTR(UplinkNASTransport_IEs,
                            UserLocationInformation, userLocInfo);
                }
                break;
                default: {
                    higLog("Invalid ngapPdu->present val %d", ngapPdu->present);
                    return NULL;
                }
            }
        }
        break;
        case ProcedureCode_id_UEContextRelease:{
            switch (ngapPdu->present) {
                case NGAP_PDU_PR_successfulOutcome: {
                    FETCH_IE_PTR(UEContextReleaseComplete_IEs,
                                        UserLocationInformation, userLocInfo);
                }
                break;
                default: {
                    higLog("Invalid ngapPdu->present val %d", ngapPdu->present);
                    return NULL;
                }
            }
        }
        break;
        default: {
            higLog("Invalid procedureCode %d", procedureCode);
            return NULL;
        }
    }

    return userLocInfo;
}

PDUSessionResourceModifyListModReq_t*
        ngapGetPDUSessionResourceModifyListModReq(NGAP_PDU_t *ngapPdu)
{
    PDUSessionResourceModifyListModReq_t* setupList;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);
    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case  ProcedureCode_id_PDUSessionResourceModify:{
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_initiatingMessage: {
            FETCH_IE_PTR( PDUSessionResourceModifyRequestIEs,
                    PDUSessionResourceModifyListModReq, setupList);
        }
        break;
        default: {
            higLog("Invalid ngapPdu->present val %d", ngapPdu->present);
            return NULL;
        }
        }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }
    return setupList;
}

PDUSessionResourceModifyListModRes_t*
        ngapGetPDUSessionResourceModifyListModRes(NGAP_PDU_t *ngapPdu)
{
    PDUSessionResourceModifyListModRes_t* setupList;
    int seqItr = 0;
    void *seqHead = ngapGetProtocolIeListPtr(ngapPdu);
    if(seqHead == NULL) {
        return NULL;
    }
    int seqCount = ASN_SEQUENCE_GET_COUNT(seqHead);
    ProcedureCode_t procedureCode = ngapGetProcedureCode(ngapPdu);

    switch(procedureCode)
    {
        case  ProcedureCode_id_PDUSessionResourceModify:{
        switch(ngapPdu->present) {
        case NGAP_PDU_PR_successfulOutcome: {
            FETCH_IE_PTR(PDUSessionResourceModifyResponseIEs,
                    PDUSessionResourceModifyListModRes, setupList);
        }
        break;
        default: {
            higLog("Invalid ngapPdu->present val %d", ngapPdu->present);
            return NULL;
        }
        }
    }
    break;

    default:
    higLog("Invalid procedureCode %d", procedureCode);
    return NULL;
    }
    return setupList;
}

/* Free the whole NGAP PDU struct.
 */
void ngapFree(NGAP_PDU_t *ngapPdu)
{
    ASN_STRUCT_FREE(asn_DEF_NGAP_PDU, ngapPdu);
}

void asnFree(PDUSessionResourceModifyResponseTransfer_t **ptr) {

    ASN_STRUCT_FREE(asn_DEF_UPTransportLayerInformation,
                    (*ptr)->uL_NGU_UP_TNLInformation);
    (*ptr)->uL_NGU_UP_TNLInformation = 0;

    ASN_STRUCT_FREE(asn_DEF_UPTransportLayerInformation,
                    (*ptr)->dL_NGU_UP_TNLInformation);
    (*ptr)->dL_NGU_UP_TNLInformation = 0;

    ASN_STRUCT_FREE(asn_DEF_QosFlowPerTNLInformation,
                    (*ptr)->additionalQosFlowPerTNLInformation);
    (*ptr)->additionalQosFlowPerTNLInformation = 0;

    ASN_STRUCT_FREE(asn_DEF_QosFlowAddOrModifyResponseList,
                    (*ptr)->qosFlowAddOrModifyResponseList);
    (*ptr)->qosFlowAddOrModifyResponseList = 0;

    ASN_STRUCT_FREE(asn_DEF_QosFlowList,
                    (*ptr)->qosFlowFailedToAddOrModifyList);
    (*ptr)->qosFlowFailedToAddOrModifyList = 0;

    ASN_STRUCT_FREE(asn_DEF_PDUSessionResourceModifyResponseTransfer,
                (*ptr));

    *ptr = 0;
}

Cause_PR getCausePR(ngap_error_cause_t errNum) {
    if(errNum >= E_RADIO_NETWORK_LAYER_UNSPECIFIED
        && errNum <= E_RADIO_NETWORK_LAYER_RELEASE_DUE_TO_CN_DETECTED_MOBILITY)
    {
        return Cause_PR_radioNetwork;
    } else if (errNum >= E_TRANSPORT_LAYER_TRANSPORT_RESOURCE_UNAVAILABLE
        && errNum <= E_TRANSPORT_LAYER_UNSPECIFIED) {
        return Cause_PR_transport;
    } else if (errNum >= E_NAS_NORMAL_RELEASE
        && errNum <= E_NAS_UNSPECIFIED) {
        return Cause_PR_nas;
    } else if (errNum >= E_PROTOCOL_TRANSFER_SYNTAX_ERROR
        && errNum <= E_PROTOCOL_UNSPECIFIED) {
        return Cause_PR_protocol;
    } else if (errNum >= E_MISCELLANEOUS_CONTROL_PROCESSING_OVERLOAD
        && errNum <= E_MISCELLANEOUS_UNSPECIFIED) {
        return Cause_PR_misc;
    }
    return Cause_PR_NOTHING;
}


int setFailureCause(Cause_t &Cause, int errNum) {

    LOG_ENTRY;

    /* Initialized to Misc Unspecified error by default */
    Cause.present = Cause_PR_misc;
    Cause.choice.misc = CauseMisc_unspecified;

    Cause_t cause = {};
    cause.present = getCausePR((ngap_error_cause_t)errNum);
    if(cause.present ==  Cause_PR_NOTHING) {
        higLog("Unknown Cause. This should not have happened for a valid errno");
        LOG_EXIT;
        return FAILURE;
    }

    switch(cause.present)
    {
        case Cause_PR_radioNetwork:
        {
            higLog("Not supporting radio Network Failure Intimation");
        }
        break;
        case Cause_PR_transport:
        {
            higLog("Not supporting transport Failure Intimation");
        }
        break;
        case Cause_PR_nas:
        {
            higLog("Not supporting nas Failure Intimation");
        }
        break;
        case Cause_PR_protocol:
        {
            higLog("Not supporting protocol Failure Intimation");
        }
        break;
        case Cause_PR_misc:
        {
            higLog("It is misc Failure");
            if(errno == E_MISCELLANEOUS_UNKNOWN_PLMN) {
                cause.choice.misc = CauseMisc_unknown_PLMN;
                higLog("Of type Unknown PLMN");
                Cause = cause;
            } else if(errno == E_MISCELLANEOUS_UNSPECIFIED) {
                cause.choice.misc = CauseMisc_unspecified;
                higLog("Of type Miscellaneous Unspecified");
                Cause = cause;
            } else {
                higLog("Of type which is unknown though");
            }
        }
        break;
        default:
        {
            higLog("Unknown Failure Cause");
            LOG_EXIT;
            return FAILURE;
        }
    }

    LOG_EXIT;
    return SUCCESS;
}

int getNgApCause(Cause_t *cause, ngApCause_t *ngapCause) {
    LOG_ENTRY;
    ngapCause->group = E_GP_NOTHING;
    if (cause == NULL) {
        higLog("Cause value sent is NULL");
        LOG_EXIT;
        return FAILURE;
    }

    switch (cause->present) {
        case Cause_PR_radioNetwork: {
            ngapCause->group = E_GP_RADIONETWORK;
            ngapCause->value = radioNetworkMap[cause->choice.radioNetwork];
            break;
        }
        case Cause_PR_transport:
            ngapCause->group = E_GP_TRANSPORT;
            ngapCause->value = causeTransportMap[cause->choice.transport];
            break;
        case Cause_PR_nas:
            ngapCause->group = E_GP_NAS;
            ngapCause->value = causeNasMap[cause->choice.nas];
            break;
        case Cause_PR_protocol:
            ngapCause->group =  E_GP_PROTOCOL;
            ngapCause->value = causeProtocolMap[cause->choice.protocol];
            break;
        case Cause_PR_misc:
            ngapCause->group =  E_GP_MISC;
            ngapCause->value = causeMiscMap[cause->choice.misc];
            break;
        default:
            higLog("Unsupported Cause group value");
            LOG_EXIT;
            return FAILURE;
    }
    LOG_EXIT;
    return SUCCESS;
}

int getCause(ngApCause_t ngapCause, Cause_t* cause) {
    LOG_ENTRY;

    if (ngapCause.group == E_GP_NOTHING) {
        higLog("Cause is NULL");
        LOG_EXIT;
        return FAILURE;
    }

    switch (ngapCause.group) {
        case E_GP_RADIONETWORK: {
            cause->present = Cause_PR_radioNetwork;
            map<CauseRadioNetwork_t, e_causeRadioNetwork_t>::iterator itr =
                                    radioNetworkMap.begin();
            while ( itr != radioNetworkMap.end()) {
                if  (itr->second == ngapCause.value)
                {
                    cause->choice.radioNetwork = itr->first;
                    break;
                }
                itr++;
            }
            if (itr == radioNetworkMap.end()) {
                higLog("Could not find the cause value corresponding to %d",
                                                             ngapCause.value);
                LOG_EXIT;
                return FAILURE;
            }
            break;
        }
        case E_GP_TRANSPORT: {
            cause->present = Cause_PR_transport;
            map <CauseTransport_t, e_causeTransport_t>::iterator itr;
            for(itr = causeTransportMap.begin(); itr != causeTransportMap.end();
                    itr++) {
                if(itr->second == ngapCause.value)
                {
                cause->choice.transport = itr->first;
                break;
                }
            }
            if (itr == causeTransportMap.end()) {
                higLog("Could not find the cause value corresponding to %d",
                                                             ngapCause.value);
                LOG_EXIT;
                return FAILURE;
            }
            break;
        }
        case E_GP_NAS: {
            cause->present = Cause_PR_nas;
            map <CauseNas_t, e_causeNas_t>::iterator itr;
            for(itr = causeNasMap.begin(); itr != causeNasMap.end();
                    itr++) {
                if(itr->second == ngapCause.value)
                {
                cause->choice.nas = itr->first;
                break;
                }
            }
            if (itr == causeNasMap.end()) {
                higLog("Could not find the cause value corresponding to %d",
                                                             ngapCause.value);
                LOG_EXIT;
                return FAILURE;
            }
            break;
        }
        case E_GP_PROTOCOL: {
            cause->present = Cause_PR_protocol;
            map <CauseProtocol_t, e_causeProtocol_t>::iterator itr;
            for(itr = causeProtocolMap.begin(); itr != causeProtocolMap.end();
                    itr++) {
                if(itr->second == ngapCause.value)
                {
                cause->choice.protocol = itr->first;
                break;
                }
            }
            if (itr == causeProtocolMap.end()) {
                higLog("Could not find the cause value corresponding to %d",
                                                             ngapCause.value);
                LOG_EXIT;
                return FAILURE;
            }
            break;
        }
        case E_GP_MISC: {
            cause->present = Cause_PR_misc;
            map <CauseMisc_t, e_causeMisc_t>::iterator itr;
            for(itr = causeMiscMap.begin(); itr != causeMiscMap.end();
                    itr++) {
                if(itr->second == ngapCause.value)
                {
                cause->choice.misc = itr->first;
                break;
                }
            }
            if (itr == causeMiscMap.end()) {
                higLog("Could not find the cause value corresponding to %d",
                                                             ngapCause.value);
                LOG_EXIT;
                return FAILURE;
            }
            break;
        }
        default:
            higLog("Unsupported Cause group value");
            LOG_EXIT;
            return FAILURE;
    }
    LOG_EXIT;
    return SUCCESS;
}

