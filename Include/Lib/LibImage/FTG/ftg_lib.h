/**
    Face Tracking Grading library.

    @file       ftg_lib.h
    @ingroup    mILibFTG

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef FTG_LIB_H
#define FTG_LIB_H



#include "dis_lib.h"
#include "ftg_fd_lib.h"


#define ftg_max_face_life 			5
#define ftg_num_max                 (ftg_fd_num_max*ftg_max_face_life)  ///< max face number in Face Tracking Grading
#define ftg_imgseq_func_open        ENABLE ///< open or close image sequence processing function
#define ftg_calfaceangle_func       ENABLE  ///< open or close face angle calculating function
#define ftg_saveinputimg_func       ENABLE  ///< open or close saving input image function
#define ftg_fd_sensitivity_func     ENABLE  ///< open or close fd sensitivity function
#define ftg_calfaceclarity_func     ENABLE  ///< open or close face clarity calculating function
#define ftg_keypoint_match_func     ENABLE ///< open or close keypoint match function

#define   ftg_max_landmark_model_len 16*1024*1024

extern UINT32 _SECTION(".kercfg_data") FTG_SEM_ID;
extern void FTG_InstallID(void) _SECTION(".kercfg_text");

/**
     @name FTG return status
*/
//@{
#define ftg_sta_ok                  (0)                   ///<  the status is ok
#define ftg_sta_error               (-1)                  ///<  some error occurred
//@}


typedef struct _ftg_face
{
    UINT16  stx;               ///< result start x
    UINT16  sty;               ///< result start y
    UINT16  width;             ///< result width
    UINT16  height;            ///< result height
    UINT32  id;                ///< result id
    UINT32  inside_id;         ///< result inside_id
	UINT16  type;              ///< result type: 0--the current detected target, other--estimated value of previous target .
    UINT16  clarity;           ///< result clarity
    UINT16  luma;              ///< result luma
    INT32  angle_horizontal;  ///< result face horizontal angle
    INT32  angle_vertical;    ///< result face vertical angle
    INT32  angle_roll;        ///< result face roll angle
    INT32  score;             ///< result face quality reference score
    IMG_BUF *p_frame;          ///< result face corresponding frame image
} ftg_face;

/**
     Face detection input image ratio.

*/
typedef enum _ftg_imageratio
{
    ftg_imageratio_4_3 = 0,                     ///< input image ratio is 4:3
    ftg_imageratio_3_2,                         ///< input image ratio is 3:2
    ftg_imageratio_16_9,                        ///< input image ratio is 16:9
    ftg_imageratio_max_cnt,                     ///< total image ratio count
    ENUM_DUMMY4WORD(ftg_imageratio)
}ftg_imageratio;

/**
     Config ID.

*/
typedef enum
{
    ftg_cfg_rotate_eng = 0,      ///< config fd rotate engine
    ftg_cfg_copy_eng,            ///< config fd copy engine
    ftg_cfg_scale_eng,           ///< config fd scale engine
    ftg_cfg_max_face,            ///< config fd maximum face number
    ftg_cfg_face_scale,          ///< config fd the minimal face to detect releative to the maximum of image width and weight
    ftg_cfg_process_rate,        ///< config fd the process rate of frame, set 3 means 3 frames detect FD 1 time.
    ftg_cfg_sensitivity,         ///< config fd the Sensitivity of FD, set 1 means High--better hit rate, more false alarm, set 2 means Low--less hit rate, less false alarm.
	ftg_cfg_min_face_score,         ///< config accepted min face score
	ftg_cfg_max_face_life,       ///< config max face life
	ENUM_DUMMY4WORD(ftg_cfg)
} ftg_cfg;


/**
     Face results type.

*/
typedef enum _ftg_rlst_type
{
    ftg_rlst_only_loc = 0,                     ///< ftg results only with face location and input image.
    ftg_rlst_loc_clarity,                      ///< ftg results with face location, input image, clarity and luma.
    ftg_rlst_all_info,                         ///< ftg results with face location, input image, clarity, luma, angle and score.
    ftg_rlst_max_cnt,                          ///total ftg_rlst count
    ENUM_DUMMY4WORD(ftg_rlst_type)
}ftg_rlst_type;



/**
     DIS results buffer.
*/
typedef struct _ftg_dis_rlstbuf
{
	MOTION_INFOR ftg_dis_mvrlst[MVNUMMAX];
}ftg_dis_rlstbuf;



/**
     Config some settings of FTG.

     @param[in] config_id: configure ID.
     @param[in] value: setting value.

     Example:
     @code
     {
         ftg_config(FTG_CFG_SCALE_ENG, FTG_SCALE_ENG2);
     }
     @endcode
*/
extern void ftg_config(ftg_cfg config_id, INT32 value);


/**
     Get some configurations of FTG.

     @param[in] config_id: configure ID.

     @return the setting value.

     Example:
     @code
     {
         UINT32 value;
         value  = ftg_getconfig(FTG_CFG_SCALE_ENG);
     }
     @endcode
*/
extern UINT32 ftg_getconfig(ftg_cfg config_id);
/**
     Init face tracking grading function.

     This function is used to initialize face tracking grading buffer and set some initial setting.

     @param[in] buf: the work buffer for face tracking grading process. the required buffer size can be get by FTG_CalcBuffSize().
     @param[in] cachebuf: the cache work buffer for face tracking grading process. the required cache buffer size can be get by FTG_CalcCacheBuffSize().
     @param[in] maxFace: the maximum number of detected faces.
     @param[in] faceScale: the value defining the minimal face to detect releative to the maximum of image width and weight.
     @param[in] ProcessRate: the process rate of FTG_Process(), if set value 3, means calling the function FTG_Process() 3 times will run the face tracking grading once.
     @return
         - @b FTG_STA_OK:       Success.
         - @b FTG_STA_ERROR:    Some error occurred.

     Example:
     @code
     {
         MEM_RANGE buf, cachebuf;

         buf.Addr = pbuf;
         buf.Size = FTG_CalcBuffSize();
         cachebuf.Addr = pcachebuf;
         cachebuf.Size = FTG_CalcCacheBuffSize();

         // Init FTG buffer , and set max face number 10, faceScale 15, process rate as 3
         ftg_init(buf,cachebuf,10, 15, 3);
     }
     @endcode


*/
extern INT32 ftg_init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 maxface, UINT32 facescale, UINT32 processrate);

/**
     Reset face tracking grading function.

     The API will reset the face tracking grading result number and params.
     @return
         - @b ftg_sta_ok:       Success.
         - @b ftg_sta_error:    Some error occurred.
*/
extern INT32 ftg_reset(void);


/**
     UnInit face tracking grading function.

     The API will reset the face tracking grading result number and release buffer.
     @return
         - @b ftg_sta_ok:       Success.
         - @b ftg_sta_error:    Some error occurred.
*/
extern INT32 ftg_uninit(void);


/**
     Process the face tracking grading function.

     This API include face detection, face tracking and face grading

     Face detection will scale the input image to temp buffer by image ratio.\n
     The temp image buffer will be the input image of HW face detection engine FDE.\n
     After FDE process, if returned any candidate faces, each face will be verified by Novatek FD lib.\n
     If Novatek FD lib return TRUE, then we treat this candidate face as a real face, and give it a id number.\n

     Face tracking will tracking the reference real face in present frame use DIS process.\n
     If DIS process return TRUE, then we treat this face and the reference real face having same id number.\n

     Face grading will choose the best face within same id.\n

     @param[in] pInputImg: the face detection input image.
     @param[in] imgRatio: the input image w/h ratio.

     @return
         - @b >=0:       The number of faces
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
        FaceNumber = ftg_process(&InputImg, FTG_IMAGERATIO_4_3);
     }
     @endcode
*/
extern INT32 ftg_process(IMG_BUF* pinputimg, ftg_imageratio  imgratio);

/**
     Calculate face tracking grading required buffer size.

     The working buffer is used for scaling temp buffer and HW engine FDE output data.

     @return the required buffer size.
*/
extern UINT32   ftg_calcbuffsize(void);

/**
     Calculate face tracking grading required cache buffer size.

     The cache buffer size is for novatek software detection & tracking & grading using.

     @return the required cache buffer size.
*/
extern UINT32   ftg_calccachebuffsize(void);


/**
     Lock/Unlock face tracking grading.

     @param[in] bLock: lock or unlock.

     Example:
     @code
     {
         // lock the FTG
         ftg_lock(TRUE);
         // doing something such as capture
         ...
         // end of capture
         // unlock the FTG
         ftg_lock(FALSE);
     }
     @endcode
*/
extern void     ftg_lock(BOOL block);

/**
     Get the result faces of face tracking grading.

     This API will return the result faces by different coordinate.\n

     @param[out] Face: the result face info.
     @param[in] TargetCoord: the target window coordinate.

     @return The number of faces in present frame.

     Example:
     @code
     {
        FTG_FACE   FTGInfo[FTG_NUM_MAX];
        URECT      FTGDispCoord={0,0,640,480};
        UINT32     FaceNumber;

        FaceNumber = ftg_getrsltface(FTGInfo, &FTGDispCoord);
     }
     @endcode
*/
extern UINT32   ftg_getrsltface(ftg_face* face, URECT* targetcoord);
extern UINT32   ftg_getrsltface2(ftg_face* face, URECT* targetcoord, ftg_rlst_type outtype);
extern UINT32   ftg_getrsltface_for_draw(ftg_face *face, URECT* targetcoord);

/**
     Map face IDs to continuous IDs.

     @param[in] face: FTG face detection results.
     @param[in] facenumber: FTG face number.

     @return void.

     Example:
     @code
     {
        FTG_FACE   FTGInfo[FTG_NUM_MAX];
        URECT      FTGDispCoord={0,0,640,480};
        UINT32     FaceNumber;

        FaceNumber = ftg_getrsltface(FTGInfo, &FTGDispCoord, ftg_rlst_all_info);
		ftg_idmap(FTGInfo, FaceNumber);
     }
     @endcode

*/
extern void     ftg_idmap(ftg_face* face, UINT32 facenumber);


/**
     Init DIS motion vecter result buffer.

*/
//extern INT32 ftg_disrlstbuf_init(MEM_RANGE *buf, MEM_RANGE *cachebuf);



/**
     Get the number of buffer which used to save newest dis result.
*/
extern UINT32   ftg_getsavedisrlstloc(void);



/**
     Save one DIS motion vecter result.
*/
extern void   ftg_savedisrslt(void);

/**
     Get one DIS motion vecter result.
*/
extern void   ftg_getdisrslt(ftg_dis_rlstbuf *p_motresult);


/**
     Set the ROI area in ftg_fd_process.

     @param[in] use_roi: the flag used to open or close the ROI function, default close inside.
     @param[in] roi_rect: the (x,y,w,h) of the ROI area.

     @return void.

     Example:
     @code
     {
        URECT my_roirect = {0,0,640,480};
        ftg_setfdroi(TRUE, &my_roirect); //open ROI function;
        //ftg_setfdroi(FALSE, 0);        //close ROI function;
     }
     @endcode

*/
extern void     ftg_setfdroi(BOOL use_roi, URECT *roi_rect);


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
        flag = ftg_getfdroi(&now_roirect);
     }
     @endcode
*/
extern BOOL     ftg_getfdroi(URECT *roi_rect);


/**
     set landmark data to pstore, need set this value before ftg_init

     @param[in] enable: enable or disable save to pstore function



     Example:
     @code
     {
        BOOL flag = TRUE;
        ftg_enablepstore(flag);
     }
     @endcode
*/
extern void ftg_enablepstore(BOOL enable);
extern INT32 ftg_savelandmarktopstore(MEM_RANGE *buf);

#if (ftg_imgseq_func_open == ENABLE)
extern INT32     ftg_imgsequence_process(IMG_BUF* pinputimg, ftg_imageratio  imgratio, UINT32 edgeimgaddr);
#endif






#endif
