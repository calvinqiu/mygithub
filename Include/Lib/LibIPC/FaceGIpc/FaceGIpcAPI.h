/*
    Copyright (c) 2015  Novatek Microelectronics Corporation

    @file FTypeIpc.h

    @version

    @date
*/

#ifndef _FACEGIPCAPI_H
#define _FACEGIPCAPI_H

#include "Type.h"

#define NVTFACEG_RET_OK                       0
#define NVTFACEG_RET_ERR                      (-1)

typedef struct _NVTFACEG_GRADE{
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
	unsigned int score;
    unsigned int id;
}NVTFACEG_GRADE;

typedef struct _NVTFACEG_IPC_INPUT{
    unsigned int img_size;
    unsigned int img_off;
    unsigned int bin_size;
    unsigned int bin_off;
}NVTFACEG_IPC_INPUT;

typedef struct _NVTFACEG_IPC_RESULT{
    unsigned int number;
    NVTFACEG_GRADE face[];
}NVTFACEG_IPC_RESULT;

extern void FaceGIpc_InstallID(void) _SECTION(".kercfg_text");

extern ER FaceGIpc_Open(UINT32 buf, UINT32 len);

extern ER FaceGIpc_Close(void);

#endif /* _FACEGIPCAPI_H  */
