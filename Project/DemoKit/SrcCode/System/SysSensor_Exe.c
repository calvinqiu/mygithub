/*
    System Sensor Callback

    System Callback for Sensor Module.

    @file       SysSensor_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#if (LCD_AUTOBACKLIGHT_FUNCTION == ENABLE)
#include "UIAppPhoto.h" //for CURR_EV()
#include "MenuSetupLCDBrt.h" //for MenuSetup_GetV2BacklightValue()
#endif
#include "GxFlash.h"
#include "Sensor.h"
#include "DxSensor.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_CamDisp.h"
#include "ImageUnit_Media.h"
#include "PStore.h"
#if (CALIBRATION_FUNC == ENABLE)
#include "IPL_CalRW.h"
#endif
//#NT#2016/06/23#Isiah Chang -begin
//#NT#Notify APP to restart RTSP live view becaues of sensor plug/unplug event.
#include "WifiAppCmd.h"
//#NT#2016/06/23#Isiah Chang -end

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysSensorExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
static INT32 uiSensorMapping[4] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
//#NT#2016/10/18#Jeah Yen -end
static BOOL bFirst = TRUE;
#if (SENSOR_CAPS_COUNT == 1)
static INT32 gSensor_iLastMode = -1; //keep last UI mode
#endif

#if (_MODEL_DSC_ == _MODEL_EVB_)
#define EVB_DUALSENSOR_PATCH    ENABLE //workaround for "sensor2 reset pin issue of EVB dual sensor board"
#else
#define EVB_DUALSENSOR_PATCH    DISABLE
#endif

#if (SENSOR_INSERT_FUNCTION == ENABLE)
void DetSensor(void);
int SX_TIMER_DET_SENSOR_ID = -1;
SX_TIMER_ITEM(DetSensor, DetSensor, 25, FALSE)
#endif

#if (LCD_AUTOBACKLIGHT_FUNCTION == ENABLE)
int SX_TIMER_DET_EV_ID = -1;
void Sensor_DetEV(void);
SX_TIMER_ITEM(Sensor_DetEV, Sensor_DetEV, 50, TRUE)
#endif

#if (FLASHLIGHT_FUNCTION == ENABLE)
int SX_TIMER_DET_FLASH_ID = -1;
int SX_TIMER_DET_RECHARGE_ID = -1;
SX_TIMER_ITEM(GxFlash_DetFlash, GxFlash_DetFlash, 5, TRUE)
SX_TIMER_ITEM(GxFlash_DetReCharge, GxFlash_DetReCharge, 50, TRUE)
#endif

void System_EnableSensor(UINT32 SensorMask);

#if (CALIBRATION_FUNC == ENABLE)
static UINT32 CalRwStoreOp_OnOpen(char *pSecName, CAL_RW_OPEN_SEL OpenSel);
static ER CalRwStoreOp_OnClose(UINT32 pSectionHandleAddr);
static ER CalRwStoreOp_OnRead(UINT8 *pcBuffer,UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr);
static ER CalRwStoreOp_OnWrite(UINT8 *pcBuffer, UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr);
static ER CalRwStoreOp_OnDelete(char *pSecName);
static CAL_RW_STORE_OP m_CalRwStoreOp = {CalRwStoreOp_OnOpen, CalRwStoreOp_OnClose, CalRwStoreOp_OnRead, CalRwStoreOp_OnWrite, CalRwStoreOp_OnDelete};

static UINT32 CalRwStoreOp_OnOpen(char *pSecName, CAL_RW_OPEN_SEL OpenSel)
{
    UINT32 hHandle = 0;
#if (PST_FUNC)
    UINT32 RWOperation = 0;
    RWOperation |= (OpenSel&_CAL_RD_ONLY)?PS_RDONLY:0;
    RWOperation |= (OpenSel&_CAL_WR_ONLY)?PS_WRONLY:0;
    RWOperation |= (OpenSel&_CAL_CREATE)?PS_CREATE:0;
    hHandle = (UINT32)PStore_OpenSection(pSecName, RWOperation);
#endif
    return hHandle;
}

static ER CalRwStoreOp_OnClose(UINT32 pSectionHandleAddr)
{
    ER er = E_SYS;
#if (PST_FUNC)
    er = PStore_CloseSection((PSTORE_SECTION_HANDLE*) pSectionHandleAddr);
#endif
    return er;
}

static ER CalRwStoreOp_OnRead(UINT8 *pcBuffer,UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr)
{
    ER er = E_SYS;
#if (PST_FUNC)
    er = PStore_ReadSection(pcBuffer, ulStartAddress, ulDataLength, (PSTORE_SECTION_HANDLE*) pSectionHandleAddr);
#endif
    return er;
}

static ER CalRwStoreOp_OnWrite(UINT8 *pcBuffer, UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr)
{
    ER er = E_SYS;
#if (PST_FUNC)
    er = PStore_WriteSection(pcBuffer, ulStartAddress, ulDataLength, (PSTORE_SECTION_HANDLE*) pSectionHandleAddr);
#endif
    return er;
}

static ER CalRwStoreOp_OnDelete(char *pSecName)
{
    ER er = E_SYS;
#if (PST_FUNC)
    er = PStore_DeleteSection(pSecName);
#endif
    return er;
}
#endif

void System_OnSensorInit(void)
{
    TM_BOOT_BEGIN("sensor","init");
    //PHASE-1 : Init & Open Drv or DrvExt
    {
        #if (CALIBRATION_FUNC == ENABLE)
        CAL_DMA_RST rst = {0};
        CAL_RegRWFunc(&m_CalRwStoreOp);
        CAL_RW()->fpSectionAllRead(OS_GetMempoolAddr(POOL_ID_CAL),POOL_SIZE_CAL);
        if(rst.RemainSize > POOL_SIZE_CAL)
        {
            DBG_ERR("POOL_SIZE_CAL is not enough. \r\n");
        }
        #endif
    }
    TM_BOOT_END("sensor","init");
}
extern void _System_InitSensor(void);
extern UINT32 System_GetConnectSensor(void);
void System_EnableSensor_FastDevice(void);
void System_EnableSensor_FastUnit(void);
void System_OnSensorInit2(void)
{
#if ((SENSOR_CAPS_COUNT == 2) && (EVB_DUALSENSOR_PATCH == ENABLE))
    {
        SENSOR_INIT_OBJ SenObj;
        SENSOR_DRVTAB *SenTab;
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
        //turn on sensor power
        DBG_MSG("Sensor PowerOn [%d]\r\n", IPL_PATH_1);
        DrvSensor_PowerTurnOn(IPL_PATH_1);
        DBG_MSG("Sensor PowerOn [%d]\r\n", IPL_PATH_2);
        DrvSensor_PowerTurnOn(IPL_PATH_2);
        //open sensor
        DBG_MSG("Sensor Open [%d]\r\n", IPL_PATH_1);
        SenObj = DrvSensor_GetObj(IPL_PATH_1);
        SenTab = DrvSensor_GetTab(IPL_PATH_1);
        Sensor_Open(IPL_PATH_1, &SenObj, SenTab);
        DBG_MSG("Sensor Open [%d]\r\n", IPL_PATH_2);
        SenObj = DrvSensor_GetObj(IPL_PATH_2);
        SenTab = DrvSensor_GetTab(IPL_PATH_2);
        Sensor_Open(IPL_PATH_2, &SenObj, SenTab);
//#NT#2016/10/18#Jeah Yen -end
    }
#endif
#if (SENSOR_CAPS_COUNT == 1)
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
        INT32 iCurrMode = System_GetBootFirstMode();
        #if (PHOTO_MODE==ENABLE)
        if(iCurrMode == PRIMARY_MODE_PHOTO)
        {
            System_EnableSensor_FastDevice(); //enable 1 sensors
        }
        else
        #endif
        if(iCurrMode == PRIMARY_MODE_MOVIE)
        {
            System_EnableSensor_FastDevice(); //enable 1 sensors
        }
    }
#endif
#if ((SENSOR_CAPS_COUNT == 2) && !(EVB_DUALSENSOR_PATCH == ENABLE))
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    if(System_GetConnectSensor() != (SENSOR_1|SENSOR_2))
    {
        INT32 iCurrMode = System_GetBootFirstMode();
        #if (PHOTO_MODE==ENABLE)
        if(iCurrMode == PRIMARY_MODE_PHOTO)
        {
            System_EnableSensor_FastDevice(); //enable 1 sensors
        }
        else
        #endif
        if(iCurrMode == PRIMARY_MODE_MOVIE)
        {
            System_EnableSensor_FastDevice(); //enable 1 sensors
        }
    }
#endif
}
INT32 System_GetFirstSensorMode(INT32 iCurrMode)
{
#if (SENSOR_CAPS_COUNT == 1)
    _System_InitSensor();
    if(System_GetConnectSensor() == 0)
    {
        gSensor_iLastMode = iCurrMode; //keep last mode
        iCurrMode = PRIMARY_MODE_MAIN;
    }
    return iCurrMode;
#elif (SENSOR_CAPS_COUNT == 2)
    _System_InitSensor();
    if(System_GetConnectSensor() != (SENSOR_1|SENSOR_2))
    {
        //keep this mode
    }
    return iCurrMode;
#else
    return iCurrMode;
#endif
}
void System_OnSensorInit3(void)
{
#if (SENSOR_CAPS_COUNT == 1)
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
        INT32 iCurrMode = System_GetBootFirstMode();
        #if (PHOTO_MODE==ENABLE)
        if(iCurrMode == PRIMARY_MODE_PHOTO)
        {
            System_EnableSensor_FastUnit(); //enable 1 sensors
        }
        else
        #endif
        if(iCurrMode == PRIMARY_MODE_MOVIE)
        {
            System_EnableSensor_FastUnit(); //enable 1 sensors
        }
    }
#endif
#if ((SENSOR_CAPS_COUNT == 2) && !(EVB_DUALSENSOR_PATCH == ENABLE))
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    if(System_GetConnectSensor() != (SENSOR_1|SENSOR_2))
    {
        INT32 iCurrMode = System_GetBootFirstMode();
        #if (PHOTO_MODE==ENABLE)
        if(iCurrMode == PRIMARY_MODE_PHOTO)
        {
            System_EnableSensor_FastUnit(); //enable 1 sensors
        }
        else
        #endif
        if(iCurrMode == PRIMARY_MODE_MOVIE)
        {
            System_EnableSensor_FastUnit(); //enable 1 sensors
        }
    }
#endif
    //PHASE-2 : Init & Open Lib or LibExt
    {
        #if (SENSOR_INSERT_FUNCTION == ENABLE)
        SX_TIMER_DET_SENSOR_ID = SxTimer_AddItem(&Timer_DetSensor);
        #endif
        #if (LCD_AUTOBACKLIGHT_FUNCTION == ENABLE)
        SX_TIMER_DET_EV_ID = SxTimer_AddItem(&Timer_Sensor_DetEV);
        #endif
        #if (FLASHLIGHT_FUNCTION == ENABLE)
        SX_TIMER_DET_FLASH_ID = SxTimer_AddItem(&Timer_GxFlash_DetFlash);
        SX_TIMER_DET_RECHARGE_ID = SxTimer_AddItem(&Timer_GxFlash_DetReCharge);
        #endif
        #if ((SENSOR_INSERT_FUNCTION == ENABLE))
        SxTimer_SetFuncActive(SX_TIMER_DET_SENSOR_ID, TRUE);
        #endif
    }
}

void System_OnSensorExit(void)
{
#if ((SENSOR_CAPS_COUNT == 2) && (EVB_DUALSENSOR_PATCH == ENABLE))
    {
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
        //close sensor
        DBG_MSG("Sensor Close [%d]\r\n", IPL_PATH_1);
        Sensor_Close(IPL_PATH_1);
        DBG_MSG("Sensor Close [%d]\r\n", IPL_PATH_2);
        Sensor_Close(IPL_PATH_2);
        //turn off sensor power
        DBG_MSG("Sensor PowerOff [%d]\r\n", IPL_PATH_1);
        DrvSensor_PowerTurnOff(IPL_PATH_1);
        DBG_MSG("Sensor PowerOff [%d]\r\n", IPL_PATH_2);
        DrvSensor_PowerTurnOff(IPL_PATH_2);
//#NT#2016/10/18#Jeah Yen -end
    }
#endif
    //PHASE-2 : Close Lib or LibExt
    {
        System_EnableSensor(0); //disable all sensors
    }
    //PHASE-1 : Close Drv or DrvExt
    {
    }
}

///////////////////////////////////////////////////////////////////////
#if (SENSOR_INSERT_FUNCTION == ENABLE)
void DetSensor(void)
{
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
#if (SENSOR_INSERT_MASK == SENSOR_1)
    static BOOL cp = 0;
    BOOL p = DrvSensor_DetPlugIn(IPL_PATH_1);
    if((cp==0) && (p==1))
    {
        Ux_PostEvent(NVTEVT_SENSOR_INSERT, 1, 0); //SENSOR 1
    }
    else
    if((cp==1) && (p==0))
    {
        Ux_PostEvent(NVTEVT_SENSOR_REMOVE, 1, 0); //SENSOR 1
    }
    cp = p;
#endif
#if (SENSOR_INSERT_MASK == SENSOR_2)
    static BOOL cp = 0;
    BOOL p = DrvSensor_DetPlugIn(IPL_PATH_2);
    if((cp==0) && (p==1))
    {
        Ux_PostEvent(NVTEVT_SENSOR_INSERT, 1, 1); //SENSOR 2
    }
    else
    if((cp==1) && (p==0))
    {
        Ux_PostEvent(NVTEVT_SENSOR_REMOVE, 1, 1); //SENSOR 2
    }
    cp = p;
#endif
//#NT#2016/10/18#Jeah Yen -end
}
#endif

#if (LCD_AUTOBACKLIGHT_FUNCTION == ENABLE)
extern BOOL g_bIsInitSystemFinish;

void Sensor_DetEV(void)
{
    //check is enable to AUTO adjust?
    if(UI_GetData(FL_BrightAdjIndex) !=  LCDBRT_ADJ_AUTO)
        return;
    //Sleep not control backlight
    if(GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) > 0)
        return;
    //Playback mode can not get LV value
    if(UI_GetData(FL_ModeIndex) == DSC_MODE_PLAYBACK)
        return;

    {
        UINT32 uiLV;
        uiLV = CURR_EV()/10;
        if (g_bIsInitSystemFinish )
        {
            //DetBLcount++;
            //if (DetBLcount > 2)
            {
                //debug_msg("CURR_EV()=%d\r\n", CURR_EV());
                //debug_msg("LV=%d\r\n", uiLV);
                //debug_msg(" set bright=%d\r\n",MenuSetup_GetV2BacklightValue(uiLV));
                Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_BRIGHTLVL,1,MenuSetup_GetV2BacklightValue(uiLV));
            }
        }
    }
}
#endif

static UINT32 uiSensorEnableState_fw = 0; //fw attach sensor
static UINT32 uiSensorEnableState_hw = SENSOR_DEFAULT_ATTACH_MASK; //hw attach sensor (default value = fix attach sensors)
static UINT32 uiSensorDispState = SENSOR_DEFAULT_DISPLAY_MASK; //display sensor
static BOOL g_bBypassImageUnit = FALSE;

#if defined(YQCONFIG_CHECK_SENSOR_2_PLUGIN_SUPPORT)
BOOL IS_SENSOR_2_PLUG = FALSE;//pgl add 20160905 mb
void System_SetSensor2Plug(BOOL is_plug)
{
    IS_SENSOR_2_PLUG = is_plug;
}
BOOL System_GetSensor2Plug(void)
{
    return IS_SENSOR_2_PLUG;
}
#endif
void _System_InitSensor(void)
{
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
#if (SENSOR_INSERT_MASK == SENSOR_1)
    if(DrvSensor_DetPlugIn(IPL_PATH_1))
        uiSensorEnableState_hw |= SENSOR_1;
#endif
#if (SENSOR_INSERT_MASK == SENSOR_2)
    if(DrvSensor_DetPlugIn(IPL_PATH_2))
    {
        uiSensorEnableState_hw |= SENSOR_2;
        #if defined(YQCONFIG_CHECK_SENSOR_2_PLUGIN_SUPPORT)
        {
            System_SetSensor2Plug(TRUE);//pgl add 20160905
        }
        #endif
    }
#endif
//#NT#2016/10/18#Jeah Yen -end
}

//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
static void _System_MapSensor(void)
{
    //assign sensor mapping table
    #if (SENSOR_CAPS_COUNT == 1)
    uiSensorMapping[0] = IPL_PATH_1;
    #elif (SENSOR_CAPS_COUNT == 2)
    uiSensorMapping[0] = IPL_PATH_1;
    uiSensorMapping[1] = IPL_PATH_2;
    #elif (SENSOR_CAPS_COUNT == 4)
    uiSensorMapping[0] = IPL_PATH_1;
    uiSensorMapping[1] = IPL_PATH_2;
    uiSensorMapping[2] = IPL_PATH_3;
    uiSensorMapping[3] = IPL_PATH_4;
    #endif
    DBG_MSG("Sensor Mapping\r\n");
    ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORMAP, (UINT32)uiSensorMapping);
}

INT32 System_MapSensorID(INT32 i)
{
    return uiSensorMapping[i];
}
//#NT#2016/10/18#Jeah Yen -end

static void _System_AttachSensor(UINT32 SensorID)
{
#if ((SENSOR_CAPS_COUNT != 2) || (EVB_DUALSENSOR_PATCH != ENABLE))
    SENSOR_INIT_OBJ SenObj;
    SENSOR_DRVTAB *SenTab;
    //turn on sensor power
    DBG_MSG("Sensor PowerOn [%d]\r\n", SensorID);
    if(bFirst){TM_BOOT_BEGIN("sensor","poweron");}
    DrvSensor_PowerTurnOn(SensorID);
    if(bFirst){TM_BOOT_END("sensor","poweron");}
    //open sensor
    DBG_MSG("Sensor Open [%d]\r\n", SensorID);
    if(bFirst){TM_BOOT_BEGIN("sensor","open");}
    SenObj = DrvSensor_GetObj(SensorID);
    SenTab = DrvSensor_GetTab(SensorID);
    Sensor_Open(SensorID, &SenObj, SenTab);
    if(bFirst){TM_BOOT_END("sensor","open");}
#endif
    if(!g_bBypassImageUnit)
    //attach sensor to ImagePipe
    ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORATTACH, SensorID);
}
static void _System_DetachSensor(UINT32 SensorID)
{
    //detach sensor from ImagePipe
    ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORDETACH, SensorID);
#if ((SENSOR_CAPS_COUNT != 2) || (EVB_DUALSENSOR_PATCH != ENABLE))
    //close sensor
    DBG_MSG("Sensor Close [%d]\r\n", SensorID);
    Sensor_Close(SensorID);
    //turn off sensor power
    DBG_MSG("Sensor PowerOff [%d]\r\n", SensorID);
    DrvSensor_PowerTurnOff(SensorID);
#endif
}
static void _System_ConnectSensor(UINT32 SensorMask)
{
    uiSensorEnableState_hw |= SensorMask;
}
static void _System_DisconnectSensor(UINT32 SensorMask)
{
    uiSensorEnableState_hw &= ~SensorMask;
}
UINT32 System_GetConnectSensor(void)
{
    return uiSensorEnableState_hw;
}

extern void System_DispSource(UINT32 SensorMask, UINT32 nSensor);

void System_DispSensor(UINT32 SensorMask)
{
    UINT32 nSensor;
    nSensor = ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORCOUNT); //return 0/1/2/4
    if(nSensor == 0)
    {
        //config display sensor
        ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORDISPLAY, SensorMask);
        //config display source
        System_DispSource(SensorMask, 0);
    }
    else
    {
        //config display sensor
        ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORDISPLAY, SensorMask);
        //config display source
        System_DispSource(SensorMask, nSensor);
    }
}

UINT32 System_GetEnableSensor(void)
{
    return uiSensorEnableState_fw;
}

void System_EnableSensor(UINT32 SensorMask)
{
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
    static UINT32 uiSensorEnable = 0;
    static UINT32 uiSensorDisable = 0;
    if(SensorMask == SENSOR_ALL)
        SensorMask = System_GetConnectSensor();

    DBG_IND("enable mask=%08x\r\n", SensorMask);
    DBG_MSG("fw=%08x\r\n", uiSensorEnableState_fw);
    DBG_MSG("hw=%08x\r\n", uiSensorEnableState_hw);
    if(SensorMask & (~uiSensorEnableState_hw))
    {
        DBG_ERR("Some sensor(s) [%08x] are NOT exist!\r\n", SensorMask & (~uiSensorEnableState_hw));
        return;
    }
    uiSensorEnable = (SensorMask & (~uiSensorEnableState_fw));
    DBG_MSG("en=%08x\r\n", uiSensorEnable);
    uiSensorDisable = ((~SensorMask) & (uiSensorEnableState_fw));
    DBG_MSG("de=%08x\r\n", uiSensorDisable);
    if((uiSensorDisable & SENSOR_1) && (uiSensorEnableState_fw & SENSOR_1))
    {
        _System_DetachSensor(IPL_PATH_1);
        uiSensorEnableState_fw &= ~SENSOR_1;
        uiSensorDisable &= ~SENSOR_1;
    }
    if((uiSensorEnable & SENSOR_1) && !(uiSensorEnableState_fw & SENSOR_1))
    {
        _System_AttachSensor(IPL_PATH_1);
        uiSensorEnableState_fw |= SENSOR_1;
        uiSensorEnable &= ~SENSOR_1;
    }
    if((uiSensorDisable & SENSOR_2) && (uiSensorEnableState_fw & SENSOR_2))
    {
        _System_DetachSensor(IPL_PATH_2);
        uiSensorEnableState_fw &= ~SENSOR_2;
        uiSensorDisable &= ~SENSOR_2;
    }
    if((uiSensorEnable & SENSOR_2) && !(uiSensorEnableState_fw & SENSOR_2))
    {
        _System_AttachSensor(IPL_PATH_2);
        uiSensorEnableState_fw |= SENSOR_2;
        uiSensorEnable &= ~SENSOR_2;
    }
    #if (SENSOR_CAPS_COUNT > 2)
    if((uiSensorDisable & SENSOR_3) && (uiSensorEnableState_fw & SENSOR_3))
    {
        _System_DetachSensor(IPL_PATH_3);
        uiSensorEnableState_fw &= ~SENSOR_3;
        uiSensorDisable &= ~SENSOR_3;
    }
    if((uiSensorEnable & SENSOR_3) && !(uiSensorEnableState_fw & SENSOR_3))
    {
        _System_AttachSensor(IPL_PATH_3);
        uiSensorEnableState_fw |= SENSOR_3;
        uiSensorEnable &= ~SENSOR_3;
    }
    if((uiSensorDisable & SENSOR_4) && (uiSensorEnableState_fw & SENSOR_4))
    {
        _System_DetachSensor(IPL_PATH_4);
        uiSensorEnableState_fw &= ~SENSOR_4;
        uiSensorDisable &= ~SENSOR_4;
    }
    if((uiSensorEnable & SENSOR_4) && !(uiSensorEnableState_fw & SENSOR_4))
    {
        _System_AttachSensor(IPL_PATH_4);
        uiSensorEnableState_fw |= SENSOR_4;
        uiSensorEnable &= ~SENSOR_4;
    }
    #endif
//#NT#2016/10/18#Jeah Yen -end
}
void System_EnableSensor_FastDevice(void)
{
    g_bBypassImageUnit = TRUE;
    #if (SENSOR_DEFAULT_ATTACH_MASK & SENSOR_1)
    System_EnableSensor(SENSOR_1);
    #endif
    #if (SENSOR_DEFAULT_ATTACH_MASK & SENSOR_2)
    System_EnableSensor(SENSOR_2);
    #endif
    g_bBypassImageUnit = FALSE;
}
void System_EnableSensor_FastUnit(void)
{
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
    #if (SENSOR_DEFAULT_ATTACH_MASK & SENSOR_1)
    ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORATTACH, IPL_PATH_1);
    #endif
    #if (SENSOR_DEFAULT_ATTACH_MASK & SENSOR_2)
    ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORATTACH, IPL_PATH_2);
    #endif
//#NT#2016/10/18#Jeah Yen -end
}

static UINT32 uiSensorLUT[4] =
{
    SENSOR_1,
    SENSOR_2,
    SENSOR_3,
    SENSOR_4,
};

INT32 System_OnSensorInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 new_device_id; //sensor id
    if(paramNum != 1)
        return NVTEVT_CONSUME;
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
    new_device_id = (UINT32)(paramArray[0]); //IPL_PATH_1 ~ IPL_PATH_4
    DBG_DUMP("^Y===================================== sensor plug [SENSOR_%d]\r\n", new_device_id+1);

    debug_msg("System_OnSensorInsert-----%d, %d\r\n", SENSOR_CAPS_COUNT,  SENSOR_INSERT_MASK);
    if((System_GetConnectSensor()&SENSOR_INSERT_MASK)==SENSOR_INSERT_MASK)//start up,maybe again change mode
    {
        return NVTEVT_CONSUME;
    }
    // connect sensor IPL_PATH_n
    _System_ConnectSensor(uiSensorLUT[new_device_id]);
//#NT#2016/10/18#Jeah Yen -end

#if 0
    //check is current open device?
    if(System_GetEnableSensor() == 0)
    {
        //not current device! => only config display for next time.
        return NVTEVT_CONSUME;
    }
#endif

    //fix for CID 44954 - begin
    #if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_INSERT_MASK == SENSOR_1))
    if(uiSensorLUT[new_device_id] == SENSOR_1)
    {
        INT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
        // exit main mode, return to last mode
        if((iCurrMode == PRIMARY_MODE_MAIN) && (gSensor_iLastMode != -1))
        {
        Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, gSensor_iLastMode);
        gSensor_iLastMode = -1; //clear last mode
        }
    }
    else
    #endif
    #if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_INSERT_MASK == SENSOR_2))
    if(uiSensorLUT[new_device_id] == SENSOR_2)
    {
        INT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
        // exit main mode, return to last mode
        if((iCurrMode == PRIMARY_MODE_MAIN) && (gSensor_iLastMode != -1))
        {
        Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, gSensor_iLastMode);
        gSensor_iLastMode = -1; //clear last mode
        }
    }
    else
    #endif
    #if ((SENSOR_CAPS_COUNT == 2) && (SENSOR_INSERT_MASK == SENSOR_1))
    if(uiSensorLUT[new_device_id] == SENSOR_1)
    {
        //#NT#2017/02/17#Isiah Chang -begin
        //#NT#Notify APP to restart RTSP live view becaues of sensor plug/unplug event.
        #if(WIFI_AP_FUNC==ENABLE)
        if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
        {
            Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE), SYS_SUBMODE_WIFI);

            Delay_DelayMs(500); // Delay to ensure preview is ready. Might use other sync. mechanism.

            WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_SENSOR_NUM_CHANGED);

        }
        else
        #endif
        {
            // change to current mode again
            Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
        }
        //#NT#2017/02/17#Isiah Chang -end
    }
    else
    #endif
    #if ((SENSOR_CAPS_COUNT == 2) && (SENSOR_INSERT_MASK == SENSOR_2))
    if(uiSensorLUT[new_device_id] == SENSOR_2)
    {
        //#NT#2017/02/17#Isiah Chang -begin
        //#NT#Notify APP to restart RTSP live view becaues of sensor plug/unplug event.
        #if(WIFI_AP_FUNC==ENABLE)
        if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
        {
            Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE), SYS_SUBMODE_WIFI);

            Delay_DelayMs(500); // Delay to ensure preview is ready. Might use other sync. mechanism.

            WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_SENSOR_NUM_CHANGED);

        }
        else
        #endif
        {
            // change to current mode again
            Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
        }
        //#NT#2017/02/17#Isiah Chang -end
    }
    else
    #endif
    //fix for CID 44954 - end
    {
        DBG_ERR("Not support this insert event!!!!!\r\n");
    }

    return NVTEVT_CONSUME;
}

INT32 System_OnSensorRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 old_device_id; //sensor id
    if(paramNum != 1)
        return NVTEVT_CONSUME;
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
    old_device_id = (UINT32)(paramArray[0]); //IPL_PATH_1 ~ IPL_PATH_4
    DBG_DUMP("^Y===================================== sensor unplug [SENSOR_%d]\r\n", old_device_id+1);

    // disconnect sensor IPL_PATH_n
    _System_DisconnectSensor(uiSensorLUT[old_device_id]);
//#NT#2016/10/18#Jeah Yen -begin

#if 0
    //check is current open device?
    if(System_GetEnableSensor() == 0)
    {
        //not current device! => only config display for next time.
        return NVTEVT_CONSUME;
    }
#endif

    //fix for CID 44955 - begin
    #if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_INSERT_MASK == SENSOR_1))
    if(uiSensorLUT[old_device_id] == SENSOR_1)
    {
        INT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
        // exit current mode, enter to main mode
        #if (PHOTO_MODE==ENABLE)
        if((iCurrMode == PRIMARY_MODE_PHOTO) || (iCurrMode == PRIMARY_MODE_MOVIE))
        #else
        if(iCurrMode == PRIMARY_MODE_MOVIE)
        #endif
        {
        gSensor_iLastMode = iCurrMode; //keep last mode
        Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
        }
    }
    else
    #endif
    #if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_INSERT_MASK == SENSOR_2))
    if(uiSensorLUT[old_device_id] == SENSOR_2)
    {
        INT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
        // exit current mode, enter to main mode
        #if (PHOTO_MODE==ENABLE)
        if((iCurrMode == PRIMARY_MODE_PHOTO) || (iCurrMode == PRIMARY_MODE_MOVIE))
        #else
        if(iCurrMode == PRIMARY_MODE_MOVIE)
        #endif
        {
        gSensor_iLastMode = iCurrMode; //keep last mode
        Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
        }
    }
    else
    #endif
    #if ((SENSOR_CAPS_COUNT == 2) && (SENSOR_INSERT_MASK == SENSOR_1))
    if(uiSensorLUT[old_device_id] == SENSOR_1)
    {
        //#NT#2017/02/17#Isiah Chang -begin
        //#NT#Notify APP to restart RTSP live view becaues of sensor plug/unplug event.
        #if(WIFI_AP_FUNC==ENABLE)
        if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
        {
            Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE), SYS_SUBMODE_WIFI);

            Delay_DelayMs(500); // Delay to ensure preview is ready. Might use other sync. mechanism.

            WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_SENSOR_NUM_CHANGED);
        }
        else
        #endif
        {
            // change to current mode again
            Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
        }
        //#NT#2017/02/17#Isiah Chang -end
    }
    else
    #endif
    #if ((SENSOR_CAPS_COUNT == 2) && (SENSOR_INSERT_MASK == SENSOR_2))
    if(uiSensorLUT[old_device_id] == SENSOR_2)
    {
        //#NT#2017/02/17#Isiah Chang -begin
        //#NT#Notify APP to restart RTSP live view becaues of sensor plug/unplug event.
        #if(WIFI_AP_FUNC==ENABLE)
        if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
        {
            Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE), SYS_SUBMODE_WIFI);

            Delay_DelayMs(500); // Delay to ensure preview is ready. Might use other sync. mechanism.

            WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_SENSOR_NUM_CHANGED);
        }
        else
        #endif
        {
            // change to current mode again
            Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
        }
        //#NT#2017/02/17#Isiah Chang -end
    }
    else
    #endif
    //fix for CID 44955 - end
    {
        DBG_ERR("Not support this remove event!!!!!\r\n");
    }

    return NVTEVT_CONSUME;
}

INT32 System_OnSensorDisplay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 new_display_mask; //sensor id
    if(paramNum != 1)
        return NVTEVT_CONSUME;

    new_display_mask = (UINT32)(paramArray[0]); //SENSOR_1/2/3/4/ALL
    new_display_mask &= (SENSOR_CAPS_MASK);
    DBG_DUMP("^Y===================================== sensor display [MASK=%08x]->[MASK=%08x]\r\n", uiSensorDispState, new_display_mask);

    #if (SENSOR_CAPS_COUNT == 1)
    DBG_ERR("single sensor, not support!\r\n");
    uiSensorDispState = SENSOR_1;
    #endif
    #if (SENSOR_CAPS_COUNT == 2)

    //check is current open device?
    if(System_GetEnableSensor() == 0)
    {
        if(uiSensorDispState != new_display_mask)
        {
            // change sensor display state
            uiSensorDispState = new_display_mask;
        }
        //not current device! => only config display for next time.
        DBG_WRN("dual sensor, but ImagePipe is not enabled, ignore!\r\n");
        return NVTEVT_CONSUME;
    }

    if(System_GetConnectSensor() != (SENSOR_1|SENSOR_2))
    {
        if(uiSensorDispState != new_display_mask)
        {
            // change sensor display state
            uiSensorDispState = new_display_mask;
        }
        DBG_WRN("dual sensor, but only connect single now, ignore!\r\n");
        return NVTEVT_CONSUME;
    }
    #if (PIP_VIEW_FASTSWITCH==ENABLE)
    {
        //if recording file => change mode
        //if NOT recording file => NOT change mode ... this is the FAST SWITCH path
        if(MovRec_IsRecording() && (uiSensorDispState != new_display_mask))
        {
            // change sensor display state
            uiSensorDispState = new_display_mask;
            // change to current UI mode again
            Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
        }
    }
    #else //(PIP_VIEW_FASTSWITCH==DISABLE)
    {
        //always change mode
        if(uiSensorDispState != new_display_mask)
        {
            // change sensor display state
            uiSensorDispState = new_display_mask;
            // change to current UI mode again
            Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
        }
    }
    #endif

    #endif

    return NVTEVT_CONSUME;
}

INT32 System_OnSensorAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    int iCurrMode = System_GetState(SYS_STATE_NEXTMODE);
    DBG_IND("\r\n");
    if(bFirst)
    {
        TM_BOOT_BEGIN("sensor","attach");
    }
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
    if(bFirst)_System_MapSensor();
//#NT#2016/10/18#Jeah Yen -end

    #if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
    DBG_IND("ZMD--%x\r\n",iCurrMode);
    debug_msg("==>iCurrMode : %d..\r\n",iCurrMode);
    #endif
    #if (PLAY_MODE==ENABLE)
    if(iCurrMode == PRIMARY_MODE_PLAYBACK)
    {
        System_EnableSensor(0); //disable all sensors
        System_DispSensor(0); //not display
    }
    else
    #endif
    #if (PHOTO_MODE==ENABLE)
    if(iCurrMode == PRIMARY_MODE_PHOTO)
    {
        #if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_1))
        System_EnableSensor(SENSOR_1); //enable 1 sensors
        System_DispSensor(SENSOR_1); //display 1 sensors
        #endif
        #if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_2))
        System_EnableSensor(SENSOR_2); //enable 1 sensors
        System_DispSensor(SENSOR_2); //display 1 sensors
        #endif
        #if (SENSOR_CAPS_COUNT == 2)
        if(System_GetConnectSensor() != (SENSOR_1|SENSOR_2))
        {
            #if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_1)
            System_EnableSensor(SENSOR_1); //enable 1 sensors
            System_DispSensor(SENSOR_1); //display 1 sensors
            #endif
            #if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_2)
            System_EnableSensor(SENSOR_2); //enable 1 sensors
            System_DispSensor(SENSOR_2); //display 1 sensors
            #endif
        }
        else
        {
            System_EnableSensor((SENSOR_1|SENSOR_2)); //enable dual sensors
            System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
        }
        #endif
    }
    else
    #endif
    if(iCurrMode == PRIMARY_MODE_MOVIE)
    {
        #if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_1))
        System_EnableSensor(SENSOR_1); //enable 1 sensors
        System_DispSensor(SENSOR_1); //display 1 sensors
        #endif
        #if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_2))
        System_EnableSensor(SENSOR_2); //enable 1 sensors
        System_DispSensor(SENSOR_2); //display 1 sensors
        #endif
        #if (SENSOR_CAPS_COUNT == 2)
        if(System_GetConnectSensor() != (SENSOR_1|SENSOR_2))
        {
            #if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_1)
            System_EnableSensor(SENSOR_1); //enable 1 sensors
            System_DispSensor(SENSOR_1); //display 1 sensors
            #endif
            #if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_2)
            System_EnableSensor(SENSOR_2); //enable 1 sensors
            System_DispSensor(SENSOR_2); //display 1 sensors
            #endif
        }
        else
        {
            System_EnableSensor((SENSOR_1|SENSOR_2)); //enable dual sensors
            System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
        }
        #endif
    }
    else
    #if (USB_MODE==ENABLE)
    if(iCurrMode == PRIMARY_MODE_USBPCC)
    {
        #if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_1))
        System_EnableSensor(SENSOR_1); //enable 1 sensors
        System_DispSensor(SENSOR_1); //display 1 sensors
        #endif
        #if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_2))
        System_EnableSensor(SENSOR_2); //enable 1 sensors
        System_DispSensor(SENSOR_2); //display 1 sensors
        #endif
        #if (SENSOR_CAPS_COUNT == 2)
        if(System_GetConnectSensor() != (SENSOR_1|SENSOR_2))
        {
            #if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_1)
            System_EnableSensor(SENSOR_1); //enable 1 sensors
            System_DispSensor(SENSOR_1); //display 1 sensors
            #endif
            #if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_2)
            System_EnableSensor(SENSOR_2); //enable 1 sensors
            System_DispSensor(SENSOR_2); //display 1 sensors
            #endif
        }
        else
        {
            System_EnableSensor((SENSOR_1|SENSOR_2)); //enable dual sensors
            System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
        }
        #endif
    }
    else
    #endif
    {
        System_EnableSensor(0); //disable all sensors
        System_DispSensor(0); //not display
    }
    if(bFirst)
    {
        TM_BOOT_END("sensor","attach");
    }
    bFirst = FALSE;
    return NVTEVT_CONSUME;
}

INT32 System_OnSensorDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}

