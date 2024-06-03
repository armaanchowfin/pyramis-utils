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

#include "platform/include/multipart.h"

int multipart::Add(uint key, std::string val)
{
    LOG_ENTRY;

    if(parts.find(key) != parts.end()) {
        higLog("part %d already present. Attempt to overwrite");
        LOG_EXIT;
        return FAILURE;
    }
    parts[key] = val;

    LOG_EXIT;
    return SUCCESS;
}

std::string multipart::getBoundary()
{
        return boundary;
}

/* overwrite existing boundary if necessary */
std::string multipart::generateBoundary()
{
    std::string str = "";
    char alphabets[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while(1) {
        str = "";
        bool match = false;

        for(int i=0;i<BOUNDARY_LEN; i++) {
            str += alphabets[rand()%52];
        }

        for(std::map<uint, std::string>::iterator iter = parts.begin();
                                                 iter != parts.end(); iter++) {
            if(iter->second.find(str) != std::string::npos) {
                match = true;
                break;
            }
        }

        if(match == false)
            break;
    }

    lowLog("generated boundary: %s", str.c_str());
    boundary = str;

    return str;
}

int multipart::Encode(std::string &mpMsg)
{
    LOG_ENTRY;
    if(parts.size() == 0) {
        higLog("Error: Zero parts added");
        LOG_EXIT;
        return FAILURE;
    }

    generateBoundary();

    for(uint i=0; i<E_MP_NUM_OF_TYPES; i++) {
        if(parts.find(i) == parts.end())
            continue;

        mpMsg += std::string("--") + boundary + HTTP_CRLF;
        mpMsg += contentTypeMap[i] + ";" + HTTP_CRLF;
        mpMsg += parts[i] + HTTP_CRLF;

        lowLog("Adding %s, length %d", contentTypeMap[i].c_str(),
                parts[i].size());
    }
    mpMsg += "--" + boundary + "--" + HTTP_CRLF;

    lowLog("Message:\n %s", mpMsg.c_str()); //TODO print ascii array
    LOG_EXIT;
    return SUCCESS;
}

int multipart::extractBoundary(std::string content_type)
{
    LOG_ENTRY;

    std::string::size_type pos = content_type.find(std::string("boundary="));
    if(pos == std::string::npos) {
        higLog("malformed multipart content");
        LOG_EXIT;
        return FAILURE;
    }

    boundary = content_type.substr(pos + std::string("boundary=").size());

    if(content_type != ("multipart/related; boundary=" + boundary)) {
        higLog("malformed multipart content");
        LOG_EXIT;
        return FAILURE;
    }

    lowLog("boundary %s", boundary.c_str());
    LOG_EXIT;
    return SUCCESS;

}

int multipart::Decode(std::string mpMsg)
{
    LOG_ENTRY;

    std::string boundary2 = HTTP_CRLF + "--" + boundary + HTTP_CRLF;
    mpMsg = HTTP_CRLF + mpMsg; /* so that it begins with boundary 2 */

    std::string::size_type pos = mpMsg.find(HTTP_CRLF + "--" + boundary + "--" + HTTP_CRLF);
    std::vector<std::string> rawParts;

    while(pos != std::string::npos) {
        std::string tmsg = mpMsg.substr(0, pos);
        pos = tmsg.rfind(boundary2);

        if(pos != std::string::npos)
            rawParts.push_back(tmsg.substr(pos + boundary2.size()));
    }

    lowLog("Found %d raw parts", rawParts.size());

    for(uint i =0; i<rawParts.size(); i++) {
        uint type = 0;
        std::string conType;

        for(type=0; type<E_MP_NUM_OF_TYPES; type++) {
            conType = contentTypeMap[type] + ";" + HTTP_CRLF;
            if(rawParts[i].size() <= conType.size())
                continue;

            std::string::size_type ctPos = rawParts[i].find(conType);
            if(ctPos != std::string::npos && ctPos == 0)
                break;
        }
        if(type == E_MP_NUM_OF_TYPES) {
            higLog("Unknown Content. %s", rawParts[i]);
            return FAILURE;
        }

        if(parts.find(type) != parts.end()) {
            higLog("Multiple parts of the same type are not supported");
            return FAILURE;
        }

        std::string tempPart = rawParts[i].substr(conType.size());
        parts[type] = tempPart;
        lowLog("Extracted part of size %d, type %d", tempPart.size(), type);
    }

    lowLog("Extracted %d parts", parts.size());

    LOG_EXIT;
    return SUCCESS;
}
