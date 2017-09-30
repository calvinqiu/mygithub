#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTToolCommand.h"
#include "FlowIPCam.h"
#include "FlowIPCamRes.c"
#include "UIFlow.h"
//#include "UIDisplay.h"
//#include "UIControlWnd.h"
//#include "UICommon.h"
#include "GxGfx.h"
#include "GxImage.h"
#include "FTypeIpcAPI.h"

#define __MODULE__          FlowIPCam
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

typedef struct _IMAGE_HEADER
{
    UINT32      w;
    UINT32      h;
    UINT32      format;
    UINT32      bpp;
    UINT32      offset;
    UINT32      size;
}IMAGE_HEADER;

typedef struct _NVTFTYPE_HDMI_OSD_IMAGE{
    unsigned int id;
    unsigned int x;
    unsigned int y;
    unsigned int font_color;
    unsigned int edge_color;
    unsigned int width;
    unsigned int height;
    unsigned int data;
}NVTFTYPE_HDMI_OSD_IMAGE;

typedef struct _NVTFTYPE_HDMI_OSD_INFO{
    unsigned int count;
    NVTFTYPE_HDMI_OSD_IMAGE image[];
}NVTFTYPE_HDMI_OSD_INFO;

NVTFTYPE_HDMI_OSD_INFO *hdmi_osd_info = NULL;

#ifdef _WIN32
#pragma pack(push,4)
#endif

#ifndef _WIN32
_ALIGNED(4)
#endif
static PALETTE_ITEM palette[256];

CTRL_LIST_BEGIN(FlowIPCam)
CTRL_LIST_ITEM(FlowIPCam_OsdFrame)
CTRL_LIST_END

//---------------------FlowIPCam_OsdFrameCtrl Control List---------------------------
CTRL_LIST_BEGIN(FlowIPCam_OsdFrame)
CTRL_LIST_END

//----------------------FlowIPCam_OsdFrame Event---------------------------
INT32 FlowIPCam_OsdFrame_OnRedraw(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(FlowIPCam_OsdFrame)
EVENT_ITEM(NVTEVT_REDRAW,FlowIPCam_OsdFrame_OnRedraw)
EVENT_END

INT32 FlowIPCam_OsdFrame_OnRedraw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    static UINT32 init = 0, color = 0xFFFFFFFF, i;
    INT32 r, g, b, y, u, v, yuv;
    DC** pDCList = (DC**)paramArray[ONEVENT_PARAM_INDEX_SCREEN];
    
#define CLAMP_TO_BYTE(v)    (((v) < 0)?0:((v>255)?255:(v)))
#define cst_prom0 21
#define cst_prom1 79
#define cst_prom2 29
#define cst_prom3 43    
#define RGB_GET_Y(r,g,b)    (((INT32)g) + ((cst_prom1 * (((INT32)r)-((INT32)g))) >> 8) + ((cst_prom2 * (((INT32)b)-((INT32)g))) >> 8) )
#define RGB_GET_U(r,g,b)    (128 + ((cst_prom3 * (((INT32)g)-((INT32)r))) >> 8) + ((((INT32)b)-((INT32)g)) >> 1) )
#define RGB_GET_V(r,g,b)    (128 + ((cst_prom0 * (((INT32)g)-((INT32)b))) >> 8) + ((((INT32)r)-((INT32)g)) >> 1) )
#define RGB_GET_YUV(r,g,b,y,u,v)    \
{   \
    y = RGB_GET_Y(r,g,b);   y = CLAMP_TO_BYTE(y);   \
    u = RGB_GET_U(r,g,b);   u = CLAMP_TO_BYTE(u);   \
    v = RGB_GET_V(r,g,b);   v = CLAMP_TO_BYTE(v);   \
}

    if(!init){
        init = 1;
        RGB_GET_YUV(0x0,0x0,0x0,y,u,v);
        yuv = COLOR_MAKE_YUVD(y,u,v);
        #define COLOR_DEMOKIT_PALETTE_00 0x00969235 //55 1F 57
        palette[0] = PALETTE_YUVA_MAKE(0xff, COLOR_DEMOKIT_PALETTE_00);
        for(i = 1 ; i < 256 ; ++i)
            palette[i] = PALETTE_YUVA_MAKE(0xff, yuv);
    }
    
    Acquire_FType_Buf();
    
    if(!hdmi_osd_info || !pDCList || !pDCList[GxGfx_OSD]){
        Release_FType_Buf();
        return NVTEVT_CONSUME;
    }
    
    if(color != hdmi_osd_info->image[0].font_color){
        color = hdmi_osd_info->image[0].font_color;
        r = COLOR_RGBD_GET_R(color);
        g = COLOR_RGBD_GET_G(color);
        b = COLOR_RGBD_GET_B(color);
        RGB_GET_YUV(r,g,b,y,u,v);
        yuv = COLOR_MAKE_YUVD(y,u,v);
        palette[255] = PALETTE_YUVA_MAKE(0xff, yuv);
        for(i=0;i<2;i++)
        {
            #if ((DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX4) ||  (DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX8))              
            GxDisplay_SetPalette(i,0,256,palette);
            GxDisplay_SetPalette(DOUT2|i,0,256,palette);
            #endif
        }
    }
    GxGfx_Clear(pDCList[GxGfx_OSD]);
    
    for(i = 0 ; i < hdmi_osd_info->count ; ++i){
        if(hdmi_osd_info->image[i].data){
            //GxGfx_SetImageStroke(ROP_KEY,0); //KEY INDEX = 0
            GxGfx_SetImageColor(palette, 0);
            GxGfx_Image(pDCList[GxGfx_OSD], 
                hdmi_osd_info->image[i].x, 
                hdmi_osd_info->image[i].y, 
                (IMAGE *)hdmi_osd_info->image[i].data);
        }else{
            GxGfx_SetImageColor(palette, 0);
            GxGfx_SetShapeColor(255, 255, 0);
            GxGfx_FillRect(pDCList[GxGfx_OSD],
                hdmi_osd_info->image[i].x, 
                hdmi_osd_info->image[i].y, 
                hdmi_osd_info->image[i].x + hdmi_osd_info->image[i].width, 
                hdmi_osd_info->image[i].y + hdmi_osd_info->image[i].height);
        }
    }
    
    hdmi_osd_info = NULL;
    
    Release_FType_Buf();
    
    return NVTEVT_CONSUME;
}

//----------------------UIFlowWndMovieCtrl Event---------------------------
INT32 FlowIPCam_OnOpen(VControl *, UINT32, UINT32 *);
INT32 FlowIPCam_OnClose(VControl *, UINT32, UINT32 *);

INT32 FlowIPCam_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_PASS;
}

INT32 FlowIPCam_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_PASS;
}

int FType_DrawHDMI(NVTFTYPE_IPC_IMAGE *image_config, UINT32 ftype_img_num, UINT32 *use_ftype_img, UINT32 *srcBuf, UINT32 bufLen)
{
    UINT32 i, total_size = 0, ImgNum, index;
    IMAGE *pImage;
    DC* pSrcDC;
    IMAGE_HEADER header;
    RESULT r;
    
    hdmi_osd_info = NULL;
    ImgNum = 0;
    total_size += sizeof(NVTFTYPE_HDMI_OSD_INFO);
    for (i = 0; i < ftype_img_num; i++)
        if(image_config[i].channel == OSD_HDMI_CHANNEL && image_config[i].on && use_ftype_img[image_config[i].channel]){
            ++ImgNum;
            if(image_config[i].bmp_ptr)
                total_size += (sizeof(IMAGE) + image_config[i].width * image_config[i].height);
                
        }
    total_size += (sizeof(NVTFTYPE_HDMI_OSD_IMAGE) * ImgNum);
    
    if(total_size > bufLen){
        DBG_ERR("HDMI OSD takes too much memory : %dbytes > %d bytes\r\n", total_size, bufLen);
        return -1;
    }
    
    hdmi_osd_info = (NVTFTYPE_HDMI_OSD_INFO*)*srcBuf;
    hdmi_osd_info->count = ImgNum;
    *srcBuf += ( sizeof(NVTFTYPE_HDMI_OSD_INFO) + (sizeof(NVTFTYPE_HDMI_OSD_IMAGE) * ImgNum) );
    
    index = 0;
    for (i = 0; i < ftype_img_num; i++){
    
        if(image_config[i].channel != OSD_HDMI_CHANNEL || !image_config[i].on || !use_ftype_img[image_config[i].channel])
            continue;
            
        memset(&hdmi_osd_info->image[index], 0, sizeof(NVTFTYPE_HDMI_OSD_IMAGE));
            
        if(image_config[i].bmp_ptr){
        
            pImage = (IMAGE *)*srcBuf;
            *srcBuf += sizeof(IMAGE);
            
            pImage->dc.uiType = TYPE_NULL;
            RECT_Set(&(pImage->rect), 0, 0, 0, 0);
            pSrcDC = &(pImage->dc);
            
            memcpy((void*)*srcBuf, (void*)((UINT32)image_config[i].bmp_ptr + (UINT32)image_config), (image_config[i].width * image_config[i].height));
            header.offset = (UINT32)*srcBuf;
            *srcBuf += (image_config[i].width * image_config[i].height);
            header.size = (image_config[i].width * image_config[i].height);
            header.w = image_config[i].width;
            header.h = image_config[i].height;
            header.format = PXLFMT_INDEX8;
            header.bpp = 8;
            r = GxGfx_AttachDC(pSrcDC, TYPE_ICON, (UINT16)header.format, header.w, header.h, 0, (UINT8*)header.offset, 0, 0);
            if(r != GX_OK){
                DBG_ERR("GxGfx_AttachDC() fail with %d\r\n", r);
                return -1;
            }
            
            RECT_SetX1Y1X2Y2(&(pImage->rect), 0, 0, image_config[i].width - 1, image_config[i].height - 1);
            pImage->uiPixelSize = 0;
            pImage->pDynamicBuf = 0;
        
            hdmi_osd_info->image[index].id = image_config[i].id;
            hdmi_osd_info->image[index].x = image_config[i].x;
            hdmi_osd_info->image[index].y = image_config[i].y;
            hdmi_osd_info->image[index].font_color = image_config[i].font_color;
            hdmi_osd_info->image[index].edge_color = image_config[i].edge_color;
            hdmi_osd_info->image[index].width = image_config[i].width;
            hdmi_osd_info->image[index].height = image_config[i].height;
            hdmi_osd_info->image[index].data = (unsigned int)pImage;
        }
        else{
            hdmi_osd_info->image[index].id = image_config[i].id;
            hdmi_osd_info->image[index].x = image_config[i].x;
            hdmi_osd_info->image[index].y = image_config[i].y;
            hdmi_osd_info->image[index].font_color = image_config[i].font_color;
            hdmi_osd_info->image[index].width = image_config[i].width;
            hdmi_osd_info->image[index].height = image_config[i].height;
        }
        
        index++;
    }
    
    return 0;
}

EVENT_BEGIN(FlowIPCam)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,FlowIPCam_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,FlowIPCam_OnClose)
EVENT_END

