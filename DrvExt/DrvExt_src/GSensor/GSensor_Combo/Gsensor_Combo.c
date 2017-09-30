#include "Type.h"
#include "i2c.h"
#include "GSensor.h"
#include "Gsensor_Combo.h"

#define __MODULE__          GSensor_Combo
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 1       //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

static BOOL  g_bGsensorOpened = FALSE;
static GSENSOR_MAKER g_GsensorMaker = GSENSOR_NONE;
static PGSENSOR_OBJ  g_pGSensorObj = NULL;


BOOL GSensor_IsOpened(void)
{
    return g_bGsensorOpened;
}

BOOL GSensor_open(void)
{
    GSENSOR_INFO GsensorInfo = {0};
    GSENSOR_INFO *pGsensorInfo;
    UINT32 ulData1;

    DBG_IND("\r\n");
    if (g_bGsensorOpened == TRUE)
    {
       debug_msg("Gsensor open already\r\n");
       return TRUE;
    }

    GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE;
    GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;
    GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;

    if (GSensor_I2C_Init(GsensorInfo) == FALSE)
    {
       DBG_ERR("Open G-sensor I2C fail\r\n");
       return FALSE;
    }

    pGsensorInfo = GSensor_I2C_GetInfo();
    pGsensorInfo->I2C_Slave_WAddr = 0x30; //GMA301 G-sensor
    pGsensorInfo->I2C_Slave_RAddr = 0x31; //GMA301 G-sensor
    GSensor_I2C_SetInfo(pGsensorInfo);
    ulData1 = GSensor_I2C_ReadReg(0x12);
    ulData1 = GSensor_I2C_ReadReg(0x12);
    //debug_msg("^M%s: GMA301 ulData1 = 0x%x\r\n", __func__, ulData1);
    if (ulData1 == 0x55)
    {
        g_GsensorMaker = GSENSOR_GMA301;
/* modify begin by ZMD, 2017-02-15 new version management*/
#if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
 debug_msg("------GSENSOR_GMA301  chip id = 0x%x-----\r\n",ulData1);
#endif
/* modify end by ZMD, 2017-02-15 */
    }
    else
    {
        pGsensorInfo->I2C_Slave_WAddr = 0x38; //DRAMD07 G-sensor
        pGsensorInfo->I2C_Slave_RAddr = 0x39; //DRAMD07 G-sensor
        GSensor_I2C_SetInfo(pGsensorInfo);
        ulData1 = GSensor_I2C_ReadReg(0x0F);
        //debug_msg("^M%s: DRAMD07 ulData1 = 0x%x\r\n", __func__, ulData1);
        if (ulData1 == 0x07)
        {
            g_GsensorMaker = GSENSOR_DRAMD07;
/* modify begin by ZMD, 2017-02-15 new version management*/
#if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
		debug_msg("------GSENSOR_DRAMD07  chip id = 0x%x-----\r\n",ulData1);
#endif
/* modify end by ZMD, 2017-02-15 */
        }
        else
        {
            pGsensorInfo->I2C_Slave_WAddr = 0x4e; //DA380 G-sensor
            pGsensorInfo->I2C_Slave_RAddr = 0x4f; //DA380 G-sensor
            GSensor_I2C_SetInfo(pGsensorInfo);
            ulData1 = GSensor_I2C_ReadReg(0x01);
            ulData1 = GSensor_I2C_ReadReg(0x01);
            ulData1 = GSensor_I2C_ReadReg(0x01);
            ulData1 = GSensor_I2C_ReadReg(0x01);
        	if(ulData1 != 0x13)
		{        	    
        		g_GsensorMaker = GSENSOR_NONE;
			debug_msg("------mir3da read chip id  error= 0x%x-----\r\n",ulData1); 
        	}
            else
            {
                g_GsensorMaker = GSENSOR_DA380;
        		debug_msg("------GSENSOR_DA380  chip id = 0x%x-----\r\n",ulData1); 
            }
        }
    }

    switch (g_GsensorMaker)
    {
    case GSENSOR_NONE:
       g_pGSensorObj = GSensor_NONE_getGSensorObj();
       break;
    case GSENSOR_DRAMD07:
       g_pGSensorObj = GSensor_DRAMD07_getGSensorObj();
	   debug_msg("Open GSensor_DRAMD07_getGSensorObj \r\n");
       break;
    case GSENSOR_GMA301:
       g_pGSensorObj = GSensor_GMA301_getGSensorObj();
	   debug_msg("Open GSensor_GMA301_getGSensorObj \r\n");
       break;
    case GSENSOR_DA380:
       g_pGSensorObj = GSensor_DA380_getGSensorObj();
	   debug_msg("Open GSensor_DA380_getGSensorObj \r\n");
       break;
    default:
       g_pGSensorObj = GSensor_NONE_getGSensorObj();
       break;
    }

    if (g_pGSensorObj->open())
    {
        g_bGsensorOpened = TRUE;
/* modify begin by ZMD, 2017-02-15 new version management*/
#if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
        debug_msg("GSensor_open TRUE end:%d..\r\n",g_bGsensorOpened);		
#endif
/* modify end by ZMD, 2017-02-15 */
        return TRUE;
    }
    else
    {
/* modify begin by ZMD, 2017-02-15 new version management*/
#if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
        debug_msg("GSensor_open FALSE end:%d..\r\n",g_bGsensorOpened);    
#endif
/* modify end by ZMD, 2017-02-15 */
        return FALSE;
    }
/* modify begin by ZMD, 2017-02-15 new version management*/
#if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
    debug_msg("--->g_GsensorMaker:%d--->\r\n",g_GsensorMaker);
#endif
/* modify end by ZMD, 2017-02-15 */
}

BOOL GSensor_close(void)
{
    if (g_bGsensorOpened == FALSE)
    {
        debug_msg("I2C close already\r\n");
        return TRUE;
    }

    g_pGSensorObj->close();

    GSensor_I2C_Exit();

    g_bGsensorOpened = FALSE;

    return TRUE;
}


BOOL GSensor_GetStatus(Gsensor_Data *GS_Data)
{
    if (g_bGsensorOpened == FALSE)
    {
        return FALSE;
    }

    return g_pGSensorObj->GetStatus(GS_Data);
}

void GSensor_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{
    if (g_bGsensorOpened == FALSE)
    {
        return;
    }

    g_pGSensorObj->SetSensitivity(GSensorSensitivity);
}

BOOL GSensor_SetParkingMode(void)
{
    if (g_bGsensorOpened == FALSE)
    {
        return FALSE;
    }

    g_pGSensorObj->ParkingMode();
   return TRUE;
}

GSENSOR_MAKER GSensor_GetGsensorMaker()
{
	return g_GsensorMaker;
}

BOOL Gsensor_GetCrashMode()
{
    if (g_bGsensorOpened == FALSE)
    {
        return FALSE;
    }

    return g_pGSensorObj->CrashMode();
}

void Gsensor_ClearCrashMode()
{
    if (g_bGsensorOpened == FALSE)
    {
        return FALSE;
    }

    return g_pGSensorObj->ClearCrashMode();
}

void Gsensor_SetInterrupt(BOOL En)
{
    if (g_bGsensorOpened == FALSE)
    {
        return FALSE;
    }

    return g_pGSensorObj->SetInterrupt(En);
}
void GSensor_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ)
{
}

