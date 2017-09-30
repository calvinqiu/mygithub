/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2015.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: DIS Library Module                                                  *
* Description:                                                             *
* Author: Connie Yu                                                         *
****************************************************************************/

/**
    DIS lib

    Sample module detailed description.

    @file       dis_alg.h
    @ingroup    mILibDIS
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

/** \addtogroup DISLib */
//@{

#ifndef _DIS_ALG_H_
#define _DIS_ALG_H_

#include "dis_alg_fw.h"

/**
    DIS Motion vector information

    Structure of DIS motion vector information.
*/
//@{
typedef struct _MV_INFO_SIZE_STRUCT
{
    MOTION_INFOR *motVec;
    UINT32 uiNumH;
    UINT32 uiNumV;
} MV_INFO_SIZE;
//@}

/**
    DIS Process Configuration

    Structure of DIS process configuration
*/
//@{
typedef enum
{
    DISALG_CFG_GLOBALMOTEN,      ///< configure enable of global motion calculation
    DISALG_CFG_ROIMOTEN,         ///< configure enable of ROI motion calculation
    DISALG_CFG_2MDSLAYEREN,      ///< configure enable of two MDS layers
    ENUM_DUMMY4WORD(DISALG_CFGID)
} DISALG_CFGID;
//@}

/**
    DIS Process Configuraiton Paramteres

    Structure of DIS process configuraiton parameters
*/
//@{
typedef struct _DISALG_CFGINFO_STRUCT
{
    INT32 iCfgVal;              ///< configuration value
} DISALG_CFGINFO;
//@}

extern CHAR *Dis_GetVerInfo(void);
extern CHAR *Dis_GetBuildDate(void);

extern void dis_initialize(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void dis_process(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void dis_end(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern UINT32 dis_getTriggerStatus(void);
extern UINT32 dis_GetPrvMaxBuffer(void);
extern void dis_setDisInfor(DISALG_PARAM *disInfo);
extern void dis_roi_setInputInfo(DIS_ROIGMV_IN* roiIn);
extern void dis_roi_getInputInfo(DIS_ROIGMV_IN* roiIn);
extern DIS_ROIGMV_OUT dis_roi_getResult(void);
extern void dis_getReadyMotionVec(MV_INFO_SIZE *mvInfoSize);
extern void dis_getReadyMotionVecL2(MV_INFO_SIZE *mvInfoSize);
extern void dis_getInputInfo(DISALG_PARAM *info);
extern void dis_SetConfig(DISALG_CFGID cfgID, DISALG_CFGINFO *pInfo);
extern void dis_GetConfig(DISALG_CFGID cfgID, DISALG_CFGINFO *pInfo);

#endif

