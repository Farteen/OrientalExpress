/*
 * Express Project Studio, Shanghai Stock Exchange (SSE), Shanghai, China
 * All Rights Reserved.
 */

/**
 * @file    stepEncoder.c
 *
 * STEP������ʵ���ļ� 
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
 19-10-2013  CXJIN           ����
 30-JUL-2014 ZHENGWU  #5011  ����ȫ�г�״̬��Ϣ
 11-AUG-2014 ZHENGWU  #5010  ����LFIXT�ỰЭ��淶����
 ================================================================================
  </pre>
*/

/*
 * ����ͷ�ļ�
 */

#include "common.h"
#include "errlib.h"
#include "epsTypes.h"
#include "stepCodecUtil.h"

#include "stepCodec.h"

/*
 * ȫ�ֶ���
 */

/*
 * STEP��Ϣ���Ͷ�Ӧ��ϵ��
 */
static const char* STEP_MSGTYPE_MAP[] =
{
    STEP_MSGTYPE_HEARTBEAT_VALUE,
    STEP_MSGTYPE_LOGOUT_VALUE,
    STEP_MSGTYPE_LOGON_VALUE,
    STEP_MSGTYPE_MD_REQUEST_VALUE,
    STEP_MSGTYPE_MD_SNAPSHOT_VALUE,
    STEP_MSGTYPE_TRADING_STATUS_VALUE,
};


/*
 * �ڲ���������
 */

static ResCodeT EncodeHeartbeatRecord(HeartbeatRecordT* pRecord, 
        char* buf, int32 bufSize, int32* pEncodeSize);
static ResCodeT EncodeLogoutRecord(LogoutRecordT* pRecord,  
        char* buf, int32 bufSize, int32* pEncodeSize);
static ResCodeT EncodeLogonRecord(LogonRecordT* pRecord, 
        char* buf, int32 bufSize, int32* pEncodeSize);
static ResCodeT EncodeMDRequestRecord(MDRequestRecordT* pRecord, 
        char* buf, int32 bufSize, int32* pEncodeSize);
static ResCodeT EncodeMDSnapshotFullRefreshRecord(MDSnapshotFullRefreshRecordT* pRecord, 
        char* buf, int32 bufSize, int32* pEncodeSize);
static ResCodeT EncodeTradingStatusRecord(TradingStatusRecordT* pRecord, 
        char* buf, int32 bufSize, int32* pEncodeSize);
static ResCodeT EncodeStepMessageBody(StepMessageT* pMsg, 
        char* buf, int32 bufSize, int32* pEncodeSize);


/*
 * ����ʵ��
 */

/**
 * ����STEP��Ϣ
 *
 * @param   pMsg            in  - STEP��Ϣ
 * @param   buf             out - ���뻺����
 * @param   bufSize         in  - ���뻺��������
 * @param   pEncodeSize     out - ���볤��
 *
 * @return  �ɹ�����NO_ERR�����򷵻ش�����
 */
ResCodeT EncodeStepMessage(StepMessageT* pMsg, char* buf, int32 bufSize, int32* pEncodeSize)
{
    TRY
    {
        char body[STEP_MSG_MAX_LEN * 2];
        int32 bodySize = 0, encodeSize = 0;

        /* ������Ϣ�� */
        THROW_ERROR(EncodeStepMessageBody(pMsg, body, sizeof(body), &bodySize));

        /* ������Ϣͷ */
        THROW_ERROR(AddStringField(STEP_BEGIN_STRING_TAG, STEP_BEGIN_STRING_VALUE, buf, 
                bufSize, &encodeSize));

        THROW_ERROR(AddUint32Field(STEP_BODY_LENGTH_TAG, bodySize, buf, bufSize, 
                &encodeSize));

        /* Ԥ����BODY��CHECKSUM�ֶε�λ�� */
        if (encodeSize + bodySize + STEP_CHECKSUM_FIELD_LEN > bufSize)
        {   
            THROW_ERROR(ERCD_STEP_BUFFER_OVERFLOW);
        }

        memcpy(buf+encodeSize, body, bodySize);
        
        encodeSize += bodySize;

        /* ������Ϣβ */
        char checksum[STEP_CHECKSUM_LEN+1];
        THROW_ERROR(CalcChecksum(buf, encodeSize, checksum));
        THROW_ERROR(AddStringField(STEP_CHECKSUM_TAG, checksum, buf, bufSize, 
                &encodeSize));
       
        *pEncodeSize = encodeSize;
    }
    CATCH
    {
    }
    FINALLY
    {
        RETURN_RESCODE;
    }
}

/**
 * ����������Ϣ
 *
 * @param   pRecord         in  - ������Ϣ
 * @param   buf             in  - ���뻺����
 *                          out - �����Ļ�����
 * @param   bufSize         in  - ���뻺��������
 * @param   pEncodeSize     in  - ����ǰ�������ѱ��볤��
 *                          out - ����󻺳����ѱ��볤��
 *
 * @return  �ɹ�����NO_ERR�����򷵻ش�����
 */
ResCodeT EncodeHeartbeatRecord(HeartbeatRecordT* pRecord, char* buf, int32 bufSize, int32* pEncodeSize)
{
    TRY
    {
        int32 recordSize = 0;

        char* bufBegin    = buf + *pEncodeSize;
        int32 bufLeftSize = bufSize - *pEncodeSize;

        if (pRecord->testReqID[0] != 0x00)
        {
            THROW_ERROR(AddStringField(STEP_TESTREQ_ID_TAG, pRecord->testReqID, 
                bufBegin, bufLeftSize, &recordSize));
        }
    
        *pEncodeSize += recordSize;
    }
    CATCH
    {
    }
    FINALLY
    {
        RETURN_RESCODE;
    }
}

/*
 * ����ע����Ϣ
 *
 * @param   pRecord         in  - ע����Ϣ
 * @param   buf             in  - ���뻺����
 *                          out - �����Ļ�����
 * @param   bufSize         in  - ���뻺��������
 * @param   pEncodeSize     in  - ����ǰ�������ѱ��볤��
 *                          out - ����󻺳����ѱ��볤��
 *
 * @return  �ɹ�����NO_ERR�����򷵻ش�����
 */
ResCodeT EncodeLogoutRecord(LogoutRecordT* pRecord, char* buf, int32 bufSize, int32* pEncodeSize)
{
    TRY
    {
        int32 recordSize = 0;

        char* bufBegin    = buf + *pEncodeSize;
        int32 bufLeftSize = bufSize - *pEncodeSize;

        if (pRecord->sessionStatus != (uint16)STEP_INVALID_UINT_VALUE)
        {
            THROW_ERROR(AddUint16Field(STEP_SESSION_STATUS_TAG, pRecord->sessionStatus, 
                bufBegin, bufLeftSize, &recordSize));
        }

        if (pRecord->text[0] != 0x00)
        {
            THROW_ERROR(AddStringField(STEP_TEXT_TAG, pRecord->text, 
                bufBegin, bufLeftSize, &recordSize));
        }
        *pEncodeSize += recordSize;
    }
    CATCH
    {
    }
    FINALLY
    {
        RETURN_RESCODE;
    }
}

/*
 * �����½��Ϣ
 *
 * @param   buf             in  - ���뻺����
 *                          out - �����Ļ�����
 * @param   bufSize         in  - ���뻺��������
 * @param   pEncodeSize     in  - ����ǰ�������ѱ��볤��
 *                          out - ����󻺳����ѱ��볤��
 *
 * @return  �ɹ�����NO_ERR�����򷵻ش�����
 */
ResCodeT EncodeLogonRecord(LogonRecordT* pRecord, char* buf, int32 bufSize, int32* pEncodeSize)
{
    TRY
    {
        int32 recordSize = 0;

        char* bufBegin    = buf + *pEncodeSize;
        int32 bufLeftSize = bufSize - *pEncodeSize;

        THROW_ERROR(AddUint32Field(STEP_ENCRYPT_METHOD_TAG, pRecord->encryptMethod, 
                bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddUint32Field(STEP_HEARTBT_INT_TAG, pRecord->heartBtInt, 
                bufBegin, bufLeftSize, &recordSize));

        if (pRecord->resetSeqNumFlag != STEP_INVALID_BOOLEAN_VALUE)
        {
            THROW_ERROR(AddInt8Field(STEP_RESET_SEQNUM_FLAG_TAG, pRecord->resetSeqNumFlag, 
                bufBegin, bufLeftSize, &recordSize));
        }

        if (pRecord->nextExpectedMsgSeqNum != (uint64)STEP_INVALID_UINT_VALUE)
        {
            THROW_ERROR(AddUint64Field(STEP_NEXTEXPECTEDMSG_SEQNUM_TAG, pRecord->nextExpectedMsgSeqNum, 
                bufBegin, bufLeftSize, &recordSize));
        }

        THROW_ERROR(AddStringField(STEP_USERNAME_TAG, pRecord->username, 
                bufBegin, bufLeftSize, &recordSize));

        if (pRecord->password[0] != 0x00)
        {
            THROW_ERROR(AddStringField(STEP_PASSWORD_TAG, pRecord->password, 
                    bufBegin, bufLeftSize, &recordSize));
        }

        THROW_ERROR(AddStringField(STEP_DEFAULT_APPLVER_ID_TAG, pRecord->defaultApplVerID, 
                bufBegin, bufLeftSize, &recordSize));

        if (pRecord->defaultApplExtID != (uint32)STEP_INVALID_UINT_VALUE)
        {
            THROW_ERROR(AddUint32Field(STEP_DEFAULT_APPLEXT_ID_TAG, pRecord->defaultApplExtID, 
                bufBegin, bufLeftSize, &recordSize));
        }

        if (pRecord->defaultCstmApplVerID[0] != 0x00)
        {
            THROW_ERROR(AddStringField(STEP_DEFAULT_CSTM_APPLVER_ID_TAG, pRecord->defaultCstmApplVerID, 
                bufBegin, bufLeftSize, &recordSize));
        }

        *pEncodeSize += recordSize;
    }
    CATCH
    {
    }
    FINALLY
    {
        RETURN_RESCODE;
    }
}



/**
 * �������鶩����Ϣ
 *
 * @param   pRecord         in  - ���鶩����Ϣ
 * @param   buf             in  - ���뻺����
 *                          out - �����Ļ�����
 * @param   bufSize         in  - ���뻺��������
 * @param   pEncodeSize     in  - ����ǰ�������ѱ��볤��
 *                          out - ����󻺳����ѱ��볤��
 *
 * @return  �ɹ�����NO_ERR�����򷵻ش�����
 */
ResCodeT EncodeMDRequestRecord(MDRequestRecordT* pRecord, char* buf, int32 bufSize, int32* pEncodeSize)
{
    TRY
    {
        int32 recordSize = 0;

        char* bufBegin    = buf + *pEncodeSize;
        int32 bufLeftSize = bufSize - *pEncodeSize;

        THROW_ERROR(AddStringField(STEP_SECURITY_TYPE_TAG, pRecord->securityType, 
                bufBegin, bufLeftSize, &recordSize));

        *pEncodeSize += recordSize;
    }
    CATCH
    {
    }
    FINALLY
    {
        RETURN_RESCODE;
    }
}

/**
 * �����������
 *
 * @param   pRecord         in  - ���������Ϣ
 * @param   buf             in  - ���뻺����
 *                          out - �����Ļ�����
 * @param   bufSize         in  - ���뻺��������
 * @param   pEncodeSize     in  - ����ǰ�������ѱ��볤��
 *                          out - ����󻺳����ѱ��볤��
 *
 * @return  �ɹ�����NO_ERR�����򷵻ش�����
 */
static ResCodeT EncodeMDSnapshotFullRefreshRecord(MDSnapshotFullRefreshRecordT* pRecord, 
        char* buf, int32 bufSize, int32* pEncodeSize)
{
    TRY
    {
        int32 recordSize = 0;

        char* bufBegin    = buf + *pEncodeSize;
        int32 bufLeftSize = bufSize - *pEncodeSize;
        
        THROW_ERROR(AddStringField(STEP_SECURITY_TYPE_TAG, pRecord->securityType, 
                bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddInt16Field(STEP_TRADE_SES_MODE_TAG, pRecord->tradSesMode, 
                bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddUint32Field(STEP_APPL_ID_TAG, pRecord->applID, 
                 bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddUint64Field(STEP_APPL_SEQ_NUM_TAG, pRecord->applSeqNum, 
                 bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddStringField(STEP_TRADE_DATE_TAG, pRecord->tradeDate, 
                 bufBegin, bufLeftSize, &recordSize));

        if (pRecord->lastUpdateTime[0] != 0x00)
        {
            THROW_ERROR(AddStringField(STEP_LAST_UPDATETIME_TAG, pRecord->lastUpdateTime, 
                     bufBegin, bufLeftSize, &recordSize));
        }
        THROW_ERROR(AddStringField(STEP_MD_UPDATETYPE_TAG, pRecord->mdUpdateType, 
                 bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddUint32Field(STEP_MD_COUNT_TAG, pRecord->mdCount, 
                 bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddUint32Field(STEP_RAWDATA_LENGTH_TAG, pRecord->mdDataLen, 
                 bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddBinaryField(STEP_RAWDATA_TAG, pRecord->mdData, pRecord->mdDataLen, 
                 bufBegin, bufLeftSize, &recordSize));
        *pEncodeSize += recordSize;
    }
    CATCH
    {
    }
    FINALLY
    {
        RETURN_RESCODE;
    }
}

/**
 * �����г�״̬��Ϣ
 *
 * @param   pRecord         in  - �г�״̬��Ϣ
 * @param   buf             in  - ���뻺����
 *                          out - �����Ļ�����
 * @param   bufSize         in  - ���뻺��������
 * @param   pEncodeSize     in  - ����ǰ�������ѱ��볤��
 *                          out - ����󻺳����ѱ��볤��
 *
 * @return  �ɹ�����NO_ERR�����򷵻ش�����
 */
static ResCodeT EncodeTradingStatusRecord(TradingStatusRecordT* pRecord, 
        char* buf, int32 bufSize, int32* pEncodeSize)
{
    TRY
    {
        int32 recordSize = 0;

        char* bufBegin    = buf + *pEncodeSize;
        int32 bufLeftSize = bufSize - *pEncodeSize;
        
        THROW_ERROR(AddStringField(STEP_SECURITY_TYPE_TAG, pRecord->securityType, 
                bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddInt16Field(STEP_TRADE_SES_MODE_TAG, pRecord->tradSesMode, 
                bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddStringField(STEP_TRADING_SESSION_ID_TAG, pRecord->tradingSessionID, 
                 bufBegin, bufLeftSize, &recordSize));

        THROW_ERROR(AddUint32Field(STEP_TOTNO_RELATEDSYM_TAG, pRecord->totNoRelatedSym, 
                 bufBegin, bufLeftSize, &recordSize));

        *pEncodeSize += recordSize;
    }
    CATCH
    {
    }
    FINALLY
    {
        RETURN_RESCODE;
    }
}

/**
 * ����STEP��Ϣ��
 *
 * @param   pMsg            in  - STEP��Ϣ
 * @param   direction       in  - ��Ϣ����
 * @param   buf             in  - ���뻺����
 *                          out - �����Ļ�����
 * @param   bufSize         in  - ���뻺��������
 * @param   pEncodeSize     in  - ����ǰ�������ѱ��볤��
 *                          out - ����󻺳����ѱ��볤��
 *
 * @return  �ɹ�����NO_ERR�����򷵻ش�����
 */
static ResCodeT EncodeStepMessageBody(StepMessageT* pMsg, char* buf, int32 bufSize, int32* pEncodeSize)
{
    TRY
    {
        if (pMsg->msgType <= STEP_MSGTYPE_INVALID ||
            pMsg->msgType >= STEP_MSGTYPE_COUNT)
        {
            THROW_ERROR(ERCD_STEP_INVALID_MSGTYPE, pMsg->msgType);
        }
     
        int encodeSize = 0;
        
        THROW_ERROR(AddStringField(STEP_MSG_TYPE_TAG, STEP_MSGTYPE_MAP[pMsg->msgType], 
                (char*)buf, bufSize, &encodeSize));

        THROW_ERROR(AddStringField(STEP_SENDER_COMP_ID_TAG, pMsg->senderCompID, 
                (char*)buf, bufSize, &encodeSize));

        THROW_ERROR(AddStringField(STEP_TARGET_COMP_ID_TAG, pMsg->targetCompID, 
                (char*)buf, bufSize, &encodeSize));

        THROW_ERROR(AddUint64Field(STEP_MSG_SEQ_NUM_TAG, pMsg->msgSeqNum, 
                (char*)buf, bufSize, &encodeSize));

        if (pMsg->possDupFlag != STEP_INVALID_BOOLEAN_VALUE)
        {
            THROW_ERROR(AddUint8Field(STEP_POSSDUP_FLAG_TAG, pMsg->possDupFlag, 
                    (char*)buf, bufSize, &encodeSize));
        }

        if (pMsg->possResend != STEP_INVALID_BOOLEAN_VALUE)
        {
            THROW_ERROR(AddUint8Field(STEP_POSSRESEND_TAG, pMsg->possResend, 
                    (char*)buf, bufSize, &encodeSize));
        }
        THROW_ERROR(AddStringField(STEP_SENDING_TIME_TAG, pMsg->sendingTime, 
                (char*)buf, bufSize, &encodeSize));

        THROW_ERROR(AddStringField(STEP_MSG_ENCODING_TAG, pMsg->msgEncoding, 
                (char*)buf, bufSize, &encodeSize));

        switch(pMsg->msgType)
        {
            case STEP_MSGTYPE_HEARTBEAT:
            {
                THROW_ERROR(EncodeHeartbeatRecord((HeartbeatRecordT*)pMsg->body,
                        (char*)buf, bufSize, &encodeSize));
                break;
            }
            case STEP_MSGTYPE_LOGOUT:
            {
                THROW_ERROR(EncodeLogoutRecord((LogoutRecordT*)pMsg->body,
                        (char*)buf, bufSize, &encodeSize));
                break;
            }
            case STEP_MSGTYPE_LOGON:
            {
                THROW_ERROR(EncodeLogonRecord((LogonRecordT*)pMsg->body,
                        (char*)buf, bufSize, &encodeSize));
                break;
            }
            case STEP_MSGTYPE_MD_REQUEST:
            {
                THROW_ERROR(EncodeMDRequestRecord((MDRequestRecordT*)pMsg->body,
                        (char*)buf, bufSize, &encodeSize));
                break;
            }
            case STEP_MSGTYPE_MD_SNAPSHOT:
            {
                THROW_ERROR(EncodeMDSnapshotFullRefreshRecord((MDSnapshotFullRefreshRecordT*)pMsg->body, 
                        (char*)buf, bufSize, &encodeSize));
                break;
            }
            case STEP_MSGTYPE_TRADING_STATUS:
            {
                THROW_ERROR(EncodeTradingStatusRecord((TradingStatusRecordT*)pMsg->body, 
                        (char*)buf, bufSize, &encodeSize));
                break;
            }
            
            default:
                THROW_ERROR(ERCD_STEP_INVALID_MSGTYPE, pMsg->msgType);
                break;
        }

        *pEncodeSize = encodeSize;
    }
    CATCH
    {
    }
    FINALLY
    {
        RETURN_RESCODE;
    }
}
