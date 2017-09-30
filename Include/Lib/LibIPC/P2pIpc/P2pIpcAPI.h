/*
    Copyright (c) 2016  Novatek Microelectronics Corporation

    @file P2pIpc.h

    @version

    @date
*/

#ifndef _P2PIPCAPI_H
#define _P2PIPCAPI_H

#include "Type.h"

#define NVTP2P_RET_OK                       0
#define NVTP2P_RET_ERR                      (-1)
typedef enum{
    NVT_P2P_CB_SERVER_CONNECTED  =  0x00000000,
    NVT_P2P_CB_SERVER_DISCONNECTED,
    NVT_P2P_CB_SERVER_ERROR,
    NVT_P2P_CB_CLIENT_CONNECTED,
    NVT_P2P_CB_CLIENT_DISCONNECTED,
    NVT_P2P_CB_CLIENT_ERROR,
    NVT_P2P_CB_DATA_ERROR,
    NVT_P2P_CB_NOTIFY_ERROR,
    NVT_P2P_CB_VENDOR_SPECIFIC,
}NVTP2P_CBID;

typedef INT32 (*NVTP2P_CB)(NVTP2P_CBID msg);

extern void P2pIpc_InstallID(void) _SECTION(".kercfg_text"); 

extern ER P2pIpc_Open(UINT32 sharedMemAddr, UINT32 sharedMemSize, NVTP2P_CB callback);

extern ER P2pIpc_Close(void);

extern ER P2pIpc_Server_Start(void);

extern ER P2pIpc_Server_Stop(void);

extern ER P2pIpc_Send_Notify(void);

extern ER P2pIpc_Get_Client_Info(void);

extern ER P2pIpc_IOCTL(void);

extern UINT32 P2pIpc_getRcvAddr(void);

extern UINT32 P2pIpc_getSndAddr(void);

extern UINT32 P2pIpc_RecvMemSize(void);

extern UINT32 P2pIpc_SendMemSize(void);

#endif /* _P2PIPCAPI_H  */
