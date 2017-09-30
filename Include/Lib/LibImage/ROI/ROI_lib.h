/**
    ROI filter library functions.

    Filtering non-ROI area of images to increase MPEG-4 compression rate.
    Rotating the filtered image. (Optional)

    @file       ROI_lib.h
    @ingroup    ROI
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef _ROI_LIB_H
#define _ROI_LIB_H

#include <Type.h>
#include "IPL_Utility.h"

/**
    Definitions.
*/
#define ROI_MIN_LEVEL    1  ///< minimum filtering level (weakest)
#define ROI_MAX_LEVEL   10  ///< maximum filtering level (strongest)

#define ROI_NUM_PATH    3   ///< number of image paths

/**
    Return status.
*/
typedef enum
{
    ROI_STA_OK                  = 0,    ///< the status is OK
    ROI_STA_ERROR               = -1,   ///< some error occurred
    ROI_STA_INVALID_PARAM_TYPE  = -2,   ///< invalid parameter type
    ROI_STA_INVALID_PARAM_VALUE = -3,   ///< invalid parameter value
    ROI_STA_INVALID_PATH        = -4,   ///< invalid image path

    ENUM_DUMMY4WORD(ROI_STA)
} ROI_STA;

/**
    Image rotation degrees.
*/
typedef enum
{
    ROI_ROTATE_0,           ///< no rotation
    ROI_ROTATE_90,          ///< 90 degree clockwise rotation
    ROI_ROTATE_270,         ///< 90 degree counterclockwise rotation

    ROI_ROTATE_MAX,
    ENUM_DUMMY4WORD(ROI_ROTATE_DEGREE)
} ROI_ROTATE_DEGREE;

/**
    @name Processing Parameters

    Parameters of filter processing.

    - @b ROI_PARAM_ENABLE:      enable the filter (0: No; 1: Yes)
    - @b ROI_PARAM_ROI_X:       x coordinate of ROI (Y plane)
    - @b ROI_PARAM_ROI_Y:   	y coordinate of ROI (Y plane)
    - @b ROI_PARAM_ROI_W:   	width of ROI        (Y plane)
    - @b ROI_PARAM_ROI_H:   	height of ROI       (Y plane)
    - @b ROI_PARAM_LEVEL:   	level of filter strength (1~10, weak to strong)
    - @b ROI_PARAM_DEGREE:  	rotation degrees in ROI_ROTATE_DEGREE
    - @b ROI_PARAM_DRAW_ROI:	whether drawing the ROI or not (0: No; 1: Yes)
    - @b ROI_PARAM_... :        parameters of image path 1
    - @b ROI_PARAM_...2:        parameters of image path 2
    - @b ROI_PARAM_...3:        parameters of image path 3
*/
#define ROI_PARAM_SETTING_FIELDS                    \
    X(ROI_PARAM_ENABLE,     UINT32, 1)              \
    X(ROI_PARAM_ROI_X,       INT32, -1)             \
    X(ROI_PARAM_ROI_Y,       INT32, -1)             \
    X(ROI_PARAM_ROI_W,      UINT32, 0)              \
    X(ROI_PARAM_ROI_H,      UINT32, 0)              \
    X(ROI_PARAM_LEVEL,      UINT32, ROI_MAX_LEVEL)  \
    X(ROI_PARAM_DEGREE,     UINT32, ROI_ROTATE_0)   \
    X(ROI_PARAM_DRAW_ROI,   UINT32, 0)              \
    X(ROI_PARAM_ENABLE2,    UINT32, 1)              \
    X(ROI_PARAM_ROI_X2,      INT32, -1)             \
    X(ROI_PARAM_ROI_Y2,      INT32, -1)             \
    X(ROI_PARAM_ROI_W2,     UINT32, 0)              \
    X(ROI_PARAM_ROI_H2,     UINT32, 0)              \
    X(ROI_PARAM_LEVEL2,     UINT32, ROI_MAX_LEVEL)  \
    X(ROI_PARAM_DEGREE2,    UINT32, ROI_ROTATE_0)   \
    X(ROI_PARAM_DRAW_ROI2,  UINT32, 0)              \
    X(ROI_PARAM_ENABLE3,    UINT32, 1)              \
    X(ROI_PARAM_ROI_X3,      INT32, -1)             \
    X(ROI_PARAM_ROI_Y3,      INT32, -1)             \
    X(ROI_PARAM_ROI_W3,     UINT32, 0)              \
    X(ROI_PARAM_ROI_H3,     UINT32, 0)              \
    X(ROI_PARAM_LEVEL3,     UINT32, ROI_MAX_LEVEL)  \
    X(ROI_PARAM_DEGREE3,    UINT32, ROI_ROTATE_0)   \
    X(ROI_PARAM_DRAW_ROI3,  UINT32, 0)

/**
    Parameters of filter processing.
*/
typedef enum
{
#define X(param, type, def_val) param,
    ROI_PARAM_SETTING_FIELDS
#undef X
} ROI_PARAM;

/**
    Parameters of a single image path.
*/
typedef struct
{
    UINT32 uiEnable;    ///< enable the filter (0: No; 1: Yes)
     INT32  iRoiX;      ///< x coordinate of ROI (Y plane)
     INT32  iRoiY;      ///< y coordinate of ROI (Y plane)
    UINT32 uiRoiW;      ///< width of ROI        (Y plane)
    UINT32 uiRoiH;      ///< height of ROI       (Y plane)
    UINT32 uiLevel;     ///< level of filter strength (1~10, weak to strong)
    UINT32 uiDegree;    ///< rotation degrees in ROI_ROTATE_DEGREE
    UINT32 uiDrawRoi;   ///< whether drawing the ROI or not (0: No; 1: Yes)
} ROI_PATH_PARAMS;


/**
    Calculate the required working buffer size.

    @return the required working buffer size
*/
UINT32 ROI_CalcBuffSize(VOID);

/**
    Initiate the ROI filter.

    @param[in] pWrkBuff: working buffer. The required buffer size can be get by ROI_CalcBuffSize().

    @return the result of initiating
*/
ROI_STA ROI_Init(MEM_RANGE *pWrkBuff);

/**
    Set one of the parameters defined by ROI_PARAM_SETTING_FIELDS.
    The fields are defined as:
    X(@Name, @Type, @DefaultValue)

    @param[in] iParamType: same as X(@Name)
    @param[in] iParamValue: parameter value;
                       default value is defined by X(@DefaultValue)

    @return the result of setting the parameter
*/
ROI_STA ROI_SetParam(ROI_PARAM iParamType, INT32 iParamValue);

/**
    Get one of the parameters defined by ROI_PARAM_SETTING_FIELDS.
    The fields are defined as:
    X(@Name, @Type, @DefaultValue)

    @param[in] iParamType: same as X(@Name)

    @return the value of X(@Name); default is X(@DefaultValue)
*/
INT32 ROI_GetParam(ROI_PARAM iParamType);

/**
    Set the parameters of a single image path.

    @param[in] uiPath: image path (1~3)
    @param[in] pParams: parameter values

    @return the result of setting the parameters
*/
ROI_STA ROI_SetPathParam(UINT32 uiPath, ROI_PATH_PARAMS *pParams);

/**
    Start processing images.

    @return the result of start
*/
ROI_STA ROI_Start(VOID);

/**
    Stop processing images.

    @return the result of stop
*/
ROI_STA ROI_Stop(VOID);

/**
    Query whether the ROI filter is running.

    @return the current state
        - @b TRUE: is running (started)
        - @b FALSE: is not running (stopped)
*/
BOOL ROI_GetRunState(VOID);

/**
    Start performance measurement.

    @return the result of start
*/
ROI_STA ROI_PerfStart(VOID);

/**
    Stop performance measurement and print the average time per frame.

    @return the result of stop
*/
ROI_STA ROI_PerfStop(VOID);

/**
    Request the ROI filter to process an image.
    Callback function called by IPL.

    @param[in] uiImePath: IME path (0~2)
    @param[in] pSrcImg: source image
    @param[in] pDstImg: destination image

    @return the result of the request
        - @b 0: OK
        - @b 1: some error occurred
*/
UINT32 ROI_ProcessCb(UINT32 uiImePath, IPL_YCC_IMG_INFO *pSrcImg, IPL_YCC_IMG_INFO *pDstImg);

#endif  //_ROI_LIB_H
