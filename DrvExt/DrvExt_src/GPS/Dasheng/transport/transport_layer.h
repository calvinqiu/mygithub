#include "../inc/types.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static const GlIntU16 FLAG_PACKET_ACK      = (1<<0); // ACK of a received packet. Flag detail contains the ACK SeqId
static const GlIntU16 FLAG_RELIABLE_PACKET = (1<<1); // Indicates that this is a reliable packet. Flag detail contains the remote reliable seqId
static const GlIntU16 FLAG_RELIABLE_ACK    = (1<<2); // A reliable SeqId was Acked. Flag detail contains the acked reliable seqId
static const GlIntU16 FLAG_RELIABLE_NACK   = (1<<3); // A reliable SeqId error was detected. Flag detail contains the last remote reliable seqId
static const GlIntU16 FLAG_MSG_LOST        = (1<<4); // Remote PacketLayer detected lost packets (jumps in SeqId). The flag details contains the last remote seqId
static const GlIntU16 FLAG_MSG_GARBAGE     = (1<<5); // Garbage bytes detected. The Flag details will contains the number of garbage bytes (capped to 255)
static const GlIntU16 FLAG_SIZE_EXTENDED   = (1<<6); // Size of packet will have one byte extension (MSB), contained in the Flags detail
static const GlIntU16 FLAG_EXTENDED        = (1<<7); // If set, then the flag details will contains a Byte representing the MSB of the 16bit flags
static const GlIntU16 FLAG_INTERNAL_PACKET = (1<<8); // Packet in that message is internal, and should be processed by the TL
static const GlIntU16 FLAG_IGNORE_SEQID    = (1<<9); // Remote side requested to ignore the seqId, i.e. error should not be accounted for in the stats

enum
{
     INT_PKT_SEQID_REQUEST     // Request to send the Sequence Ids
    ,INT_PKT_SEQID_RESPONSE    // Sequence Ids responses
    ,INT_PKT_STATS_REQUEST     // Request for the remote stats to be sent
    ,INT_PKT_STATS_RESPONSE    // Response with the stats
    ,INT_PKT_SETTINGS_REQUEST  // Request to get internal settings of remote side
    ,INT_PKT_SETTINGS_RESPONSE // Response with the settings
};

enum
{
     WAIT_FOR_ESC_SOP = 0
    ,WAIT_FOR_SOP
    ,WAIT_FOR_MESSAGE_COMPLETE
    ,WAIT_FOR_EOP
};

typedef struct stTransportLayerStats
{
    GlIntU32 ulRxGarbageBytes;
    GlIntU32 ulRxPacketLost;
    GlIntU32 ulRemotePacketLost;   // this is approximate as it is reported and the report could be lost.
    GlIntU32 ulRemoteGarbage;      // this is approximate as it is reported and the report could be lost.
    GlIntU32 ulPacketSent;         // number of normal packet sent
    GlIntU32 ulPacketReceived;
    GlIntU32 ulAckReceived;
    GlIntU32 ulReliablePacketSent;
    GlIntU32 ulReliableRetransmit;
    GlIntU32 ulReliablePacketReceived;
    GlIntU32 ulMaxRetransmitCount;
} stTransportLayerStats;

#define TLCUST_MAX_OUTGOING_PACKET_SIZE        1024
#define TLCUST_MAX_INCOMING_PACKET_SIZE        1024
#define TLCUST_RELIABLE_RETRY_TIMEOUT_MS       500
#define TLCUST_RELIABLE_MAX_RETRY              10
#define TLCUST_RELIABLE_SYNC_RETRY_TIMEOUT_MS  50
#define TLCUST_RELIABLE_SYNC_MAX_RETRY         100
#define TLCUST_RELIABLE_MAX_PACKETS            150

static const GlIntU16 MAX_OUTGOING_PACKET_SIZE       = TLCUST_MAX_OUTGOING_PACKET_SIZE; // see custom file
static const GlIntU16 MAX_INCOMING_PACKET_SIZE       = TLCUST_MAX_INCOMING_PACKET_SIZE; // see custom file
static const GlIntU16 RELIABLE_RETRY_TIMEOUT_MS      = TLCUST_RELIABLE_RETRY_TIMEOUT_MS; // see custom file
static const GlIntU16 RELIABLE_MAX_RETRY             = TLCUST_RELIABLE_MAX_RETRY;   // see custom file
static const GlIntU16 RELIABLE_SYNC_RETRY_TIMEOUT_MS = TLCUST_RELIABLE_SYNC_RETRY_TIMEOUT_MS; // see custom file
static const GlIntU16 RELIABLE_SYNC_MAX_RETRY        = TLCUST_RELIABLE_SYNC_MAX_RETRY;   // see custom file
static const GlIntU16 RELIABLE_MAX_PACKETS           = TLCUST_RELIABLE_MAX_PACKETS;  // see custom file
static const GlIntU16 MAX_HEADER_SIZE                = 14;

static const GlIntU8 ESCAPE_CHARACTER                = 0xB0;
static const GlIntU8 SOP_CHARACTER                   = 0x00;
static const GlIntU8 EOP_CHARACTER                   = 0x01;
static const GlIntU8 ESCAPED_ESCAPE_CHARACTER        = 0x03;
static const GlIntU8 ESCAPED_XON_CHARACTER           = 0x04;
static const GlIntU8 ESCAPED_XOFF_CHARACTER          = 0x05;
static const GlIntU8 XON_CHARACTER                   = 0x11;
static const GlIntU8 XOFF_CHARACTER                  = 0x13;

int StartRemoteSync(void);
void SendInternalSeqIdRequest(void);
GlIntU8 BuildAndSendPacket(GlIntU16 usFlags, GlIntU8 *pucData, GlIntU16 usSize, GlIntU8 ucReliableTxSeqId);
GlIntU32 EscapeData(GlIntU8 *pucDestBuff, const GlIntU8 *pucSrcBuff, GlIntU16 usSize);
GlIntU32 EscapeDataAndUpdateCrc(GlIntU8 *pucDestBuff, const GlIntU8 *pucSrcBuff, GlIntU16 usSize);
void ParseIncomingData(GlIntU8 *pucData, GlIntU16 usLen);
int PacketReceived(void);
void OnInternalPacket(GlIntU8 *pucData, GlIntU16 usSize);
void OnInternalSeqIdResponse(GlIntU8 ucReqId, GlIntU8 ucLastRxSeqId, GlIntU8 ucLastAckSeqId, GlIntU8 ucReliableSeqId, GlIntU8 ucTxSeqId, GlIntU8 ucTxReliableSeqId, GlIntU8 ucTxLastAckedSeqId);
int RequestVersion(void);
void SendVersionRequest(void);
void ReceivedReliableAck(GlIntU8 ucAckedReliableSeqId);
void OnPacketReceived(GlIntU8 *pucData, GlIntU16 usSize);
bool CheckPacketSanity(GlIntU8 *pucData, GlIntU16 usSize);
void OnRpcReceived(GlIntU16 usRpcId, GlIntU8 *pRpcPayload, GlIntU16 usRpcLen);
void VersionResponse(GlIntU32 ulAsicVersion, GlIntU32 ulRomVersion, GlIntU32 ulPatchVersion);
bool CheckPatch(void);
bool CheckRomVersion(void);
bool InitiatePatchDownload(void);
GlIntU8 OnDataPush(GlIntU8 *pucData, GlIntU16 usSize);
bool IsPECmd(GlIntU8 ucCmd);
GlIntU8 OnCmdReceive(void);
void StartServiceAPP(void);
void reset_transport(void);