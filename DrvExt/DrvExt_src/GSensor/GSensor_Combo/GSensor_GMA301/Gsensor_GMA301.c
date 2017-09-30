#include "GSensor.h"
#include "Gsensor_Combo.h"
#include "Gsensor_GMA301.h"
#include "SwTimer.h"
#include <stdlib.h>
#include "ErrorNo.h"
#include "Delay.h"

#define __MODULE__          GSensor_GMA301
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
static GSENSOR_SENSITIVITY g_GSensorSensitivity = GSENSOR_SENSITIVITY_OFF;
static GSENSOR_OBJ g_GsensorGMA301Obj = {GSensor_GMA301_open, GSensor_GMA301_close, GSensor_GMA301_GetStatus, GSensor_GMA301_ParkingMode, GSensor_GMA301_CrashMode, GSensor_GMA301_SetSensitivity};


BOOL GSensor_GMA301_CrashMode(void)
{
    return FALSE;
}

void GSensor_GMA301_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{
    g_GSensorSensitivity = GSensorSensitivity;
}

//**********************************************************************************
//
//
//**********************************************************************************
static GSENSOR_INFO2 g_GsensorInfo;
static UINT32 GMA301Threshold=0xFF;/*1G= X/16;*/

static UINT32 GSensor_GMA301_ReadReg_2Byte(UINT32 uiAddr, UINT32 uidata)
{
    UINT32      ulWriteAddr, ulReadAddr, ulReg1;
    static UINT32 ulData1,ulData2;

    ulWriteAddr =0x30;
    ulReadAddr  =0x31;
    ulReg1      =(uiAddr&0x000000ff);
    ulData1     = 0;
    ulData2 = 0;
    //debug_err((">>> read register\r\n"));
    if(i2c_lock(g_GsensorInfo.I2C_Channel) != E_OK)
    {
        DBG_ERR("GSensor: readReg I2C Lock Fail\r\n");
        return ulData1;
    }

    if (GSensor_I2C_Transmit(ulWriteAddr, 1, 0) != I2C_STS_OK)
    {
        DBG_ERR("Error transmit data1!!\r\n");
        i2c_unlock(g_GsensorInfo.I2C_Channel);
        return  ulData1;
    }

    if (GSensor_I2C_Transmit(ulReg1, 0, 0) != I2C_STS_OK)
    {
        DBG_ERR("Error transmit data2!!\r\n");
        i2c_unlock(g_GsensorInfo.I2C_Channel);
        return  ulData1;
    }

    if (GSensor_I2C_Transmit(ulReadAddr, 1, 0) != I2C_STS_OK)
    {
        DBG_ERR("Error transmit data3!!\r\n");
        i2c_unlock(g_GsensorInfo.I2C_Channel);
        return  ulData1;
    }

    if (GSensor_I2C_Receive(&ulData1, 0, 0) != I2C_STS_OK)
    {
        DBG_ERR("Error Receive data!!\r\n");
        i2c_unlock(g_GsensorInfo.I2C_Channel);
        return  ulData1;
    }

    if (GSensor_I2C_Receive(&ulData2, 1, 1) != I2C_STS_OK)
    {
        DBG_ERR("Error Receive data!!\r\n");
        i2c_unlock(g_GsensorInfo.I2C_Channel);
        return  ulData1;
    }
    if(i2c_unlock(g_GsensorInfo.I2C_Channel) != E_OK)
    {
        DBG_ERR("GSensor: readReg I2C UnLock Fail\r\n");
        return  ulData1;
    }
   // debug_err((">>>>>uidata = %d %d\r\n",ulData1,ulData2));

    uidata =  (ulData1<<8) + ulData2;
   //debug_err((">>>uidata = %d\r\n",uidata));
    return  uidata;
}

INT32 GSensor_GMA301_GetAxisMsg(UINT32 uiInt)
{
    INT32 Data  = 0;

    if (uiInt >511) //0x3F
    {
        //negative number
        Data =-(uiInt-511);
        DBG_MSG("GSensor_GMA301_GetAxisMsg1  = %d\r\n",Data);

    }
    else
    {
        Data = uiInt;
        DBG_MSG("GSensor_GMA301_GetAxisMsg2  = %d\r\n",Data);
    }

    return Data;//(bzNegative)?(-1*Data):(Data);

}

static UINT32 GSensor_GMA301_GetSensitivityLevel(void)
{
    switch(g_GSensorSensitivity)
    {
        case GSENSOR_SENSITIVITY_OFF:
            GMA301Threshold =0xFF;
            break;
        case GSENSOR_SENSITIVITY_LOW:
            GMA301Threshold =0x95;//25
            break;
        case GSENSOR_SENSITIVITY_MED:
            GMA301Threshold =0x85;//20
            break;
        case GSENSOR_SENSITIVITY_HIGH:
            GMA301Threshold =0x65;//16
            break;
        default:
            GMA301Threshold =0xFF;
            break;
    }
    return GMA301Threshold;
}

static void GSensor_GMA301_OpenInterface(void)
{

}

static void GSensor_GMA301_CloseInterface(void)
{

}

PGSENSOR_OBJ GSensor_GMA301_getGSensorObj(void)
{
    return &g_GsensorGMA301Obj;
}

static BOOL GSensor_GMA301_Init(void)
{
    UINT32 value = 0;

    //init
    GSensor_I2C_WriteReg(0x21,0x52);//
    GSensor_I2C_WriteReg(0x00,0x02);//
    GSensor_I2C_WriteReg(0x00,0x12);//
    GSensor_I2C_WriteReg(0x00,0x02);//
    GSensor_I2C_WriteReg(0x00,0x82);//
    GSensor_I2C_WriteReg(0x00,0x02);//
    GSensor_I2C_WriteReg(0x1F,0x28);//
    GSensor_I2C_WriteReg(0x0C,0x8F);//
    GSensor_I2C_WriteReg(0x00,0x06);//

    //interrupt setup
    GSensor_I2C_WriteReg(0x11,0x04);//IIC 0X07 for no pullup //0x06 High active  0x04 low active
    //Gsensor_WriteReg(0x11,0x06);//IIC 0X06 for no pullup
    //set Gsensor Level
    //0x08-->0.5G
    //0X10-->1G
    GSensor_I2C_WriteReg(0x38,0X00);//
    GSensor_I2C_WriteReg(0x39,0X30);//10 1g 20 2g 30 3g 40 4g 50 5g 60 6g

    GSensor_I2C_WriteReg(0x0F,0x00);//
    GSensor_I2C_WriteReg(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt
    GSensor_I2C_WriteReg(0x1F,0x28);//To disable micro motion interrupt.
    //TimerDelayMs(10);
    //Latched reference data of micro motion.
    GSensor_I2C_ReadReg(0x12);
    GSensor_I2C_ReadReg(0x13);
    GSensor_I2C_ReadReg(0x14);
    GSensor_I2C_ReadReg(0x15);
    GSensor_I2C_ReadReg(0x16);
    GSensor_I2C_ReadReg(0x17);
    GSensor_I2C_ReadReg(0x18);
    GSensor_I2C_ReadReg(0x19);
    //GSensor_I2C_ReadReg(0x1A);
    //GSensor_I2C_ReadReg(0x1B);

    //GSensor_I2C_ReadReg(0x12);
    //GSensor_I2C_ReadReg(0x13);
    #if 0
    DBG_MSG("^M%s:reg 0x12:%x\r\n",__func__, GSensor_I2C_ReadReg(0x12));//X-H
    //DBG_MSG("^M%s:reg 0x13:%x\r\n",__func__, GSensor_I2C_ReadReg(0x13));//X-L
    //DBG_MSG("^M%s:reg 0x14:%x\r\n",__func__, GSensor_I2C_ReadReg(0x14));//X-H
    //DBG_MSG("^M%s:reg 0x15:%x\r\n",__func__, GSensor_I2C_ReadReg(0x15));//X-L
    //DBG_MSG("^M%s:reg 0x16:%x\r\n",__func__, GSensor_I2C_ReadReg(0x16));//Y-H
    //DBG_MSG("^M%s:reg 0x17:%x\r\n",__func__, GSensor_I2C_ReadReg(0x17));//Y-L
    //DBG_MSG("^M%s:reg 0x18:%x\r\n",__func__, GSensor_I2C_ReadReg(0x18));//Z-H
    //DBG_MSG("^M%s:reg 0x19:%x\r\n",__func__, GSensor_I2C_ReadReg(0x19));//Z-L
    DBG_MSG("^M%s:reg 0x0C:%x\r\n",__func__, GSensor_I2C_ReadReg(0x0C));//X-H
    DBG_MSG("^M%s:reg 0x11:%x\r\n",__func__, GSensor_I2C_ReadReg(0x11));//X-H
    DBG_MSG("^M%s:reg 0x21:%x\r\n",__func__, GSensor_I2C_ReadReg(0x21));//X-H
    DBG_MSG("^M%s:reg 0x1F:%x\r\n",__func__, GSensor_I2C_ReadReg(0x1F));//X-H
    DBG_MSG("^M%s:reg 0x38:%x\r\n",__func__, GSensor_I2C_ReadReg(0x38));//X-H
    DBG_MSG("^M%s:reg 0x39:%x\r\n",__func__, GSensor_I2C_ReadReg(0x39));//X-H
    #endif

    GSensor_I2C_WriteReg(0x1F,0x38);//To enable micro motion interrupt.
    SwTimer_DelayMs(2); //2014_0819 added 1ms delay for micro motion setup itself. Please SwTimer_DelayMs

    GSensor_I2C_WriteReg(0x0E,0x1C);//To enable interrupt.
    GSensor_GMA301_ReadReg_2Byte(0x1C,value);

    return TRUE;
}

//for  Gensor POWER UP
void GSensor_GMA301_PowerUp(INT32 sen)
{
    UINT32 value = 0;

    DBG_MSG("InitGensor_POWERDOWN 111!!\r\n");

    //interrupt setup
    GSensor_I2C_WriteReg(0x11,0x06);//IIC 0X07 for no pullup //0x06 High active  0x04 low active
    //Gsensor_WriteReg(0x11,0x06);//IIC 0X06 for no pullup
    //set Gsensor Level
    //0x08-->0.5G
    //0X10-->1G
    GSensor_I2C_WriteReg(0x38,0X00);//
    GSensor_I2C_WriteReg(0x39,sen);//10 1g 20 2g 30 3g 40 4g 50 5g 60 6g

    GSensor_I2C_WriteReg(0x0F,0x00);//
    GSensor_I2C_WriteReg(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt
    GSensor_I2C_WriteReg(0x1F,0x28);//To disable micro motion interrupt.

    GSensor_I2C_ReadReg(0x12);
    GSensor_I2C_ReadReg(0x13);
    GSensor_I2C_ReadReg(0x14);
    GSensor_I2C_ReadReg(0x15);
    GSensor_I2C_ReadReg(0x16);
    GSensor_I2C_ReadReg(0x17);
    GSensor_I2C_ReadReg(0x18);
    GSensor_I2C_ReadReg(0x19);

    GSensor_I2C_WriteReg(0x1F,0x38);//To enable micro motion interrupt.
    Delay_DelayMs(2); //2014_0819 added 1ms delay for micro motion setup itself.

    GSensor_I2C_WriteReg(0x0E,0x1C);//To enable interrupt.
    GSensor_GMA301_ReadReg_2Byte(0x1C,value);

    DBG_MSG("InitGensor_POWERDOWN 22222!!=%d\r\n",value);

}

void GSensor_GMA301_PowerDown(void)
{
    UINT32 value = 0;
    GSensor_I2C_WriteReg(0x21,0x52);//
    GSensor_I2C_WriteReg(0x00,0x02);//
    GSensor_I2C_WriteReg(0x00,0x12);//
    GSensor_I2C_WriteReg(0x00,0x02);//
    GSensor_I2C_WriteReg(0x00,0x82);//
    GSensor_I2C_WriteReg(0x00,0x02);//
    GSensor_I2C_WriteReg(0x1F,0x28);//
    GSensor_I2C_WriteReg(0x0C,0x8F);//
    GSensor_I2C_WriteReg(0x00,0x06);//

    //interrupt setup
    GSensor_I2C_WriteReg(0x11,0x06);//IIC 0X07 for no pullup //0x06 High active  0x40 low active
    //Gsensor_WriteReg(0x11,0x06);//IIC 0X06 for no pullup
    //set Gsensor Level
    //0x08-->0.5G
    //0X10-->1G

    GSensor_I2C_WriteReg(0x38,0x00);//
    GSensor_I2C_WriteReg(0x39,0X00);//
    GSensor_I2C_WriteReg(0x0E,0x00);//To enable interrupt.
    GSensor_GMA301_ReadReg_2Byte(0x1C,value);

}

BOOL GSensor_GMA301_open(void)
{

   if (g_bGsensorOpened == TRUE)
   {
       DBG_ERR("GSensor open already\r\n");
       return TRUE;
   }

   g_bGsensorOpened = TRUE;
   // open Gsensor interface
   GSensor_GMA301_OpenInterface();

   // Gsensor init
   GSensor_GMA301_Init();

   return TRUE;
}

BOOL GSensor_GMA301_close(void)
{

    if (g_bGsensorOpened==FALSE)
    {
        DBG_ERR("I2C close already\r\n");
        return TRUE;
    }

    // close Gsensor interface
    GSensor_GMA301_CloseInterface();

    g_bGsensorOpened = FALSE;

    return TRUE;
}


BOOL GSensor_GMA301_GetStatus(Gsensor_Data *GS_Data)
{
    UINT8  abs_cal_x=0,abs_cal_y=0,abs_cal_z=0;
    UINT8 G_INT_COUNT=0;
    UINT8 Threshold;
    UINT32 g_X_Data_L=0,g_X_Data_H=0,g_Y_Data_L=0,g_Y_Data_H=0,g_Z_Data_L=0,g_Z_Data_H=0;

    if(g_GSensorSensitivity == GSENSOR_SENSITIVITY_OFF)
        return FALSE;

    Threshold = GSensor_GMA301_GetSensitivityLevel();

    GSensor_I2C_ReadReg(0x12);
    GSensor_I2C_ReadReg(0x13);

    g_X_Data_L=GSensor_I2C_ReadReg(OUT_X_L_REG);
    g_Y_Data_L=GSensor_I2C_ReadReg(OUT_Y_L_REG);
    g_Z_Data_L=GSensor_I2C_ReadReg(OUT_Z_L_REG);

    g_X_Data_H=GSensor_I2C_ReadReg(OUT_X_H_REG);
    g_Y_Data_H=GSensor_I2C_ReadReg(OUT_Y_H_REG);
    g_Z_Data_H=GSensor_I2C_ReadReg(OUT_Z_H_REG);


      if((g_X_Data_H&0x03)>=0x02)
        GsXData =-(1023-((g_X_Data_L +((g_X_Data_H&0x03)<<8))));
    else
        GsXData =(g_X_Data_L +((g_X_Data_H&0x03)<<8));

      if((g_Y_Data_H&0x3)>=0x02)
        GsYData =-(1023-(g_Y_Data_L +((g_Y_Data_H&0x03)<<8)));
    else
        GsYData =(g_Y_Data_L +((g_Y_Data_H&0x03)<<8));

      if((g_Z_Data_H&0x3)>=0x02)
        GsZData =-(1023-(g_Z_Data_L +((g_Z_Data_H&0x03)<<8)));
    else
        GsZData =(g_Z_Data_L +((g_Z_Data_H&0x03)<<8));

/*
    DBG_MSG("@@reg 0x12:%x\r\n",GSensor_I2C_ReadReg(0x12));
    DBG_MSG("@@reg 0x13:%x\r\n",GSensor_I2C_ReadReg(0x13));
    DBG_MSG("@@reg 0x14:%x %d\r\n",GSensor_I2C_ReadReg(0x14),g_X_Data_L);//X-L
    DBG_MSG("@@reg 0x15:%x %d\r\n",GSensor_I2C_ReadReg(0x15),g_X_Data_H);//X-H
    DBG_MSG("@@reg 0x16:%x %d\r\n",GSensor_I2C_ReadReg(0x16),g_Y_Data_L);//Y-L
    DBG_MSG("@@reg 0x17:%x %d\r\n",GSensor_I2C_ReadReg(0x17),g_Y_Data_H);//Y-H
    DBG_MSG("@@reg 0x18:%x %d\r\n",GSensor_I2C_ReadReg(0x18),g_Z_Data_L);//Z-H
    DBG_MSG("@@reg 0x19:%x %d\r\n",GSensor_I2C_ReadReg(0x19),g_Z_Data_H);//Z-L
*/
    abs_cal_x =abs(GsXData-OldGsXData);
    if(abs_cal_x>Threshold)   //30
        G_INT_COUNT++;

    abs_cal_y =abs(GsYData-OldGsYData);
    if(abs_cal_y>Threshold) //20
        G_INT_COUNT++;

    abs_cal_z =abs(GsZData-OldGsZData);
    if(abs_cal_z>Threshold) //20
        G_INT_COUNT++;

    OldGsXData = GsXData;
    OldGsYData = GsYData;
    OldGsZData = GsZData;

    //DBG_MSG("@@reg 0x1c:%x\r\n",GSensor_I2C_ReadReg(0x1c));//X-H
    //DBG_MSG("@@reg 0x1d:%x\r\n",GSensor_I2C_ReadReg(0x1d));//X-L

/*
    DBG_MSG("@@reg 0x12:%x\r\n",GSensor_I2C_ReadReg(0x12));//X-H
    DBG_MSG("@@reg 0x13:%x\r\n",GSensor_I2C_ReadReg(0x13));//X-L
    DBG_MSG("@@reg 0x14:%x\r\n",GSensor_I2C_ReadReg(0x14));//X-H
    DBG_MSG("@@reg 0x15:%x\r\n",GSensor_I2C_ReadReg(0x15));//X-L
    DBG_MSG("@@reg 0x16:%x\r\n",GSensor_I2C_ReadReg(0x16));//Y-H
    DBG_MSG("@@reg 0x17:%x\r\n",GSensor_I2C_ReadReg(0x17));//Y-L
    DBG_MSG("@@reg 0x18:%x\r\n",GSensor_I2C_ReadReg(0x18));//Z-H
    DBG_MSG("@@reg 0x19:%x\r\n",GSensor_I2C_ReadReg(0x19));//Z-L
   //  DBG_MSG("..........X = %d,Y = %d,Z = %d\r\n",GsXData,GsYData,GsZData);
    //DBG_MSG("..........DX = %d,DY = %d,DZ = %d th=%d \r\n",abs_cal_x,abs_cal_y,abs_cal_z,Threshold);
*/
    DBG_IND("..........X = %d,Y = %d,Z = %d\r\n",GsXData,GsYData,GsZData);
    DBG_IND("..........DX = %d,DY = %d,DZ = %d th=%d, G_INT_COUNT=%d \r\n",abs_cal_x,abs_cal_y,abs_cal_z,Threshold,G_INT_COUNT);
    if(G_INT_COUNT>=2)
    {
        DBG_MSG("----------GMA301_isr1 EMG----------\n\r");
        return TRUE;
    }
    else
       return FALSE;
}

BOOL GSensor_GMA301_ParkingMode(void)
{
    UINT32  abs_cal_x=0,abs_cal_y=0,abs_cal_z=0;
    UINT32 P_INT_COUNT=0;
    UINT32 Threshold=0x20;
    UINT32 g_X_Data_L=0,g_X_Data_H=0,g_Y_Data_L=0,g_Y_Data_H=0,g_Z_Data_L=0,g_Z_Data_H=0;

    GSensor_I2C_ReadReg(0x12);
    GSensor_I2C_ReadReg(0x13);

    g_X_Data_L=GSensor_I2C_ReadReg(OUT_X_L_REG);
    g_Y_Data_L=GSensor_I2C_ReadReg(OUT_Y_L_REG);
    g_Z_Data_L=GSensor_I2C_ReadReg(OUT_Z_L_REG);

    g_X_Data_H=GSensor_I2C_ReadReg(OUT_X_H_REG);
    g_Y_Data_H=GSensor_I2C_ReadReg(OUT_Y_H_REG);
    g_Z_Data_H=GSensor_I2C_ReadReg(OUT_Z_H_REG);


      if((g_X_Data_H&0x03)>=0x02)
        GsXData =-(1023-((g_X_Data_L +((g_X_Data_H&0x03)<<8))));
    else
        GsXData =(g_X_Data_L +((g_X_Data_H&0x03)<<8));

      if((g_Y_Data_H&0x3)>=0x02)
        GsYData =-(1023-(g_Y_Data_L +((g_Y_Data_H&0x03)<<8)));
    else
        GsYData =(g_Y_Data_L +((g_Y_Data_H&0x03)<<8));

      if((g_Z_Data_H&0x3)>=0x02)
        GsZData =-(1023-(g_Z_Data_L +((g_Z_Data_H&0x03)<<8)));
    else
        GsZData =(g_Z_Data_L +((g_Z_Data_H&0x03)<<8));


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
    if(abs_cal_x>Threshold)   //30
        P_INT_COUNT++;

    abs_cal_y =abs(GsYData-OldGsYData);
    if(abs_cal_y>Threshold) //20
        P_INT_COUNT++;

    abs_cal_z =abs(GsZData-OldGsZData);
    if(abs_cal_z>Threshold) //20
        P_INT_COUNT++;

    OldGsXData = GsXData;
    OldGsYData = GsYData;
    OldGsZData = GsZData;

    //DBG_MSG("..........X = %d,Y = %d,Z = %d\r\r",GsXData,GsYData,GsZData);
    //DBG_MSG("..........DX = %d,DY = %d,DZ = %d th=%d \r\n",abs_cal_x,abs_cal_y,abs_cal_z,Threshold);

    if(P_INT_COUNT>=1)
    {
        DBG_MSG("----------GMA301 GSensor_ParkingMode----------\n\r");
        return 1;
    }
    else
        return 0;
}

