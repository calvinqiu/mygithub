/**
    IPL Display control

    @file       IPL_Display_int.h
    @ingroup    mISYSAlg
    @note       This moudle is used to control the relationship of IME/display survice/video encoder/DIS

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_DISPLAY_H_
#define _IPL_DISPLAY_H_

#include "IPL_Utility.h"
#define DisplayInSrc            4
#define DisplaySourcePath       4
#define DisplayTotalRingBuffer  10//8//5// 4
#define DisplayMaxBufIndexer       10

#if 0
 - Display Ctrl task
#endif
#define FLGPTN_DISPLAYCTRL_IDLE         FLGPTN_BIT(0)
#define FLGPTN_DISPLAYCTRL_RESET        FLGPTN_BIT(1)
#define FLGPTN_DISPLAYCTRL_CHANGE       FLGPTN_BIT(2)
#define FLGPTN_DISPLAYCTRL_RESET2       FLGPTN_BIT(3)
#define FLGPTN_DISPLAYCTRL_CHANGE2      FLGPTN_BIT(4)
#define FLGPTN_DISPLAYCTRL_RESET3       FLGPTN_BIT(5)
#define FLGPTN_DISPLAYCTRL_CHANGE3      FLGPTN_BIT(6)
#define FLGPTN_DISPLAYCTRL_RESET4       FLGPTN_BIT(7)
#define FLGPTN_DISPLAYCTRL_CHANGE4      FLGPTN_BIT(8)
#define FLGPTN_DISPLAYCTRL_LOCK         FLGPTN_BIT(9)
#define FLGPTN_DISPLAYCTRL_OUTPUT_READY         FLGPTN_BIT(10)

typedef struct
{
    UINT32 FrameIdx;
    UINT32 OffsetX;
    UINT32 OffsetY;
} DIS_Infor;

typedef struct
{
    INT32 MotionX;
    INT32 MotionY;
} Disp_Motion_Infor;

/**
    Open display control task
    @return None
*/
extern void IPL_Display_OpenTsk(void);


/**
    Close display control task
    @return None
*/
extern void IPL_Display_CloseTsk(void);


/**
    Change buffer index, and it is called by IME_ISR
    @return None
*/
extern void IPL_AskDisplay2ChangeBuffer(void);


/**
    Get specified IME output buffer
    
    @param[in] UINT32 IME output path
    @param[in] UINT32 buffer index ID
    @param[in] IPL_ID set the pipeline input ID
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
//extern IPL_YCC_IMG_INFO IPL_GetSpecifiedIMEoutputAddr(UINT32 path, UINT32 BufferID);
extern IPL_YCC_IMG_INFO IPL_GetSpecifiedIMEoutputAddr(UINT32 path, UINT32 BufferID,  IPL_PROC_ID IPL_ID);


/**
    Reset buffer control flow, and DisplayCtrl_Tsk() will control buffer automaticly.
    @param[in] IPL_ID set the pipeline input ID
    @return None
*/
//extern void IPL_ResetDisplayCtrlFlow(void);
extern void IPL_ResetDisplayCtrlFlow(IPL_PROC_ID IPL_ID);


/**
    Enable/disable to freeeze image buffer. If enable that, the display image will be frozen.

    @param[in] BOOL enable/disable
    @param[in] IPL_ID set the pipeline input ID
    @return None
*/
//extern void IPL_FreezeDisplayImage(BOOL OnOFF);
extern void IPL_FreezeDisplayImage(BOOL OnOFF, IPL_PROC_ID IPL_ID);


/**
    Get ring buffer number

    @param[in] IPL_ID set the pipeline input ID
    @return UINT32
        - @b ring buffer number
*/
extern UINT32 IPL_GetDisplayCtrlBufferNum(IPL_PROC_ID IPL_ID);


/**
    Assign buffer address to specified buffer

    @param[in] UINT32 specified path
    @param[in] UINT32 specified buffer id
    @param[in] IPL_YCC_IMG_INFO Y/UV buffer address
    @param[in] IPL_ID set the pipeline input ID
    @return None
*/
//extern void IPL_InitDisplayBuffer(UINT32 Path, UINT32 BufferID,IPL_YCC_IMG_INFO *BufferInfo);
extern void IPL_InitDisplayBuffer(UINT32 Path, UINT32 BufferID,IPL_YCC_IMG_INFO *BufferInfo, IPL_PROC_ID IPL_ID);


/**
    Get ready buffer address after post processing, it is update in IPL_Display task
    
    @param[in] UINT32 IME output path
    @param[in] IPL_ID set the pipeline input ID
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
extern IPL_YCC_IMG_INFO IPL_GetIMEReadyBufferAddr(UINT32 path,  IPL_PROC_ID IPL_ID);

/**
    Get IME ready buffer address befor post process, it is update in Ime frame interrupt, and used for IME_H264 direct mode
    
    @param[in] UINT32 IME output path
    @param[in] IPL_ID set the pipeline input ID
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
extern IPL_YCC_IMG_INFO IPL_GetIMEReadyBufferAddr2(UINT32 path,  IPL_PROC_ID IPL_ID);


/**
    Get ready buffer address of display after post processing
    
    @param[in] IPL_ID set the pipeline input ID
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
//extern IPL_YCC_IMG_INFO IPL_GetDisplayBufferAddr(void);
extern IPL_YCC_IMG_INFO IPL_GetDisplayBufferAddr(IPL_PROC_ID IPL_ID);


/**
    Enable/disable post processing to specified IME output path

    @param[in] UINT32 IME output path
    @param[in] BOOL enable/disable post processing
    @return None
*/
extern void IPL_SetPostImageProcessOnOff(UINT32 Path, BOOL Enable);


/**
    Get post processing status(enable/disable) to specified IME output path

    @param[in] UINT32 IME output path
    @return BOOL
        - @b enable/disable
*/
extern BOOL IPL_GetPostImageProcessOnOff(UINT32 Path);


/**
    callback function in IME_ISR to exchange address & size information

    @param[in] UINT32 IME output image index(count)
    @param[in/out] IPL_YCC_IMG_INFO exchange IME output path1 information. in: type, size. out: next buffer address
    @param[in/out] IPL_YCC_IMG_INFO exchange IME output path2 information. in: type, size. out: next buffer address
    @param[in/out] IPL_YCC_IMG_INFO exchange IME output path3 information. in: type, size. out: next buffer address
    @param[in/out] IPL_YCC_IMG_INFO exchange IME output path4 information. in: type, size. out: next buffer address
    @param[in] fpCallBack callback for post processing done
    @param[in] IPL_ID set the pipeline input ID
    @return None
*/
//extern void IPL_CtrlPostImage(UINT32 ImgIdx, IPL_YCC_IMG_INFO* path1,IPL_YCC_IMG_INFO* path2,IPL_YCC_IMG_INFO* path3,fpCallBack fpDone);
extern void IPL_CtrlPostImage(UINT32 ImgIdx, IPL_YCC_IMG_INFO* path1,IPL_YCC_IMG_INFO* path2,IPL_YCC_IMG_INFO* path3,IPL_YCC_IMG_INFO* path4,fpCallBack fpDone, IPL_PROC_ID IPL_ID);


/**
    Set each device(path) size, such as LCD, HDMI, Video, etc.

    @param[in] UINT32 IME output path
    @param[in] IPL_YCC_CH_INFO device size info 
    @param[in] IPL_ID set the pipeline input ID
    @return None
*/
//extern void IPL_SetDeviceInputInfo(UINT32 path, IPL_YCC_CH_INFO SizeInfo);
extern void IPL_SetDeviceInputInfo(UINT32 path, IPL_YCC_CH_INFO SizeInfo, IPL_PROC_ID IPL_ID);


/**
    Pause display control
    @return None
*/
extern void IPL_Display_Pause(void);


/**
    Resume display control
    @return None
*/
extern void IPL_Display_Resume(void);


/**
    Check buffer ready or not
    
    @param[in] IPL_ID set the pipeline input ID
    @return BOOL
        - @b TRUE/FALSE
*/
//extern BOOL IPL_Display_ChkBufferReady(void);
extern BOOL IPL_Display_ChkBufferReady(IPL_PROC_ID IPL_ID);


/**
    Set skip frames to ignore invaild image befor starting display
    If user don,t set this parameter, the default value is 2

    @param[in] UINT32 skip frames
    @return None
*/
extern void IPL_Display_InitSkipFrame(UINT32 SkipFrame);

/**
    Set IME output path to display
 
    @param[in] UINT32 IME output path (0~2)
    @param[in] IPL_ID set the pipeline input ID
    @return none
*/
extern void IPL_SetDisplayInputPath(UINT32 path, IPL_PROC_ID IPL_ID);


/**
    Set IME output path to display
 
    @param[in] UINT32 IME output path (0~2)
    @param[in] IPL_ID set the pipeline input ID
    @return none
*/
extern void IPL_SetDisplayInputPath2(UINT32 path, IPL_PROC_ID IPL_ID);


/**
    Get next IME output path information
 
    @param[in] UINT32 IME output path (0~2)
    @param[in] IPL_ID set the pipeline input ID
 
    @return IPL_YCC_IMG_INFO
        - @b image information(type, address, size)
*/
//extern IPL_YCC_IMG_INFO IPL_GetNextIMEoutputAddr(UINT32 path);
extern IPL_YCC_IMG_INFO IPL_GetNextIMEoutputAddr(UINT32 path, IPL_PROC_ID IPL_ID);

/**
    The display control task, should be installed by OS
    @return None
*/
extern void IPL_DisplayCtrl_Tsk(void);


/**
    Set IME path3 to dual display source (HDMI)
    This API should be called before IPL_ResetDisplayCtrlFlow( )

    @param[in] UINT32 IME output path (0~2)

    @return None
*/
extern void IPL_DisplayDualOutput(BOOL DualOutput);

extern void IPL_Display_SetBufferNum(UINT32 BufNum, IPL_PROC_ID IPL_ID);

extern BOOL IPL_Display_ClosePath(IPL_PROC_ID IPL_ID);

/**
    Used to enable/disable display when input path is NULL
    
    @param[in] ID set the pipeline input ID 0~3
    @param[in] enable set the display enable/disable (TRUE/FALSE)

    @return None
*/
extern void IPL_Display_ID_Enable(UINT32 ID, BOOL enable);

/**
    Used to lock buffer index to avoid over-write it.
    
    @param[in] IPL_ID set the pipeline ID 0~3
    @param[in] BufIdx lock the buffer index

    @return ER
        - @b status (E_OK, E_PAR: index > max)
*/
extern ER IPL_DisplayLockBufIdx(IPL_PROC_ID IPL_ID, UINT32 BufIdx);

/**
    Used to unlock all buffer index.
    
    @param[in] IPL_ID set the pipeline ID 0~3

    @return None
*/
extern void IPL_DisplayUnlockBuf_All(IPL_PROC_ID IPL_ID);

/**
    Used to lock current display/video buffer index.
    
    @param[in] IPL_ID set the pipeline ID 0~3

    @return None
*/
extern void IPL_DisplayLockCurrentBufIdx(IPL_PROC_ID IPL_ID);


extern void IPL_Display_Set_StitchSize(IPL_YCC_IMG_INFO StitchInfo, UINT32 Path);
extern void IPL_Display_SetStitchEn(BOOL StitchEn);
extern BOOL IPL_Display_GetStitchEn(void);

extern void IPL_Display_SetPostProcessEn(BOOL PstPrcs);
//extern Disp_Motion_Infor IPL_GetIMEReadyBufferMotion(UINT32 path);


/***************** To be reviewed *******************/
//extern void IPL_CtrlPostImage2(UINT32 ImgIdx, IPL_YCC_IMG_INFO* path1,IPL_YCC_IMG_INFO* path2);
//extern void IPL_SetDeviceInputInfo2(UINT32 path, IPL_YCC_CH_INFO SizeInfo);
//extern IPL_YCC_IMG_INFO IPL_GetReadyBufferAddr2(UINT32 path);
//extern void IPL_InitDisplayBuffer2(UINT32 Path, UINT32 BufferID,IPL_YCC_IMG_INFO *BufferInfo);
//extern void IPL_FreezeDisplayImage2(BOOL OnOFF);
//extern void IPL_ResetDisplayCtrlFlow2(void);
//extern IPL_YCC_IMG_INFO IPL_GetSpecifiedIMEoutputAddr2(UINT32 path, UINT32 BufferID);
//extern void IPL_Display_CloseTsk2(void);
/*************************************************/

typedef UINT32 (*fpIPLDisplayStitchCB)(UINT32 BufIdx, IPL_YCC_IMG_INFO StitchBuf, IPL_YCC_IMG_INFO LeftBuf, IPL_YCC_IMG_INFO RightBuf);
typedef UINT32 (*fpIPLDisplayPostProcessCB)(UINT32 Path, IPL_YCC_IMG_INFO *SrcImg, IPL_YCC_IMG_INFO *DstImg);
extern void IPL_Display_InitFPCB(fpIPLDisplayStitchCB FPCB);
extern void IPL_Display_InitPstPrcCB(fpIPLDisplayPostProcessCB FPCB);
extern void IPL_InitStitchBuffer(UINT32 Path, UINT32 BufferID,IPL_YCC_IMG_INFO *BufferInfo_Stitch, IPL_YCC_IMG_INFO *BufferInfo_1, IPL_YCC_IMG_INFO *BufferInfo_2);
extern void IPL_InitSprtBuffer(IPL_PROC_ID IPL_ID, UINT32 Path, UINT32 BufferID, IPL_YCC_IMG_INFO *BufferInfo);
extern void IPL_InitSeamBuffer(UINT32 Path, UINT32 BufferID, IPL_YCC_IMG_INFO *BufferInfo);
#endif
