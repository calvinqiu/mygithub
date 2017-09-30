/**
    Public header file for image setting information

    This file is the header file that define the API and data type for image information.

    @file       IPL_HeaderInfor_Raw.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IPL_HEADER_RAW_INFOR_H_
#define _IPL_HEADER_RAW_INFOR_H_

#include "Type.h"
#include "IPL_Utility.h"
#include "IPL_Hal_SIE_Com_Info.h"

/** \addtogroup mIIPL */
//@{

/**
     raw size structure

*/
typedef IPL_RAW_IMG_INFO IPL_HEADER_RAW_INFO;

/**
     raw exposure structure

*/
typedef struct _IPL_HEADER_RAW_EXP_INFO
{
    UINT32 iso;
    UINT32 expt;
    UINT32 iris;
} IPL_HEADER_RAW_EXP_INFO;

/**
     raw dce information

*/
typedef struct _IPL_HEADER_RAW_DCE_INFO
{
    UINT32 NFact;
    UINT32 NBit;
} IPL_HEADER_RAW_DCE_INFO;


/**
     raw header structure

*/
typedef struct _HEADER_RAW_DATA
{
    UINT32 start_tag;
    UINT32 status;
    UINT32 Proc_id;
    UINT32 frame_cnt;
    UINT32 rsc_addr;
    IPL_HEADER_RAW_INFO raw_sie_crop;
    IPL_HEADER_RAW_INFO raw_sie_ch0_out;
    IPL_HEADER_RAW_INFO raw_ipl_in;
    IPL_HEADER_RAW_EXP_INFO exp;
    IPL_HEADER_RAW_DCE_INFO dce;
    Coordinate ipl_ofs;
    iCoordinate is_ofs;
    iCoordinate rsc_gdc_cen_ofs;
    SIE_LAPARAM la_info;
    UINT32 rhe_addr;
    UINT32 flip_info;
    UINT32 timestamp;
    UINT32 end_tag;
} HEADER_RAW_DATA;

/**
    IPL raw header information selection index
*/
typedef enum _IPL_HEADER_RAW_INFOR
{
    IPL_HEADER_RAW_STATUS = 0,              ///< data type UINT32
    IPL_HEADER_RAW_PROC_ID,                 ///< data type UINT32
    IPL_HEADER_RAW_FM_CNT,                  ///< data type UINT32
    IPL_HEADER_RAW_SIE_CROP,                ///< data type IPL_HEADER_RAW_INFO
    IPL_HEADER_RAW_SIE_OUT,                 ///< data type IPL_HEADER_RAW_INFO
    IPL_HEADER_RAW_IPL_IN,                  ///< data type IPL_HEADER_RAW_INFO
    IPL_HEADER_RAW_IPL_OFS,                 ///< data type Coordinate
    IPL_HEADER_RAW_IS_OFS,                  ///< data type iCoordinate
    IPL_HEADER_RAW_EXP,                     ///< data type IPL_HEADER_RAW_EXP_INFO
    IPL_HEADER_RAW_DCE,                     ///< data type IPL_HEADER_RAW_DCE_INFO
    IPL_HEADER_RAW_RSC,                     ///< data type UINT32
    IPL_HEADER_RAW_RSC_GDC_CEN_OFS,         ///< data type iCoordinate
    IPL_HEADER_RAW_RDY,                     ///< data type NULL
    IPL_HEADER_RAW_LA,                      ///< data type SIE_LAPARAM
    IPL_HEADER_RAW_RHE,                     ///< data type UINT32
    IPL_HEADER_RAW_RSC_ON,                  ///< data type NULL
    IPL_HEADER_RAW_FLIP,                    ///< data type UINT32
    IPL_HEADER_RAW_TIMESTAMP,               ///< data type UINT32
    IPL_HEADER_RAW_INFOR_MAX_CNT,
    ENUM_DUMMY4WORD(IPL_HEADER_RAW_INFOR)
} IPL_HEADER_RAW_INFOR;

/**
    raw header status
*/
typedef enum _HEADER_RAW_STATUS
{
    HEADER_RAW_STS_PROC_ID = (1 << IPL_HEADER_RAW_PROC_ID),
    HEADER_RAW_STS_FM_CNT = (1 << IPL_HEADER_RAW_FM_CNT),
    HEADER_RAW_STS_SIE_CROP = (1 << IPL_HEADER_RAW_SIE_CROP),
    HEADER_RAW_STS_SIE_OUT = (1 << IPL_HEADER_RAW_SIE_OUT),
    HEADER_RAW_STS_IPL_IN = (1 << IPL_HEADER_RAW_IPL_IN),
    HEADER_RAW_STS_IPL_OFS = (1 << IPL_HEADER_RAW_IPL_OFS),
    HEADER_RAW_STS_IS_OFS = (1 << IPL_HEADER_RAW_IS_OFS),
    HEADER_RAW_STS_EXP = (1 << IPL_HEADER_RAW_EXP),
    HEADER_RAW_STS_DCE = (1 << IPL_HEADER_RAW_DCE),
    HEADER_RAW_STS_RSC = (1 << IPL_HEADER_RAW_RSC),
    HEADER_RAW_STS_RSC_GDC_CEN_OFS = (1 << IPL_HEADER_RAW_RSC_GDC_CEN_OFS),
    HEADER_RAW_STS_RAWRDY = (1 << IPL_HEADER_RAW_RDY),
    HEADER_RAW_STS_LA = (1 << IPL_HEADER_RAW_LA),
    HEADER_RAW_STS_RHE = (1 << IPL_HEADER_RAW_RHE),
    HEADER_RAW_STS_RSC_ON = (1 << IPL_HEADER_RAW_RSC_ON),
    HEADER_RAW_STS_FLIP = (1 << IPL_HEADER_RAW_FLIP),
    HEADER_RAW_STS_TIMESTAMP = (1 << IPL_HEADER_RAW_TIMESTAMP),
    ENUM_DUMMY4WORD(HEADER_RAW_STATUS)
} HEADER_RAW_STATUS;

/**
     IPL raw header module open

     @param[in] Id          ipl process id
     @param[in] CountIdOff
     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_HeaderRawOpen(IPL_PROC_ID Id, UINT32 CountIdOff);

/**
     IPL raw header module close

     @param[in] Id       ipl process id

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_HeaderRawClose(IPL_PROC_ID Id);

/**
     IPL create raw header

     @param[in] Id       ipl process id
     @param[in] RawAddr  raw buffer address
     @param[in] CountIdOff

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_HeaderRawCreate(IPL_PROC_ID Id, UINT32 RawAddr, UINT32 IdCounterOff);

/**
     IPL raw header module lock(avoid multi-task condition)

     @param[in] Id       ipl process id

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_HeaderRawLock(IPL_PROC_ID Id);

/**
     IPL raw header module unlock(avoid multi-task condition)

     @param[in] Id       ipl process id

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_HeaderRawUnlock(IPL_PROC_ID Id);

/**
     IPL raw header get total size

     @param[in] Id       ipl process id

     @return Description of data returned.
         - @b UINT32:    header size
*/
extern UINT32 IPL_HeaderRawGetBufSize(void);

/**
     notify IPL common to process extend information
     (NOTE: sync timing issue)

     @param[in] Id       ipl process id
     @param[in] Sts      update status

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:

*/
extern ER IPL_HeaderRawSetChkStatus(IPL_PROC_ID Id, HEADER_RAW_STATUS Sts);


/**
     notify IPL common to skip extend information
     (NOTE: sync timing issue)

     @param[in] Id       ipl process id
     @param[in] Sts      update status

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_HeaderRawClrChkStatus(IPL_PROC_ID Id, HEADER_RAW_STATUS Sts);

/**
     IPL set current header information

     @param[in] Id       ipl process id
     @param[in] Idx      raw header index
     @param[in] data     raw header data

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_HeaderRawSetInfo(IPL_PROC_ID Id, IPL_HEADER_RAW_INFOR Idx, void *data);
extern ER IPL_HeaderRawSetInfo_Dly1VD(IPL_PROC_ID Id, IPL_HEADER_RAW_INFOR Idx, void *data);
/**
     IPL get current ready status

     @param[in] Id       ipl process id
     @param[in] sts      ready status

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_HeaderRawGetSts(IPL_PROC_ID Id, HEADER_RAW_STATUS *sts);
extern ER IPL_HeaderRawGetSts_Dly1VD(IPL_PROC_ID Id, HEADER_RAW_STATUS *sts);
/**
     IPL get the current raw header information pass through IPL_PROC_ID

     @param[in] Id IPL process ID
     @param[out] data  header information

     @return Description of data returned.
         - @b E_OK:       header ready
         - @b E_SYS:      header error
*/
extern ER IPL_HeaderRawGetCurInfo(IPL_PROC_ID Id, HEADER_RAW_DATA *data);
extern ER IPL_HeaderRawGetCurInfo_Dly1VD(IPL_PROC_ID Id, HEADER_RAW_DATA *data);
//@}
#endif //_IPL_HEADER_RAW_INFOR_H_
