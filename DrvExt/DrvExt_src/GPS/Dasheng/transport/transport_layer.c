#include "transport_layer.h"
#include "../utils/crc8bits.h"
#include "../hal/hal.h"
#include "rpc.h"
#include "../patch_reader/patch_reader.h"

#define THIS_DBGLVL         2       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          SampleDrv
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

GlIntU32 m_ulRetryCnt      = 0;
GlIntU8  m_ucUnackCount    = 0;
GlIntU8  m_ucTxSeqId       = 0;
GlIntU8  m_ucLastRxSeqId   = 0xFF;
GlIntU8  m_ucReliableSeqId = 0xFF;
GlIntU8  m_aucTxBuffer[2080] = {};
GlIntU32 m_ulByteCntSinceLastValidPacket = 0;
GlIntU32 m_uiParserState = WAIT_FOR_ESC_SOP;
GlIntU32 m_uiRxLen = 0;
GlIntU8  m_aucRxMessageBuf[1038] = {};
GlIntU8 m_ucLastAckSeqId = 0;
bool m_bOngoingSync = false;
bool m_bRemoteSyncComplete = false;
bool m_bGotVersion = false;
GlIntU8 m_ucReliableCrc = 0;
GlIntU16 m_usReliableLen = 0;
GlIntU8 m_ucTxReliableSeqId = 0;
GlIntU8 m_ucTxLastAckedSeqId = 0xFF;

GlIntU32 m_ulAsicVersion = 0;
GlIntU32 m_ulRomVersion = 0;
GlIntU32 m_ulPatchVersion = 0;

#define ASIC_VERSION_UNKNOWN        0xFFFFFFFF
#define ASIC_VERSION_4773_A3        0x47730A30
#define ASIC_VERSION_4773_A2        0x47730A20
#define ASIC_VERSION_4774_A1        0x004774A0
#define ASIC_VERSION_4774_A1_PATCH  0x004774A1
#define ASIC_VERSION_43018_A0       0x043018A0

#define ASIC_ROM_VERSION_4773_A3    233377
#define ASIC_ROM_VERSION_4773_A2    200533
#define ASIC_ROM_VERSION_4774_A1    223751
#define ASIC_ROM_VERSION_43018_A0   0


#define PE_ERR_NONE                    0
#define PE_CMD_FAILED                  1
#define PE_ERR_INVALID_CMD             2
#define PE_ERR_INVALID_PAYLOAD_LEN     3
#define PE_ERR_PAYLOAD_LEN_EXCEED_MAX  4
#define PE_ERR_INVALID_PAYLOAD         5

enum PE_STATE {
    PE_IDLE,
    PE_CMD_ID_RECV,
    PE_CMD_PAYLOAD,
    PE_CMD_PROCESS,
    PE_CMD_DONE,
};

GlIntU32  m_uiState = PE_IDLE;
GlIntU8   m_ucCmdId;

enum PE_CMD {
    PE_CMD_FIRST           = 0xF0,
    PE_CMD_MEM_MASK_WR     = 0xF0,
    PE_CMD_MEM_WR          = 0xF1,
    PE_CMD_MEM_CRC         = 0xF2,  // Decide later which is faster:
    PE_CMD_MEM_CHECKSUM    = 0xF3,  // CRC or CHECKSUM
    PE_CMD_SERVICE         = 0xF4,
    PE_CMD_MEM_MASK_VERIFY = 0xF5,
    PE_CMD_LAST
};

GlIntU16 m_usPayloadLen = 0;
GlIntU16 m_usPayloadIn  = 0;
bool     PatchCmdSent   = false;

#define MAX_PATCH_CMD_PAYLOAD  950

GlIntU8 m_ucPayload[MAX_PATCH_CMD_PAYLOAD] = {};

int StartRemoteSync(void)
{
    int rc = 0;
    int len = 0;
    GlIntU8 buffer[100];
    
    m_bOngoingSync = true;

    SendInternalSeqIdRequest();        
    //hal_usleep(500000);
    //DBG_ERR("1111!\n");

    len = ReadData(buffer, sizeof(buffer));

    //uart_write(buffer, sizeof(buffer));
    if (len > 0)
    {   //DBG_ERR("ParseIncomingData");
        ParseIncomingData(buffer, len);
        //ParseIncomingData(buffer, sizeof(buffer));
    }
    //DBG_ERR("2222!\n");
    if (m_bRemoteSyncComplete)
    {   //DBG_ERR("m_bRemoteSyncComplete");
        rc = 1;
    }
        
    return rc;
    
}

void SendInternalSeqIdRequest(void)
{
    m_ulRetryCnt++;
    GlIntU16 usFlags  = FLAG_INTERNAL_PACKET | FLAG_IGNORE_SEQID; 
    GlIntU8  ucBuf[2] = {INT_PKT_SEQID_REQUEST, (GlIntU8)(m_ulRetryCnt & 0xFF)};
    //DBG_ERR("3333!\n");
    BuildAndSendPacket(usFlags, ucBuf, sizeof(ucBuf), 0);
    //DBG_ERR("4444!\n");
}

extern bool datago;

GlIntU8 BuildAndSendPacket(GlIntU16 usFlags, GlIntU8 *pucData, GlIntU16 usSize, GlIntU8 ucReliableTxSeqId)
{
    GlIntU8 ucSizeMSB = (usSize >> 8) & 0xFF;
    GlIntU8 ucSizeLSB = usSize & 0xFF;
    GlIntU8 ucTxSeqId = m_ucTxSeqId++; 
    GlIntU8 ucTmp = 0;

    
    if (m_ucUnackCount > 0)
    {
        usFlags |= FLAG_PACKET_ACK;         // piggyback our ACK
    }

    if (ucSizeMSB > 0)
    {
        // if size is over 255B, then we need to mark the size_extension
        usFlags |= FLAG_SIZE_EXTENDED;
    }

    if (usFlags > 0xFF)
    {
        usFlags |= FLAG_EXTENDED; // if the flags are using more than 1B, we need to set the appropriate flag
    }

    GlIntU32 ulPacketSize = 0;
    
    Crc8Bits_Reset(0);

#if 0
    // Todo
    // update the flags
    if (m_otLastPLUpdatedStats.ulRxGarbageBytes != m_otCurrentStats.ulRxGarbageBytes)
    {
        usFlags |= FLAG_MSG_GARBAGE;
    }
    if (m_otLastPLUpdatedStats.ulRxPacketLost != m_otCurrentStats.ulRxPacketLost)
    {
        usFlags |= FLAG_MSG_LOST;
    }
#endif    


    // Header
    m_aucTxBuffer[ulPacketSize++] = ESCAPE_CHARACTER;
    m_aucTxBuffer[ulPacketSize++] = SOP_CHARACTER;
    ulPacketSize += EscapeData(&m_aucTxBuffer[ulPacketSize], &ucTxSeqId, 1);

    ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], &ucSizeLSB, 1);

    // first we encode the flags LSB (flags MSB is part of an extension)
    ucTmp = (GlIntU8)(usFlags & 0xFF);
    ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], &ucTmp, 1);
    
    // and then we encode the flags_details
    GlIntU32 i;
    for (i = 0; (i < 16 && usFlags != 0); i++)
    {
        GlIntU16 usFlagMask = (1 << i);
        GlIntU16 usFlagBit = usFlags & usFlagMask;
        GlIntU8  usDummy = 0x0;

        if (usFlagBit == 0)
        {
            continue;
        }

        if ( usFlagBit == FLAG_PACKET_ACK) // acknowledgement
        {
            ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], &m_ucLastRxSeqId, 1);
            m_ucUnackCount = 0;
        }
        else if (usFlagBit == FLAG_RELIABLE_PACKET) // This is a reliable packet. we need to provide the proper Ack
        {
            ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], &ucReliableTxSeqId, 1);
        }
        else if (usFlagBit == FLAG_RELIABLE_ACK)
        {
            ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], &m_ucReliableSeqId, 1);
        }
        else if (usFlagBit == FLAG_RELIABLE_NACK)
        {
            ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], &m_ucReliableSeqId, 1);
        }
        else if (usFlagBit == FLAG_MSG_LOST)
        {
            // Todo
#if 0
            // It cannot be more than 255 as we are acking every packet
            GlIntU8 ucNumPacketLost = (GlIntU8)(m_otCurrentStats.ulRxPacketLost - m_otLastPLUpdatedStats.ulRxPacketLost);
            m_otLastPLUpdatedStats.ulRxPacketLost = m_otCurrentStats.ulRxPacketLost;
            ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], ucNumPacketLost);
#endif            
        }
        else if (usFlagBit == FLAG_MSG_GARBAGE)
        {
            // Todo
#if 0
            GlIntU32 ulNumGarbageLost = m_otCurrentStats.ulRxGarbageBytes - m_otLastPLUpdatedStats.ulRxGarbageBytes;
            if (ulNumGarbageLost > 0xFF)
            {
                ulNumGarbageLost = 0xFF; // max we can send is 1B
            }
            m_otLastPLUpdatedStats.ulRxGarbageBytes += ulNumGarbageLost;

            ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], (GlIntU8)ulNumGarbageLost);
#endif
        }
        else if (usFlagBit == FLAG_SIZE_EXTENDED)
        {
            ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], &ucSizeMSB, 1);
        }
        else if (usFlagBit == FLAG_EXTENDED)
        {
            // encode the MSB of the 16bit flags
            ucTmp = (GlIntU8)((usFlags >> 8) & 0xFF);
            ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], &ucTmp, 1);
        }
        else if (usFlagBit == FLAG_INTERNAL_PACKET)
        {
            // we don't care about the details, but need to encode a dummy byte
            ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], &usDummy, 1);
        }
        else if (usFlagBit == FLAG_IGNORE_SEQID)
        {
            // we don't care about the details, but need to encode a dummy byte
            ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], &usDummy, 1);
        }
        else
        {
            break;
        }

        usFlags &= ~usFlagBit;
    }

    ulPacketSize += EscapeDataAndUpdateCrc(&m_aucTxBuffer[ulPacketSize], pucData, usSize);

    GlIntU8 ucCrc = Crc8Bits_GetState(0);
    ucCrc = ((ucCrc & 0x0F) << 4) | ((ucCrc & 0xF0) >> 4);

    ulPacketSize += EscapeData(&m_aucTxBuffer[ulPacketSize], &ucCrc, 1);

    m_aucTxBuffer[ulPacketSize++] = ESCAPE_CHARACTER;
    m_aucTxBuffer[ulPacketSize++] = EOP_CHARACTER;

    SendData(m_aucTxBuffer, ulPacketSize);
    //if(datago==TRUE){
    //for(i=0;i<ulPacketSize;i++)
    //{
    //    //DBG_ERR("value %x %d \r\n",m_aucTxBuffer[i],ulPacketSize);
    //}
    //}
    ////DBG_ERR("5555!\r\n");
    return ucTxSeqId;
    
    
}

GlIntU32 EscapeData(GlIntU8 *pucDestBuff, const GlIntU8 *pucSrcBuff, GlIntU16 usSize)
{
    GlIntU32 ulCnt = 0;
    GlIntU32 i;
    
    for (i = 0; i < usSize; i++)
    {
        if (pucSrcBuff[i] == ESCAPE_CHARACTER)
        {
            pucDestBuff[ulCnt++] = ESCAPE_CHARACTER;
            pucDestBuff[ulCnt++] = ESCAPED_ESCAPE_CHARACTER;
        }
        else if (pucSrcBuff[i] == XON_CHARACTER)
        {
            pucDestBuff[ulCnt++] = ESCAPE_CHARACTER;
            pucDestBuff[ulCnt++] = ESCAPED_XON_CHARACTER;
        }
        else if (pucSrcBuff[i] == XOFF_CHARACTER)
        {
            pucDestBuff[ulCnt++] = ESCAPE_CHARACTER;
            pucDestBuff[ulCnt++] = ESCAPED_XOFF_CHARACTER;
        }
        else // regular data
        {
            pucDestBuff[ulCnt++] = pucSrcBuff[i];
        }
    }
    return ulCnt;
}

GlIntU32 EscapeDataAndUpdateCrc(GlIntU8 *pucDestBuff, const GlIntU8 *pucSrcBuff, GlIntU16 usSize)
{
    Crc8Bits_Update(0, pucSrcBuff, usSize);
    return EscapeData(pucDestBuff, pucSrcBuff, usSize);
}

void ParseIncomingData(GlIntU8 *pucData, GlIntU16 usLen)
{
    GlIntU16 usIdx = 0;
    
    while (usIdx != usLen)
    {
        GlIntU8 ucData = pucData[usIdx++];
        m_ulByteCntSinceLastValidPacket++;

        if (ucData == XON_CHARACTER || ucData == XOFF_CHARACTER)
        {
            continue;
        }

        switch(m_uiParserState)
        {
            case WAIT_FOR_ESC_SOP:
            {
                if (ucData == ESCAPE_CHARACTER)
                {
                    m_uiParserState = WAIT_FOR_SOP;
                    // Todo
                    //m_otCurrentStats.ulRxGarbageBytes += (m_ulByteCntSinceLastValidPacket -1); // if we had only one byte, then there is no garbage, any extra is garbage
                    m_ulByteCntSinceLastValidPacket = 1;
                }
            }
            break;

            case WAIT_FOR_SOP:
            {
                if (ucData == SOP_CHARACTER)
                {
                    m_uiParserState = WAIT_FOR_MESSAGE_COMPLETE;
                    m_uiRxLen = 0;
                }
                else
                {
                    if (ucData != ESCAPE_CHARACTER)
                    {
                        m_uiParserState = WAIT_FOR_ESC_SOP;
                    }
                    else
                    {
                        // Todo
                        //m_otCurrentStats.ulRxGarbageBytes += 2;
                        m_ulByteCntSinceLastValidPacket = 1;
                    }

                }
            }
            break;

            case WAIT_FOR_MESSAGE_COMPLETE:
            {
                if (ucData == ESCAPE_CHARACTER)
                {
                    m_uiParserState = WAIT_FOR_EOP;
                }
                else if (m_uiRxLen < sizeof(m_aucRxMessageBuf))
                {
                    m_aucRxMessageBuf[m_uiRxLen++] = ucData;
                }
                else
                {
                    m_uiParserState = WAIT_FOR_ESC_SOP;
                }
            }
            break;

            case WAIT_FOR_EOP:
            {
                if (ucData == EOP_CHARACTER)
                {   //DBG_ERR("PacketReceived!\n");
                    if (PacketReceived())
                    {
                        m_ulByteCntSinceLastValidPacket = 0; // we had a valid packet, restart the cnt

                    }

                    m_uiParserState = WAIT_FOR_ESC_SOP;
                }
                else if (ucData == ESCAPED_ESCAPE_CHARACTER)
                {
                    if (m_uiRxLen < sizeof(m_aucRxMessageBuf))
                    {
                        m_aucRxMessageBuf[m_uiRxLen++] = ESCAPE_CHARACTER;
                        m_uiParserState = WAIT_FOR_MESSAGE_COMPLETE;
                    }
                    else
                    {
                        m_uiParserState = WAIT_FOR_ESC_SOP;
                    }
                }
                else if (ucData == ESCAPED_XON_CHARACTER)
                {
                    if (m_uiRxLen < sizeof(m_aucRxMessageBuf))
                    {
                        m_aucRxMessageBuf[m_uiRxLen++] = XON_CHARACTER;
                        m_uiParserState = WAIT_FOR_MESSAGE_COMPLETE;
                    }
                    else
                    {
                        m_uiParserState = WAIT_FOR_ESC_SOP;
                    }
                }
                else if (ucData == ESCAPED_XOFF_CHARACTER)
                {
                    if (m_uiRxLen < sizeof(m_aucRxMessageBuf))
                    {
                        m_aucRxMessageBuf[m_uiRxLen++] = XOFF_CHARACTER;
                        m_uiParserState = WAIT_FOR_MESSAGE_COMPLETE;
                    }
                    else
                    {
                        m_uiParserState = WAIT_FOR_ESC_SOP;
                    }
                }
                else if (ucData == SOP_CHARACTER)
                {
                    // we probably missed the ESC EOP, but we start receiving a new packet!
                    m_uiParserState = WAIT_FOR_MESSAGE_COMPLETE;
                    // init the parser!
                    m_uiRxLen = 0;
                    
                    // Todo
                    //m_otCurrentStats.ulRxGarbageBytes += (m_ulByteCntSinceLastValidPacket-2); // if we had only one byte, then there is no garbage, any extra is garbage
                    m_ulByteCntSinceLastValidPacket = 2;
                }
                else if (ucData == ESCAPE_CHARACTER)
                {
                     m_uiParserState = WAIT_FOR_SOP;
                }
                else
                {
                    m_uiParserState = WAIT_FOR_ESC_SOP;
                }
            }
            break;
        }
    }
}

int PacketReceived(void)
{   //DBG_ERR("PacketReceived!\n");
    Crc8Bits_Reset(0);
    
    if (m_uiRxLen >= 4) // minimum is seqId, payload size, flags, and Crc
    {//DBG_ERR("9111!\n");
        // let's compute CRC
        GlIntU8 ucCrc = Crc8Bits_Update(0, &m_aucRxMessageBuf[1], m_uiRxLen - 2); // CRC is not applied on itsefl, nor on the SeqId
        // CRC has its nibble inverted for stronger CRC (as CRC of a packet with itself is always 0, if EoP is not detected, taht always reset the CRC)
        ucCrc = ((ucCrc & 0x0F) << 4) | ((ucCrc & 0xF0) >> 4);
        if (ucCrc != m_aucRxMessageBuf[m_uiRxLen - 1]) // CRC is last
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

    // At that point we passed CRC check
    GlIntU8 *pucData = &m_aucRxMessageBuf[0];
    GlIntU16 usLen = m_uiRxLen-1;

    GlIntU8 ucSeqId = *pucData++; usLen--;
    GlIntU16 usPayloadSize = *pucData++; usLen--;
    GlIntU16 usFlags = *pucData++; usLen--;

    bool bReliablePacket = false;
    GlIntU8 ucReliableSeqId = 0;

    bool bReliableAckReceived = false;
    GlIntU8 ucReliableAckSeqId = 0;

    bool bReliableNackReceived = false;
    GlIntU8 ucReliableNackSeqId = 0;

    bool bAckReceived = false;

    bool bInternalPacket = false;
    bool bIgnoreSeqId = false;

    GlIntU16 usAckFlags = 0;
    
    GlIntU32 i;

    for (i = 0; (i < 16 && usFlags != 0 && usLen > 0); i++)
    {//DBG_ERR("9222!\n");
        GlIntU16 usFlagMask = (1 << i);
        GlIntU16 usFlagBit = (usFlags & usFlagMask);
        if (usFlagBit == 0)
        {
            continue;
        }

        usFlags &= ~usFlagBit; // clear the flag
        GlIntU8 ucFlagDetail = *pucData++; usLen--; // extract flag details


        if (usFlagBit == FLAG_PACKET_ACK) // acknowledgement
        {
            GlIntU8 ucReceivedAckSeqId = ucFlagDetail; // flag detail contain the acknowledged SeqId

            m_ucLastAckSeqId = ucReceivedAckSeqId; // update LastAckSeqId
            bAckReceived = true;
        }
        else if (usFlagBit == FLAG_RELIABLE_PACKET) // This is a reliable packet. we need to provide the proper Ack
        {
            ucReliableSeqId = ucFlagDetail;
            bReliablePacket = true;
        }
        else if (usFlagBit == FLAG_RELIABLE_ACK)
        {
            bReliableAckReceived = true;
            ucReliableAckSeqId = ucFlagDetail;
        }
        else if (usFlagBit == FLAG_RELIABLE_NACK)
        {
            bReliableNackReceived = true;
            ucReliableNackSeqId = ucFlagDetail;
        }
        else if (usFlagBit == FLAG_MSG_LOST)
        {
            //DBG_ERR("PacketReceived: FLAG_MSG_LOST++\n");
        }
        else if (usFlagBit == FLAG_MSG_GARBAGE)
        {
            //DBG_ERR("PacketReceived: FLAG_MSG_GARBAGE++\n");
        }
        else if (usFlagBit == FLAG_SIZE_EXTENDED)
        {
            usPayloadSize |= (ucFlagDetail << 8); // flag detail contains the MSB of the payload size
        }
        else if (usFlagBit == FLAG_EXTENDED)
        {
            // the flags are extended, which means that the details contains the MSB of the 16bit flags
            usFlags |= (ucFlagDetail << 8);
        }
        else if (usFlagBit == FLAG_INTERNAL_PACKET)
        {
            // dont' care about the detail
            bInternalPacket = true;
        }
        else if (usFlagBit == FLAG_IGNORE_SEQID)
        {
            // dont' care about the detail
            bIgnoreSeqId = true;
        }
        else
        {
            //DBG_ERR("PacketReceived: Don't process!\n");
            usFlags |= usFlagBit;  // we did not process the flag, just put it back
            // this is an error, so we can break now, as there is no point in continuing
            break;
        }
    }

    if (usFlags == 0 && // if flag is not garbage, it should all be consumed
        usPayloadSize == usLen) // remaining lenght of the buffer should be the payload size
    {//DBG_ERR("9333!\n");
        // at that point, we know we have a valid packet (at least it passed all our validity checks, so we are going to trust it
        GlIntU8 ucExpectedTxSeqId = ((m_ucLastRxSeqId + 1) & 0xFF);
        if (ucSeqId != ucExpectedTxSeqId && !bIgnoreSeqId && !m_bOngoingSync)
        {
            // Some packets were lost, jump in the RxSeqId
            // Todo
            //m_otCurrentStats.ulRxPacketLost += ((ucSeqId - ucExpectedTxSeqId)&0xFF);
        }
        m_ucLastRxSeqId = ucSeqId; // increase expected SeqId


        if (!bAckReceived || usLen > 0 )
        {
            // Todo
            //m_otCurrentStats.ulPacketReceived++;
            if (++m_ucUnackCount > 200)
            {
                usAckFlags |= FLAG_PACKET_ACK; // if this is a payload packet, we need to acknowledge it.
            }
        }
        else
        {
            // Todo
            //m_otCurrentStats.ulAckReceived++;
        }
        

        bool bProcessPacket = bReliablePacket || usLen > 0;
        if (bReliablePacket)
        {//DBG_ERR("9444!\n");
            // if this is a reliable message, we need to make sure we didn't received it before!
            // if we did, the Host probably didn't received the Ack, so let's just send the ack
            // Reliable seqId is not enough, so we also use CRC and Lenght to confirm this was the same message received!
            if (ucReliableSeqId == m_ucReliableSeqId && Crc8Bits_GetState(0) == m_ucReliableCrc && usLen == m_usReliableLen)
            {
                usAckFlags |= FLAG_PACKET_ACK | FLAG_RELIABLE_ACK; // already received that packet, remote TransportLayer probably lost the Acknowledgement, send it again
                bProcessPacket = false; // we should not process it again
            }
            else if (ucReliableSeqId == ((m_ucReliableSeqId + 1) & 0xFF))
            {
                // this is a valid message, just do nothing but update the reliable info. the message will be processed below
                usAckFlags |= FLAG_PACKET_ACK | FLAG_RELIABLE_ACK; // already received that packet, remote TransportLayer probably lost the Acknowledgement, send it again
                m_usReliableLen = usLen;
                m_ucReliableSeqId = ucReliableSeqId;
                m_ucReliableCrc = Crc8Bits_GetState(0);
                // Todo
                //m_otCurrentStats.ulReliablePacketReceived++;
            }
            else
            {
                usAckFlags |= FLAG_PACKET_ACK | FLAG_RELIABLE_NACK; // we received the wrong reliable SeqId
                bProcessPacket = false; // we cannot accept that packet
            }
        }        

        // we have all the flags we need, time to send the acknowledgement
        // Todo
#if 0
        if (usAckFlags != 0)
        {
            SendPacketAck(usAckFlags);
        }
#endif        

        if (bReliableAckReceived)
        {
            ReceivedReliableAck(ucReliableAckSeqId);
        }

        // Todo
#if 0
        if (bReliableNackReceived)
        {
            ReceivedReliableNack(ucReliableNackSeqId, m_ucLastAckSeqId);
        }
#endif

        if (bProcessPacket)
        {//DBG_ERR("9555!\n");
            if (bInternalPacket)
            {   //DBG_ERR("PacketReceived!\n");
                OnInternalPacket(pucData, usLen);
            }
            else
            {//DBG_ERR("9666!\n");
                // everything good, notify upper layer that we have the payload of a packet available
                // Todo
                OnPacketReceived(pucData, usLen);
            }
        }
        return true;
    }
    else
    {//DBG_ERR("9777!\n");
        return false;
    }    
}

void OnInternalPacket(GlIntU8 *pucData, GlIntU16 usSize)
{
    GlIntU8 ucId = *pucData++; usSize--;
    //DBG_ERR("OnInternalPacket!\n");
    switch (ucId)
    {
        case INT_PKT_SEQID_REQUEST:
        {
            //GlIntU8 ucReqId = *pucData++; usSize--;
            // Todo
            //OnInternalSeqIdRequest(ucReqId);
        }
        break;

        case INT_PKT_SEQID_RESPONSE:
        {   
            GlIntU8 ucReqId = *pucData++;
            GlIntU8 ucLastRxSeqId = *pucData++;
            GlIntU8 ucLastAckSeqId = *pucData++;
            GlIntU8 ucReliableSeqId = *pucData++;
            GlIntU8 ucTxSeqId = *pucData++;
            GlIntU8 ucTxReliableSeqId = *pucData++;
            GlIntU8 ucTxLastAckedSeqId = *pucData++;
            
            OnInternalSeqIdResponse(ucReqId, ucLastRxSeqId, ucLastAckSeqId, ucReliableSeqId, ucTxSeqId, ucTxReliableSeqId, ucTxLastAckedSeqId);
        }
        break;

        case INT_PKT_STATS_REQUEST:
        {
            // Todo
            //OnInternalStatsRequest();
        }
        break;

        case INT_PKT_STATS_RESPONSE:
        {
            // Todo
            /*
            StreamDecoder otStream(pucData, usSize);

            stTransportLayerStats otStats;
            otStats.ulRxGarbageBytes = otStream.GetU32();
            otStats.ulRxPacketLost = otStream.GetU32();  
            otStats.ulRemotePacketLost = otStream.GetU32();
            otStats.ulRemoteGarbage = otStream.GetU32();   
            otStats.ulPacketSent = otStream.GetU32();
            otStats.ulPacketReceived = otStream.GetU32();
            otStats.ulAckReceived = otStream.GetU32();
            otStats.ulReliablePacketSent = otStream.GetU32(); 
            otStats.ulReliableRetransmit = otStream.GetU32();
            otStats.ulReliablePacketReceived = otStream.GetU32();
            otStats.ulMaxRetransmitCount = otStream.GetU32();
            
            
            OnInternalStatsResponse(otStats);
            */
        }
        break;

        case INT_PKT_SETTINGS_REQUEST:
        {
            // Todo
            //ASSERT_RET(usSize == 0);
            //OnInternalSettingsRequest();
        }
        break;

        case INT_PKT_SETTINGS_RESPONSE:
        {
            // Todo
            /*
            StreamDecoder otStream(pucData, usSize);
            
            GlIntU16 usMaxOutgoingPacketSize = otStream.GetU16();
            GlIntU16 usMaxIncomingPacketSize = otStream.GetU16();
            GlIntU16 usReliableRetryTimeoutMs = otStream.GetU16();
            GlIntU16 usReliableMaxRetry = otStream.GetU16();
            GlIntU16 usReliableMaxPackets = otStream.GetU16();

            ASSERT_RET(!otStream.Fail() && otStream.GetAvailableSize() == 0);
            OnInternalSettingsResponse(usMaxOutgoingPacketSize, usMaxIncomingPacketSize, usReliableRetryTimeoutMs, 
                                        usReliableMaxRetry, usReliableMaxPackets);
                                        */
        }
        break;

        default:
            break;
    }
}

void OnInternalSeqIdResponse(GlIntU8 ucReqId, GlIntU8 ucLastRxSeqId, GlIntU8 ucLastAckSeqId, GlIntU8 ucReliableSeqId, 
                             GlIntU8 ucTxSeqId, GlIntU8 ucTxReliableSeqId, GlIntU8 ucTxLastAckedSeqId)
{
    // is there ongoing rquest, and is the reqId matching?
    if (m_bOngoingSync && ucReqId == m_ulRetryCnt)
    {   //DBG_ERR("OnInternalSeqIdResponse!\n");
        // Great, let's set our seqId appropriately, so we can start communicating with the remote side, and not have any garbage detected

        // +1 because the next TxSeqId is expected to be one more than last received.
        // other +1 because this response triggered an ACK to be sent to the remote party, so it indeed already have incremented its seqId
        m_ucTxSeqId = ((ucLastRxSeqId + 1 + 1) & 0xFF); 
        m_ucTxReliableSeqId = ((ucReliableSeqId + 1) & 0xFF);
        m_ucTxLastAckedSeqId = ((m_ucTxReliableSeqId - 1) & 0xFF); // last acked should be -1
        m_ucLastRxSeqId = ((ucTxSeqId) & 0xFF);
        m_ucReliableSeqId = ((ucTxReliableSeqId - 1) & 0xFF);


        // let's clean the Reliable state
        m_ucReliableCrc = 0;
        m_usReliableLen = 0;

        m_bOngoingSync = false;   
        m_bRemoteSyncComplete = true;     
    }

}

int RequestVersion(void)
{
    int len = 0;
    int len2 = 0;
    int rc = 0;
    GlIntU8 buffer[100];
    
    m_bGotVersion = false;

    SendVersionRequest();    
    hal_usleep(500000);
    
    len = ReadData(buffer, sizeof(buffer));    
    len2 = ReadData(buffer+11, sizeof(buffer));    
    if (len > 0)
    {
        ParseIncomingData(buffer, len+len2);
    }
    
    if (m_bGotVersion)
    {
        rc = 1;
    }
    
    return rc;
}

void SendVersionRequest(void)
{
    GlIntU8  ucBuf[2] = {0x00, 0x00};
    
    m_ucTxReliableSeqId = ((m_ucTxReliableSeqId + 1) & 0xFF);
    BuildAndSendPacket(FLAG_RELIABLE_PACKET, ucBuf, sizeof(ucBuf), 0);
}

void ReceivedReliableAck(GlIntU8 ucAckedReliableSeqId)
{
    m_ucTxLastAckedSeqId = ucAckedReliableSeqId;
}

void OnPacketReceived(GlIntU8 *pucData, GlIntU16 usSize)
{
    if (CheckPacketSanity(pucData, usSize))
    {
        GlIntS32 lSize = (GlIntS32)usSize;

        while (lSize > 0)
        {
            GlIntU16 usRpcId = *pucData++; lSize--;
            if (usRpcId & 0x80)
            {
                usRpcId &= ~0x80;
                usRpcId <<= 8;
                usRpcId |= *pucData++; lSize--;
            }

            GlIntU16 usRpcLen = *pucData++; lSize--;
            if (usRpcLen & 0x80)
            {
                usRpcLen &= ~0x80;
                usRpcLen <<= 8;
                usRpcLen |= *pucData++; lSize--;
            }

            OnRpcReceived(usRpcId, pucData, usRpcLen);

            pucData += usRpcLen;
            lSize -= usRpcLen;
        }
    }
}

bool CheckPacketSanity(GlIntU8 *pucData, GlIntU16 usSize)
{
    GlIntS32 lSize = (GlIntS32) usSize;
    
    while (lSize > 0)
    {
        GlIntU16 usRpcId = *pucData++; lSize--;
        if (usRpcId & 0x80)
        {
            usRpcId &= ~0x80;
            usRpcId <<= 8;
            usRpcId |= *pucData++; lSize--;
        }

        GlIntU16 usRpcLen = *pucData++; lSize--;
        if (usRpcLen&0x80)
        {
            usRpcLen &= ~0x80;
            usRpcLen <<= 8;
            usRpcLen |= *pucData++; lSize--;
        }

        pucData += usRpcLen;
        lSize -= usRpcLen;
    }

    return lSize == 0;
}

void OnRpcReceived(GlIntU16 usRpcId, GlIntU8 *pRpcPayload, GlIntU16 usRpcLen)
{
    switch(usRpcId)
    {
        case RPC_DEFINITION(IRpcGlobalResponse, VersionResponse):
        {
              GlIntU32 ulAsicVersion  = GetU32(pRpcPayload);
            GlIntU32 ulRomVersion   = GetU32(pRpcPayload + 4);
            GlIntU32 ulPatchVersion = GetU32(pRpcPayload + 8);
            VersionResponse(ulAsicVersion, ulRomVersion, ulPatchVersion);
        }
        break;
        
        default:
        {
            //DBG_ERR("Unsupported RPC!\n");
        }
        break;
    }
    
}


void VersionResponse(GlIntU32 ulAsicVersion, GlIntU32 ulRomVersion, GlIntU32 ulPatchVersion)
{   
    m_ulRomVersion   = ulRomVersion;
    m_ulPatchVersion = ulPatchVersion;
    m_ulAsicVersion  = ulAsicVersion;
    
    m_bGotVersion = true;
}


GlIntU32 GetU32(GlIntU8 *pRpcPayload)
{
    GlIntU32 ulValue = 0;
    ulValue = *pRpcPayload++;
    ulValue |= *pRpcPayload++ << 8;
    ulValue |= *pRpcPayload++ << 16;
    ulValue |= *pRpcPayload++ << 24;

    return ulValue;
}

bool CheckPatch(void)
{
#if 0
    GlIntU32 ulAsicVersion = GetAsicVersion();
    bool asic_match = (ulAsicVersion == m_ulAsicVersion || (m_ulAsicVersion ==  ASIC_VERSION_4774_A1 && ulAsicVersion == ASIC_VERSION_4774_A1_PATCH));
    
    bool bRomVersionOk = CheckRomVersion();
    
    
    //Neil*************************************
    GlIntU32 ulPatchLevel = GetPatchLevel();
    bool pl_match = (ulPatchLevel  == m_ulPatchVersion);
    
    bool blob_ok = (GetBlobLength() > 0);
#else
    GetAsicVersion();
    CheckRomVersion();
    GetPatchLevel();
    GetBlobLength();
#endif
    
    return TRUE;
}

bool CheckRomVersion(void)
{
    bool bRomVersionOk = false;

    if (m_ulAsicVersion == ASIC_VERSION_4773_A3)
    {
        bRomVersionOk = m_ulRomVersion == ASIC_ROM_VERSION_4773_A3;
    }
    if (m_ulAsicVersion == ASIC_VERSION_4773_A2)
    {
        bRomVersionOk = m_ulRomVersion == ASIC_ROM_VERSION_4773_A2;
    } 
    if (m_ulAsicVersion == ASIC_VERSION_4774_A1 && !bRomVersionOk) // due to bug where A1 and A0 asic version are the same
    {
        bRomVersionOk = m_ulRomVersion == ASIC_ROM_VERSION_4774_A1;
    }
    if (m_ulAsicVersion == ASIC_VERSION_43018_A0)
    {
        bRomVersionOk = m_ulRomVersion == ASIC_ROM_VERSION_43018_A0;
    }
    return bRomVersionOk;
}

bool InitiatePatchDownload(void)
{  
    GlIntU8 aTmp[512];
    GlIntU16 len = 0;
    
    while(1)
    {
        len = GetBlob(aTmp, sizeof(aTmp));
        //DBG_DUMP("len size=%d \r\n",len);
        if (len > 0)
        {
            OnDataPush(aTmp, len);
            
            if (!PatchCmdSent)
            {
                continue;
            }
            else
            {
                PatchCmdSent = false;
            }
        }
        
        if (len < sizeof(aTmp))
        {
            ClosePatchFile();
            break;
        }
    }
    
    GlIntU32 ulPatchAddress = GetPatchAddress();
    bool bJumpToScratchApp = (ulPatchAddress != 0xFFFFFFFF);
    GlIntU32 ulType = GetType();
    
    if (bJumpToScratchApp)
    {
        if (ulType == (GlIntU32)'A') //APP? An APP is a full scratch app and we reboot to the start address
        {
            // Todo: do something here is necessary.
        }
        else
        {
            StartServiceAPP();
        }
    }
    
    return true;
}

GlIntU8 OnDataPush(GlIntU8 *pucData, GlIntU16 usSize)
{
    GlIntU16 i = 0;
  
    while (i < usSize)
    {
        switch(m_uiState)
        {
            case PE_IDLE: // Search for cmd ID
            {
                m_ucCmdId = *pucData++;
                i++;
                if (IsPECmd(m_ucCmdId))
                {
                    m_uiState = PE_CMD_ID_RECV;
                    m_usPayloadLen = (GlIntU16)(-1);
                    m_usPayloadIn = 0;
                }
                else
                {
                    m_ucCmdId = 0;
                    return PE_ERR_INVALID_CMD;
                }
            }
                break;

            case PE_CMD_ID_RECV: // Search for payload length
            {
                if (m_usPayloadLen == (GlIntU16)(-1)) // first byte of payload length
                {
                    if ((usSize - i) >= 2)
                    {
                        m_usPayloadLen = *pucData++;
                        m_usPayloadLen |= (*pucData) << 8;
                        pucData++;
                        i += 2;
                        m_uiState = PE_CMD_PAYLOAD;
                    }
                    else if ((usSize - i) == 1)
                    {
                        m_usPayloadLen = *pucData++;
                        i++;
                    }
                }
                else
                {
                    m_usPayloadLen +=  (*pucData << 8);
                    pucData++;
                    i++;
                    m_uiState = PE_CMD_PAYLOAD;
                }
                
                if (m_usPayloadLen > MAX_PATCH_CMD_PAYLOAD)
                {
                    m_ucCmdId = 0;
                    m_uiState = PE_IDLE; // restart cmd parser from beginning.
                    return PE_ERR_PAYLOAD_LEN_EXCEED_MAX;
                }
            }
                break;

            case PE_CMD_PAYLOAD: // Wait for command payload
            {
                for(;;)
                {
                    GlIntU16 usPayloadRemain = m_usPayloadLen - m_usPayloadIn; 
                    GlIntU16 usLen = MIN(usPayloadRemain, (usSize - i));
                    if (usLen)
                    {
                        memcpy(m_ucPayload + m_usPayloadIn, pucData, usLen);
                        m_usPayloadIn += usLen;
                        pucData += usLen;
                        i += usLen;
                    }
                    else
                    {
                        break;
                    }
                }

                if (m_usPayloadIn == m_usPayloadLen) // Got whole command, it's time to process it.
                {
                    //PE_ERROR err = OnCmdReceive();
                    OnCmdReceive();
                    m_uiState = PE_IDLE; // restart cmd parser from beginning.
                    /*
                    if (err)
                    {
                        return err;
                    }
                    */
                }
            }
                break;
        }
    }
    
    return PE_ERR_NONE;
}

bool IsPECmd(GlIntU8 ucCmd)
{
    return (ucCmd >= PE_CMD_FIRST && ucCmd < PE_CMD_LAST);
}

GlIntU8 OnCmdReceive(void)
{
    GlIntU8 err = PE_ERR_NONE;
    
    switch (m_ucCmdId)
    {
        case PE_CMD_MEM_WR:
        {
            if (m_usPayloadLen > 4)
            {
                GlIntU32 ulAddr  = GetU32(&m_ucPayload[0]);
                GlIntU16 usWrLen = m_usPayloadLen - 4;
                GlIntU16 usRPCLen = sizeof(ulAddr) + sizeof(usWrLen) + (m_usPayloadLen - 4);
                GlIntU8  ucBuf[1024] = {(RPC_DEFINITION(IRpcMemoryAccess, MemoryWrite) & 0xFF), };
                int i = 1;
                int j;
                
                PatchCmdSent = true;
                
                if (usRPCLen >= 0x80)
                {
                    ucBuf[i++] = ((usRPCLen >> 8 ) & 0xFF) | 0x80;
                }
                ucBuf[i++] = (usRPCLen & 0xFF);
                j = i;
                memcpy(&ucBuf[i], &ulAddr, sizeof(ulAddr));
                i += sizeof(ulAddr);
                memcpy(&ucBuf[i], &usWrLen, sizeof(usWrLen));
                i += sizeof(usWrLen);
                memcpy(&ucBuf[i], &m_ucPayload[4], usWrLen);
                
                BuildAndSendPacket(FLAG_RELIABLE_PACKET, &ucBuf[0], (usRPCLen + j), m_ucTxReliableSeqId);
                m_ucTxReliableSeqId = ((m_ucTxReliableSeqId + 1) & 0xFF);
            }
            else
            {
                err = PE_ERR_INVALID_PAYLOAD_LEN;
            }
        }
            break;
        
        default:
        {
            //DBG_ERR("Unsupported patch command!\n");
        }
            break;
    }

    return PE_ERR_NONE;
}

void StartServiceAPP(void)
{
    GlIntU8  ucBuf[1024] = {(RPC_DEFINITION(IRpcGlobalRequest, ServiceStart) & 0xFF), 0x14};    
    GlIntU32 ulPatchAddress = GetPatchAddress();
    int i = 2;
    GlIntU32 ulDummy = 0;
    
    memcpy(&ucBuf[i], &ulPatchAddress, sizeof(ulPatchAddress));
    i += sizeof(ulPatchAddress);
    
    memcpy(&ucBuf[i], &ulDummy, sizeof(ulDummy));
    i += sizeof(ulDummy);
    
    memcpy(&ucBuf[i], &ulDummy, sizeof(ulDummy));
    i += sizeof(ulDummy);
    
    memcpy(&ucBuf[i], &ulDummy, sizeof(ulDummy));
    i += sizeof(ulDummy);
    
    memcpy(&ucBuf[i], &ulDummy, sizeof(ulDummy));
    i += sizeof(ulDummy);

    BuildAndSendPacket(0, ucBuf, 22, 0);
}

void reset_transport(void)
{
    m_ulRetryCnt      = 0;
    m_ucUnackCount    = 0;
    m_ucTxSeqId       = 0;
    m_ucLastRxSeqId   = 0xFF;
    m_ucReliableSeqId = 0xFF;
    memset(m_aucTxBuffer, 0, sizeof(m_aucTxBuffer));
    m_ulByteCntSinceLastValidPacket = 0;
    m_uiParserState = WAIT_FOR_ESC_SOP;
    m_uiRxLen = 0;
    memset(m_aucRxMessageBuf, 0, sizeof(m_aucRxMessageBuf));
    m_ucLastAckSeqId = 0;
    m_bOngoingSync = false;
    m_bRemoteSyncComplete = false;
    m_bGotVersion = false;
    m_ucReliableCrc = 0;
    m_usReliableLen = 0;
    m_ucTxReliableSeqId = 0;
    m_ucTxLastAckedSeqId = 0xFF;

    m_ulAsicVersion = 0;
    m_ulRomVersion = 0;
    m_ulPatchVersion = 0;

    m_uiState = PE_IDLE;

    m_usPayloadLen = 0;
    m_usPayloadIn  = 0;
    PatchCmdSent   = false;

    memset(m_ucPayload, 0, sizeof(m_ucPayload));
}