/*
 * Copyright (C) 2013, 2014 Shanghai Stock Exchange (SSE), Shanghai, China
 * All Rights Reserved.
 */

/*
 * @file    stepMessage.h
 *
 * STEPЭ�鶨��ͷ�ļ�
 *
 * @version $Id
 * @since   2013/10/19
 * @author  Jin Chengxun
 *    
 */

/*
 MODIFICATION HISTORY:
 <pre>
 ================================================================================
 DD-MMM-YYYY INIT.    SIR    Modification Description
 ----------- -------- ------ ----------------------------------------------------
 19-OCT-2013 CXJIN           ����
 30-JUL-2014 ZHENGWU  #5011  ����ȫ�г�״̬��Ϣ
 11-AUG-2014 ZHENGWU  #5010  ����LFIXT�ỰЭ��淶����
 ================================================================================
  </pre>
*/

#ifndef EPS_STEP_MESSAGE_H
#define EPS_STEP_MESSAGE_H

/**
 * ����ͷ�ļ�
 */

#include "epsTypes.h"


#ifdef __cplusplus
extern "C" {
#endif

/*
 * STEP�ֶγ��ȶ���
 */
#define STEP_MSGTYPE_MAX_LEN                        16
#define STEP_COMPID_MAX_LEN                         32
#define STEP_MSGBODY_MAX_LEN                        8192
#define STEP_USERNAME_MAX_LEN                       32
#define STEP_PASSWORD_MAX_LEN                       32
#define STEP_DATE_LEN                               8
#define STEP_TIME_LEN                               8
#define STEP_DATETIME_LEN                           21
#define STEP_SECURITY_TYPE_LEN                      2
#define STEP_MD_UPDATETYPE_LEN                      3
#define STEP_MD_DATA_MAX_LEN                        4096
#define STEP_MSGENCODING_MAX_LEN                    16
#define STEP_CHECKSUM_LEN                           3
#define STEP_TEXT_MAX_LEN                           1024
#define STEP_TRADING_SESSION_ID_LEN                 8
#define STEP_TESTREQ_ID_MAX_LEN                     32
#define STEP_APPLVER_ID_MAX_LEN                     8
#define STEP_CSTM_APPLVER_ID_MAX_LEN                32

#define STEP_CHECKSUM_FIELD_LEN                     7
#define STEP_MSG_MAX_LEN                            4096        
#define STEP_MSG_MIN_LEN                            50
#define STEP_MD_MSG_WRAP_SIZE                       200         



/*
 * STEP��Ϣ����ֵ����
 */

#define STEP_MSGTYPE_HEARTBEAT_VALUE                "0"
#define STEP_MSGTYPE_LOGOUT_VALUE                   "5"
#define STEP_MSGTYPE_LOGON_VALUE                    "A"
#define STEP_MSGTYPE_MD_REQUEST_VALUE               "V"
#define STEP_MSGTYPE_MD_SNAPSHOT_VALUE              "W"
#define STEP_MSGTYPE_TRADING_STATUS_VALUE           "h"


/*
 * STEP�ֶ�TAG����
 */

#define STEP_BEGIN_SEQNO_TAG                        7
#define STEP_BEGIN_STRING_TAG                       8
#define STEP_BODY_LENGTH_TAG                        9
#define STEP_CHECKSUM_TAG                           10
#define STEP_END_SEQNO_TAG                          16
#define STEP_MSG_SEQ_NUM_TAG                        34
#define STEP_MSG_TYPE_TAG                           35
#define STEP_NEW_SEQNO_TAG                          36
#define STEP_POSSDUP_FLAG_TAG                       43
#define STEP_REF_SEQNUM_TAG                         45 
#define STEP_SENDER_COMP_ID_TAG                     49 
#define STEP_SENDING_TIME_TAG                       52 
#define STEP_TARGET_COMP_ID_TAG                     56 
#define STEP_TEXT_TAG                               58 
#define STEP_TRADE_DATE_TAG                         75 
#define STEP_RAWDATA_LENGTH_TAG                     95 
#define STEP_RAWDATA_TAG                            96
#define STEP_POSSRESEND_TAG                         97
#define STEP_ENCRYPT_METHOD_TAG                     98
#define STEP_HEARTBT_INT_TAG                        108
#define STEP_TESTREQ_ID_TAG                         112
#define STEP_GAPFILL_FLAG_TAG                       123
#define STEP_RESET_SEQNUM_FLAG_TAG                  141
#define STEP_SECURITY_TYPE_TAG                      167
#define STEP_MD_UPDATETYPE_TAG                      265
#define STEP_TRADING_SESSION_ID_TAG                 336
#define STEP_TRADE_SES_MODE_TAG                     339
#define STEP_MSG_ENCODING_TAG                       347
#define STEP_REFTAG_ID_TAG                          371
#define STEP_REFMSG_TYPE_TAG                        372
#define STEP_SESSION_REJECT_REASON_TAG              373
#define STEP_TOTNO_RELATEDSYM_TAG                   393
#define STEP_USERNAME_TAG                           553 
#define STEP_PASSWORD_TAG                           554
#define STEP_LAST_UPDATETIME_TAG                    779
#define STEP_NEXTEXPECTEDMSG_SEQNUM_TAG             789
#define STEP_DEFAULT_APPLVER_ID_TAG                 1137
#define STEP_APPL_ID_TAG                            1180
#define STEP_APPL_SEQ_NUM_TAG                       1181
#define STEP_DEFAULT_APPLEXT_ID_TAG                 1407
#define STEP_DEFAULT_CSTM_APPLVER_ID_TAG            1408
#define STEP_SESSION_STATUS_TAG                     1409
#define STEP_MD_COUNT_TAG                           5468

/*
 * STEP�ַ���������
 */
 
#define STEP_BEGIN_STRING_VALUE                     "FIXT.1.1"
#define STEP_SENDER_COMPID_VALUE                    "OEPS.1.1"
#define STEP_TARGET_COMPID_VALUE                    "EzEI.1.1"
#define STEP_MSG_ENCODING_VALUE                     "GBK"
#define STEP_DEF_APPLVER_ID_VALUE                   "9"
#define STEP_DEF_APPLEXT_ID_VALUE                   124

#define STEP_SECURITY_TYPE_ALL_VALUE                "00"
#define STEP_SECURITY_TYPE_STK_VALUE                "01"
#define STEP_SECURITY_TYPE_DEV_VALUE                "02"

#define STEP_INVALID_BOOLEAN_VALUE                  0x00
#define STEP_INVALID_STRING_VALUE                   ""
#define STEP_INVALID_INT_VALUE                      -1
#define STEP_INVALID_UINT_VALUE                     -1


/*
 * STEP��Ϣ����ö��
 */
typedef enum StepMsgTypeTag
{
    STEP_MSGTYPE_INVALID          = -1, /* ��Ч��Ϣ���� */             
    STEP_MSGTYPE_HEARTBEAT        = 0,  /* ������Ϣ, '0' */
    STEP_MSGTYPE_LOGOUT           = 1,  /* �ǳ���Ϣ, '5' */
    STEP_MSGTYPE_LOGON            = 2,  /* ��½��Ϣ, 'A' */
    STEP_MSGTYPE_MD_REQUEST       = 3,  /* ������Ϣ, 'V' */
    STEP_MSGTYPE_MD_SNAPSHOT      = 4,  /* ȫ��������Ϣ, 'W' */
    STEP_MSGTYPE_TRADING_STATUS   = 5,  /* �г�״̬��Ϣ, 'h' */
    STEP_MSGTYPE_COUNT
} StepMsgTypeT;

/*
 * STEP��Ϣ����ö��
 */
typedef enum StepEncryptMethodTag
{
    STEP_ENCRYPT_METHOD_NONE    = 0,    /* �޼��� */
} StepEncryptMethodT;


/*
 * STEP��Ϣ���䷽��ö��
 */
typedef enum StepDirectionTag
{
    STEP_DIRECTION_REQ          = 0,     /* ���󣬿ͻ��� -> ����� */
    STEP_DIRECTION_RSP          = 1,     /* Ӧ�𣬷���� -> �ͻ��� */
    STEP_DIRECTION_DAT          = 2,     /* ���ݣ������ -> �ͻ��� */
} StepDirectionT;

/*
 * STEP��Ϣ�ṹ
 */
typedef struct StepMessageTag
{
    StepMsgTypeT msgType;
    char    senderCompID[STEP_COMPID_MAX_LEN+1];
    char    targetCompID[STEP_COMPID_MAX_LEN+1];
    uint64  msgSeqNum;
    char    possDupFlag;
    char    possResend;
    char    sendingTime[STEP_DATETIME_LEN+1];
    char    msgEncoding[STEP_MSGENCODING_MAX_LEN+1];
    char    body[STEP_MSGBODY_MAX_LEN+1];
} StepMessageT;

/*
 * ������Ϣ��ṹ
 */
typedef struct HeartbeatRecordTag
{
    char    testReqID[STEP_TESTREQ_ID_MAX_LEN+1];
} HeartbeatRecordT;

/*
 * �ǳ���Ϣ��ṹ
 */
typedef struct LogoutRecordTag
{
    uint16  sessionStatus;
    char    text[STEP_TEXT_MAX_LEN+1];
} LogoutRecordT;

/*
 * ��½��Ϣ��ṹ
 */
typedef struct LogonRecordTag
{
    uint32  encryptMethod;
    uint32  heartBtInt;
    char    resetSeqNumFlag;
    uint64  nextExpectedMsgSeqNum;
    char    username[STEP_USERNAME_MAX_LEN+1];
    char    password[STEP_PASSWORD_MAX_LEN+1];
    char    defaultApplVerID[STEP_APPLVER_ID_MAX_LEN+1];
    uint32  defaultApplExtID;
    char    defaultCstmApplVerID[STEP_CSTM_APPLVER_ID_MAX_LEN+1];
} LogonRecordT;

/*
 * ���鶩����Ϣ��ṹ
 */
typedef struct MDRequestRecordTag
{
    char    securityType[STEP_SECURITY_TYPE_LEN+1];
} MDRequestRecordT;

/*
 * ȫ��������Ϣ��ṹ
 */
typedef struct MDSnapshotFullRefreshRecordTag
{
    char    securityType[STEP_SECURITY_TYPE_LEN+1];
    int16   tradSesMode;
    uint32  applID;
    uint64  applSeqNum;
    char    tradeDate[STEP_DATE_LEN+1];
    char    lastUpdateTime[STEP_TIME_LEN+1];
    char    mdUpdateType[STEP_MD_UPDATETYPE_LEN+1];
    uint32  mdCount;
    uint32  mdDataLen;
    char    mdData[STEP_MD_DATA_MAX_LEN+1];
} MDSnapshotFullRefreshRecordT;

/*
 * ȫ�г�״̬��Ϣ�ṹ
 */
typedef struct TradingStatusRecordTag
{
    char    securityType[STEP_SECURITY_TYPE_LEN+1];
    int16   tradSesMode;
    char    tradingSessionID[STEP_TRADING_SESSION_ID_LEN+1];
    uint32  totNoRelatedSym;
} TradingStatusRecordT;

#ifdef __cplusplus
}
#endif

#endif /* EPS_STEP_MESSAGE_H */
