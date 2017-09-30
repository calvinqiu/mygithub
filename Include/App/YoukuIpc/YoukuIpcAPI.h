/////////////////////////////////////////////////////////////////
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file YoukuIpcAPI.h

    @version

    @date

*//////////////////////////////////////////////////////////////////
#ifndef _YOUKUIPCAPI_H
#define _YOUKUIPCAPI_H
#include "Type.h"

/**
     @name Youku IPC needed buffer size.
*/
#define YOUKUIPC_NEED_BUFSIZE   512


/**
   Warning Code.

   Any warning code occured will display on uart or return.
*/
typedef enum _YOUKUIPC_WR{
    YOUKUIPC_WR_OK                =   0, ///< no warning
    YOUKUIPC_WR_ALREADY_OPEN      =  -1, ///< YoukuIpc_Open called twice
    YOUKUIPC_WR_NOT_OPEN          =  -2, ///< YoukuIpc_Open not to call before.
    ENUM_DUMMY4WORD(YOUKUIPC_WR)
}YOUKUIPC_WR;


/*-----------------------------------
    functions declare
-----------------------------------*/

/**
     Install Youku flag and semaphore id.

*/
extern void   YoukuIpc_InstallID(void) _SECTION(".kercfg_text");

/**
    Open YoukuIPC for Youku SDK.

    This API should be invoked once before any other Youku SDK APIs.

    @param[in] pWorkBuf Available working buffer for YoukuIpc lib, recommended size is 512 byte.

    @return
        - @b E_OK: initial success.
        - @b E_SYS: the initial parameter has some error.

    Example:
    @code
    {
        MEM_RANGE WorkBuf;

        WorkBuf.Addr = OS_GetMempoolAddr(POOL_ID_YOUKUIPC);
        WorkBuf.Size = POOL_SIZE_YOUKUIPC;
        YoukuIpc_Open(&WorkBuf);
    }
    @endcode

*/
extern ER YoukuIpc_Open(PMEM_RANGE pWorkBuf);

/**
    Close YoukuIpc.

    @return
        - @b E_OK: close success.

*/
extern ER YoukuIpc_Close(void);

#endif  // _YOUKUIPCAPI_H

