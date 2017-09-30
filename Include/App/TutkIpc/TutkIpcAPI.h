/*
    Copyright (c) 2014  Novatek Microelectronics Corporation

    @file TutkIpcAPI.h

    @version

    @date
*/

#ifndef _TUTKIPCAPI_H
#define _TUTKIPCAPI_H

#include "Type.h"

typedef void (*TutkIpc_loginInfoCB)(unsigned int nLoginInfo);
typedef int (*TutkIpc_authFn)(char *szViewAccount,char *szViewPassword);

typedef struct _TUTKIPC_INIT_ARG
{
    UINT32  MemPoolAddr;
    UINT32  MemPoolSize;
}TUTKIPC_INIT_ARG, *PTUTKIPC_INIT_ARG;

extern void TutkIpc_InstallID(void) _SECTION(".kercfg_text");

extern INT32 TutkIpc_Open(TUTKIPC_INIT_ARG *pInitArg);
extern INT32 TutkIpc_Close(void);

int   TutkIpc_IOTC_Device_Login(const char *cszUID, const char *cszDeviceName, const char *cszDevicePWD);
int   TutkIpc_IOTC_Listen(unsigned int nTimeout);
void  TutkIpc_IOTC_Get_Version(unsigned int *pnVersion);
void  TutkIpc_IOTC_Get_Login_Info_ByCallBackFn(TutkIpc_loginInfoCB pfxLoginInfoFn);
int   TutkIpc_IOTC_Initialize2(unsigned short nUDPPort);
void  TutkIpc_IOTC_Set_Max_Session_Number(unsigned int nMaxSessionNum);
void  TutkIpc_IOTC_Session_Close(int nIOTCSessionID);
int   TutkIpc_IOTC_SetEcosThreadPriority(int nPriority);
int   TutkIpc_IOTC_Session_Get_Free_Channel(int nIOTCSessionID);
int   TutkIpc_IOTC_DeInitialize(void);
int   TutkIpc_IOTC_Session_Check(int nIOTCSessionID, char *psSessionInfo);

int   TutkIpc_avSendFrameData(int nAVChannelID, const char *cabFrameData, int nFrameDataSize, const void *cabFrameInfo, int nFrameInfoSize);
int   TutkIpc_avSendAudioData(int nAVChannelID, const char *cabAudioData, int nAudioDataSize, const void *cabFrameInfo, int nFrameInfoSize);
float TutkIpc_avResendBufUsageRate (int nAVChannelID);
int   TutkIpc_avRecvAudioData(int nAVChannelID, char *abAudioData, int nAudioDataMaxSize, char *abFrameInfo, int nFrameInfoMaxSize, unsigned int *pnFrameIdx);
int   TutkIpc_avServStart3(int nIOTCSessionID, TutkIpc_authFn pfxAuthFn, unsigned int nTimeout, unsigned int nServType, unsigned char nIOTCChannelID, int *pnResend);
int   TutkIpc_avRecvIOCtrl(int nAVChannelID, unsigned int *pnIOCtrlType, char *abIOCtrlData, int nIOCtrlMaxDataSize, unsigned int nTimeout);
int   TutkIpc_avInitialize(int nMaxChannelNum);
int   TutkIpc_avSendIOCtrl(int nAVChannelID, unsigned int nIOCtrlType, char *cabIOCtrlData, int nIOCtrlDataSize);
int   TutkIpc_avCheckAudioBuf(int nAVChannelID);
int   TutkIpc_avGetAVApiVer(void);
int   TutkIpc_avSetMemPoolAddr(unsigned int addr, unsigned int size);
int   TutkIpc_avSetSendFrameLimitCount (int limit_count, int sleep_ms);
int   TutkIpc_avClientStart(int nIOTCSessionID, char *cszViewAccount, char *cszViewPassword, unsigned int nTimeout,unsigned int *pnServType, unsigned char nIOTCChannelID);
void  TutkIpc_avClientStop(int nAVChannelID);
void  TutkIpc_avServStop(int nAVChannelID);
int   TutkIpc_avDeInitialize(void);
#endif
