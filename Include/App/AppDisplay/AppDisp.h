#ifndef _DSCDISP_H_
#define _DSCDISP_H_

#include "Type.h"
#include "GxImage.h"
#include "DispSrvApi.h"

#define APPDISP_CB_FRAMESTABLE     1

/**
     User handle method

     (bDirectMode == FALSE) In OnDraw callback., the methods to generated display output.
*/
typedef enum _VIEW_HANDLE{
    VIEW_HANDLE_PRIMARY,    ///<In condition of daul mode, set it to DISPSRV_DISP_IDX_PRIMARY means scale primary to secondary.
    VIEW_HANDLE_SECONDARY,  ///<If set it to DISPSRV_DISP_IDX_SECONDARY, the uiNumDisp of secondary must be 2
    VIEW_HANDLE_BOTH,       ///<In condition of dual mode, user have to draw both pImg[0] and pImg[1]
    ENUM_DUMMY4WORD(VIEW_HANDLE)
}VIEW_HANDLE;



//view source - provided by app mode
typedef struct _VIEW_SRC
{
    BOOL bDirectMode;
    VIEW_HANDLE UserHandle; ///<if (bDirectMode == FALSE), user handle view of Dual-Display (otherwise are handled by built-in scaling method)
    INT32 (*fpDrawCb)(DISPSRV_DRAW* pDraw);
    void (*fpSetImgCb)(ISIZE* pImgSize, USIZE* pAspectRatio);
}
VIEW_SRC;

//open DispSrv
extern void AppDisp_Init(void);
//close DispSrv
extern void AppDisp_Exit(void);
//set disp-device
extern void AppDisp_ConfigDest(UINT32 DevID, UINT32 gDevObj);
//set disp-working-buffer
extern void AppDisp_ConfigWorkingBuffer(UINT32 addr, UINT32 size);
//set disp-enable
extern void AppDisp_ConfigEnable(UINT32 DevID, BOOL bEn);
//set disp-buf-fmt
extern void AppDisp_ConfigBufFmt(UINT32 DevID, UINT32 buf_mt);
//set disp-image-size
extern void AppDisp_ConfigImgSize(UINT32 DevID, UINT32 buf_w, UINT32 buf_h, UINT32 img_ratio_w, UINT32 img_ratio_h); //buf(0,0): buf is full-device-size, img_ratio(0,0): img is full-buf-size
//set disp-window
extern void AppDisp_ConfigWindow(UINT32 DevID, UINT32 x, UINT32 y, UINT32 w, UINT32 h); //win(0,0,0,0): win is full-device-range
//set disp-attr (rotate, ...)
extern void AppDisp_ConfigAttr(UINT32 DevID, UINT32 attr, UINT32 value);
//set disp-source
extern void AppDisp_ConfigSource(VIEW_SRC* src);
//set disp-callback
extern void AppDisp_ConfigCB(void (*fpUpdateCb)(UINT32 event, UINT32 param1, UINT32 param2));
//open
extern void AppDisp_Open(void); //
//open
extern void AppDisp_SetMode(void); //alloc disp buffer
//close
extern void AppDisp_Close(void); //free disp buffer
//pause
extern void AppDisp_Pause(void); //free disp buffer
//resume
extern void AppDisp_Resume(void); //alloc disp buffer
//update
extern void AppDisp_Update(void); //update display

//get disp DIRECT(push buffer) or pull buffer(INDIRECT)
BOOL AppDisp_IsDirect(void);
//get disp enable
BOOL AppDisp_GetEnable(UINT32 DevID);
//get disp buffer size
extern USIZE AppDisp_GetBufSize(UINT32 DevID);
//get disp buffer size
extern UINT32 AppDisp_GetBufFmt(UINT32 DevID);
//get disp image size
extern USIZE AppDisp_GetImgSize(UINT32 DevID);
//get aspect ratio
extern USIZE AppDisp_GetAR(UINT32 DevID);
//get disp window
extern URECT AppDisp_GetWindow(UINT32 DevID);

/*
Struct

APP
    + DSCDISP
        + VIEWPORT -> DEVID
        + VIEWPORT -> DEVID
*/

/*
Flow

1.OpenApp/CloseApp flow
DeviceOpen()
AppDisp_SetEnable(DevID, TRUE);
AppDisp_SetBuffer(DevID, w, h, fmt);
AppDisp_SetWindow(DevID, x,y,w,h, dir);
AppDisp_SetSource(src);
AppDisp_Open(); -------- alloc disp buffer
AppOpen();
 :
AppClose();
AppDisp_Close();
DeviceClose()

2.ChangeDispDevice or ChangeDispMode flow
AppDisp_Pause();
DeviceClose()
  :
AppDisp_SetEnable(DevID, TRUE);
AppDisp_SetBuffer(DevID, w, h, fmt);
AppDisp_SetWindow(DevID, x,y,w,h, dir);
DeviceOpen()
AppDisp_Resume(); -------- alloc disp buffer

3.ChangeAppView flow
AppDisp_Pause();
  :
AppDisp_SetEnable(DevID, TRUE);
AppDisp_SetBuffer(DevID, w, h, fmt);
AppDisp_SetWindow(DevID, x,y,w,h, dir);
AppDisp_SetSource(src);
AppDisp_Resume(); -------- alloc disp buffer
*/



#endif //_DSCDISP_H_
