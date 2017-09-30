/**
    Image Stream Framework.

    A framework to integrate image data flow of all related application tasks.

    @file       ImageStream.h
    @ingroup    mIImgStm
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _IMAGESTREAM_H_
#define _IMAGESTREAM_H_

#include "Type.h"
#include "GxImage.h"

/**
    @addtogroup mIImgStm
*/
//@{


extern void ImageStream_InstallID(void) _SECTION(".kercfg_text");


/**
     @name Signature value
*/
//@{
#define ISF_SIGN                    0x0000AAAA ///< a signature is "AAAA"
//@}

/**
     @name Result value
*/
//@{
typedef enum
{
    ISF_OK                      = 0, ///< Success
    ISF_ERR_FAILED              = 1, ///< General failure
    ISF_ERR_VER_NOTMATCH        = 10, ///< version not match
    ISF_ERR_ALREADY_OPEN        = 11, ///< open is already called
    ISF_ERR_NOT_OPEN            = 12, ///< open is not called yet
    ISF_ERR_NULL_OBJECT         = 13, ///< object pointer is NULL
    ISF_ERR_NOT_SUPPORT         = 14, ///< not support operation
    ISF_ERR_NULL_FUNCCB         = 15, ///< the object has null function pointer
    ISF_ERR_NULL_POINTER        = 16, ///< parameter pointer is NULL
    ISF_ERR_INVALID_VALUE       = 17, ///< parameter value is invalid
    ISF_ERR_INVALID_PORT_ID     = 18, ///< invalid input port id
    ISF_ERR_INVALID_OBJECT      = 19, ///< invalid object
    ISF_ERR_OVERFLOW_LIST       = 20, ///< list is overflow
    ISF_ERR_CONNECT_NOTMATCH    = 21, ///< build connect step1: match connect type is failed!
    ISF_ERR_CONNECT_BINDDEST    = 22, ///< build connect step2: bind to dest unit is failed!
    ISF_ERR_CONNECT_BINDSRC     = 23, ///< build connect step3: bind to src unit is failed!
    ISF_ERR_CONNECT_CHANGED     = 24, ///< connect is changed
    ISF_ERR_CONNECT_NOTBUILD    = 25, ///< connect is not build
    ISF_ERR_INVALID_STATE       = 30, ///< invalid state
    ISF_ERR_INVALID_MAXBUF      = 32, ///< connect with invalid max-buffer config!
    ISF_ERR_NAME_NOTMATCH       = 40, ///< name match is failed!
}
ISF_RV;
//@}

/**
     @name VALUE for keep last value
*/
#define ISF_VALUE_KEEP              0xffffffff ///< keep last value

/**
     Stream operate mode.
*/
typedef enum
{
    ISF_OMD_OFF, ///< off
    ISF_OMD_PHOTOINIT, ///< photo initial
    ISF_OMD_PHOTOPRV, ///< photo preview
    ISF_OMD_PHOTOCAP, ///< photo capture start
    ISF_OMD_PHOTOCAPSTOP, ///< photo capture stop
    ISF_OMD_MOVIEINIT, ///< movie initial
    ISF_OMD_MOVIEPRV, ///< movie preview
    ISF_OMD_MOVIEREC, ///< movie record start
    ISF_OMD_MOVIERECSTOP, ///< movie record stop
    ISF_OMD_MOVIESNAPSHOT, ///< movie snapshot
    ISF_OMD_PLAYINIT, ///< playback initial
    ISF_OMD_PLAYPRV, ///< playback preview
    ISF_OMD_MAX
}
ISF_OMD;

#define ISF_OMD_BIND            0x80000000  ///< bind mode
#define ISF_OMD_PAUSE_PATH      0x40000000 ///< pause path
#define ISF_OMD_RESUME_PATH     0x20000000 ///< resume path
#define ISF_OMD_PAUSE(Path)     (0x40000000+(Path))  ///< pause path for ISF_INn or ISF_OUTn
#define ISF_OMD_RESUME(Path)    (0x20000000+(Path))  ///< resume path for ISF_INn or ISF_OUTn

/**
     Stream active mode.
*/
typedef enum
{
    ISF_AMD_NORMAL, ///< normal
    ISF_AMD_SLEEP1, ///< sleep level 1
    ISF_AMD_SLEEP2, ///< sleep level 2
    ISF_AMD_SLEEP3, ///< sleep level 3
    ISF_AMD_MAX
}
ISF_AMD;


/**
     Stream output port ID.
*/
typedef enum
{
    ISF_OUT_BASE = 0,
    ISF_OUT1 = ISF_OUT_BASE, ///< output 1
    ISF_OUT2, ///< output 2
    ISF_OUT3, ///< output 3
    ISF_OUT4, ///< output 4
    ISF_OUT5, ///< output 5
    ISF_OUT6, ///< output 6
    ISF_OUT7, ///< output 7
    ISF_OUT8, ///< output 8
    ISF_OUT9, ///< output 9
    ISF_OUT10, ///< output 10
    ISF_OUT11, ///< output 11
    ISF_OUT12, ///< output 12
    ISF_OUT_MAX,
}
ISF_OUT;

#define ISF_OUTC ISF_OUT5

/**
     Stream input port ID.
*/
typedef enum
{
    ISF_IN_BASE = 128,
    ISF_IN1 = ISF_IN_BASE, ///< input 1
    ISF_IN2, ///< input 2
    ISF_IN3, ///< input 3
    ISF_IN4, ///< input 4
    ISF_IN5, ///< input 5
    ISF_IN6, ///< input 6
    ISF_IN7, ///< input 7
    ISF_IN8, ///< input 8
    ISF_IN9, ///< input 9
    ISF_IN10, ///< input 10
    ISF_IN11, ///< input 11
    ISF_IN12, ///< input 12
    ISF_IN_MAX,
}
ISF_IN;

#define ISF_EOC     0x00f0fffc //end of cmd

/**
     Stream connect type of port.
*/
typedef enum
{
    ISF_CONNECT_NONE = 0,  ///< dummy connect type
    ISF_CONNECT_PUSH = 0x01,  ///< push connect type
    ISF_CONNECT_PULL = 0x02,  ///< pull connect type
    ISF_CONNECT_ALL = ISF_CONNECT_PUSH|ISF_CONNECT_PULL,  ///< push & pull connect type
    ISF_CONNECT_MAX,
}
ISF_CONNECT_TYPE;

/**
     Stream video raw data. (see IMG_BUF of GxImage)
*/
typedef IMG_BUF ISF_IMG_BUF;

/*
typedef struct _IMG_BUF
{
    UINT32              flag;                      ///< MAKEFOURCC('G','X','I','M')
    UINT32              Width;                     ///< image width
    UINT32              Height;                    ///< image height
    GX_IMAGE_PIXEL_FMT  PxlFmt;                    ///< pixel format
    UINT32              PxlAddr[MAX_PLANE_NUM];    ///< pixel address
    UINT32              LineOffset[MAX_PLANE_NUM]; ///< pixel lineoffset
    IPOINT              ScaleRatio;                ///< Scale ratio for virtual/Real coordinate translation
    IRECT               VirtCoord;                 ///< Virtual coordinate
    IRECT               RealCoord;                 ///< Real    coordinate
}IMG_BUF, *PIMG_BUF;
*/

/**
     Stream audio bitstream data.
*/
typedef struct _ISF_AUDIO_STREAM_BUF
{
    UINT32              flag;                      ///< MAKEFOURCC('A','S','T','M')
    UINT32              ChannelCnt;                ///< ChannelCnt
    UINT32              Resv1;                     ///< Resv
    UINT32              CodecType;                 ///< codec type
    UINT32              DataAddr;                  ///< data address
    UINT32              DataSize;                  ///< data size = SampePerSecond * ChannelCnt * BitsPerSample / 8
    UINT32              SampePerSecond;            ///< SampePerSecond
    UINT32              BitsPerSample;             ///< BitsPerSample
    UINT32              Resv[12];                  ///< 
}ISF_AUDIO_STREAM_BUF, *PISF_AUDIO_STREAM_BUF;

/**
     Stream video bitstream data.
*/
typedef struct _ISF_VIDEO_STREAM_BUF
{
    UINT32              flag;                      ///< MAKEFOURCC('V','S','T','M')
    UINT32              Width;                     ///< image width
    UINT32              Height;                    ///< image height
    UINT32              CodecType;                 ///< codec type
    UINT32              DataAddr;                  ///< data address
    UINT32              DataSize;                  ///< data size
    UINT32              FramePerSecond;            ///< FramePerSecond
    UINT32              BytePerSecond;             ///< BytePerSecond
    UINT32              Resv[12];                  ///< 
}ISF_VIDEO_STREAM_BUF, *PISF_VIDEO_STREAM_BUF;

//NOTE: for VideoCodec == H264,
//      Resv[0]=SPS addr, Resv[1]=SPS size, Resv[2]=PPS addr, Resv[3]=PPS size

/**
     Stream image info.
*/
typedef struct _ISF_IMG_INFO_
{
    UINT32 ImgFmt; ///< image format, refer to GX_IMAGE_PIXEL_FMT
    UINT32 Dirty; ///< dirty
    USIZE MaxImgSize; ///< maximum image size, set by ImageUnit_CfgImgSize() with ImageStream_SetMode(ISF_OMD_XXXXXINIT)
    USIZE ImgSize; ///< current image size, set by ImageUnit_CfgImgSize() with ImageStream_SetMode(ISF_OMD_XXXXXXXX) or ImageStream_UpdateSize()
    USIZE ImgAspect; ///< image aspect size, set by ImageUnit_CfgImgSize() with ImageStream_SetMode(ISF_OMD_XXXXXXXX) or ImageStream_UpdateSize()
    URECT Window; ///< crop or cast window range, set by ImageUnit_CfgWindow() with ImageStream_SetMode(ISF_OMD_XXXXXXXX) or ImageStream_UpdateSize()
    UINT32 Direct; ///< direction, set by ImageUnit_CfgDirect() width with ImageStream_SetMode(ISF_OMD_XXXXXXXX) or ImageStream_UpdateSize()
    UINT32 FramePerSecond; ///< frame per second, for PULL connect type, this is TargetFrameRate
}
ISF_IMG_INFO;

struct _ISF_UNIT;

/**
     Stream port.
*/
typedef struct _ISF_PORT_
{
    UINT32 iPort; ///< input-port ID of pDestUnit
    ISF_CONNECT_TYPE ConnectType; ///< connect type
    struct _ISF_UNIT* pDestUnit; ///< pointer to dest unit (cannot be NULL)
    struct _ISF_UNIT* pSrcUnit; ///< pointer to src unit (NULL if disconnect)
    ISF_IMG_INFO Info; ///< image info of this port
    UINT32 FlagCfg; ///< flag (config)
    UINT32 Flag; ///< flag
    // interface for transfer image buffer
    void (*PushImgBufToDest)(struct _ISF_PORT_* pPort, ISF_IMG_BUF* pImgBuf); ///< CB to push data to dest unit (for PUSH connect type)(provided by port-caps of pDestUnit, called by pSrcUnit)
    void (*PullImgBufFromSrc)(struct _ISF_PORT_* pPort, ISF_IMG_BUF* pImgBuf); ///< CB to pull data from src unit (for PULL connect type)(provided by port-caps of pSrcUnit, called by pDestUnit)
}
ISF_PORT;

/**
     @name flag of port
*/
#define ISF_PORT_KEEP               ((ISF_PORT*)0xffffffff) ///< keep flag
#define ISF_PORT_FLAG_AUTO          0 ///< auto trigger image data transfer (default value)
#define ISF_PORT_FLAG_PAUSE         0x00000001 ///< pause image data transfer
#define ISF_PORT_FLAG_STEP          0x00000002 ///< step trigger image data transfer
#define ISF_PORT_FLAG_CHANGE        0x00000004 ///< change parameter

/**
     @name direction of port
*/
#define ISF_PORT_DIR_NONE           0 ///< no special direction (default value)
#define ISF_PORT_DIR_MIRRORX        0x00000001 ///< mirror in x
#define ISF_PORT_DIR_MIRRORY        0x00000002 ///< mirror in y

/**
     Caps of Stream port.
*/
typedef struct _ISF_PORT_CAPS
{
    ISF_CONNECT_TYPE ConnectTypeCaps; ///< caps of connect type
    // interface for transfer image buffer
    void (*PushImgBufToDest)(struct _ISF_PORT_* pPort, ISF_IMG_BUF* pImgBuf); ///< CB to push data to dest unit (for PUSH connect type)
    void (*PullImgBufFromSrc)(struct _ISF_PORT_* pPort, ISF_IMG_BUF* pImgBuf); ///< CB to pull data from src unit (for PULL connect type)
}
ISF_PORT_CAPS;

/**
     Stream Unit.
*/
typedef struct _ISF_UNIT
{
    UINT32 Sign; ///< signature, must equal to ISF_SIGN
    CHAR* Name; ///< name string
    UINT32 ListID; ///< id of stream working list
    UINT32 nIn; ///< input-port count
    UINT32 nOut; ///< output-port count
    ISF_PORT** In; ///< input-port pointer array (active)
    ISF_PORT** Out; ///< output-port pointer array (active)
    ISF_PORT** OutCfg; ///< output-port pointer array (config)
    ISF_PORT_CAPS** InCaps; ///< input-port-caps pointer array
    ISF_PORT_CAPS** OutCaps; ///< output-port-caps pointer array
    UINT32 FlagCfg; ///< flag (config)
    UINT32 Flag; ///< flag
    UINT32 Lock; ///< lock state
    UINT32 Dirty; ///< dirty
    MEM_RANGE MemUsed; ///< used memory range
    // interface before connect
    ISF_RV (*BindInput)(struct _ISF_UNIT* pThisUnit, UINT32 iPort, struct _ISF_UNIT* pSrcUnit, UINT32 oPort); ///< CB to check input port
    ISF_RV (*BindOutput)(struct _ISF_UNIT* pThisUnit, UINT32 oPort, struct _ISF_UNIT* pDestUnit, UINT32 iPort); ///< CB to check output port
    // interface after connect
    ISF_RV (*Open)(struct _ISF_UNIT* pThisUnit, ISF_OMD mode, MEM_RANGE* pMem); ///< CB to open
    void   (*SetParam)(struct _ISF_UNIT* pThisUnit, UINT32 param, UINT32 value); ///< CB to set parameter
    UINT32 (*GetParam)(struct _ISF_UNIT* pThisUnit, UINT32 param); ///< CB to get parameter
    ISF_RV (*SetMode)(struct _ISF_UNIT* pThisUnit, ISF_OMD mode, MEM_RANGE* pMem); ///< CB to set op-mode (see ISF_OMD)
    ISF_RV (*SetActive)(struct _ISF_UNIT* pThisUnit, ISF_AMD active); ///< CB to set act-mode (see ISF_AMD)
    ISF_RV (*UpdateSize)(struct _ISF_UNIT* pThisUnit, UINT32 cmd);  ///< CB to update data size
    ISF_RV (*Close)(struct _ISF_UNIT* pThisUnit);  ///< CB to close
    void   (*Dump)(void);
}
ISF_UNIT;

/**
     @name flag of unit
*/
#define ISF_UNIT_FLAG_DEF           0 ///< default
#define ISF_UNIT_FLAG_MAXBUF        0x00000001 ///< allocate maximum buffer for this unit
#define ISF_UNIT_FLAG_PAUSE         0x00000002
#define ISF_UNIT_FLAG_USERMODE      0x00000004 ///< SetMode is controlled by other Unit (through specified SetParam cmd)


/**
     Get input-port.

     Get export input-port of unit.
     @note Nothing.

     @param[in] pUnit           pointer to unit.
     @param[in] iPort           input port ID. (see ISF_IN)
     @return                    pointer to port.
*/
extern ISF_PORT* ImageUnit_In(ISF_UNIT* pUnit, UINT32 iPort);

/**
     Get output-port.

     Get export output-port of unit.
     @note Nothing.

     @param[in] pUnit           pointer to unit.
     @param[in] oPort           output port ID. (see ISF_OUT)
     @return                    pointer to port.
*/
extern ISF_PORT* ImageUnit_Out(ISF_UNIT* pUnit, UINT32 oPort);

/**
     Get input-port-caps.

     Get export input-port-caps of unit.
     @note Nothing.

     @param[in] pUnit           pointer to unit.
     @param[in] iPort           input port ID. (see ISF_IN)
     @return                    pointer to port caps.
*/
extern ISF_PORT_CAPS* ImageUnit_InCaps(ISF_UNIT* pUnit, UINT32 iPort);

/**
     Get output-port-caps.

     Get export output-port-caps of unit.
     @note Nothing.

     @param[in] pUnit           pointer to unit.
     @param[in] oPort           input port ID. (see ISF_IN)
     @return                    pointer to port caps.
*/
extern ISF_PORT_CAPS* ImageUnit_OutCaps(ISF_UNIT* pUnit, UINT32 oPort);

/**
     Begin connect and config of unit.

     Begin connect operation and config data of unit.
     @note For connect output-port to another's input-port.

     @param[in] pUnit           pointer to unit.
     @param[in] flag            set unit flag.
*/
extern void ImageUnit_Begin(ISF_UNIT* pUnit, UINT32 flag);

/**
     End connect and config of unit.

     End connect operation and config data of unit.
     @note For connect output-port to another's input-port.
*/
extern void ImageUnit_End(void);

/**
     Connect output-port to another's input-port

     Config output-port of current unit to dest input-port of another unit.
     @note For connect output-port to another's input-port.

     @param[in] oPort           output port ID. (see ISF_OUT)
     @param[in] pDest           pointer to dest port.
     @param[in] flag            set port flag.
*/
extern void ImageUnit_CfgOut(ISF_OUT oPort, ISF_PORT* pDest, UINT32 flag);

/**
     Setup image size of port.

     Setup image size of port of current unit.
     @note For connect output-port to another's input-port.
           Call this before photo init, this is mean maxinum image size.
           Call this before movie init, this is mean maxinum image size.
           Otherwise, this is mean current image size.

     @param[in] nPort           port ID. (see ISF_IN and ISF_OUT)
     @param[in] img_w           image width.
     @param[in] img_h           image height.
*/
extern void ImageUnit_CfgImgSize(UINT32 nPort, UINT32 img_w, UINT32 img_h);

/**
     Setup image window of port.

     Setup image crop window or cast window port of current unit.
     @note For connect output-port to another's input-port.
           This is specified crop window for some input unit.
           This is specified cast window for some output unit.

     @param[in] nPort           port ID. (see ISF_IN and ISF_OUT)
     @param[in] win_x           window x-offset.
     @param[in] win_y           window y-offset.
     @param[in] win_w           window width.
     @param[in] win_h           window height.
*/
extern void ImageUnit_CfgWindow(UINT32 nPort, UINT32 win_x, UINT32 win_y, UINT32 win_w, UINT32 win_h);

/**
     Setup image aspect ratio of port.

     Setup image aspect ratio of port of current unit.
     @note For connect output-port to another's input-port.
           This is specified for some unit.

     @param[in] nPort           port ID. (see ISF_IN and ISF_OUT)
     @param[in] img_ratio_x     image aspect ratio in x direction.
     @param[in] img_ratio_y     image aspect ratio in y direction.
*/
extern void ImageUnit_CfgAspect(UINT32 nPort, UINT32 img_ratio_x, UINT32 img_ratio_y);

/**
     Setup image direction of port.

     Setup image direction of port of current unit.
     @note For connect output-port to another's input-port.
           This is specified for some unit.

     @param[in] nPort           port ID. (see ISF_IN and ISF_OUT)
     @param[in] img_dir         image direction flag.
*/
extern void ImageUnit_CfgDirect(UINT32 nPort, UINT32 img_dir);

/**
     Default rules of binding input port.

     Default rules of binding input port.
     @note Utility for implement unit.

     @param[in] pThisUnit       pointer to this unit.
     @param[in] iPort           input port ID. (see ISF_IN)
     @param[in] pSrcUnit        pointer to src unit.
     @param[in] oPort           output port ID. (see ISF_OUT)
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_Default_BindInput(struct _ISF_UNIT* pThisUnit, UINT32 iPort, struct _ISF_UNIT* pSrcUnit, UINT32 oPort);

/**
     Default rules of binding output port.

     Default rules of binding output port.
     @note Utility for implement unit.

     @param[in] pThisUnit       pointer to this unit.
     @param[in] oPort           output port ID. (see ISF_OUT)
     @param[in] pSrcUnit        pointer to dest unit.
     @param[in] iPort           input port ID. (see ISF_IN)
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_Default_BindOutput(struct _ISF_UNIT* pThisUnit, UINT32 oPort, struct _ISF_UNIT* pDestUnit, UINT32 iPort);

/**
     Default image info mapping of port.

     Default image info mapping of port.
     @note Utility for implement unit.

     @param[in] pPort           pointer to port.
     @param[in] pImgInfo        pointer to image info.
*/
extern void ImageUnit_Default_GetImgInfo(ISF_PORT* pPort, ISF_IMG_INFO* pImgInfo);

/**
     Set parameter of unit.

     Set parameter of unit.
     @note None.

     @param[in] pUnit           pointer to unit.
     @param[in] param           parameter name.
     @param[in] value           parameter value.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_SetParam(ISF_UNIT* pUnit, UINT32 param, UINT32 value);

/**
     Get parameter of unit.

     Get parameter of unit.
     @note None.

     @param[in] pUnit           pointer to unit.
     @param[in] param           parameter name.
     @return                    parameter value.
*/
extern UINT32 ImageUnit_GetParam(ISF_UNIT* pUnit, UINT32 param);

/**
     Config user dump callback of unit.

     Config user dump callback of unit.
     @note None.

     @param[in] pUnit           pointer to unit.
     @param[in] d               given dump callback function pointer.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_CfgUserDump(ISF_UNIT* pUnit, void (*d)(void));

/**
     Match name of unit.

     Match name of unit, if it is matched, return ISF_OK
     @note None.

     @param[in] pUnit           pointer to unit.
     @param[in] name            given match name.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_MatchName(ISF_UNIT* pUnit, char* name);

/**
     Match name of unit and its output units.

     Match name of unit and its output units, if any one is matched, return ISF_OK
     @note None.

     @param[in] pUnit           pointer to unit.
     @param[in] name            given match name.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_MatchOutputName(ISF_UNIT* pUnit, char* name);

/**
     Match name of unit and its input units.

     Match name of unit and its input units, if any one is matched, return ISF_OK
     @note None.

     @param[in] pUnit           pointer to unit.
     @param[in] name            given match name.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_MatchInputName(ISF_UNIT* pUnit, char* name);

/**
     Check if open stream.

     Check if open stream.
     @note This API will return ISF_OK or ISF_ERR_NOT_OPEN.

     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_IsOpen(void);

/**
     Open stream.

     Open stream.
     @note This API will register root unit of stream.

     @param[in] pRootUnit       pointer to root unit.
     @param[in] pMem            pointer to free memory range.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_Open(ISF_UNIT* pRootUnit, MEM_RANGE* pMem);

/**
     Close stream.

     Close stream.
     @note This API will force wakeup all units in stream.
           This API will dispatch ISF_OMD_OFF cmd to all units in stream.
           This API will break connect between all units in stream.

     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_Close(void);

/**
     Set operate mode of stream.

     Set operate mode of stream.
     @note This API will break connect between all units in stream (if already build).
           This API will build connect between all units in stream.
           This API will force wakeup all units in stream.
           This API will dispatch ISF_OMD_XXX cmd to all units in stream.

     @param[in] OpMode          operate mode. (see ISF_OMD)
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_SetMode(ISF_OMD OpMode);

/**
     Set active mode of stream.

     Set active mode of stream.
     @note This API will dispatch ISF_AMD_XXX cmd to all units in stream.

     @param[in] ActMode         active mode. (see ISF_AMD)
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_SetActive(ISF_AMD ActMode);

/**
     Update image-size, aspect ration and window of stream.

     Update image-size, aspect ration and window of stream.
     @note This API will force wakeup all units in stream.
           This API will dispatch update-size to all units in stream.

     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_UpdateSize(void);



/**
     Update all of stream.

     Update all of stream.
     @note This API will force wakeup all units in stream.
           This API will dispatch update-output to all units in stream.

     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_UpdateAll(void);



/**
     Dump debug info of stream.

     Dump debug info of stream.
     @note This will dump 3 thing of all units in stream:
           (1) connect relationship, (2) used memory range, (3) port image info.
*/
extern void ImageStream_DumpInfo(void);

/**
     Dump debug info of stream.

     Dump debug info of stream.
     @note This will dump this of all units in stream:
           (1) used memory range of POOL_ID_APP
*/
extern void ImageStream_DumpInfo2(void);

/**
     Dump debug info of stream.

     Dump debug info of stream.
     @note This will dump these of all units in stream:
           (1) used memory range of POOL_ID_APP (2) used memory range of other POOL_ID
*/
extern void ImageStream_DumpInfo3(void);


extern void ImageUnit_DumpPort(ISF_UNIT* pUnit, UINT32 nPort);

/**
     Get free memory info of stream.

     Get free memory info of stream.
     
     @return                    memory range of free space.
*/
extern MEM_RANGE* ImageStream_GetFreeBuffer(void);

//@}

#endif //_IMAGESTREAM_H_
