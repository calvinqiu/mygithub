#include <stdlib.h>
#include "GSensor.h"
#include "SwTimer.h"
#include "ErrorNo.h"
#include "Delay.h"
#include "GyroDrv.h"

#define __MODULE__          GSensor_LSM6DS33
#define __DBGLVL__          1        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"


static INT32 GsXData = 0,OldGsXData = 0;
static INT32 GsYData = 0,OldGsYData = 0;
static INT32 GsZData = 0,OldGsZData = 0;
static BOOL  g_bGsensorOpened = FALSE;
static GSENSOR_SENSITIVITY g_GSensorSensitivity = GSENSOR_SENSITIVITY_OFF;



//**********************************************************************************
//
//
//**********************************************************************************
static UINT32 LSM6DS33Threshold=0xFF;/*1G= X/16;*/



static UINT32 GSensor_GetSensitivityLevel(void)
{
    switch(g_GSensorSensitivity)
    {
        case GSENSOR_SENSITIVITY_OFF:
            LSM6DS33Threshold =0xFF;
            break;
        case GSENSOR_SENSITIVITY_LOW:
            LSM6DS33Threshold =0x95;//25
            break;
        case GSENSOR_SENSITIVITY_MED:
            LSM6DS33Threshold =0x85;//20
            break;
        case GSENSOR_SENSITIVITY_HIGH:
            LSM6DS33Threshold =0x65;//16
            break;
        default:
            LSM6DS33Threshold =0xFF;
            break;
    }
    return LSM6DS33Threshold;
}



static BOOL GSensor_Init(void)
{
    if (gyro_open(NULL) != E_OK )
        return FALSE;
    return TRUE;
}



BOOL GSensor_open(void)
{

   // Gsensor init
   GSensor_Init();

   g_bGsensorOpened = TRUE;
   return TRUE;
}

BOOL GSensor_close(void)
{
    if (g_bGsensorOpened==FALSE)
    {
        return TRUE;
    }
    g_bGsensorOpened = FALSE;
    return TRUE;
}

static UINT32 GSensor_GetIntCount(void)
{
    static BOOL  g_bFirstStatus = TRUE;

    UINT8  abs_cal_x=0,abs_cal_y=0,abs_cal_z=0;
    UINT8 G_INT_COUNT=0;
    UINT8 Threshold;

    if(g_GSensorSensitivity == GSENSOR_SENSITIVITY_OFF)
        return FALSE;

    Threshold = GSensor_GetSensitivityLevel();

    gyro_getGsData(&GsXData, &GsYData, &GsZData);


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

    if (g_bFirstStatus)
    {
        g_bFirstStatus = FALSE;
        return 0;
    }

    DBG_IND("abs_cal_x=%d, abs_cal_y=%d, abs_cal_z=%d\n\r",abs_cal_x,abs_cal_y,abs_cal_z);
    DBG_IND("Threshold=%d, G_INT_COUNT=%d\n\r",Threshold,G_INT_COUNT);
    return G_INT_COUNT;

}
BOOL GSensor_GetStatus(Gsensor_Data *GS_Data)
{
    UINT8 G_INT_COUNT=0;

    G_INT_COUNT = GSensor_GetIntCount();
    if(G_INT_COUNT>=2)
    {
        DBG_MSG("----------LSM6DS33_isr1 EMG----------\n\r");
        return TRUE;
    }
    else
        return FALSE;
}

BOOL GSensor_ParkingMode(void)
{

    UINT8 G_INT_COUNT=0;

    G_INT_COUNT = GSensor_GetIntCount();
    if(G_INT_COUNT>=1)
    {
        DBG_MSG("----------LSM6DS33 GSensor_ParkingMode----------\n\r");
        return TRUE;
    }
    else
        return FALSE;
}

BOOL GSensor_CrashMode(void)
{
    return FALSE;
}

void GSensor_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{
    g_GSensorSensitivity = GSensorSensitivity;
}

void GSensor_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ)
{
}

