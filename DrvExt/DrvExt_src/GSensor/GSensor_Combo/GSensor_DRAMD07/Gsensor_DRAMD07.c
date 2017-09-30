#include "GSensor.h"
#include "Gsensor_Combo.h"
#include "Gsensor_DRAMD07.h"
#include "SwTimer.h"
#include "Debug.h"
#include <stdlib.h>
#include "ErrorNo.h"

#define __MODULE__          GSensor_DRAMD07
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 1        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

//********************************************************************************
//
//GSENSOR DRA07
//
//********************************************************************************
static INT32 GsXData = 0,OldGsXData = 0;
static INT32 GsYData = 0,OldGsYData = 0;
static INT32 GsZData = 0,OldGsZData = 0;
static BOOL  g_bGsensorOpened = FALSE;
static UINT32 DRAM07Threshold=0xFF;/*1G= X/16;*/
static GSENSOR_SENSITIVITY g_GSensorSensitivity = GSENSOR_SENSITIVITY_OFF;
static GSENSOR_OBJ g_GsensorDRAMD07Obj = {GSensor_DRAMD07_open, GSensor_DRAMD07_close, GSensor_DRAMD07_GetStatus, GSensor_DRAMD07_ParkingMode, GSensor_DRAMD07_CrashMode, GSensor_DRAMD07_SetSensitivity};

static INT32 GSensor_DRAMD07_GetAxisMsg(UINT32 uiInt)
{
    return ((INT8)(uiInt));
}

static UINT32 GSensor_DRAMD07_GetSensitivityLevel(void)
{
    switch(g_GSensorSensitivity)
    {
        case GSENSOR_SENSITIVITY_OFF:
            DRAM07Threshold =0xFF;
            break;
        case GSENSOR_SENSITIVITY_LOW:
            DRAM07Threshold =0x25;//25
            break;
        case GSENSOR_SENSITIVITY_MED:
            DRAM07Threshold =0x15;//20
            break;
        case GSENSOR_SENSITIVITY_HIGH:
            DRAM07Threshold =0x10;//16
            break;
        default:
            DRAM07Threshold =0xFF;
            break;
    }
    return DRAM07Threshold;
}

static void GSensor_DRAMD07_OpenInterface(void)
{

}

static void GSensor_DRAMD07_CloseInterface(void)
{

}

PGSENSOR_OBJ GSensor_DRAMD07_getGSensorObj(void)
{
    return &g_GsensorDRAMD07Obj;
}

static BOOL GSensor_DRAMD07_Init(void)
{
    //DARMD07 init register setting
    GSensor_I2C_ReadReg(0x53);//soft reset,clear out all reg
    SwTimer_DelayMs(10);
    GSensor_I2C_WriteReg(0x44,0x27);//Normal Mode, 342Hz,XYZ Int enable
    GSensor_I2C_WriteReg(0x45,0xE0);//2G mode,High Pass Filter for INT1
    GSensor_I2C_WriteReg(0x46,0x00);//High-pass Filter Cutoff for 6.4Hz
    GSensor_I2C_WriteReg(0x47,0x00);//INT1 latch,INT SRC1 enable,Int pin Low active
    //        GSensor_I2C_WriteReg(0x48,0x20);
    GSensor_I2C_WriteReg(0x4A,0x2A);//0x7F //OR all interrupt events,High-G XYZ
    GSensor_I2C_WriteReg(0x4C,0x3F);//Threshold = 16*15.7 = 251.2 mg
    GSensor_I2C_WriteReg(0x4D,0x10);//Duration = 16*2.9 = 46.4ms

    #if 0
    DBG_MSG("^M%s: devID:%x\r\n", __func__, GSensor_I2C_ReadReg(0x0F));
    DBG_MSG("^M%s: reg 0x44:%x\r\n", __func__, GSensor_I2C_ReadReg(0x44));
    DBG_MSG("^M%s: reg 0x45:%x\r\n", __func__, GSensor_I2C_ReadReg(0x45));
    DBG_MSG("^M%s: reg 0x46:%x\r\n", __func__, GSensor_I2C_ReadReg(0x46));
    DBG_MSG("^M%s: reg 0x47:%x\r\n", __func__, GSensor_I2C_ReadReg(0x47));
    DBG_MSG("^M%s: reg 0x48:%x\r\n", __func__, GSensor_I2C_ReadReg(0x48));
    DBG_MSG("^M%s: reg 0x4A:%x\r\n", __func__, GSensor_I2C_ReadReg(0x4A));
    DBG_MSG("^M%s: reg 0x4C:%x\r\n", __func__, GSensor_I2C_ReadReg(0x4C));
    DBG_MSG("^M%s: reg 0x4D:%x\r\n", __func__, GSensor_I2C_ReadReg(0x4D));
    #endif

    return TRUE;
}

BOOL GSensor_DRAMD07_open(void)
{
   if (g_bGsensorOpened == TRUE)
   {
       DBG_ERR("GSensor open already\r\n");
       return TRUE;
   }

   g_bGsensorOpened = TRUE;
   // open Gsensor interface
   GSensor_DRAMD07_OpenInterface();

   // Gsensor init
   GSensor_DRAMD07_Init();

   return TRUE;
}

BOOL GSensor_DRAMD07_close(void)
{

    if (g_bGsensorOpened==FALSE)
    {
        DBG_ERR("I2C close already\r\n");
        return TRUE;
    }

    // close Gsensor interface
    GSensor_DRAMD07_CloseInterface();

    g_bGsensorOpened = FALSE;

    return TRUE;
}


BOOL GSensor_DRAMD07_GetStatus(Gsensor_Data *GS_Data)
{
    UINT8  abs_cal_x=0,abs_cal_y=0,abs_cal_z=0;
    UINT8 G_INT_COUNT=0;
    UINT8 D07Threshold;


    if(g_GSensorSensitivity == GSENSOR_SENSITIVITY_OFF)
        return FALSE;


    D07Threshold = GSensor_DRAMD07_GetSensitivityLevel();

    GsXData = GSensor_DRAMD07_GetAxisMsg(GSensor_I2C_ReadReg(OUT_X_REG));

    GsYData = GSensor_DRAMD07_GetAxisMsg(GSensor_I2C_ReadReg(OUT_Y_REG));

    GsZData = GSensor_DRAMD07_GetAxisMsg(GSensor_I2C_ReadReg(OUT_Z_REG));
/*
      if(GsXData <(g_D07Threshold>>1))
        {
        GsXData  =OldGsXData;
        }
      if(GsYData <(g_D07Threshold>>1))
        {
         GsYData  =OldGsYData;
        }
      if(GsZData <(g_D07Threshold>>1))
        {
         GsZData  =OldGsZData;
        }
*/
    abs_cal_x =abs(GsXData-OldGsXData);
    if(abs_cal_x>D07Threshold)   //30
        G_INT_COUNT++;

    abs_cal_y =abs(GsYData-OldGsYData);
    if(abs_cal_y>D07Threshold) //20
        G_INT_COUNT++;

    abs_cal_z =abs(GsZData-OldGsZData);
    if(abs_cal_z>D07Threshold) //20
        G_INT_COUNT++;

    OldGsXData = GsXData;
    OldGsYData = GsYData;
    OldGsZData = GsZData;

    //debug_msg("..........X = %d,Y = %d,Z = %d\r\r",GsXData,GsYData,GsZData);
    //debug_msg("..........DX = %d,DY = %d,DZ = %d th=%d \r\n",abs_cal_x,abs_cal_y,abs_cal_z,D07Threshold);

    if(G_INT_COUNT>=2)
    {
        DBG_MSG("----------DRAMD07_isr1 EMG----------\n\r");
        return TRUE;
    }
    else
       return FALSE;
}

BOOL GSensor_DRAMD07_ParkingMode(void)
{
    UINT32  abs_cal_x=0,abs_cal_y=0,abs_cal_z=0;
    UINT32 P_INT_COUNT=0;
    UINT32 g_D07ParkenThreshold=0x07;

    GsXData = GSensor_DRAMD07_GetAxisMsg(GSensor_I2C_ReadReg(OUT_X_REG));
    GsYData = GSensor_DRAMD07_GetAxisMsg(GSensor_I2C_ReadReg(OUT_Y_REG));
    GsZData = GSensor_DRAMD07_GetAxisMsg(GSensor_I2C_ReadReg(OUT_Z_REG));

/*
      if(GsXData <(g_D07ParkenThreshold>>1))
      {
        GsXData  =OldGsXData;
      }
      if(GsYData <(g_D07ParkenThreshold>>1))
      {
        GsYData  =OldGsYData;
      }
      if(GsZData <(g_D07ParkenThreshold>>1))
      {
        GsZData  =OldGsZData;
      }
*/
    abs_cal_x=abs(GsXData-OldGsXData);
    if(abs_cal_x>g_D07ParkenThreshold)   //30
        P_INT_COUNT++;

    abs_cal_y =abs(GsYData-OldGsYData);
    if(abs_cal_y>g_D07ParkenThreshold) //20
        P_INT_COUNT++;

    abs_cal_z =abs(GsZData-OldGsZData);
    if(abs_cal_z>g_D07ParkenThreshold) //20
        P_INT_COUNT++;

    OldGsXData = GsXData;
    OldGsYData = GsYData;
    OldGsZData = GsZData;

    //DBG_MSG("..........X = %d,Y = %d,Z = %d\r\r",GsXData,GsYData,GsZData);
    //DBG_MSG("..........DX = %d,DY = %d,DZ = %d th=%d \r\n",abs_cal_x,abs_cal_y,abs_cal_z,D07Threshold);

    if(P_INT_COUNT>=1)
    {
        DBG_MSG("----------DRAMD07 GSensor_ParkingMode----------\n\r");
        return 1;
    }
    else
        return 0;
}

BOOL GSensor_DRAMD07_CrashMode(void)
{
    return FALSE;
}

void GSensor_DRAMD07_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{
    g_GSensorSensitivity = GSensorSensitivity;
}

