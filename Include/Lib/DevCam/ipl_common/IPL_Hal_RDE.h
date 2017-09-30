/**
    IPH module (RDE type define)

    @file       IPL_Hal_RDE.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _IPL_Hal_RDE_H_
#define _IPL_Hal_RDE_H_
#include "IPL_Hal_RDE_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

/**
     Query enum

     read from hw register.
*/
typedef enum _IPH_RDE_QUE
{
    RDE_Q_CLK              = 0x00000001,   ///< read Input addr from reg.      Data type: UINT32
    ENUM_DUMMY4WORD(IPH_RDE_QUE)
}IPH_RDE_QUE;

/**
     enum of RDE update selection
*/
typedef enum _RDE_UPDATE
{
    RDE_BRCQINIT           = 0x00000001,       ///< update Initial Q table set(0~7)
    RDE_IOADDR             = 0x00000002,       ///< rde input/output address
    RDE_CROPINFO           = 0x00000004,       ///< update Crop parameter
    RDE_SIZEINFO           = 0x00000008,       ///< update Size parameter
    RDE_INIT               = 0x00000010,       ///< init RDE
    RDE_ST                 = 0x00000020,       ///< MST/SST (only used for runtime change, otherwise RDE_INIT)
    RDE_FLIP               = 0x00000040,       ///< flip information
    ENUM_DUMMY4WORD(RDE_UPDATE)
}RDE_UPDATE;

/**
     enum of RDE read selection

     read from inner table, not from hw register.
*/
typedef enum _RDE_READ
{
    RDE_R_IOADDR           = 0X00000001,       ///< read in/out address,                Data type: UINT32[3] (BS addr, Img1 addr, Img2 addr)
    RDE_R_SUBFUNCEN        = 0X00000002,       ///< read func enable/disable,           Data type: RDEFuncEn
    RDE_R_GAMMATBLADDR     = 0X00000003,       ///< read addr of degamma table,         Data type: UINT32
    RDE_R_BRCQINIT         = 0X00000004,       ///< read Initial Q table set(0~7),      Data type: UINT32
    RDE_R_IMGINFO          = 0X00000005,       ///< read RDE size info,                 Data type: RDE_IMG_INFO
    RDE_R_CROPINFO         = 0X00000006,       ///< read RDE size info,                 Data type: RDE_CROP
    ENUM_DUMMY4WORD(RDE_READ)
}RDE_READ;

/**
     RDE configuration

     all RDE configuration including all RDE sub function

*/
typedef struct _RDE_Manager
{
    RDE_OPMODE      OpMode;         ///< RDE operation mode
    RDE_STRPMODE    StripeMode;     ///< RDE stripe mode

    //frame mode
    RDE_BS_INFO     FrameBSInfo;    ///< RDE bit stream information(FRAME MODE)
    UINT32 FrameOutAddr;            ///< RDE frame out address(FRAME MODE)
    RDE_IMG_INFO    FrameOutInfo;   ///< RDE frame out information(FRAME MODE)

    //line mode
    UINT32          Width;          ///< image width
    UINT32          Height;         ///< image height
    UINT32          Total_Height;   ///< image total height for flip
    UINT32          Img1Addr;       ///< image1 address (for 1st stripe)
    UINT32          Img1LineOfs;    ///< image1 line offset (for 1st stripe)
    UINT32          Img2Addr;       ///< image2 address (for 2nd stripe)
    UINT32          Img2LineOfs;    ///< image2 line offset (for 2nd stripe)

    UINT32          Strip2ndSPT;    ///< RDE image2 start point (for MST mode)
    RDE_CROP        CropPt;         ///< RDE Start point of Crop image (image left point == original point)
    UINT32          BrcQInit;       ///< RDE Initial Q table set (0~7)
    RDE_SUBFUNC     *pSubFunc;      ///< RDE sub function pointer
}RDE_Manager,*pRDE_Manager;

/**
     RDE configuration
*/
typedef RDE_Manager RDE;

//@}
#endif
