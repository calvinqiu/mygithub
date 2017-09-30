/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       PrjCfg.h
    @ingroup

    @brief

    @note       Nothing.
*/

#ifndef _PRJCFG_COMM_HEAD_H_
#define _PRJCFG_COMM_HEAD_H_

////////////////////////////////////////////////////////////////////////////////
// Project

#define PRJ_DBG_LVL_NONE                0
#define PRJ_DBG_LVL_ERR                 1
#define PRJ_DBG_LVL_TRC                 2

//reference bigger brother "DEBUG_LEVEL"
#if DEBUG_LEVEL
#define PRJ_DBG_LVL                     PRJ_DBG_LVL_ERR //switch project debug msg
#else
#define PRJ_DBG_LVL                     PRJ_DBG_LVL_NONE
#endif

//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
#define NA                              0xffffffff
//#NT#2016/10/18#Jeah Yen -end

//..............................................................................
// Display mask
#define DISPLAY_1                       0x00000001
#define DISPLAY_2                       0x00000002
#define DISPLAY_ALL                     0xffffffff

//..............................................................................
// Sensor mask
#define SENSOR_1                        0x00000001
#define SENSOR_2                        0x00000002
#define SENSOR_3                        0x00000004
#define SENSOR_4                        0x00000008
#define SENSOR_ALL                      0xffffffff

//#NT#2016/10/18#Jeah Yen -begin
//#NT#define default sensor map
#define IPL_PATH_MAX_NUM                4
#define IPL_PATH_1                      SENSOR_ID_1 //equal IPL_ID_1
#define IPL_PATH_2                      SENSOR_ID_2 //equal IPL_ID_2
#define IPL_PATH_3                      SENSOR_ID_3 //equal IPL_ID_3
#define IPL_PATH_4                      SENSOR_ID_4 //equal IPL_ID_4
//#NT#2016/10/18#Jeah Yen -end

//..............................................................................
// Audio format
#define _VM_FORMAT_PCM8_                0
#define _VM_FORMAT_PCM16_               1
#define _VM_FORMAT_ADPCM_               2
#define _VM_FORMAT_ASFPCM_              3

//..............................................................................
// MSDC modes : For the MSDC card HotPlug & Multi-LUNs function
#define _MSDC_SINGLE_                   0           // Only the current active storage device can be accessed through USB MSDC, i.e. card "OR" nand.
#define _MSDC_ALL_                      1           // All the storage devices can be accessed concurrently through USB MSDC, i.e. card "AND" nand.
#define _MSDC_SINGLE_NAND2_             2           // Only the active storage device plus the NAND2 can be accessed concurrently through USB MSDC.
#define _MSDC_ALL_NAND2_                3           // All the storage devices plus the NAND2 can be accessed concurrently through USB MSDC.

//..............................................................................
#define _INSIDECODE_                    0
#define _PSTORE_                        1

//..............................................................................
#define _LOGO_NOVATEK_                  0
#define _LOGO_GE_                       1
#define _LOGO_POLAROID_                 2
#define _LOGO_AGFA_                     3

//..............................................................................
#define _SCREENNAIL_SIZE_VGA_           0
#define _SCREENNAIL_SIZE_3M_            1

#define _QUICKVIEW_SIZE_VGA_            0
#define _QUICKVIEW_SIZE_SUIT_FOR_DISP   1



#define UPDATE_CFG_NO                   1
#define UPDATE_CFG_YES                  2

#define UPDATE_CFG                      UPDATE_CFG_NO

//..............................................................................
#define _CVHOST_EX_                     DISABLE
#define _CVHOST_EX_ENDSP_               DISABLE

#define _NVTCV_EX_                      DISABLE
#define _NVTCV_EX_ENDSP_                DISABLE

#endif //_PRJCFG_COMM_HEAD_H_
