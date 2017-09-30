/*
    Copyright (c) 2015  Novatek Microelectronics Corporation

    @file FTypeIpc.h

    @version

    @date
*/

#ifndef _FTYPEIPCAPI_H
#define _FTYPEIPCAPI_H

#include "Type.h"

#define NVTFTYPE_RET_OK                       0
#define NVTFTYPE_RET_ERR                      (-1)
#define OSD_MAX_CHANNEL                       4
#define OSD_HDMI_CHANNEL                      (OSD_MAX_CHANNEL - 1) //+HDMI

typedef struct _NVTFTYPE_IPC_IMAGE{
    unsigned int id;
    unsigned int channel;
    unsigned int x;
    unsigned int y;
    unsigned int on;
    unsigned int font_color;
    unsigned int edge_color;
    unsigned int width;
    unsigned int height;
    unsigned int bmp_ptr;
    unsigned int alpha_ptr;
}NVTFTYPE_IPC_IMAGE;

typedef INT32 (*NVTFTYPE_CB)(unsigned int image_number);

extern void FtypeIpc_InstallID(void) _SECTION(".kercfg_text"); 

extern ER FtypeIpc_Open(UINT8 *buf, UINT32 len, NVTFTYPE_CB callback);

extern ER FtypeIpc_Close(void);

extern void Acquire_FType_Buf(void);

extern void Release_FType_Buf(void);

#endif /* _FTYPEIPCAPI_H  */
