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


#ifndef __PFCP_COMMON_H
#define __PFCP_COMMON_H


#define PFCP_IPV4_ADDR_LEN      4
#define PFCP_IPV6_ADDR_LEN      16

#define PFCP_IP_TYPE_IPV4       0
#define PFCP_IP_TYPE_IPV6       1

#define PFCP_SERVER_PORT        8805








#define PFCP_PDI_MASK_UE_SRC_IP_ADDR         (1<<0)
#define PFCP_PDI_MASK_UE_DEST_IP_ADDR        (1<<1)
#define PFCP_PDI_MASK_TUNNEL_INFO            (1<<2)
#define PFCP_PDI_MASK_QFI                    (1<<3)
#define PFCP_PDI_MASK_SDF_FILTER             (1<<4)



#define PFCP_PDR_MASK_OHR                    (1<<0)
#define PFCP_PDR_MASK_QER                    (1<<1)
#define PFCP_PDR_MASK_FAR                    (1<<2)
#define PFCP_PDR_MASK_NEW                    (1<<31)



#define PFCP_FAR_MASK_OHC                    (1<<0)
#define PFCP_FAR_MASK_BAR                    (1<<1)



#define PFCP_QER_MASK_CORRELATION_ID         (1<<0)
#define PFCP_QER_MASK_GBR                    (1<<1)
#define PFCP_QER_MASK_MBR                    (1<<2)
#define PFCP_QER_MASK_QFI                    (1<<3)
#define PFCP_QER_MASK_PACKET_RATE            (1<<4)
#define PFCP_QER_MASK_GATE_STATUS            (1<<5)
#define PFCP_QER_MASK_NEW                    (1<<31)



#define PFCP_BAR_MASK_DELAY                  (1<<0)
#define PFCP_BAR_MASK_PKTCNT                 (1<<1)

#endif 
