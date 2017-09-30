#ifndef __NVTFD__H__
#define __NVTFD__H__

#include <Type.h>
#include <Debug.h>
#include <Perf.h>
#include "FileSysTsk.h"

#include "ife_lib.h"
#include "ime_lib.h"
#include "grph.h"
#include "ise_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "clock.h"
#include "Debug.h"
#include "dma.h"
#include "kernel.h"
#include "pll.h"
#include "sdio.h"
#include "top.h"
#include "Type.h"
#include "uart.h"
#include "Utility.h"
#include "IPL_Hal_IME_Info.h"
#include "fde_lib.h"
#include "GxImage.h"

//nvt face detection interfaces

/**
    Detect faces from HW candidiates.
    @param[in] imgBuf: gray image info.
    @param[in && out] pFace: HW candidates input and detected faces output
    @param[in] nFace: HW candidates number
    @param[in] pBuff: FD buff addr
    @return detected faces number
*/
extern UINT32 NvtFD_DetectFaces( IMG_BUF *imgBuf, FACE *pFace, UINT32 nFace, UINT32 pBuff );

/**
    Draw face boxes and save the image.
    @param[in] imgBuf: gray image info.
    @param[in] pFace: faces to draw
    @param[in] nFace: faces number
    @param[in] flag: flag for saving images(0 - HW, 1 - ARC, 2 - NVT)
    @return void
*/
extern void NvtFD_SaveResultImage( IMG_BUF *imgBuf, FACE *pFace, UINT32 nFace, UINT32 flag );

/**
    Get the working buffer(no chache buffer) that NvtFD_DetectFaceOrNot need.
	@return UINT32: the size of no cache buffer that NvtFD_DetectFaceOrNot need.
*/
extern UINT32 NvtFD_GetBuffSize(VOID);

/**
    Detect the faces region which in imgBuf are true faces
    @param[in] imgBuf: gray image info.
    @param[in] pFace: pointer os faces, the faces could be array.
    @param[in] nFace: faces regions number that need to be detected.
    @param[in] pBuff: the no cache buffer pointer which size could be calculate by NvtFD_GetBuffSize
    @return UINT32: the true faces number.
*/
extern UINT32 NvtFD_DetectFaceOrNot(const IMG_BUF* imgBuf, FACE* pFace, UINT32 nFace, UINT32 pBuff);


//nvt face detection interfaces(new)

typedef enum
{
    NVTFD_FLIP_NEVER = 0,
    NVTFD_FLIP_ROT30,
    NVTFD_FLIP_ROT330,
    NVTFD_FLIP_LR,
    ENUM_DUMMY4WORD(NVTFD_FLIP_TYPE)
}NVTFD_FLIP_TYPE;

typedef struct
{
    UINT32 flag;
    NVTFD_FLIP_TYPE flip;
    INT32 *table;
    UINT8 **pbuf;
}NVTFD_DETECTOR_PRMS;

typedef struct
{
    IMG_BUF *imgbuf;
    FACE *faces;
    UINT32 facenum;
    UINT32 merge;
    UINT8 *buf;
}NVTFD_PRMS;

typedef struct
{
    FACE *faces;
    UINT32 facenum;
    UINT32 mode;
}NVTFD_MERGE_PRMS;


/**
    Get the working buffer size that nvtfd need
    @return UINT32: the working buffer size that nvtfd need.
*/
UINT32 nvtfd_calcbuffsize(VOID);


/**
    Enroll the detector for nvtfd.
    @param[in] detectorprms: detector info structures.
    @return VOID.
*/
VOID nvtfd_enrolldetector(NVTFD_DETECTOR_PRMS *detectorprms);


/**
    Detect faces from FDE proposals.
    @param[in] prms: detection info structures.
    @return VOID.
*/
UINT32 nvtfd_process(NVTFD_PRMS *prms);


/**
    Merge detected faces.
    @param[in && out] faces: detected faces.
    @param[in] facenum: the number of detected face.
    @param[in] mode: flag of how to merge detected faces(0 - not merge, 1 - prefer larger uiScroe, other - prepfer larger face size).
    @return UINT32: the true faces number.
*/
UINT32 nvtfd_mergefaces(NVTFD_MERGE_PRMS *prms);


#endif

