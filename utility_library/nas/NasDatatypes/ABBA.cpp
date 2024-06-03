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

#include "nas/nasUtil/nasLogging.h"
#include "nas/NasDatatypes/ABBA.h"

int decodeABBA(uint8_t *buffer, ABBA_t *abba, uint32_t bufLen,
                      uint32_t* decodedLen)
{
    nasLogENTRY;                                                                   
    if(bufLen < ABBA_LEN_MIN || buffer == NULL || abba == NULL ||       
       decodedLen == NULL) {                                                       
        higLog("%s", "Invalid Input, Failure decoding");                           
        errno = E_NAS_ERROR_INVALID_INPUT;                                         
        nasLogEXIT;                                                                
        return FAILURE;                                                            
    }                                                                              
                   
    int abbaLen = buffer[0];
    /* check if the length is valid */                                          
    if(abbaLen < ABBA_LEN_MIN                                      
       || abbaLen > ABBA_LEN_MAX) {                                
        higLog("Invalid length %d", abbaLen);                                 
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;                                  
        nasLogEXIT;                                                             
        return FAILURE;                                                         
    }                                                                           
    abba->len = abbaLen;                                                      
    *decodedLen = sizeof(abba->len);                                            
                                                                                
    if(abba->len > bufLen - (*decodedLen)) {     
        higLog("Buffer insufficient to copy AUTN");                             
        errno = E_NAS_ERROR_BUFF_INSUFF;                                        
        nasLogEXIT;                                                             
        return FAILURE;                                                         
    }                                                                           
                                                                                
    memcpy(abba->contents, buffer + (*decodedLen),                                  
            abba->len );                                
    *decodedLen += abba->len;                          
    nasLog("ABBA Param decoded abba len %d | decoded %d", abba->len,            
            *decodedLen);                                                       
                                                                                
    nasLogEXIT;                                                                 
    return SUCCESS;                
}

int encodeABBA(uint8_t *buffer, uint32_t bufLen, ABBA_t *abba,
                      uint32_t *encodedLen)
{
    nasLogENTRY;                                                                
    if(bufLen < ABBA_LEN_MIN || buffer == NULL || abba == NULL ||    
       encodedLen == NULL) {                                                    
        higLog("%s", "Invalid Input, Failure decoding");                        
        errno = E_NAS_ERROR_INVALID_INPUT;                                      
        nasLogEXIT;                                                             
        return FAILURE;                                                         
    }                                                                           
                                                                                
    /* check if the length is valid */                                          
    if(abba->len < ABBA_LEN_MIN                                      
       || abba->len > ABBA_LEN_MAX) {                                
        higLog("Invalid length %d", abba->len);                                 
        errno = E_NAS_ERROR_CONSTR_CHECK_FAIL;                                  
        nasLogEXIT;                                                             
        return FAILURE;                                                         
    }                                                                           
    buffer[0] = abba->len;                                                      
    *encodedLen = sizeof(abba->len);                                            
                                                                                
    if(abba->len > bufLen - (*encodedLen)) {     
        higLog("Buffer of insufficient len");                                   
        errno = E_NAS_ERROR_BUFF_INSUFF;                                        
        nasLogEXIT;                                                             
        return FAILURE;                                                         
    }                                                                           
    memcpy(buffer + (*encodedLen), abba->contents,
            abba->len);
    *encodedLen += abba->len;
    nasLog("ABBA Param encoded abba len %d | encoded %d", abba->len,            
            *encodedLen);                                                       
                                                                                
    nasLogEXIT;                                                                 
    return SUCCESS;                        
}
