/**
    SRaw encoder applications.

    To encode YUV raw data to JPEG during movie recording.

    @file       VSRawEncAPI.h
    @ingroup    mIAPPSMEDIAREC
    @note

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _VSMRAWRECAPI_H
#define _VSMRAWRECAPI_H

/**
    @addtogroup mIAPPSMEDIAREC
*/
//@{



/**
    Changeable parameters of raw encoder.

    Changeable parameters of raw encoder.
    @note
*/
typedef enum
{
    VSMRAWENC_JPEG_WIDTH   = 0x0,  ///< output JPEG width, p1: width(in), p2: fileid (in)
    VSMRAWENC_JPEG_HEIGHT  = 0x1,  ///< output JPEG height, p1: height(in), p2: fileid (in)
    VSMRAWENC_Q            = 0x2,  ///< input encoding quality, p1: Q(in, 0~100), p2: fileid (in)
    VSMRAWENC_SRC_WIDTH    = 0x3,  ///< source width, p1: width(in), p2: fileid (in)
    VSMRAWENC_SRC_HEIGHT   = 0x4,  ///< source height, p1: width(in), p2: fileid (in)
    ENUM_DUMMY4WORD(VSMRAW_CHANGE_TYPE)
} VSMRAW_CHANGE_TYPE;

typedef enum
{
    VSMRAW_FORMAT_PATH1 = 0x0,   ///< fileid 0
    VSMRAW_FORMAT_PATH2 = 0x1,   ///< fileid 1
    ENUM_DUMMY4WORD(VSMRAW_TRIGENC_PATH)
} VSMRAW_TRIGENC_PATH;

typedef enum
{
    VSMRAW_FORMAT_ONEPATH = 0x0,   ///< with fileid SMRAW_FORMAT_PATH1/SMRAW_FORMAT_PATH2
    VSMRAW_FORMAT_ALLPATH = 0x1,   ///< valid, if multi-recording
    ENUM_DUMMY4WORD(VSMRAW_TRIGENC_TYPE)
} VSMRAW_TRIGENC_TYPE;

/**
    Export parameters of raw encoder.

    Export parameters of raw encoder.
    @note
*/
typedef struct
{
    //BOOL                bEnable;            ///< enable raw encode or not
    //UINT32              uiMemAddr;          ///< starting address for recording
    //UINT32              uiMemSize;          ///< size for recording
    //UINT32              uiMaxWidth;         ///< max encode pic width
    //UINT32              uiMaxHeight;        ///< max encode pic height
    //UINT32              uiJpegWidth;        ///< output JPEG width
    //UINT32              uiJpegHeight;       ///< output JPEG height
    UINT32              uiEncQ;             ///< input encoding Q
    //RAW_FORMAT_TYPE     uiRawFormat;        ///< raw format, RAW_FORMAT_444 or others
    //RawEncCallbackType  *CallBackFunc;      ///< event inform callback, remove 2012/08/30
} VSMRAW_ENC_DATA;

typedef struct
{
    UINT32              uiWidth;            ///< image width
    UINT32              uiHeight;           ///< image height
    UINT32              uiYAddr;            ///< image Y address
    UINT32              uiUVAddr;           ///< image UV address
    UINT32              uiYLineOffset;      ///< image Y line-offset
    UINT32              uiUVLineOffset;     ///< image UV line-offset
} VSMRAW_ENC_YUV;

//
//  Export APIs
//

/**
    Change raw encoding parameters.

    Change raw encoding parameters, such as width, height, encoding Q value.
    @note

    @param[in] type parameter type
    @param[in] p1 parameter1
    @param[in] p2 parameter2
    @param[in] p3 parameter3
    @return
        - @b E_OK:  change successfully.
*/
extern ER VSMRawEnc_ChangeParameter(VSMRAW_CHANGE_TYPE type, UINT32 p1, UINT32 p2, UINT32 p3) __attribute__ ((weak));

/**
    Trigger raw encoding.

    Trigger YUV raw encoding to output JPEG file.
    @note

    @return
        - @b SMRAWENC_OK:                     encoding successfully
        - @b SMRAWENC_ERR_NOT_OPEN:           raw encode task is not opened
        - @b SMRAWENC_ERR_BUSY:               raw encode task is busy
        - @b SMRAWENC_ERR_IMG_NOT_READY:      YUV raw image is not ready
        - @b SMRAWENC_ERR_UNSAFE_TO_WRITE:    memory may be not enough for writing JPEG file

    Example:
    @code
    {
        // Example for applying raw encoding function
        SMRawEnc_ChangeParameter(RAWENC_JPEG_WIDTH,  1920, 0, 0);
        SMRawEnc_ChangeParameter(RAWENC_JPEG_HEIGHT, 1080, 0, 0);
        SMRawEnc_TriggerEncode();
    }
    @endcode
*/
extern INT32 VSMRawEnc_TriggerEncode(VSMRAW_TRIGENC_TYPE type, UINT32 pathID) __attribute__ ((weak));

/**
    Get last rawencoding JPG data.

    @param[in] puiAddr output JPG addr
    @param[in] puiSize output JPG size
    @param[in] fileID 0:path0, 1:path1
*/
extern void VSMRawEnc_GetJpgData(UINT32 *puiAddr, UINT32 *puiSize, UINT32 fileid) __attribute__ ((weak));

//extern void     SMRawEnc_GetScreenYUV(SMRAW_ENC_YUV *pYuv);

//@}
#endif //_VSMRAWRECAPI_H
