/*
 * TestAsn1.cpp
 *
 *  Created on: Sep 18, 2017
 *      Author: j.zhou
 */
 
#include "liblte_mme.h"
#include "liblte_rrc.h"
#include "TestAsn1.h"
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

using namespace std;

TEST_F(TestAsn1, Decode_NAS_IdentityResp) {
    // 07 56 08 49 06 00 00 65 95 70
    // 79
    // Identity Response
    // Type: EPS MM
    // Direction: Uplink
    // Computer Timestamp: 11:32:05.572
    // UE Timestamp: 5097762 (ms)
    // Radio Technology: LTE
    //   Identity Response
    //     Security header type: (0) Plain NAS message, not security protected 
    //     protocol_discriminator: EPS Mobility Management
    //     NAS EPS Mobility Management Message Type: (0x56) Identity response 
    //     Mobile identity
    //       Length: 8 octets
    //       Odd/even indication: (1) odd number of identity digits
    //       Type of identity: (1) IMSI
    //       Identity digits (BCD): 460000056590797
    uint8 nasMsg[] = {
        0x07, 0x56, 0x08, 0x49, 0x06, 0x00, 0x00, 0x65, 0x95, 0x70,
        0x79
    };
    uint32 nasMsgLen = 11;
    uint8 imsi[15] = {4,6,0,0,0,0,0,5,6,5,9,0,7,9,7};

    LIBLTE_BYTE_MSG_STRUCT nasBuf;
    LIBLTE_MME_ID_RESPONSE_MSG_STRUCT identityResp;
    uint8 pd;
    uint8 msgType;
    LIBLTE_ERROR_ENUM ret;

    nasBuf.msg = (uint8*)nasMsg;
    nasBuf.N_bytes = nasMsgLen;
    liblte_mme_parse_msg_header(&nasBuf, &pd, &msgType);
    EXPECT_EQ(LIBLTE_MME_MSG_TYPE_IDENTITY_RESPONSE, msgType);
    EXPECT_EQ(LIBLTE_MME_PD_EPS_MOBILITY_MANAGEMENT, pd);
    ret = liblte_mme_unpack_identity_response_msg(&nasBuf, &identityResp);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(LIBLTE_MME_MOBILE_ID_TYPE_IMSI, identityResp.mobile_id.type_of_id);
    EXPECT_EQ(0, memcmp(imsi, identityResp.mobile_id.imsi, 15));
    cout << "Parsed IMSI: "; 
    for (int i=0; i<15; i++) {
        cout << (int)identityResp.mobile_id.imsi[i];
    }
    cout << endl;  
}

TEST_F(TestAsn1, Decode_ULDCCH_DirectTx_NAS_IdentityResp) {
// 48 02 22 f2 fb 17 87 c9 40 ea
// c1 09 20 c0 00 0c b2 ae 0f 20
// ULInformationTransfer
// Type: UL_DCCH
// Direction: Uplink
// Computer Timestamp: 10:10:45.906
// UE Timestamp: 217949 (ms)
// Radio Technology: LTE
//   UL-DCCH-Message
//     message
//       c1
//         ulInformationTransfer
//           criticalExtensions
//             c1
//               ulInformationTransfer-r8
//                 dedicatedInfoType
//                   dedicatedInfoNAS: 0x1797D8BC3E4A0756084906000065957079
//   Identity Response
//     Security header type: (1) Integrity protected 
//     protocol_discriminator: EPS Mobility Management
//     Message authentication code: 0x97d8bc3e 
//     Sequence number: 74 
//     Security header type: (0) Plain NAS message, not security protected 
//     protocol_discriminator: (7) EPS mobility management messages 
//     NAS EPS Mobility Management Message Type: (0x56) Identity response 
//     Mobile identity
//       Length: 8 octets
//       Odd/even indication: (1) odd number of identity digits
//       Type of identity: (1) IMSI
//       Identity digits (BCD): 460000056590797
    uint8 rrcUlDirTxMsg[] = {
        0x48, 0x02, 0x22, 0xf2, 0xfb, 0x17, 0x87, 0xc9, 0x40, 0xea,
        0xc1, 0x09, 0x20, 0xc0, 0x00, 0x0c, 0xb2, 0xae, 0x0f, 0x20
    };
    uint32 rrcMsgLen = 20;    
    uint32 rrcMsgBitLen = rrcMsgLen << 3;

    uint8 nasMsg[] = {
        0x17, 0x97, 0xD8, 0xBC, 0x3E, 0x4A, 
        0x07, 0x56, 0x08, 0x49, 0x06, 0x00, 0x00, 0x65, 0x95, 0x70, 
        0x79};
    uint32 nasMsgLen = 17;

    uint8 imsi[15] = {4,6,0,0,0,0,0,5,6,5,9,0,7,9,7};

    LIBLTE_BYTE_MSG_STRUCT rrculDirTxByteBuf;
    LIBLTE_BIT_MSG_STRUCT rrcUlDirTxBitBuf;
    LIBLTE_RRC_UL_INFORMATION_TRANSFER_STRUCT ulDirTxMsg;
    LIBLTE_ERROR_ENUM ret;
    uint8 ext;
    uint8 msgType;

    // only parse msg type
    rrculDirTxByteBuf.msg = (uint8*)rrcUlDirTxMsg;
    rrculDirTxByteBuf.N_bytes = rrcMsgLen;
    ret = liblte_rrc_parse_dcch_msg_header(&rrculDirTxByteBuf, &ext, &msgType);
    printf("ul dcch msgType = %d\n", msgType);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(0, ext);
    EXPECT_EQ(LIBLTE_RRC_UL_DCCH_MSG_TYPE_UL_INFO_TRANSFER, msgType);

    // parse the whole ul direct transfer msg
    uint32 dcchHeaderBitLen = 5;
    convert_bytes_to_bits_vector(rrcUlDirTxBitBuf.msg, rrcUlDirTxMsg, rrcMsgBitLen);
    rrcUlDirTxBitBuf.msg += dcchHeaderBitLen;
    rrcUlDirTxBitBuf.N_bits = rrcMsgBitLen - dcchHeaderBitLen;;
    ret = liblte_rrc_unpack_ul_information_transfer_msg(&rrcUlDirTxBitBuf, &ulDirTxMsg);    
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(LIBLTE_RRC_UL_INFORMATION_TRANSFER_TYPE_NAS, ulDirTxMsg.dedicated_info_type);
    cout << "NAS message[" << ulDirTxMsg.dedicated_info.N_bytes << "]: " << endl;
    for (uint32 i=0; i<ulDirTxMsg.dedicated_info.N_bytes; i++) {
        printf("%02x ", ulDirTxMsg.dedicated_info.msg[i]);
    }
    printf("\n");    
    EXPECT_EQ(nasMsgLen, ulDirTxMsg.dedicated_info.N_bytes);
    EXPECT_EQ(0, memcmp(nasMsg, ulDirTxMsg.dedicated_info.msg, nasMsgLen));
    // continue to parse nas msg
    uint8 pd;
    LIBLTE_MME_ID_RESPONSE_MSG_STRUCT identityResp;
    LIBLTE_BYTE_MSG_STRUCT nasBuf;
    nasBuf.N_bytes = ulDirTxMsg.dedicated_info.N_bytes;
    nasBuf.msg = (uint8*)ulDirTxMsg.dedicated_info.msg;
    liblte_mme_parse_msg_header(&nasBuf, &pd, &msgType);
    printf("NAS msgType = 0x%02x\n", msgType);
    EXPECT_EQ(LIBLTE_MME_MSG_TYPE_IDENTITY_RESPONSE, msgType);
    EXPECT_EQ(LIBLTE_MME_PD_EPS_MOBILITY_MANAGEMENT, pd);
    ret = liblte_mme_unpack_identity_response_msg(&nasBuf, &identityResp);
    EXPECT_EQ(ret, LIBLTE_SUCCESS);
    EXPECT_EQ(LIBLTE_MME_MOBILE_ID_TYPE_IMSI, identityResp.mobile_id.type_of_id);
    EXPECT_EQ(0, memcmp(imsi, identityResp.mobile_id.imsi, 15));
    cout << "Parsed IMSI: "; 
    for (int i=0; i<15; i++) {
        cout << (int)identityResp.mobile_id.imsi[i];
    }
    cout << endl;  
}

TEST_F(TestAsn1, Decode_NAS_IdentityReq) {
    // 07 55 01
    // Identity Request
    // Type: EPS MM
    // Direction: Downlink
    // Computer Timestamp: 18:10:46.598
    // UE Timestamp: 29019461 (ms)
    // Radio Technology: LTE
    //   Identity Request
    //     Security header type: (0) Plain NAS message, not security protected 
    //     protocol_discriminator: EPS Mobility Management
    //     NAS EPS Mobility Management Message Type: (0x55) Identity request 
    //     Spare bit(s): 0 
    //     Identity type 2: (1) IMSI 
    uint8 nasMsg[] = {0x07, 0x55, 0x01};
    uint32 nasMsgLen = 3;

    LIBLTE_BYTE_MSG_STRUCT nasBuf;
    LIBLTE_MME_ID_REQUEST_MSG_STRUCT identityReq;
    uint8 pd;
    uint8 msgType;
    LIBLTE_ERROR_ENUM ret;

    nasBuf.msg = (uint8*)nasMsg;
    nasBuf.N_bytes = nasMsgLen;
    liblte_mme_parse_msg_header(&nasBuf, &pd, &msgType);
    printf("NAS msgType = 0x%02x\n", msgType);
    EXPECT_EQ(LIBLTE_MME_MSG_TYPE_IDENTITY_REQUEST, msgType);
    EXPECT_EQ(LIBLTE_MME_PD_EPS_MOBILITY_MANAGEMENT, pd);
    ret = liblte_mme_unpack_identity_request_msg(&nasBuf, &identityReq);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(LIBLTE_MME_MOBILE_ID_TYPE_IMSI, identityReq.id_type);
}

TEST_F(TestAsn1, Decode_DLDCCH_DirectTx_NAS_IdentityReq) {
    // 0a 00 18 3a a8 08
    // DLInformationTransfer
    // Type: DL_DCCH
    // Direction: Downlink
    // Computer Timestamp: 18:08:13.634
    // UE Timestamp: 28866498 (ms)
    // Radio Technology: LTE
    //   DL-DCCH-Message
    //     message
    //       c1
    //         dlInformationTransfer
    //           rrc-TransactionIdentifier: 1
    //           criticalExtensions
    //             c1
    //               dlInformationTransfer-r8
    //                 dedicatedInfoType
    //                   dedicatedInfoNAS: 0x075501
    //   Identity Request
    //     Security header type: (0) Plain NAS message, not security protected 
    //     protocol_discriminator: EPS Mobility Management
    //     NAS EPS Mobility Management Message Type: (0x55) Identity request 
    //     Spare bit(s): 0 
    //     Identity type 2: (1) IMSI 
    uint8 rrcDlDirTxMsgData[] = {0x0a, 0x00, 0x18, 0x3a, 0xa8, 0x08};
    uint32 rrcMsgLen = 6;    
    uint32 rrcMsgBitLen = rrcMsgLen << 3;

    uint8 nasMsg[] = {0x07, 0x55, 0x01};
    uint32 nasMsgLen = 3;

    LIBLTE_BYTE_MSG_STRUCT rrcDlDirTxByteBuf;
    LIBLTE_BIT_MSG_STRUCT rrcDlDirTxBitBuf;
    LIBLTE_RRC_DL_INFORMATION_TRANSFER_STRUCT dlDirTxMsg;
    LIBLTE_ERROR_ENUM ret;
    uint8 ext;
    uint8 msgType;

    // only parse msg type
    rrcDlDirTxByteBuf.msg = (uint8*)rrcDlDirTxMsgData;
    rrcDlDirTxByteBuf.N_bytes = rrcMsgLen;
    ret = liblte_rrc_parse_dcch_msg_header(&rrcDlDirTxByteBuf, &ext, &msgType);
    printf("dl dcch msgType = %d\n", msgType);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(0, ext);
    EXPECT_EQ(LIBLTE_RRC_DL_DCCH_MSG_TYPE_DL_INFO_TRANSFER, msgType);

    // parse the whole dl direct transfer msg
    uint32 dcchHeaderBitLen = 5;
    convert_bytes_to_bits_vector(rrcDlDirTxBitBuf.msg, rrcDlDirTxMsgData, rrcMsgBitLen);
    rrcDlDirTxBitBuf.msg += dcchHeaderBitLen;
    rrcDlDirTxBitBuf.N_bits = rrcMsgBitLen - dcchHeaderBitLen;
    ret = liblte_rrc_unpack_dl_information_transfer_msg(&rrcDlDirTxBitBuf, &dlDirTxMsg);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(LIBLTE_RRC_DL_INFORMATION_TRANSFER_TYPE_NAS, dlDirTxMsg.dedicated_info_type);
    EXPECT_EQ(1, dlDirTxMsg.rrc_transaction_id);
    cout << "NAS message[" << dlDirTxMsg.dedicated_info.N_bytes << "]: " << endl;
    for (uint32 i=0; i<dlDirTxMsg.dedicated_info.N_bytes; i++) {
        printf("%02x ", dlDirTxMsg.dedicated_info.msg[i]);
    }
    printf("\n");    
    EXPECT_EQ(nasMsgLen, dlDirTxMsg.dedicated_info.N_bytes);
    EXPECT_EQ(0, memcmp(nasMsg, dlDirTxMsg.dedicated_info.msg, nasMsgLen));
    // continue to parse nas msg
    LIBLTE_BYTE_MSG_STRUCT nasBuf;
    LIBLTE_MME_ID_REQUEST_MSG_STRUCT identityReq;
    uint8 pd;

    nasBuf.msg = (uint8*)dlDirTxMsg.dedicated_info.msg;
    nasBuf.N_bytes = nasMsgLen;
    liblte_mme_parse_msg_header(&nasBuf, &pd, &msgType);
    printf("NAS msgType = 0x%02x\n", msgType);
    EXPECT_EQ(LIBLTE_MME_MSG_TYPE_IDENTITY_REQUEST, msgType);
    EXPECT_EQ(LIBLTE_MME_PD_EPS_MOBILITY_MANAGEMENT, pd);
    ret = liblte_mme_unpack_identity_request_msg(&nasBuf, &identityReq);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(LIBLTE_MME_MOBILE_ID_TYPE_IMSI, identityReq.id_type);
}

TEST_F(TestAsn1, Decode_NAS_AttachReq_WithImsi) {
    // 07 41 71 08 49 06 00 00 65 95 
    // 70 79 04 e0 e0 c0 c0 00 23 02 
    // 01 d0 31 27 1d 80 80 21 10 01 
    // 01 00 10 81 06 00 00 00 00 83 
    // 06 00 00 00 00 00 03 00 00 03 
    // 00 00 0a 00 5c 20 00 5d 01 04
    // Attach Request
    // Type: EPS MM
    // Direction: Uplink
    // Computer Timestamp: 19:12:14.505
    // UE Timestamp: 32707448 (ms)
    // Radio Technology: LTE
    //   Attach Request
    //     Security header type: (0) Plain NAS message, not security protected 
    //     protocol_discriminator: EPS Mobility Management
    //     NAS EPS Mobility Management Message Type: (0x41) Attach request 
    //     Type of security context flag (TSC): (0) Native security context 
    //     NAS key set identifier: (7) No key is available
    //     Spare bit(s): 0 
    //     EPS attach type: (1) EPS attach 
    //     EPS mobile identity - Old GUTI or IMSI
    //       Length: 8 octets
    //       odd/even indic: 1
    //       Type of identity: (1) IMSI 
    //       IMSI: 460000056590797
    //     UE network capability
    //       Length: 4 octets
    //       EEA0: (1) Support
    //       128-EEA1: (1) Support
    //       128-EEA2: (1) Support
    //       EEA3: (0) Not support
    //       EEA4: (0) Not support
    //       EEA5: (0) Not support
    //       EEA6: (0) Not support
    //       EEA7: (0) Not support
    //       EIA0: (1) Support
    //       128-EIA1: (1) Support
    //       128-EIA2: (1) Support
    //       EIA3: (0) Not support
    //       EIA4: (0) Not support
    //       EIA5: (0) Not support
    //       EIA6: (0) Not support
    //       EIA7: (0) Not support
    //       UEA0: (1) Support
    //       UEA1: (1) Support
    //       UEA2: (0) Not support
    //       UEA3: (0) Not support
    //       UEA4: (0) Not support
    //       UEA5: (0) Not support
    //       UEA6: (0) Not support
    //       UEA7: (0) Not support
    //       UCS2 support (UCS2): (1) Support
    //       UMTS integrity algorithm UIA1: (1) Support
    //       UMTS integrity algorithm UIA2: (0) Not support
    //       UMTS integrity algorithm UIA3: (0) Not support
    //       UMTS integrity algorithm UIA4: (0) Not support
    //       UMTS integrity algorithm UIA5: (0) Not support
    //       UMTS integrity algorithm UIA6: (0) Not support
    //       UMTS integrity algorithm UIA7: (0) Not support
    //     ESM message container
    //       Length: 35 octets
    //       ESM message container contents: 2 1 d0 31 27 1d 80 80 21 10 1 1 0 10 81 6 0 0 0 0 83 6 0 0 0 0 0 3 0 0 3 0 0 a 0 
    //         0000 .... = EPS bearer identity: 0
    //         .... 0010 = protocol_discriminator: (2) EPS session management messages 
    //         Procedure transaction identity: 1
    //         NAS EPS session management messages: (0xd0) PDN connectivity request 
    //         PDN type: (3) IPv4v6 
    //         Request type: (1) Initial request 
    //         Protocol Configuration Options
    //           Element ID: 39 
    //           Length: 29 octets
    //           Ext: 1
    //           Configuration protocol: (0) PPP
    //           Protocol information:
    //             [0]:
    //               Protocol ID: IPCP (Hex 8021)
    //               Contents length: 16 octets
    //               PPP IP Control Protocol
    //                 Code:(1) Configuration Request 
    //                 Identifier: 1
    //                 Length: 16
    //                 Options: 12 bytes
    //                   Primary DNS server IP address: 0.0.0.0
    //                   Secondary DNS server IP address: 0.0.0.0
    //             [1]:
    //               Protocol ID: DNS Server IPv6 Address Request (Hex 3)
    //               Contents length: 0 octets
    //             [2]:
    //               Protocol ID: DNS Server IPv6 Address Request (Hex 3)
    //               Contents length: 0 octets
    //             [3]:
    //               Protocol ID: IP address allocation via NAS signalling (Hex A)
    //               Contents length: 0 octets
    //     DRX Parameter
    //       Element ID: 92 
    //       DRX Parameter
    //         Split PG Cycle Code: (32) 32
    //         Split on CCCH: (0) Split pg cycle on CCCH is supported by the mobile station
    //         Non-DRX timer: (0) no non-DRX mode after transfer state
    //         CN Specific DRX cycle length coefficient: (0) CN Specific DRX cycle length coefficient / value not specified by the MS
    //     Voice domain preference and UE's usage setting
    //       Element ID: 93 
    //       Length: 1 octets
    //       Length of Voice domain preference and UE's usage setting contents: 1
    //       Voice domain preference for E-UTRAN: (0) CS Voice only
    //       UE's usage setting: (1) Data centric        
    uint8 nasMsg[] = {
        0x07, 0x41, 0x71, 0x08, 0x49, 0x06, 0x00, 0x00, 0x65, 0x95, 
        0x70, 0x79, 0x04, 0xe0, 0xe0, 0xc0, 0xc0, 0x00, 0x23, 0x02, 
        0x01, 0xd0, 0x31, 0x27, 0x1d, 0x80, 0x80, 0x21, 0x10, 0x01, 
        0x01, 0x00, 0x10, 0x81, 0x06, 0x00, 0x00, 0x00, 0x00, 0x83, 
        0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 
        0x00, 0x00, 0x0a, 0x00, 0x5c, 0x20, 0x00, 0x5d, 0x01, 0x04
    };
    uint32 nasMsgLen = 60;

    uint8 imsi[15] = {4,6,0,0,0,0,0,5,6,5,9,0,7,9,7};

    LIBLTE_BYTE_MSG_STRUCT nasBuf;
    LIBLTE_MME_ATTACH_REQUEST_MSG_STRUCT attachReq;
    uint8 pd;
    uint8 msgType;
    LIBLTE_ERROR_ENUM ret;

    nasBuf.msg = (uint8*)nasMsg;
    nasBuf.N_bytes = nasMsgLen;
    liblte_mme_parse_msg_header(&nasBuf, &pd, &msgType);
    printf("NAS msgType = 0x%02x\n", msgType);
    EXPECT_EQ(LIBLTE_MME_MSG_TYPE_ATTACH_REQUEST, msgType);
    EXPECT_EQ(LIBLTE_MME_PD_EPS_MOBILITY_MANAGEMENT, pd);
    ret = liblte_mme_unpack_attach_request_msg(&nasBuf, &attachReq);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(1, attachReq.eps_attach_type);
    EXPECT_EQ(LIBLTE_MME_EPS_MOBILE_ID_TYPE_IMSI, attachReq.eps_mobile_id.type_of_id);
    cout << "Parsed IMSI: "; 
    for (int i=0; i<15; i++) {
        cout << (int)attachReq.eps_mobile_id.imsi[i];
    }
    cout << endl;  

    EXPECT_EQ(0, memcmp(imsi, attachReq.eps_mobile_id.imsi, 15));
}

TEST_F(TestAsn1, Decode_NAS_AttachReq_WithMTmsi) {
    // 07 41 01 0b f6 64 f0 00 03 62 
    // cc f2 01 90 c4 04 e0 e0 c0 c0 
    // 00 23 02 01 d0 31 27 1d 80 80 
    // 21 10 01 01 00 10 81 06 00 00 
    // 00 00 83 06 00 00 00 00 00 03 
    // 00 00 03 00 00 0a 00 52 64 f0 
    // 00 24 d1 5c 20 00 5d 01 04
    // Attach Request
    // Type: EPS MM
    // Direction: Uplink
    // Computer Timestamp: 20:03:18.575
    // UE Timestamp: 35771563 (ms)
    // Radio Technology: LTE
    //   Attach Request
    //     Security header type: (0) Plain NAS message, not security protected 
    //     protocol_discriminator: EPS Mobility Management
    //     NAS EPS Mobility Management Message Type: (0x41) Attach request 
    //     Type of security context flag (TSC): (0) Native security context 
    //     NAS key set identifier: (0) 
    //     Spare bit(s): 0 
    //     EPS attach type: (1) EPS attach 
    //     EPS mobile identity - Old GUTI or IMSI
    //       Length: 11 octets
    //       odd/even indic: 0
    //       Type of identity: (6) GUTI 
    //       Mobile Country Code (MCC): (460) China (People's Republic of) 
    //       Mobile Network Code (MNC): (00) China Mobile  
    //       MME Group ID: 866
    //       MME Code: 204
    //       M-TMSI: 0xf20190c4
    //     UE network capability
    //       Length: 4 octets
    //       EEA0: (1) Support
    //       128-EEA1: (1) Support
    //       128-EEA2: (1) Support
    //       EEA3: (0) Not support
    //       EEA4: (0) Not support
    //       EEA5: (0) Not support
    //       EEA6: (0) Not support
    //       EEA7: (0) Not support
    //       EIA0: (1) Support
    //       128-EIA1: (1) Support
    //       128-EIA2: (1) Support
    //       EIA3: (0) Not support
    //       EIA4: (0) Not support
    //       EIA5: (0) Not support
    //       EIA6: (0) Not support
    //       EIA7: (0) Not support
    //       UEA0: (1) Support
    //       UEA1: (1) Support
    //       UEA2: (0) Not support
    //       UEA3: (0) Not support
    //       UEA4: (0) Not support
    //       UEA5: (0) Not support
    //       UEA6: (0) Not support
    //       UEA7: (0) Not support
    //       UCS2 support (UCS2): (1) Support
    //       UMTS integrity algorithm UIA1: (1) Support
    //       UMTS integrity algorithm UIA2: (0) Not support
    //       UMTS integrity algorithm UIA3: (0) Not support
    //       UMTS integrity algorithm UIA4: (0) Not support
    //       UMTS integrity algorithm UIA5: (0) Not support
    //       UMTS integrity algorithm UIA6: (0) Not support
    //       UMTS integrity algorithm UIA7: (0) Not support
    //     ESM message container
    //       Length: 35 octets
    //       ESM message container contents: 2 1 d0 31 27 1d 80 80 21 10 1 1 0 10 81 6 0 0 0 0 83 6 0 0 0 0 0 3 0 0 3 0 0 a 0 
    //         0000 .... = EPS bearer identity: 0
    //         .... 0010 = protocol_discriminator: (2) EPS session management messages 
    //         Procedure transaction identity: 1
    //         NAS EPS session management messages: (0xd0) PDN connectivity request 
    //         PDN type: (3) IPv4v6 
    //         Request type: (1) Initial request 
    //         Protocol Configuration Options
    //           Element ID: 39 
    //           Length: 29 octets
    //           Ext: 1
    //           Configuration protocol: (0) PPP
    //           Protocol information:
    //             [0]:
    //               Protocol ID: IPCP (Hex 8021)
    //               Contents length: 16 octets
    //               PPP IP Control Protocol
    //                 Code:(1) Configuration Request 
    //                 Identifier: 1
    //                 Length: 16
    //                 Options: 12 bytes
    //                   Primary DNS server IP address: 0.0.0.0
    //                   Secondary DNS server IP address: 0.0.0.0
    //             [1]:
    //               Protocol ID: DNS Server IPv6 Address Request (Hex 3)
    //               Contents length: 0 octets
    //             [2]:
    //               Protocol ID: DNS Server IPv6 Address Request (Hex 3)
    //               Contents length: 0 octets
    //             [3]:
    //               Protocol ID: IP address allocation via NAS signalling (Hex A)
    //               Contents length: 0 octets
    //     Tracking area identity - Last visited registered TAI
    //       Element ID: 82 
    //       Mobile Country Code (MCC): (460) China (People's Republic of) 
    //       Mobile Network Code (MNC): (00) China Mobile  
    //       Tracking area code(TAC): 0x24d1
    //     DRX Parameter
    //       Element ID: 92 
    //       DRX Parameter
    //         Split PG Cycle Code: (32) 32
    //         Split on CCCH: (0) Split pg cycle on CCCH is supported by the mobile station
    //         Non-DRX timer: (0) no non-DRX mode after transfer state
    //         CN Specific DRX cycle length coefficient: (0) CN Specific DRX cycle length coefficient / value not specified by the MS
    //     Voice domain preference and UE's usage setting
    //       Element ID: 93 
    //       Length: 1 octets
    //       Length of Voice domain preference and UE's usage setting contents: 1
    //       Voice domain preference for E-UTRAN: (0) CS Voice only
    //       UE's usage setting: (1) Data centric     
    uint8 nasMsg[] = {
        0x07, 0x41, 0x01, 0x0b, 0xf6, 0x64, 0xf0, 0x00, 0x03, 0x62, 
        0xcc, 0xf2, 0x01, 0x90, 0xc4, 0x04, 0xe0, 0xe0, 0xc0, 0xc0, 
        0x00, 0x23, 0x02, 0x01, 0xd0, 0x31, 0x27, 0x1d, 0x80, 0x80, 
        0x21, 0x10, 0x01, 0x01, 0x00, 0x10, 0x81, 0x06, 0x00, 0x00, 
        0x00, 0x00, 0x83, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
        0x00, 0x00, 0x03, 0x00, 0x00, 0x0a, 0x00, 0x52, 0x64, 0xf0, 
        0x00, 0x24, 0xd1, 0x5c, 0x20, 0x00, 0x5d, 0x01, 0x04
    };
    uint32 nasMsgLen = 69;

    uint16 mcc = 460;
    uint16 mnc = 0;
    uint16 tac = 0x24d1;
    uint16 mmeGrpId = 866;
    uint8  mmeCode = 204;
    uint32 mTmsi = 0xf20190c4;    

    LIBLTE_BYTE_MSG_STRUCT nasBuf;
    LIBLTE_MME_ATTACH_REQUEST_MSG_STRUCT attachReq;
    uint8 pd;
    uint8 msgType;
    LIBLTE_ERROR_ENUM ret;

    nasBuf.msg = (uint8*)nasMsg;
    nasBuf.N_bytes = nasMsgLen;
    liblte_mme_parse_msg_header(&nasBuf, &pd, &msgType);
    printf("NAS msgType = 0x%02x\n", msgType);
    EXPECT_EQ(LIBLTE_MME_MSG_TYPE_ATTACH_REQUEST, msgType);
    EXPECT_EQ(LIBLTE_MME_PD_EPS_MOBILITY_MANAGEMENT, pd);
    ret = liblte_mme_unpack_attach_request_msg(&nasBuf, &attachReq);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(1, attachReq.eps_attach_type);
    EXPECT_EQ(LIBLTE_MME_EPS_MOBILE_ID_TYPE_GUTI, attachReq.eps_mobile_id.type_of_id);
    printf("Parsed GUTI: MCC(%d), MNC(%d), MME Group ID(%d), MME Code(%d), M-TMSI(0x%x)\n", 
        attachReq.eps_mobile_id.guti.mcc,
        attachReq.eps_mobile_id.guti.mnc,
        attachReq.eps_mobile_id.guti.mme_group_id,
        attachReq.eps_mobile_id.guti.mme_code,
        attachReq.eps_mobile_id.guti.m_tmsi);
    EXPECT_EQ(mcc, attachReq.eps_mobile_id.guti.mcc);
    EXPECT_EQ(mnc, attachReq.eps_mobile_id.guti.mnc);
    EXPECT_EQ(mmeGrpId, attachReq.eps_mobile_id.guti.mme_group_id);
    EXPECT_EQ(mmeCode, attachReq.eps_mobile_id.guti.mme_code);
    EXPECT_EQ(mTmsi, attachReq.eps_mobile_id.guti.m_tmsi);
    EXPECT_TRUE(attachReq.last_visited_registered_tai_present);
    printf("Parsed Last Visted TAI: MCC(%d), MNC(%d), TAC(0x%x)\n", 
        attachReq.last_visited_registered_tai.mcc,
        attachReq.last_visited_registered_tai.mnc,
        attachReq.last_visited_registered_tai.tac);
    EXPECT_EQ(mcc, attachReq.last_visited_registered_tai.mcc);
    EXPECT_EQ(mnc, attachReq.last_visited_registered_tai.mnc);
    EXPECT_EQ(tac, attachReq.last_visited_registered_tai.tac);
}

TEST_F(TestAsn1, Decode_PCCH_Paging) {
    // 40 0c cd 53 6d ae 20
    // Paging
    // Type: PCCH
    // Direction: Downlink
    // Computer Timestamp: 10:45:04.406
    // UE Timestamp: 3092777 (ms)
    // Radio Technology: LTE
    //   PCCH-Message
    //     message
    //       c1
    //         paging
    //           pagingRecordList
    //             PagingRecord
    //               ue-Identity
    //                 s-TMSI
    //                   mmec: 0xCC
    //                   m-TMSI: 0xD536DAE2
    //               cn-Domain: (0) ps    
    uint8 rrcMsgData[] = {0x40, 0x0c, 0xcd, 0x53, 0x6d, 0xae, 0x20};
    uint32 rrcMsgLen = 7;    
    uint32 rrcMsgBitLen = rrcMsgLen << 3;

    uint32 mTmsi = 0xD536DAE2;
    uint8  mmec = 0xCC;

    LIBLTE_BIT_MSG_STRUCT rrcMsgBitBuf;
    LIBLTE_RRC_PCCH_MSG_STRUCT pagingMsg;
    LIBLTE_ERROR_ENUM ret;

    uint32 pcchHeaderBitLen = 1;
    convert_bytes_to_bits_vector(rrcMsgBitBuf.msg, rrcMsgData, rrcMsgBitLen);
    rrcMsgBitBuf.msg += pcchHeaderBitLen;
    rrcMsgBitBuf.N_bits = rrcMsgBitLen - pcchHeaderBitLen;
    ret = liblte_rrc_unpack_paging_msg(&rrcMsgBitBuf, &pagingMsg);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_FALSE(pagingMsg.system_info_modification_present);
    EXPECT_FALSE(pagingMsg.etws_indication_present);
    EXPECT_FALSE(pagingMsg.non_crit_ext_present);
    EXPECT_TRUE(1 == pagingMsg.paging_record_list_size);
    EXPECT_EQ(LIBLTE_RRC_CN_DOMAIN_PS, pagingMsg.paging_record_list[0].cn_domain);
    EXPECT_EQ(LIBLTE_RRC_PAGING_UE_IDENTITY_TYPE_S_TMSI, pagingMsg.paging_record_list[0].ue_identity.ue_identity_type);
    printf("Parsed M-TMSI(0x%x), MMEC(0x%x)\n", pagingMsg.paging_record_list[0].ue_identity.s_tmsi.m_tmsi, pagingMsg.paging_record_list[0].ue_identity.s_tmsi.mmec);
    EXPECT_EQ(mTmsi, pagingMsg.paging_record_list[0].ue_identity.s_tmsi.m_tmsi);
    EXPECT_EQ(mmec, pagingMsg.paging_record_list[0].ue_identity.s_tmsi.mmec);
}

TEST_F(TestAsn1, Decode_ULCCCH_RrcConnReq) {
    // 53 28 53 ae 81 26
    // RRCConnectionRequest
    // Type: UL_CCCH
    // Direction: Uplink
    // Computer Timestamp: 11:48:29.947
    // UE Timestamp: 6898428 (ms)
    // Radio Technology: LTE
    //   UL-CCCH-Message
    //     message
    //       c1
    //         rrcConnectionRequest
    //           criticalExtensions
    //             rrcConnectionRequest-r8
    //               ue-Identity
    //                 randomValue: 0x32853AE812
    //               establishmentCause: (3) mo-Signalling
    //               spare: 0x0  
    uint8 rrcMsgData[] = {0x53, 0x28, 0x53, 0xae, 0x81, 0x26};
    uint32 rrcMsgLen = 6;    
    uint32 rrcMsgBitLen = rrcMsgLen << 3;

    uint64 randomValue = 0x32853AE812LL;

    LIBLTE_BYTE_MSG_STRUCT rrcMsgByteBuf;
    LIBLTE_BIT_MSG_STRUCT rrcMsgBitBuf;
    LIBLTE_RRC_CONNECTION_REQUEST_STRUCT rrcReqMsg;
    LIBLTE_ERROR_ENUM ret;
    uint8 ext;
    uint8 msgType;

    rrcMsgByteBuf.msg = rrcMsgData;
    rrcMsgByteBuf.N_bytes = rrcMsgLen;
    ret = liblte_rrc_parse_ul_ccch_msg_header(&rrcMsgByteBuf, &ext, &msgType);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    cout << "UL CCCH msgType = " << (uint32)msgType << endl;
    EXPECT_EQ(0, ext);
    EXPECT_EQ(LIBLTE_RRC_UL_CCCH_MSG_TYPE_RRC_CON_REQ, msgType);

    uint32 ulCcchHeaderBitLen = 2;
    convert_bytes_to_bits_vector(rrcMsgBitBuf.msg, rrcMsgData, rrcMsgBitLen);
    rrcMsgBitBuf.msg += ulCcchHeaderBitLen;
    rrcMsgBitBuf.N_bits = rrcMsgBitLen - ulCcchHeaderBitLen;
    ret = liblte_rrc_unpack_rrc_connection_request_msg(&rrcMsgBitBuf, &rrcReqMsg);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(LIBLTE_RRC_CON_REQ_EST_CAUSE_MO_SIGNALLING, rrcReqMsg.cause);
    EXPECT_EQ(LIBLTE_RRC_CON_REQ_UE_ID_TYPE_RANDOM_VALUE, rrcReqMsg.ue_id_type);
    cout << "Parsed random value = 0x" << hex << rrcReqMsg.ue_id.random << endl;
    EXPECT_EQ(randomValue, rrcReqMsg.ue_id.random);
}

TEST_F(TestAsn1, Decode_DLCCCH_RrcConnSetup) {
    // 68 12 98 09 fd d0 01 83 b0 99
    // 98 67 96 a4 b3 21 83 08 10 00
    // 4b 00
    // RRCConnectionSetup
    // Type: DL_CCCH
    // Direction: Downlink
    // Computer Timestamp: 11:48:29.997
    // UE Timestamp: 6898500 (ms)
    // Radio Technology: LTE
    //   DL-CCCH-Message
    //     message
    //       c1
    //         rrcConnectionSetup
    //           rrc-TransactionIdentifier: 1
    //           criticalExtensions
    //             c1
    //               rrcConnectionSetup-r8
    //                 radioResourceConfigDedicated
    //                   srb-ToAddModList
    //                     SRB-ToAddMod
    //                       srb-Identity: 1
    //                       rlc-Config
    //                         explicitValue
    //                           am
    //                             ul-AM-RLC
    //                               t-PollRetransmit: (9) ms50
    //                               pollPDU: (7) pInfinity
    //                               pollByte: (14) kBinfinity
    //                               maxRetxThreshold: (7) t32
    //                             dl-AM-RLC
    //                               t-Reordering: (8) ms40
    //                               t-StatusProhibit: (0) ms0
    //                       logicalChannelConfig
    //                         explicitValue
    //                           ul-SpecificParameters
    //                             priority: 1
    //                             prioritisedBitRate: (7) infinity
    //                             bucketSizeDuration: (3) ms300
    //                             logicalChannelGroup: 0
    //                   mac-MainConfig
    //                     explicitValue
    //                       ul-SCH-Config
    //                         maxHARQ-Tx: (3) n4
    //                         periodicBSR-Timer: (3) sf20
    //                         retxBSR-Timer: (0) sf320
    //                         ttiBundling: false
    //                       timeAlignmentTimerDedicated: (6) sf10240
    //                   physicalConfigDedicated
    //                     pdsch-ConfigDedicated
    //                       p-a: (2) dB-3
    //                     pucch-ConfigDedicated
    //                       ackNackRepetition: release
    //                       tdd-AckNackFeedbackMode: (0) bundling
    //                     pusch-ConfigDedicated
    //                       betaOffset-ACK-Index: 9
    //                       betaOffset-RI-Index: 6
    //                       betaOffset-CQI-Index: 6
    //                     uplinkPowerControlDedicated
    //                       p0-UE-PUSCH: 0
    //                       deltaMCS-Enabled: (0) en0
    //                       accumulationEnabled: true
    //                       p0-UE-PUCCH: 0
    //                       pSRS-Offset: 3
    //                       filterCoefficient: (4) fc4
    //                     cqi-ReportConfig
    //                       nomPDSCH-RS-EPRE-Offset: 0
    //                     antennaInfo
    //                       explicitValue
    //                         transmissionMode: (0) tm1
    //                         ue-TransmitAntennaSelection: release
    //                     schedulingRequestConfig
    //                       setup
    //                         sr-PUCCH-ResourceIndex: 0
    //                         sr-ConfigIndex: 37
    //                         dsr-TransMax: (4) n64    
    uint8 rrcMsgData[] = {
        0x68, 0x12, 0x98, 0x09, 0xfd, 0xd0, 0x01, 0x83, 0xb0, 0x99,
        0x98, 0x67, 0x96, 0xa4, 0xb3, 0x21, 0x83, 0x08, 0x10, 0x00,
        0x4b, 0x00
    };
    uint32 rrcMsgLen = 22;    
    uint32 rrcMsgBitLen = rrcMsgLen << 3;

    uint32 srPucchResIdx = 0;
    uint32 srConfigIdx = 37;

    LIBLTE_BYTE_MSG_STRUCT rrcMsgByteBuf;
    LIBLTE_BIT_MSG_STRUCT rrcMsgBitBuf;
    LIBLTE_RRC_CONNECTION_SETUP_STRUCT rrcSetupMsg;
    LIBLTE_ERROR_ENUM ret;
    uint8 ext;
    uint8 msgType;

    rrcMsgByteBuf.msg = rrcMsgData;
    rrcMsgByteBuf.N_bytes = rrcMsgLen;
    ret = liblte_rrc_parse_dl_ccch_msg_header(&rrcMsgByteBuf, &ext, &msgType);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    cout << "DL CCCH msgType = " << (uint32)msgType << endl;
    EXPECT_EQ(0, ext);
    EXPECT_EQ(LIBLTE_RRC_DL_CCCH_MSG_TYPE_RRC_CON_SETUP, msgType);

    uint32 dlCcchHeaderBitLen = 3;
    convert_bytes_to_bits_vector(rrcMsgBitBuf.msg, rrcMsgData, rrcMsgBitLen);
    rrcMsgBitBuf.msg += dlCcchHeaderBitLen;
    rrcMsgBitBuf.N_bits = rrcMsgBitLen - dlCcchHeaderBitLen;
    ret = liblte_rrc_unpack_rrc_connection_setup_msg(&rrcMsgBitBuf, &rrcSetupMsg);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_TRUE(rrcSetupMsg.rr_cnfg.phy_cnfg_ded_present);
    EXPECT_TRUE(rrcSetupMsg.rr_cnfg.phy_cnfg_ded.sched_request_cnfg_present);
    EXPECT_TRUE(rrcSetupMsg.rr_cnfg.phy_cnfg_ded.sched_request_cnfg.setup_present);
    EXPECT_EQ(srPucchResIdx, rrcSetupMsg.rr_cnfg.phy_cnfg_ded.sched_request_cnfg.sr_pucch_resource_idx);
    EXPECT_EQ(srConfigIdx, rrcSetupMsg.rr_cnfg.phy_cnfg_ded.sched_request_cnfg.sr_cnfg_idx);
    EXPECT_EQ(4, rrcSetupMsg.rr_cnfg.phy_cnfg_ded.sched_request_cnfg.dsr_trans_max);
}

TEST_F(TestAsn1, Decode_ULDCCH_RrcSetupCompl_NAS_AttachReq) {
    // 22 00 78 0e 82 e2 10 92 0c 00 
    // 00 cb 2a e0 f2 09 c1 c1 81 80 
    // 00 46 04 03 a0 62 4e 3b 01 00 
    // 42 20 02 02 00 21 02 0c 00 00 
    // 00 01 06 0c 00 00 00 00 00 06 
    // 00 00 06 00 00 14 00 b8 40 00
    // ba 02 08
    // RRCConnectionSetupComplete
    // Type: UL_DCCH
    // Direction: Uplink
    // Computer Timestamp: 11:48:29.998
    // UE Timestamp: 6898505 (ms)
    // Radio Technology: LTE
    //   UL-DCCH-Message
    //     message
    //       c1
    //         rrcConnectionSetupComplete
    //           rrc-TransactionIdentifier: 1
    //           criticalExtensions
    //             c1
    //               rrcConnectionSetupComplete-r8
    //                 selectedPLMN-Identity: 1
    //                 dedicatedInfoNAS: 0x07417108490600006595707904E0E0C0C000230201D031271D8080211001010010810600000000830600000000000300000300000A005C20005D0104
    //   Attach Request
    //     Security header type: (0) Plain NAS message, not security protected 
    //     protocol_discriminator: EPS Mobility Management
    //     NAS EPS Mobility Management Message Type: (0x41) Attach request 
    //     Type of security context flag (TSC): (0) Native security context 
    //     NAS key set identifier: (7) No key is available
    //     Spare bit(s): 0 
    //     EPS attach type: (1) EPS attach 
    //     EPS mobile identity - Old GUTI or IMSI
    //       Length: 8 octets
    //       odd/even indic: 1
    //       Type of identity: (1) IMSI 
    //       IMSI: 460000056590797
    //     UE network capability
    //       Length: 4 octets
    //       EEA0: (1) Support
    //       128-EEA1: (1) Support
    //       128-EEA2: (1) Support
    //       EEA3: (0) Not support
    //       EEA4: (0) Not support
    //       EEA5: (0) Not support
    //       EEA6: (0) Not support
    //       EEA7: (0) Not support
    //       EIA0: (1) Support
    //       128-EIA1: (1) Support
    //       128-EIA2: (1) Support
    //       EIA3: (0) Not support
    //       EIA4: (0) Not support
    //       EIA5: (0) Not support
    //       EIA6: (0) Not support
    //       EIA7: (0) Not support
    //       UEA0: (1) Support
    //       UEA1: (1) Support
    //       UEA2: (0) Not support
    //       UEA3: (0) Not support
    //       UEA4: (0) Not support
    //       UEA5: (0) Not support
    //       UEA6: (0) Not support
    //       UEA7: (0) Not support
    //       UCS2 support (UCS2): (1) Support
    //       UMTS integrity algorithm UIA1: (1) Support
    //       UMTS integrity algorithm UIA2: (0) Not support
    //       UMTS integrity algorithm UIA3: (0) Not support
    //       UMTS integrity algorithm UIA4: (0) Not support
    //       UMTS integrity algorithm UIA5: (0) Not support
    //       UMTS integrity algorithm UIA6: (0) Not support
    //       UMTS integrity algorithm UIA7: (0) Not support
    //     ESM message container
    //       Length: 35 octets
    //       ESM message container contents: 2 1 d0 31 27 1d 80 80 21 10 1 1 0 10 81 6 0 0 0 0 83 6 0 0 0 0 0 3 0 0 3 0 0 a 0 
    //         0000 .... = EPS bearer identity: 0
    //         .... 0010 = protocol_discriminator: (2) EPS session management messages 
    //         Procedure transaction identity: 1
    //         NAS EPS session management messages: (0xd0) PDN connectivity request 
    //         PDN type: (3) IPv4v6 
    //         Request type: (1) Initial request 
    //         Protocol Configuration Options
    //           Element ID: 39 
    //           Length: 29 octets
    //           Ext: 1
    //           Configuration protocol: (0) PPP
    //           Protocol information:
    //             [0]:
    //               Protocol ID: IPCP (Hex 8021)
    //               Contents length: 16 octets
    //               PPP IP Control Protocol
    //                 Code:(1) Configuration Request 
    //                 Identifier: 1
    //                 Length: 16
    //                 Options: 12 bytes
    //                   Primary DNS server IP address: 0.0.0.0
    //                   Secondary DNS server IP address: 0.0.0.0
    //             [1]:
    //               Protocol ID: DNS Server IPv6 Address Request (Hex 3)
    //               Contents length: 0 octets
    //             [2]:
    //               Protocol ID: DNS Server IPv6 Address Request (Hex 3)
    //               Contents length: 0 octets
    //             [3]:
    //               Protocol ID: IP address allocation via NAS signalling (Hex A)
    //               Contents length: 0 octets
    //     DRX Parameter
    //       Element ID: 92 
    //       DRX Parameter
    //         Split PG Cycle Code: (32) 32
    //         Split on CCCH: (0) Split pg cycle on CCCH is supported by the mobile station
    //         Non-DRX timer: (0) no non-DRX mode after transfer state
    //         CN Specific DRX cycle length coefficient: (0) CN Specific DRX cycle length coefficient / value not specified by the MS
    //     Voice domain preference and UE's usage setting
    //       Element ID: 93 
    //       Length: 1 octets
    //       Length of Voice domain preference and UE's usage setting contents: 1
    //       Voice domain preference for E-UTRAN: (0) CS Voice only
    //       UE's usage setting: (1) Data centric    
    uint8 rrcMsgData[] = {
        0x22, 0x00, 0x78, 0x0e, 0x82, 0xe2, 0x10, 0x92, 0x0c, 0x00, 
        0x00, 0xcb, 0x2a, 0xe0, 0xf2, 0x09, 0xc1, 0xc1, 0x81, 0x80, 
        0x00, 0x46, 0x04, 0x03, 0xa0, 0x62, 0x4e, 0x3b, 0x01, 0x00, 
        0x42, 0x20, 0x02, 0x02, 0x00, 0x21, 0x02, 0x0c, 0x00, 0x00, 
        0x00, 0x01, 0x06, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
        0x00, 0x00, 0x06, 0x00, 0x00, 0x14, 0x00, 0xb8, 0x40, 0x00,
        0xba, 0x02, 0x08
    };
    uint32 rrcMsgLen = 63;    
    uint32 rrcMsgBitLen = rrcMsgLen << 3;

    uint8 nasMsg[] = {
        0x07, 0x41, 0x71, 0x08, 0x49, 0x06, 0x00, 0x00, 0x65, 0x95, 
        0x70, 0x79, 0x04, 0xE0, 0xE0, 0xC0, 0xC0, 0x00, 0x23, 0x02, 
        0x01, 0xD0, 0x31, 0x27, 0x1D, 0x80, 0x80, 0x21, 0x10, 0x01, 
        0x01, 0x00, 0x10, 0x81, 0x06, 0x00, 0x00, 0x00, 0x00, 0x83,
        0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 
        0x00, 0x00, 0x0A, 0x00, 0x5C, 0x20, 0x00, 0x5D, 0x01, 0x04};
    uint32 nasMsgLen = 60;

    uint8 imsi[15] = {4,6,0,0,0,0,0,5,6,5,9,0,7,9,7};

    LIBLTE_BYTE_MSG_STRUCT rrcMsgByteBuf;
    LIBLTE_BIT_MSG_STRUCT rrcMsgBitBuf;
    LIBLTE_RRC_CONNECTION_SETUP_COMPLETE_STRUCT rrcSetupComplMsg;
    LIBLTE_ERROR_ENUM ret;
    uint8 ext;
    uint8 msgType;

    // only parse msg type
    rrcMsgByteBuf.msg = (uint8*)rrcMsgData;
    rrcMsgByteBuf.N_bytes = rrcMsgLen;
    ret = liblte_rrc_parse_dcch_msg_header(&rrcMsgByteBuf, &ext, &msgType);
    printf("UL DCCH msgType = %d\n", msgType);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(0, ext);
    EXPECT_EQ(LIBLTE_RRC_UL_DCCH_MSG_TYPE_RRC_CON_SETUP_COMPLETE, msgType);

    // parse the whole ul direct transfer msg
    uint32 dcchHeaderBitLen = 5;
    convert_bytes_to_bits_vector(rrcMsgBitBuf.msg, rrcMsgData, rrcMsgBitLen);
    rrcMsgBitBuf.msg += dcchHeaderBitLen;
    rrcMsgBitBuf.N_bits = rrcMsgBitLen - dcchHeaderBitLen;;
    ret = liblte_rrc_unpack_rrc_connection_setup_complete_msg(&rrcMsgBitBuf, &rrcSetupComplMsg);    
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_FALSE(rrcSetupComplMsg.registered_mme_present);
    EXPECT_TRUE(1 == rrcSetupComplMsg.selected_plmn_id);
    cout << "NAS message[" << dec << rrcSetupComplMsg.dedicated_info_nas.N_bytes << "]: " << endl;
    for (uint32 i=0; i<rrcSetupComplMsg.dedicated_info_nas.N_bytes; i++) {
        printf("%02x ", rrcSetupComplMsg.dedicated_info_nas.msg[i]);
    }
    printf("\n");    
    EXPECT_EQ(nasMsgLen, rrcSetupComplMsg.dedicated_info_nas.N_bytes);
    EXPECT_EQ(0, memcmp(nasMsg, rrcSetupComplMsg.dedicated_info_nas.msg, nasMsgLen));

    // continue to parse nas msg
    LIBLTE_BYTE_MSG_STRUCT nasBuf;
    LIBLTE_MME_ATTACH_REQUEST_MSG_STRUCT attachReq;
    uint8 pd;

    nasBuf.msg = (uint8*)nasMsg;
    nasBuf.N_bytes = nasMsgLen;
    liblte_mme_parse_msg_header(&nasBuf, &pd, &msgType);
    printf("NAS msgType = 0x%02x\n", msgType);
    EXPECT_EQ(LIBLTE_MME_MSG_TYPE_ATTACH_REQUEST, msgType);
    EXPECT_EQ(LIBLTE_MME_PD_EPS_MOBILITY_MANAGEMENT, pd);
    ret = liblte_mme_unpack_attach_request_msg(&nasBuf, &attachReq);
    EXPECT_EQ(LIBLTE_SUCCESS, ret);
    EXPECT_EQ(1, attachReq.eps_attach_type);
    EXPECT_EQ(LIBLTE_MME_EPS_MOBILE_ID_TYPE_IMSI, attachReq.eps_mobile_id.type_of_id);
    cout << "Parsed IMSI: "; 
    for (int i=0; i<15; i++) {
        cout << (int)attachReq.eps_mobile_id.imsi[i];
    }
    cout << endl;      
    EXPECT_EQ(0, memcmp(imsi, attachReq.eps_mobile_id.imsi, 15)); 
}
