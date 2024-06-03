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

#ifndef __PLATFORM_MULTIPART
#define __PLATFORM_MULTIPART

#include <string>
#include <map>
#include <vector>
#include "platform/include/logging.h"
#include "common/include/datatypes.h"

/* *Tiny* utility to construct & decode a multipart message.
 *
 * Reference: https:
 */

#define HTTP_CRLF  std::string("\r\n")
#define BOUNDARY_LEN 20


enum {
    E_MP_JSON = 0,
    E_MP_5GNAS,
    E_MP_NGAP,

    E_MP_NUM_OF_TYPES
};

static std::map<uint, std::string> contentTypeMap = {
    {E_MP_JSON,     "Content-type: application/json"},
    {E_MP_5GNAS,    "Content-type: application/vnd.3gpp.5gnas"},
    {E_MP_NGAP,     "Content-type: application/vnd.3gpp.ngap"},
};

class multipart {
public:
    std::map <uint, std::string> parts;
    std::string boundary;

    std::string getBoundary();
    int extractBoundary(std::string);
    int Add(uint, std::string);
    int Encode(std::string &mpMsg);
    int Decode(std::string mpMsg);
    multipart() {
        parts = {};
        boundary = "";
    }

private:
    std::string generateBoundary();
};

/* A simple Encode example:
 *
 *  multipart mp;       
 *  mp.Add(E_MP_JSON, smContextCreateData.serialize());     
 *  mp.Add(E_MP_5GNAS, std::string((char*)someNasMsg, 16)); 
 *
 *  std::string encodedStr;
 *  mp.Encode(encodedStr)
 *
 *  http::http_request smfReq(methods::POST);     
 *  smfReq.set_body(encodedStr,
 *          "multipart/related; boundary=" + mp.getBoundary());
 *  
 *
 *
 * A simple Decode example:
 *
 *  http_request message;
 *
 *  string content_type = message.headers().content_type(); 
 *  if(content_type contains "multipart/related") {
 *      multipart mp;
 *      mp.extractBoundary(content_type);       
 *
 *      
 *      mp.Decode(msg_str);     
 *
 */



#endif
