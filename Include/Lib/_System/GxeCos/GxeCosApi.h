/**
    eCos Utility

    for start and close Core - eCos

    @file       GxeCosApi.h
    @ingroup    mGXECOS

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _GXECOSAPI_H
#define _GXECOSAPI_H

#define GXECOS_PRELOAD_ECOS_SIZE  0x580

typedef struct _GXECOS_OPEN
{
    UINT32 uiAddr; ///< eCos start of memory region
    UINT32 uiSize; ///< ecos size of memory region
}GXECOS_OPEN,*PGXECOS_OPEN;

extern BOOL GxeCos_Init(void); ///< for install command
extern BOOL GxeCos_Open(GXECOS_OPEN* pOpen);
extern BOOL GxeCos_Close(void);
extern BOOL GxeCos_IsRunning(void);

//for checking eCos, the context have to load GXECOS_PRELOAD_UBOOT_SIZE as least
extern BOOL GxeCos_ChkValid(UINT32 uiAddr);
extern UINT32 GxeCos_GetBinSize(UINT32 uiAddr);

#endif