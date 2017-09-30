/**
    Face detection library used by FTG.

    @file       ftg_fd_lib.h
    @ingroup    mILibFTG

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef _FTG_FD_H
#define _FTG_FD_H

#include    "GxImage.h"
#include    "fde_lib.h"

/**
    @addtogroup mILibFTG
*/

#define ftg_fd_num_max                    36       ///< max face detection number


/**
     @name FD return status
*/
//@{
#define ftg_fd_sta_ok                  (0)                   ///<  the status is ok
#define ftg_fd_sta_error               (-1)                  ///<  some error occurred
//@}



#define   ftg_min_fd_accept_score 33554432

/**
     Face detection input image ratio.

*/
typedef enum _ftg_fd_imageratio
{
    ftg_fd_imageratio_4_3 = 0,                     ///< input image ratio is 4:3
    ftg_fd_imageratio_3_2,                         ///< input image ratio is 3:2
    ftg_fd_imageratio_16_9,                        ///< input image ratio is 16:9
    ftg_fd_imageratio_max_cnt,                     ///< total image ratio count
    ENUM_DUMMY4WORD(ftg_fd_imageratio)
}ftg_fd_imageratio;

/**
    Face Orientation of detected
*/
typedef enum _ftg_face_orient
{
    ftg_fd_orient_degree_0    = 0,                      ///< face orient is degree 0
    ftg_fd_orient_degree_90   = 1,                      ///< face orient is degree 90
    ftg_fd_orient_degree_270  = 2,                      ///< face orient is degree 270
    ftg_fd_orient_degree_180  = 3,                      ///< face orient is degree 180
    ENUM_DUMMY4WORD(ftg_face_orient)
}ftg_face_orient;


/**
     @name Face Orientation of supported
*/
//@{
#define ftg_fd_support_orient_0        0x0001                   ///<  support face orient degree 0
#define ftg_fd_support_orient_90       0x0002                   ///<  support face orient degree 90
#define ftg_fd_support_orient_270      0x0004                   ///<  support face orient degree 270
#define ftg_fd_support_orient_180      0x0008                   ///<  support face orient degree 180
//@}


typedef struct _ftg_fd_face
{
    UINT16  x;               ///< result start x
    UINT16  y;               ///< result start y
    UINT16  w;               ///< result width
    UINT16  h;               ///< result height
    UINT8   trgidx;           ///< trigger index
} ftg_fd_face, *p_ftg_fd_face;

/**
     Face detection result info.

*/
typedef struct _ftg_fdmsg_info
{
    UINT32      facedetectnum;                   ///<Detected total face number
    ftg_fd_face     facedetectrect[FACE_NUM_MAX*2];      ///<Detected face rectangle
} ftg_fdmsg_info, *p_ftg_fdmsg_info;

/**
     Face detection callback message.

*/
typedef enum
{
    ftg_fd_cbmsg_detend = 0,        ///< user will receive this notification when face detection process is end.
    ftg_fd_cbmsg_max_cnt,           ///< total callback message count
    ENUM_DUMMY4WORD(ftg_fd_cbmsg)
} ftg_fd_cbmsg;

/**
     Rotation engine ID.

*/
typedef enum
{
    ftg_fd_rotate_eng1 = 0,         ///< Rotation Engine 1
    ftg_fd_rotate_eng2,             ///< Rotation Engine 2
    ENUM_DUMMY4WORD(ftg_fd_rotate_eng)
} ftg_fd_rotate_eng;

/**
     Copy Engine ID.
*/
typedef enum
{
    ftg_fd_copy_eng1 = 0,           ///< Copy Engine 1
    ftg_fd_copy_eng2,               ///< Copy Engine 2
    ENUM_DUMMY4WORD(ftg_fd_copy_eng)
} ftg_fd_copy_eng;

/**
     Scale engine ID.

*/
typedef enum
{
    ftg_fd_scale_eng1 = 0,          ///< Scale Engine 1 (scale by FD)
    ftg_fd_scale_eng2,              ///< Scale Engine 2 (scale by ISE)
    ENUM_DUMMY4WORD(ftg_fd_scale_eng)
} ftg_fd_scale_eng;


/**
     Config ID.

*/
typedef enum
{
    ftg_fd_cfg_rotate_eng = 0,      ///< config rotate engine
    ftg_fd_cfg_copy_eng,            ///< config copy engine
    ftg_fd_cfg_scale_eng,           ///< config scale engine
    ftg_fd_cfg_max_face,            ///< config maximum face number
    ftg_fd_cfg_face_scale,          ///< config the minimal face to detect releative to the maximum of image width and weight
    ftg_fd_cfg_process_rate,        ///< config the process rate of frame, set 3 means 3 frames detect FD 1 time.
    ftg_fd_cfg_sensitivity,         ///< config the Sensitivity of FD, set 1 means High--better hit rate, more false alarm, set 2 means Low--less hit rate, less false alarm.
	ENUM_DUMMY4WORD(ftg_fd_cfg)
} ftg_fd_cfg;


/**
    FD callback funcion proto type
*/
typedef void (*ftg_fd_cbmsg_fp)(ftg_fd_cbmsg uimsg_id, void *data);



/**
     Config some settings of FD.

     @param[in] config_id: configure ID.
     @param[in] value: setting value.

     Example:
     @code
     {
         ftg_fd_config(fd_cfg_scale_eng, fd_scale_eng2);
     }
     @endcode
*/
extern void ftg_fd_config(ftg_fd_cfg config_id, UINT32 value);


/**
     Get some configurations of FD.

     @param[in] config_id: configure ID.

     @return the setting value.

     Example:
     @code
     {
         UINT32 value;
         value  = ftg_fd_getconfig(fd_cfg_scale_eng);
     }
     @endcode
*/
extern UINT32 ftg_fd_getconfig(ftg_fd_cfg config_id);
/**
     Init Face detection function.

     This function is used to initialize face detection buffer and set some initial setting.

     @param[in] buf: the work buffer for face detection process. the required buffer size can be get by FD_CalcBuffSize().
     @param[in] cachebuf: the cache work buffer for face detection process. the required cache buffer size can be get by FD_CalcCacheBuffSize().
     @param[in] maxFace: the maximum number of detected faces.
     @param[in] faceScale: the value defining the minimal face to detect releative to the maximum of image width and weight.
     @param[in] ProcessRate: the process rate of FD_Process(), if set value 3, means calling the function FD_Process() 3 times will run the face detection once.
     @return
         - @b ftg_fd_sta_ok:       Success.
         - @b ftg_fd_sta_error:    Some error occurred.

     Example:
     @code
     {
         MEM_RANGE buf, cachebuf;

         buf.Addr = pbuf;
         buf.Size = ftg_fd_calcbuffsize();
         cachebuf.Addr = pcachebuf;
         cachebuf.Size = ftg_fd_calccachebuffsize();

         // Init FD buffer , and set max face number 10, faceScale 15, process rate as 3
         ftg_fd_init(buf,cachebuf,10, 15, 3);
     }
     @endcode


*/
extern INT32 ftg_fd_init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 maxface, UINT32 facescale, UINT32 processrate);

/**
     UnInit face detection function.

     The API will reset the face detection result number and release buffer.
     @return
         - @b ftg_fd_sta_ok:       Success.
         - @b ftg_fd_sta_error:    Some error occurred.
*/
extern INT32 ftg_fd_uninit(void);
/**
     Process the face detection function.

     This API will scale the input image to temp buffer by image ratio.\n
     The temp image buffer will be the input image of HW face detection engine FDE.\n
     After FDE process, if returned any candidate faces, each face will be verified by Arcsoft FD lib.\n
     If Arcsoft FD lib return TRUE, then we treat this candidate face as a real face, and the face number count add 1.\n
     This API also uses the face tracking to enhance the stability of face detection.

     @note The process function will really run the face detection when FD is unlocked. It will just returned when FD is locked.\n
           User can lock/unlock the FD function by calling FD_Lock().\n

     @param[in] pInputImg: the face detection input image.
     @param[in] imgRatio: the input image w/h ratio.

     @return
         - @b >=0:       The number of detected faces
         - @b <0:        Some error occurred.

     Example:
     @code
     {
        IMG_BUF          InputImg;
        IPL_IME_BUF_ADDR CurInfo;
        UINT32           srcW,srcH,PxlFmt;
        UINT32           PxlAddr[3];
        UINT32           LineOff[3];
        INT32            FaceNumber;

        CurInfo.Id = IPL_ID_1;
        IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);
        if (CurInfo.ImeP2.type == IPL_IMG_Y_PACK_UV422)
        {
            PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
        }
        else
        {
            PxlFmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
        }
        srcW = CurInfo.ImeP2.Ch[0].Width;
        srcH = CurInfo.ImeP2.Ch[0].Height;
        LineOff[0] = CurInfo.ImeP2.Ch[0].LineOfs;
        LineOff[1] = CurInfo.ImeP2.Ch[1].LineOfs;
        LineOff[2] = CurInfo.ImeP2.Ch[2].LineOfs;
        PxlAddr[0] = CurInfo.ImeP2.PixelAddr[0];
        PxlAddr[1] = CurInfo.ImeP2.PixelAddr[1];
        PxlAddr[2] = CurInfo.ImeP2.PixelAddr[2];
        GxImg_InitBufEx(&InputImg, srcW, srcH, PxlFmt, LineOff, PxlAddr);
        FaceNumber = ftg_fd_process(&InputImg, FTG_FD_IMAGERATIO_4_3);
     }
     @endcode
*/
extern INT32 ftg_fd_process(IMG_BUF* pinputimg, ftg_fd_imageratio  imgratio);


/**
     Detect face of one picture.

     This API will detect the face of one input image buffer.

     @param[in] pImgBuf: the input image buffer.
     @param[in] FDBuffAddr: the work buffer for face detection process. the buffer size can be get by FD_CalcBuffSize().
     @param[in] FDCacheBuffAddr: the cache buffer for face detection process. the cache buffer size can be get by FD_CalcCacheBuffSize().
     @param[out] Face_rslt: the result face rectangle, the array size should be FD_NUM_MAX.

     @return the face number detected.

     Example:
     @code
     {
         UINT32  NumFaces, WorkMem, CacheMem;
         IMG_BUF g_TmpImgBuf;
         FACE    Face_rslt[FD_NUM_MAX];


         // prepare input image buffer
         g_TmpImgBuf = ...
         // assign work buffer
         WorkMem = ...
         // assing cache buffer
         CacheMem = ...
         NumFaces = ftg_fd_detectonepic(&g_TmpImgBuf, WorkMem, CacheMem, Face_rslt);
     }
     @endcode

*/
extern UINT32 ftg_fd_detectonepic(PIMG_BUF pimgbuf, UINT32 fdbuffaddr, UINT32 fdcachebuffaddr,ftg_fd_face *face_rslt);

/**
     Calculate face detection required buffer size.

     The working buffer is used for scaling temp buffer and HW engine FDE output data.

     @return the required buffer size.
*/
extern UINT32   ftg_fd_calcbuffsize(void);

/**
     Calculate face detection required cache buffer size.

     The cache buffer size is for arcsoft lib using.

     @return the required cache buffer size.
*/
extern UINT32   ftg_fd_calccachebuffsize(void);

/**
     Register callback function.

     @note User must register callback funcion then can receive the callback message notification when detected faces.

     @param[in] CB: the callback function pointer.

     Example:
     @code
     {
        void Photo_FdCB(ftg_fd_cbmsg uiMsgID, void *Data)
        {
            switch (uiMsgID)
            {
                case ftg_fd_cbmsg_detend:
                   DBG_IND("===FDEND===\r\n");
                   Ux_PostEvent(NVTEVT_ALGMSG_FDEND,1, NVTEVT_ALGMSG_FDEND);
                   break;
                default:
                   break;
            }
        }
        ftg_fd_registercb(Photo_FdCB);
     }
     @endcode
*/
extern void     ftg_fd_registercb(ftg_fd_cbmsg_fp cb);

/**
     Lock/Unlock face detction.

     @param[in] bLock: lock or unlock.

     Example:
     @code
     {
         // lock the FD
         ftg_fd_lock(TRUE);
         // doing something such as capture
         ...
         // end of capture
         // unlock the FD
         ftg_fd_lock(FALSE);
     }
     @endcode
*/
extern void     ftg_fd_lock(BOOL block);


/**
     Clear the face number.

     This API will clear the result face number of face detection.

*/
extern void     ftg_fd_clrrsltfacenum(void);

/**
     Get the face number.

     This API will return the result face number of face detection.

     @return The number of detected faces.
*/
extern UINT32   ftg_fd_getrsltfacenum(void);


/**
     Get the result faces of different coordinate.

     This API will return the result faces by different coordinate.\n

     @param[out] Face: the result face info.
     @param[in] TargetCoord: the target window coordinate.

     @return The number of detected faces.

     Example:
     @code
     {
        ftg_fd_face    FdInfo[FD_NUM_MAX];
        URECT      FdDispCoord={0,0,640,480};
        UINT32     FaceNumber;

        FaceNumber = ftg_fd_getrsltface(FdInfo, &FdDispCoord);
     }
     @endcode
*/
extern UINT32   ftg_fd_getrsltface(ftg_fd_face* face, URECT* targetcoord);


/**
     Get the face orientation.

     @return The face orientation.
*/
extern ftg_face_orient   ftg_fd_getfaceorient(void);


/**
     Set supported face orientations.

     The API will set the supported face orientation of FD engine.\n
     The orientations that FD engine supported are 0, 90, 270, 180, user can configure which orientation to support.\n
     The default setting of supported orientations are 0, 90, 270.

     @return void

     Example:
     @code
     {
         UINT32  SupportOrient = FD_SUPPORT_ORIENT_0|FD_SUPPORT_ORIENT_90|FD_SUPPORT_ORIENT_270;

         ftg_fd_setsupportfaceorient(SupportOrient);
     }
     @endcode
*/
extern void   ftg_fd_setsupportfaceorient(UINT32   faceorient);

/**
     Get the FTG_FD_Done flag to judge if the FTG_FD process finished.

     @return the FTG_FD_Done flag.

     @return
         - @b == 0:       1.The FTG_FD is in process.
                       or 2.The FTG_FD process finished and the results have been got.
         - @b == 1:       1.The FTG_FD process finished but the results have not been got.
*/
extern UINT32   ftg_fd_getfddoneflag(void);


/**
     Get the FTG_FD_IsInProcess flag to judge if the FTG_FD is in process.

     @return the FTG_FD_IsInProcess flag.
     
     @return
         - @b == 0:       1.The FTG_FD is not in process.                       
         - @b == 1:       1.The FTG_FD is in process. 
*/
extern UINT32   ftg_fd_getfdisinprocessflag(void);


/**
     Get the buffer number of input image dis result.

     @return ftg_fd_inputimgdisloc.
     
*/
extern UINT32   ftg_fd_getinputimgdisloc(void);


/**
     Set the ROI area in ftg_fd_process.

     @param[in] use_roi: the flag used to open or close the ROI function, default close inside.
     @param[in] roi_rect: the (x,y,w,h) of the ROI area.

     @return void.

     Example:
     @code
     {
        URECT my_roirect = {0,0,640,480};
        ftg_fd_setfdroi(TRUE, &my_roirect); //open ROI function;
        //ftg_fd_setfdroi(FALSE, 0);        //close ROI function;
     }
     @endcode

*/
extern void     ftg_fd_setfdroi(BOOL use_roi, URECT *roi_rect);


/**
     Get the ROI area in ftg_fd_process.

     @param[out] roi_rect: the (x,y,w,h) of the ROI area.

     @return
         - @b == TRUE:       1.The ROI function is opened.                       
         - @b == FALSE:      1.The ROI function is closed.

     Example:
     @code
     {
        URECT now_roirect = {0};
        BOOL flag;
        flag = ftg_fd_getfdroi(&now_roirect);
     }
     @endcode
*/
extern BOOL     ftg_fd_getfdroi(URECT *roi_rect);



#endif

