#include "string.h"
#include "stdio.h"
#include "IPL_CAL_Debug.h"
#include "IPL_Cal_Ctrl_OV4689_IMX322LQJ_EVB_FF_Int.h"
#include "IPL_Cal_Setting_OV4689_IMX322LQJ_EVB_FF_Int.h"
#include "ae_task.h"
#include "awb_task.h"
#include "IPL_AlgInfor.h"
#include "IPL_Calibration.h"
#include "IPL_CalRW.h"
#include "IPL_Cal_DataPassing_OV4689_IMX322LQJ_EVB_FF_Int.h"
#include "IPL_Display.h"
#include "IPL_Hal.h"
#include "display.h"
#include "FileSysTsk.h"
#include "ide.h"
#include "IPL_Hal_IPE_Info.h"
#include "awb_api.h"
#include "IPL_Ctrl_flow_Int.h"
#include "IPL_Ctrl_Int.h"
#include "IPL_Hal_IFE.h"
#include "IPL_Display.h"

#define DP_Bright_only ENABLE // For lens shading, skip dp dark calibration
#define AE_TimeOut ENABLE // if ae timeout, cal status result is CAL_AE_TIMEOUT (CAL FAIL)
#define DetailOSD DISABLE // show every sensor ID/sensor mode/condition calibration result on OSD

CalIQInfo gCalIQData = {0};
//static CHAR m_CalMsg[100];
INT32 CalStringBuffer[32];
char    sFileName[64];
FST_FILE    pFileHdl;

CAL_AWB_FMT gAwbFmt = {0};
CAL_AWB_FMT gAwbGSFmt = {540,520};
UINT32 CalStrColor = UICOL_GREEN;
ER setIplNum = FALSE;
static UINT32 CalDbg[CALDEBUG_MAX_NUM] = {0};
static CAL_DEBUG_INFO CalDebugInfor1 = {0};
static CAL_DEBUG_INFO_DP CalDebugDP1 = {0};
static CAL_DEBUG_INFO_ISO CalDebugISO1 = {0};
static CAL_DEBUG_INFO_ECS CalDebugECS1 = {0};
static CAL_DEBUG_INFO_AWB CalDebugAWB1 = {0};
static CAL_DEBUG_INFO_LENSCEN CalDebugLENSCEN1 = {0};

static ER CopyTag(UINT32 TargetAdd, CAL_TAG *pTag)
{
    UINT32 Cpysize = TAG_TypeSize + pTag->TagDataLen;
    CAL_TAG *_ptmpTag;

    memcpy((void*)TargetAdd,(const void*)pTag,Cpysize);
    _ptmpTag = (CAL_TAG *)TargetAdd;

    return E_OK;
}


/**
    add section
    return:
        E_EXS:  ID duplicated
        E_NOEXS: header or init or current ID in section table not exist
        E_BOVR::"header buffer" or "section buffer" overflow
*/
_INLINE CAL_DMA_RST CfgSectionBuf(CAL_SECMAP SecItemID,UINT32 DmaAddr,UINT32 DmaSize)
{
    CAL_DMA_RST rst = {0};
    UINT32 iplID;

    for (iplID = 0; iplID < IPL_ID_MAX_NUM; iplID++)
    {
        UINT32 SectionCnt = 0;
        while (CalSectionInfo[iplID].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
        {
            if (CalSectionInfo[iplID].CalSecInfo[SectionCnt].CalItem == SecItemID)
            {
                CAL_SECMAP currentItem = CalSectionInfo[iplID].CalSecInfo[SectionCnt].CalItem;
                UINT32 currentItemSize = CalSectionInfo[iplID].CalSecInfo[SectionCnt].SecSize;

                rst.erCode = CAL_RW()->fpAddSection(currentItem,DmaAddr,currentItemSize);
                if(rst.erCode == E_OK)
                {
                    rst.RemainAddr = DmaAddr + currentItemSize;
                    rst.RemainSize = DmaSize - currentItemSize;
                }
                return rst;
            }
            else
            {
                SectionCnt++;
                if (CalSectionInfo[iplID].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
                {
                    if (iplID == IPL_ID_MAX_NUM)
                    {
                        DBG_ERR("section is not in the current IPL\r\n");
                        rst.erCode = E_NOEXS;
                        return rst;
                    }
                    else
                    {
                        continue;// to search next ipl
                    }
                }
                if (SectionCnt > MaxSectionNum)
                {
                    if (iplID == IPL_ID_MAX_NUM)
                    {
                        DBG_ERR("section is out of range\r\n");
                        rst.erCode = E_NOEXS;
                        return rst;
                    }
                    else
                    {
                        continue;// to search next ipl
                    }
                }
            }
        }
    }
    rst.erCode = E_NOEXS;
    return rst;
}

static Cal_SECTION GetCALSetting(IPL_PROC_ID Id, CAL_SECMAP_GETDATA CalSection)
{
    CAL_SECMAP CalSectionItem;
    Cal_SECTION SectionInfo = {0};
    UINT32 SectionCnt = 0;

    static CAL_SECMAP secMap[IPL_ID_MAX_NUM][Section_End] =
    {{SEC_AWB_1, SEC_ECS_1, SEC_DP_1, SEC_MSHUT_1, SEC_ISO_1, SEC_AECHK_1, SEC_LENSCEN_1},
     {SEC_AWB_2, SEC_ECS_2, SEC_DP_2, SEC_MSHUT_2, SEC_ISO_2, SEC_AECHK_2, SEC_LENSCEN_2},
     {SEC_AWB_3, SEC_ECS_3, SEC_DP_3, SEC_MSHUT_3, SEC_ISO_3, SEC_AECHK_3, SEC_LENSCEN_3},
     {SEC_AWB_4, SEC_ECS_4, SEC_DP_4, SEC_MSHUT_4, SEC_ISO_4, SEC_AECHK_4, SEC_LENSCEN_4}};

     CalSectionItem = secMap[Id][CalSection];

    /*
        get current Section information
    */
    while (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
    {
        if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == CalSectionItem)
        {
            SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
            break;
        }
        else
        {
            SectionCnt++;
            if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
            {
                SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
                DBG_ERR("section is not in the current IPL\r\n");
            }
            if (SectionCnt > MaxSectionNum)
            {
                DBG_ERR("section is out of range\r\n");
                return SectionInfo;
            }
        }
    }
    return SectionInfo;
}

static BOOL SetAdustRst(IPL_PROC_ID Id, fpUI_cb fpUIcb, CAL_DEBUG_INFO *CalDebugInfor)
{

    UINT32 ConditionCnt = 0;
    UINT32 SenModeCnt = 0;
    UINT32 MaxSenModeCnt = 0;
    Cal_SECTION SettingInfo = {0};

    //set DP data
    if (CalDebugInfor->CalDebugDP != NULL)
    {
        SettingInfo =  GetCALSetting(Id, SEC_DP);
        ConditionCnt = 0;
        MaxSenModeCnt = 0;
        while (SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
        {
            SenModeCnt = 0;
            while (SettingInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
            {
                if (fpUIcb != NULL) // DP dark
                {
                    CalDebugInfo[Id].CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].AeTimeout = CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].AeTimeout;
                    CalDebugInfo[Id].CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].Lum       = CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].Lum;
                    CalDebugInfo[Id].CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].ISO       = CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].ISO;
                    CalDebugInfo[Id].CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].ISOBase   = CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].ISOBase;
                    CalDebugInfo[Id].CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].ExpT      = CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].ExpT;
                    CalDebugInfo[Id].CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].Ratio     = CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].Ratio;
                }
                else //DP bright
                {
                    CalDebugInfo[Id].CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].AeTimeout = CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].AeTimeout;
                    CalDebugInfo[Id].CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].Lum       = CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].Lum;
                    CalDebugInfo[Id].CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].ISO       = CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].ISO;
                    CalDebugInfo[Id].CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].ISOBase   = CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].ISOBase;
                    CalDebugInfo[Id].CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].ExpT      = CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].ExpT;
                    CalDebugInfo[Id].CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].Ratio     = CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].Ratio;
                }
                SenModeCnt++;
            }
            if (ConditionCnt == 0)
            {
                MaxSenModeCnt = SenModeCnt;
            }
            ConditionCnt++;
        }
    }

    //set iso data
    if (CalDebugInfor->CalDebugISO != NULL)
    {
        SettingInfo =  GetCALSetting(Id, SEC_ISO);
        ConditionCnt = 0;
        MaxSenModeCnt = 0;
        while (SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
        {
            SenModeCnt = 0;
            while (SettingInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
            {
                CalDebugInfo[Id].CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout;
                CalDebugInfo[Id].CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum       = CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum;
                CalDebugInfo[Id].CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO       = CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO;
                CalDebugInfo[Id].CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase   = CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase;
                CalDebugInfo[Id].CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT      = CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT;
                CalDebugInfo[Id].CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio     = CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio;

                SenModeCnt++;
            }
            if (ConditionCnt == 0)
            {
                MaxSenModeCnt = SenModeCnt;
            }
            ConditionCnt++;
        }
    }

    //set ecs data
    if (CalDebugInfor->CalDebugECS != NULL)
    {
        SettingInfo =  GetCALSetting(Id, SEC_ECS);
        ConditionCnt = 0;
        MaxSenModeCnt = 0;
        while (SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
        {
            SenModeCnt = 0;
            while (SettingInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
            {
                CalDebugInfo[Id].CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout;
                CalDebugInfo[Id].CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum       = CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum;
                CalDebugInfo[Id].CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO       = CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO;
                CalDebugInfo[Id].CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase   = CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase;
                CalDebugInfo[Id].CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT      = CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT;
                CalDebugInfo[Id].CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio     = CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio;

                SenModeCnt++;
            }
            if (ConditionCnt == 0)
            {
                MaxSenModeCnt = SenModeCnt;
            }
            ConditionCnt++;
        }
    }

    //set awb data
    if (CalDebugInfor->CalDebugAWB != NULL)
    {
        SettingInfo =  GetCALSetting(Id, SEC_AWB);
        ConditionCnt = 0;
        MaxSenModeCnt = 0;
        while (SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
        {
            SenModeCnt = 0;
            while (SettingInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
            {
                CalDebugInfo[Id].CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout;
                CalDebugInfo[Id].CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum       = CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum;
                CalDebugInfo[Id].CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO       = CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO;
                CalDebugInfo[Id].CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase   = CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase;
                CalDebugInfo[Id].CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT      = CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT;
                CalDebugInfo[Id].CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio     = CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio;

                SenModeCnt++;
            }
            if (ConditionCnt == 0)
            {
                MaxSenModeCnt = SenModeCnt;
            }
            ConditionCnt++;
        }
    }
    //set lenscenter data
    if (CalDebugInfor->CalDebugLENSCEN != NULL)
    {
        SettingInfo =  GetCALSetting(Id, SEC_LENSCEN);
        ConditionCnt = 0;
        MaxSenModeCnt = 0;
        while (SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
        {
            SenModeCnt = 0;
            while (SettingInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
            {
                CalDebugInfo[Id].CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout;
                CalDebugInfo[Id].CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum       = CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum;
                CalDebugInfo[Id].CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO       = CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO;
                CalDebugInfo[Id].CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase   = CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase;
                CalDebugInfo[Id].CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT      = CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT;
                CalDebugInfo[Id].CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio     = CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio;

                SenModeCnt++;
            }
            if (ConditionCnt == 0)
            {
                MaxSenModeCnt = SenModeCnt;
            }
            ConditionCnt++;
        }
    }
    return TRUE;
}

static BOOL GetAdjustRst(IPL_PROC_ID Id, fpUI_cb fpUIcb, CAL_DEBUG_INFO *CalDebugInfor)
{
    UINT32 ConditionCnt = 0;
    UINT32 SenModeCnt = 0;
    UINT32 MaxSenModeCnt = 0;
    pCAL_TAG pTag;
    Cal_SECTION SettingInfo = {0};
    CAL_DATA_INFO CaldataInfo = {0};
    BOOL Rst = TRUE;

    // get ae information from SetAdustRst
    CalDebugInfor->CalDebugDP = CalDebugInfo[Id].CalDebugDP;
    CalDebugInfor->CalDebugISO = CalDebugInfo[Id].CalDebugISO;
    CalDebugInfor->CalDebugECS = CalDebugInfo[Id].CalDebugECS;
    CalDebugInfor->CalDebugAWB = CalDebugInfo[Id].CalDebugAWB;
    CalDebugInfor->CalDebugLENSCEN= CalDebugInfo[Id].CalDebugLENSCEN;

    /*
        get DP calibration result
    */
    // get section(DP) information from IPL_Cal_Setting.c
    SettingInfo =  GetCALSetting(Id, SEC_DP);
    ConditionCnt = 0;
    MaxSenModeCnt = 0;
    // get DP calibration data from PStore
    while (SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
        SenModeCnt = 0;
        while (SettingInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            CaldataInfo.CalCondition[0] = SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin;
            CaldataInfo.StationID = SEC_DP;
            CaldataInfo.SenMode = SettingInfo.SenModeInfo[SenModeCnt].SenMode;
            // read cal status from DRAM
            pTag = GetCalData(Id, CaldataInfo);
            if (pTag != NULL)
            {
                CAL_DP_FMT* DP_pTag;
                DP_pTag = (CAL_DP_FMT*)&pTag->TagData;
                /*
                    [condition 0 + sensor mode 0,1,2,3,4,
                     condition 1 + sensor mode 0,1,2,3,4,
                     condition 2 + sensor mode 0,1,2,3,4]
                */
                CalDebugInfor->CalDebugDP->CalStatusRst[MaxSenModeCnt + SenModeCnt] = pTag->CAL_STATUS;

                if (pTag->CAL_STATUS == _CAL_DP_D_OK)
                {
                    CalDebugInfor->CalDebugDP->DCnt[MaxSenModeCnt + SenModeCnt] = DP_pTag->DPCnt.PixelCnt;
                    CalDebugInfor->CalDebugDP->BCnt[MaxSenModeCnt + SenModeCnt] = 0;
                }
                else if (pTag->CAL_STATUS == _CAL_DP_B_OK)
                {
                    CalDebugInfor->CalDebugDP->DCnt[MaxSenModeCnt + SenModeCnt] = 0;
                    CalDebugInfor->CalDebugDP->BCnt[MaxSenModeCnt + SenModeCnt] = DP_pTag->DPCnt.PixelCnt;
                }
                else
                {
                    CalDebugInfor->CalDebugDP->DCnt[MaxSenModeCnt + SenModeCnt] = DP_pTag->DPCnt.PixelCnt;
                    CalDebugInfor->CalDebugDP->BCnt[MaxSenModeCnt + SenModeCnt] = DP_pTag->DPCnt.PixelCnt;
                }
            }

            SenModeCnt++;
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt++;
    }

    /*
        get ISO calibration result
    */
    // get section(ISO) information from IPL_Cal_Setting.c
    SettingInfo =  GetCALSetting(Id, SEC_ISO);
    ConditionCnt = 0;
    MaxSenModeCnt = 0;
    // get ISO calibration data from PStore
    while (SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
        SenModeCnt = 0;
        while (SettingInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            CaldataInfo.CalCondition[0] = SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin;
            CaldataInfo.StationID = SEC_ISO;
            CaldataInfo.SenMode = SettingInfo.SenModeInfo[SenModeCnt].SenMode;
            // read cal status from DRAM
            pTag = GetCalData(Id, CaldataInfo);
            if (pTag != NULL)
            {
                CAL_ISO_FMT* ISO_pTag;
                ISO_pTag = (CAL_ISO_FMT*)&pTag->TagData;
                /*
                    [condition 0 + sensor mode 0,1,2,3,4,
                     condition 1 + sensor mode 0,1,2,3,4,
                     condition 2 + sensor mode 0,1,2,3,4]
                */
                CalDebugInfor->CalDebugISO->CalStatusRst[MaxSenModeCnt + SenModeCnt] = pTag->CAL_STATUS;
                CalDebugInfor->CalDebugISO->CalISOBase[MaxSenModeCnt + SenModeCnt] = ISO_pTag->GainBase;
            }

            SenModeCnt++;
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt++;
    }

    /*
        get ECS calibration result
    */
    // get section(ECS) information from IPL_Cal_Setting.c
    SettingInfo =  GetCALSetting(Id, SEC_ECS);
    ConditionCnt = 0;
    MaxSenModeCnt = 0;
    // get ECS calibration data from PStore
    while (SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
        SenModeCnt = 0;
        while (SettingInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            CaldataInfo.CalCondition[0] = SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin;
            CaldataInfo.StationID = SEC_ECS;
            CaldataInfo.SenMode = SettingInfo.SenModeInfo[SenModeCnt].SenMode;
            // read cal status from DRAM
            pTag = GetCalData(Id, CaldataInfo);
            if (pTag != NULL)
            {
                /*
                    [condition 0 + sensor mode 0,1,2,3,4,
                     condition 1 + sensor mode 0,1,2,3,4,
                     condition 2 + sensor mode 0,1,2,3,4]
                */
                CalDebugInfor->CalDebugECS->CalStatusRst[MaxSenModeCnt + SenModeCnt] = pTag->CAL_STATUS;
            }

            SenModeCnt++;
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt++;
    }

    /*
        get AWB calibration result
    */
    // get section(AWB) information from IPL_Cal_Setting.c
    SettingInfo =  GetCALSetting(Id, SEC_AWB);
    ConditionCnt = 0;
    MaxSenModeCnt = 0;
    // get AWB calibration data from PStore
    while (SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
        SenModeCnt = 0;
        while (SettingInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            CaldataInfo.CalCondition[0] = SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin;
            CaldataInfo.StationID = SEC_AWB;
            CaldataInfo.SenMode = SettingInfo.SenModeInfo[SenModeCnt].SenMode;
            // read cal status from DRAM
            pTag = GetCalData(Id, CaldataInfo);
            if (pTag != NULL)
            {
                CAL_AWB_FMT* AWB_pTag;
                AWB_pTag = (CAL_AWB_FMT*)&pTag->TagData;
                /*
                    [condition 0 + sensor mode 0,1,2,3,4,
                     condition 1 + sensor mode 0,1,2,3,4,
                     condition 2 + sensor mode 0,1,2,3,4]
                */
                CalDebugInfor->CalDebugAWB->CalStatusRst[MaxSenModeCnt + SenModeCnt] = pTag->CAL_STATUS;
                CalDebugInfor->CalDebugAWB->CalRgain[MaxSenModeCnt + SenModeCnt] = AWB_pTag->Rg;
                CalDebugInfor->CalDebugAWB->CalBgain[MaxSenModeCnt + SenModeCnt] = AWB_pTag->Bg;
            }

            SenModeCnt++;
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt++;
    }

    /*
        get LensCenter calibration result
    */
    // get section(LENSCEN) information from IPL_Cal_Setting.c
    SettingInfo =  GetCALSetting(Id, SEC_LENSCEN);
    ConditionCnt = 0;
    MaxSenModeCnt = 0;
    // get LENSCEN calibration data from PStore
    while (SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
        SenModeCnt = 0;
        while (SettingInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            CaldataInfo.CalCondition[0] = SettingInfo.CalAeInfoCon[ConditionCnt].CalConditin;
            CaldataInfo.StationID = SEC_LENSCEN;
            CaldataInfo.SenMode = SettingInfo.SenModeInfo[SenModeCnt].SenMode;
            // read cal status from DRAM
            pTag = GetCalData(Id, CaldataInfo);
            if (pTag != NULL)
            {
                CAL_LENSCEN_FMT* LENSCEN_pTag;
                LENSCEN_pTag = (CAL_LENSCEN_FMT*)&pTag->TagData;
                /*
                    [condition 0 + sensor mode 0,1,2,3,4,
                     condition 1 + sensor mode 0,1,2,3,4,
                     condition 2 + sensor mode 0,1,2,3,4]
                */
                CalDebugInfor->CalDebugLENSCEN->CalStatusRst[MaxSenModeCnt + SenModeCnt] = pTag->CAL_STATUS;
                CalDebugInfor->CalDebugLENSCEN->CalActStrX[MaxSenModeCnt + SenModeCnt] = LENSCEN_pTag->ActStr_X;
                CalDebugInfor->CalDebugLENSCEN->CalActStrY[MaxSenModeCnt + SenModeCnt] = LENSCEN_pTag->ActStr_Y;
                CalDebugInfor->CalDebugLENSCEN->CalCenSftX[MaxSenModeCnt + SenModeCnt] = LENSCEN_pTag->CenterSft_X;
                CalDebugInfor->CalDebugLENSCEN->CalCenSftY[MaxSenModeCnt + SenModeCnt] = LENSCEN_pTag->CenterSft_Y;
                CalDebugInfor->CalDebugLENSCEN->CalReliability[MaxSenModeCnt + SenModeCnt] = LENSCEN_pTag->Reliability;
            }

            SenModeCnt++;
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt++;
    }

    return Rst;
}

static ER ISO_Adjust(IPL_PROC_ID Id, fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(Id);
    CAL_AE_RST aeRst = {0};
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor = {0};
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    //#NT#2016/06/07#Silvia Wu -begin
    //#NT# fixed coveritty warning
    //CAL_ISO_FMT pISOFmt1;
    //CAL_ISO_FMT* pISOFmt = &pISOFmt1;
    CAL_ISO_FMT* pISOFmt;
    //#NT#2016/06/07#Silvia Wu -end
    UINT32 Defalut_Gain_Base = 0;   //default gain base.
    UINT32 Gain_Thr;            //default gain base threshold (%).
    CAL_SECMAP CalSectionItem;
    Cal_SECTION SectionInfo = {0};
    CAL_ISO_INFO CalInfo1 = {0};
    CAL_ISO_INFO* CalInfo = &CalInfo1;
    CTRL_PTR CtrlInfo = {0};
    CalBuf buf;
    UINT32 TempExpT;
    UINT32 buf_cnt = 0;
    static CHAR m_CalMsg[100];
    UINT32 MaxSenModeCnt = 0;
    UINT32 SenModeCnt = 0;
    ER rt = E_OK;

    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_ISO_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_ISO_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_ISO_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_ISO_4;
    }
    else
    {
        CalSectionItem = SEC_ISO_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    CAL_RW()->fpDeleteSection(CalSectionItem);

    /*
        get current Section (ISO) information
    */
    UINT32 SectionCnt = 0;
    while (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
    {
        if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == CalSectionItem)
        {
            SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
            CalInfo = CalSectionInfo[Id].CalInfo->IsoInfor;
            break;
        }
        else
        {
            SectionCnt++;
            if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
            {
                SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
                DBG_ERR("section is not in the current IPL\r\n");
            }
            if (SectionCnt > MaxSectionNum)
            {
                DBG_ERR("section is out of range\r\n");
                return E_SYS;
            }
        }
    }

    //set debug information for SetAdjustRst()
    CAL_DEBUG_INFO *CalDebugInfor = &CalDebugInfor1;
    CAL_DEBUG_INFO_ISO* CalDebugISO = &CalDebugISO1;
    CalDebugInfor->CalDebugDP = NULL;
    CalDebugInfor->CalDebugISO = CalDebugISO;
    CalDebugInfor->CalDebugECS = NULL;
    CalDebugInfor->CalDebugAWB = NULL;

    // stop all pipeline
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //allocate section buffer
    dmaRst = CfgSectionBuf(CalSectionItem,DmaAddr,DmaSize);
    if(dmaRst.erCode != E_OK)
        return E_SYS;
    else
    {
        DmaAddr = dmaRst.RemainAddr;
        DmaSize = dmaRst.RemainSize;
    }

    //shift current dram address (shift size: 1 tag size), temporal buf
    TagTempAddr = DmaAddr;
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_ISO_FMT);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);


    if(DmaSize >= TagSize)
    {
        DmaAddr += TagSize;
        DmaSize -= TagSize;
    }
    else
    {
        DBG_ERR("section buffer overflow\r\n");
        return E_SYS;
    }

    /*
        init buffer
        -> start run all pipeline (with sie only flag) to re-set sie output address
        -> pause sie
        -> release buffer
    */
    // ipp buf init
    //buf.Id = Id;
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    buf.Id = bufID;
    buf.StartAddr = DmaAddr;
    buf.Size = DmaSize;
    CalApi()->fpBufEvent()->fpInit(buf);

    SenModeCnt = 0;
    UINT32 maxSize = 0;
    UINT32 senModeSize = 0;
    UINT32 maxSizeSenMode = 0;
    while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
    {
        rt = Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

         //coverity warning
        if (rt != E_OK || Drv_Sensor.Mode == NULL)
           return E_SYS;

        senModeSize = Drv_Sensor.Mode->TransHD[0].DataSize * Drv_Sensor.Mode->TransVD[0].DataSize;
        if (maxSize < senModeSize)
        {
            maxSize = senModeSize;
            maxSizeSenMode = SenModeCnt;
        }
        SenModeCnt++;
    }

    // start run
    Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[maxSizeSenMode].SenMode, &Drv_Sensor);
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;
    CtrlInfo.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[maxSizeSenMode]);
    CtrlInfo.fpRdyImg = NULL;
    CalApi()->fpSetMode(Id,CAL_IPL_ALLRUN,&CtrlInfo,Cal_CtrlInit()->fpCalStation_CtrlInit);

    // pause sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

    //release buf
    CalApi()->fpBufEvent()->fpUnInit(buf.Id);

    //clean OSD to show calibration result on OSD
#if DetailOSD
    if(fpUIcb)
        fpUIcb(UICB_CLEAN,(CHAR*)m_CalMsg,UICOL_RED);
#endif

    /*
        calibration for per sensor mode
    */
    UINT32 ConditionCnt = 0;
    while (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
        switch (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin)
        {
            case CAL_CONDITION_DEFAULT:
            default:
                buf_cnt = snprintf(NULL, 0, "sensor %d Set to Bright", Id);
                snprintf((char*)m_CalMsg, buf_cnt, "sensor %d Set to Bright", Id);
                break;
        }

        SenModeCnt = 0;
        while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            //ipp buf init
            //buf.Id = Id;
            buf.Id = bufID;
            buf.StartAddr = DmaAddr;
            buf.Size = DmaSize;
            CalApi()->fpBufEvent()->fpInit(buf);

            //init sensor info
            CtrlPtr.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[SenModeCnt]);
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);
            CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;

            //config target sensor mode
            IPC_WaitVD(Id,TRUE); //IPC_WaitVD(SensorID,TRUE);
            Sensor_ChgMode(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
            IPC_WaitVD(Id,TRUE); //IPC_WaitVD(SensorID,TRUE);

            //get sensor info
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

            //chg sie to capture mode
            CalApi()->fpSetMode(Id,CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpISO_CtrlInit);
            Bayer = CalApi()->fpGetRawInfo(Id);

            //cal 3a converage //ae
            //run alg

            TempExpT = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0];
            SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0]*100/Drv_Sensor.Mode->biningRatio;

            aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

            SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = TempExpT;

            //pause sie
            CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

            pISOFmt = (CAL_ISO_FMT*)&pTag->TagData;
            pISOFmt->GainBase = aeRst.uiISOBase;

            DBG_IND("ISO Rst(SensorMode = %d): Gain Base = %d\r\n",SectionInfo.SenModeInfo[SenModeCnt].SenMode, pISOFmt->GainBase);

            //save calibration raw
            if (CalDbg[CALDEBUG_RAW] == 1)
            {
                CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
            }

            Defalut_Gain_Base = CalInfo->GS_GainBase;
            Gain_Thr = CalInfo->Threshold;
#if 0   //support update ISO threshold from text file
            UINT32 ISO_THR_Size = 2;
            UB ISO_THR_Addr[2];
            UINT32 Gain_Threshold_In; //Input hreshold from text file if necessary.

            sprintf(sFileName, "A:\\ISO_THR.txt", 0);
            pFileHdl = FileSys_OpenFile(sFileName, FST_OPEN_READ);

            if (pFileHdl)
            {
                FileSys_ReadFile(pFileHdl, (UINT8 *)ISO_THR_Addr, &ISO_THR_Size, 0, NULL);
                Gain_Threshold_In =(ISO_THR_Addr[0]-0x30)*10+(ISO_THR_Addr[1]-0x30);

                DBG_ERR("GAIN THR CHANGED: %d\r\n",Gain_Threshold_In);

                if ((Gain_Threshold_In >= 0) && (Gain_Threshold_In <= 100))
                {
                    Default_Gain_Thr = Gain_Threshold_In;
                }
                FileSys_CloseFile(pFileHdl);
            }
#endif
#if 0   //support update ISO BASE from text file
            UINT32 ISO_BASE_Size = 3;
            UB ISO_BASE_Addr[3];
            UINT32 Gain_Base_In; //Input gain base from text file if necessary.

            sprintf(sFileName, "A:\\ISO_BASE.txt", 0);
            pFileHdl = FileSys_OpenFile(sFileName, FST_OPEN_READ);

            if (pFileHdl)
            {
                FileSys_ReadFile(pFileHdl, (UINT8 *)ISO_BASE_Addr, &ISO_BASE_Size, 0, NULL);
                Gain_Base_In =(ISO_BASE_Addr[0]-0x30)*100+(ISO_BASE_Addr[1]-0x30)*10+(ISO_BASE_Addr[2]-0x30);

                DBG_ERR("GAIN BASE CHANGED: %d\r\n",Gain_Base_In);

                if (Gain_Base_In != 0)
                {
                    Defalut_Gain_Base = Gain_Base_In;
                }
                FileSys_CloseFile(pFileHdl);
            }
#endif

            //chk calibration rst OK or NG
            if (((100*abs(Defalut_Gain_Base - pISOFmt->GainBase))/Defalut_Gain_Base) <= Gain_Thr)
            {
                pTag->CAL_STATUS = _CAL_OK;
                gCalIQData.Cal_Status.ISO_STATUS = _CAL_OK;
                buf_cnt = snprintf(NULL, 0, "ISO  PASS");
                snprintf((char*)m_CalMsg, buf_cnt, "ISO  PASS");
                DBG_ERR("ISO  PASS\r\n");


#if AE_TimeOut
                if (aeRst.status == CAL_AE_TIMEOUT)
                {
                    DBG_WRN("ISO  AE timeout\r\n");
                    pTag->CAL_STATUS = _CAL_AE_TIMEOUT;
                    gCalIQData.Cal_Status.ISO_STATUS = _CAL_AE_TIMEOUT;
                }
#endif
            }
            else
            {
                pTag->CAL_STATUS = _CAL_NONE;
                gCalIQData.Cal_Status.ISO_STATUS = _CAL_NONE;
                buf_cnt = snprintf(NULL, 0, "ISO  NG");
                snprintf((char*)m_CalMsg, buf_cnt, "ISO  NG");
                DBG_ERR("ISO  NG\r\n");
            }

            pTag->TagDataLen = sizeof(CAL_ISO_FMT);
            pTag->TagID = (UINT32)GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin);

            if (CalDbg[CALDEBUG_RESULT_DATA] == 1)
            {
                CalApi()->fpSaveRAW((UINT32)&pTag->TagID,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->CAL_STATUS,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagDataLen,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagData,1,pTag->TagDataLen);
            }

            //write tag
            CAL_RW()->fpAddTag(CalSectionItem,pTag);

            //release buf
            CalApi()->fpBufEvent()->fpUnInit(buf.Id);

            DBG_ERR("DEFAULT BASE: %d\r\n",Defalut_Gain_Base);
            DBG_ERR("CAL GAIN BASE: %d\r\n",pISOFmt->GainBase);

#if DetailOSD //show calibration result on OSD
            if (pTag->CAL_STATUS != _CAL_OK)
            {
                buf_cnt = snprintf(NULL, 0, "ISO FAIL %d(mode %d)", aeRst.uiISOBase, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "ISO FAIL %d(mode %d)", aeRst.uiISOBase, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
            else
            {
                buf_cnt = snprintf(NULL, 0, "ISO PASS %d(mode %d)", aeRst.uiISOBase, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "ISO PASS %d(mode %d)", aeRst.uiISOBase, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
#endif

            //set adjust result information for debug
            if (aeRst.status == CAL_AE_TIMEOUT)
                CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = TRUE;
            else
                CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = FALSE;

            CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum        = aeRst.convergeLum;
            CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO        = aeRst.uiISO[0];
            CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase    = aeRst.uiISOBase;
            CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT       = aeRst.uiExpT[0];
            CalDebugInfor->CalDebugISO->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio      = aeRst.uiRatio[0];
            SetAdustRst(Id, fpUIcb, CalDebugInfor);

            //next sensor mode
            SenModeCnt++;
            if (SenModeCnt > MaxSenModeNum)
            {
                DBG_ERR("sensor mode number is out of range\r\n");
                break;
            }
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt ++;
    }

    //resume sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIERESUME,NULL,NULL);

    //stop all
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(CalSectionItem);

#if DetailOSD //show calibration result on OSD
    if(fpUIcb)
        fpUIcb(UICB_WAITKEY,NULL,UICOL_GREEN);
#endif

    return E_OK;
}

static ER ECS_Adjust(IPL_PROC_ID Id, fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(Id);
    CAL_AE_RST aeRst = {0};
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor = {0};
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    CAL_ECS_FMT* pECSFmt;
    USIZE tblSize = {0};
    BOOL ECS_Over_Spec;
    CAL_SECMAP CalSectionItem;
    Cal_SECTION SectionInfo = {0};
    CAL_ECS_INFO CalInfo1 = {0};
    CAL_ECS_INFO* CalInfo = &CalInfo1;
    CTRL_PTR CtrlInfo = {0};
    CalBuf buf;
    UINT32 TempExpT;
    UINT32 buf_cnt = 0;
    static CHAR m_CalMsg[100];
    UINT32 MaxSenModeCnt = 0;
    UINT32 SenModeCnt = 0;
    ER rt = E_OK;

    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_ECS_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_ECS_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_ECS_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_ECS_4;
    }
    else
    {
        CalSectionItem = SEC_ECS_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    CAL_RW()->fpDeleteSection(CalSectionItem);

    /*
        get current Section (ECS) information
    */
    UINT32 SectionCnt = 0;
    while (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
    {
        if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == CalSectionItem)
        {
            SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
            CalInfo = CalSectionInfo[Id].CalInfo->EcsInfor;
            break;
        }
        else
        {
            SectionCnt++;
            if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
            {
                SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
                DBG_ERR("section is not in the current IPL\r\n");
            }
            if (SectionCnt > MaxSectionNum)
            {
                DBG_ERR("section is out of range\r\n");
                return E_SYS;
            }
        }
    }

    // stop all pipeline
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    // allocate section buffer
    dmaRst = CfgSectionBuf(CalSectionItem,DmaAddr,DmaSize);
    if(dmaRst.erCode != E_OK)
        return E_SYS;
    else
    {
        DmaAddr = dmaRst.RemainAddr;
        DmaSize = dmaRst.RemainSize;
    }

    ECS_MAP_SIZE(CalInfo->Ecs_map, &tblSize.w);
    tblSize.h = tblSize.w;

    // shift current dram address (shift size: 1 tag size), temporal buf
    TagTempAddr = DmaAddr;
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_ECS_HEADER)+sizeof(UINT32)*tblSize.w*tblSize.h;
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize >= TagSize)
    {
        DmaSize -= TagSize;
        DmaAddr += TagSize;
    }
    else
    {
        DBG_ERR("section buffer overflow\r\n");
        return E_SYS;
    }

    /*
        init buffer
        -> start run all pipeline (with sie only flag) to re-set sie output address
        -> pause sie
        -> release buffer
    */
    // ipp buf init
    //buf.Id = Id;
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    buf.Id = bufID;
    buf.StartAddr = DmaAddr;
    buf.Size = DmaSize;
    CalApi()->fpBufEvent()->fpInit(buf);

    SenModeCnt = 0;
    UINT32 maxSize = 0;
    UINT32 senModeSize = 0;
    UINT32 maxSizeSenMode = 0;
    while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
    {
        rt = Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

        //coverity warning
        if (rt != E_OK || Drv_Sensor.Mode == NULL)
           return E_SYS;

        senModeSize = Drv_Sensor.Mode->TransHD[0].DataSize * Drv_Sensor.Mode->TransVD[0].DataSize;
        if (maxSize < senModeSize)
        {
            maxSize = senModeSize;
            maxSizeSenMode = SenModeCnt;
        }
        SenModeCnt++;
    }

    // start run
    Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[maxSizeSenMode].SenMode, &Drv_Sensor);
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;
    CtrlInfo.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[maxSizeSenMode]);
    CtrlInfo.fpRdyImg = NULL;
    CalApi()->fpSetMode(Id,CAL_IPL_ALLRUN,&CtrlInfo,Cal_CtrlInit()->fpCalStation_CtrlInit);

    // pause sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

    //release buf
    CalApi()->fpBufEvent()->fpUnInit(buf.Id);

    //set debug information for SetAdjustRst()
    CAL_DEBUG_INFO *CalDebugInfor = &CalDebugInfor1;
    CAL_DEBUG_INFO_ECS* CalDebugECS = &CalDebugECS1;
    CalDebugInfor->CalDebugDP = NULL;
    CalDebugInfor->CalDebugISO = NULL;
    CalDebugInfor->CalDebugECS = CalDebugECS;
    CalDebugInfor->CalDebugAWB = NULL;

    //clean OSD to show calibration result on OSD
#if DetailOSD
    if(fpUIcb)
        fpUIcb(UICB_CLEAN,(CHAR*)m_CalMsg,UICOL_RED);
#endif

    /*
        calibration for per sensor mode
    */
    UINT32 ConditionCnt = 0;
    while (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
#if 0
        switch (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin)
        {
            case CAL_CONDITION_ZOOM0:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright & zoom 0", Id);
                break;
            case CAL_CONDITION_ZOOM1:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright & zoom 1", Id);
                break;
            case CAL_CONDITION_ZOOM2:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright & zoom 2", Id);
                break;
            case CAL_CONDITION_ZOOM3:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright & zoom 3", Id);
                break;
            case CAL_CONDITION_DEFAULT:
            default:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright", Id);
                break;
        }

        if(fpUIcb)
            fpUIcb(UICB_NONE,(CHAR*)m_CalMsg,UICOL_GREEN);
#endif
        SenModeCnt = 0;
        while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            //ipp buf init
            buf.Id = bufID;
            buf.StartAddr = DmaAddr;
            buf.Size = DmaSize;
            CalApi()->fpBufEvent()->fpInit(buf);

            //init sensor info
            CtrlPtr.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[SenModeCnt]);
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);
            CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;

            //config target sensor mode
            IPC_WaitVD(Id,TRUE); //IPC_WaitVD(SensorID,TRUE);
            Sensor_ChgMode(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
            IPC_WaitVD(Id,TRUE); //IPC_WaitVD(SensorID,TRUE);

            //get sensor info
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

            //chg sie to capture mode
            CalApi()->fpSetMode(Id,CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpECS_CtrlInit);
            Bayer = CalApi()->fpGetRawInfo(Id);

            //cal 3a converage
            //ae
            //aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon[ConditionCnt].CalAeInfo,Id);
            TempExpT = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0];
            SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0]*100/Drv_Sensor.Mode->biningRatio;

            aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

            SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = TempExpT;

            //pause sie
            CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

            //run alg
            pECSFmt = (CAL_ECS_FMT*)&pTag->TagData;
            pECSFmt->header.Ecs_bit = CalInfo->Ecs_bit;
            pECSFmt->header.Ecs_map = CalInfo->Ecs_map;

            //save raw
            if (CalDbg[CALDEBUG_RAW] == 1)
            {
                CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
            }

            ECS_Over_Spec = CalApi()->fpAlgEvent()->fpECS_Tbl(&Bayer.BaseInfo, CalInfo->CompenRatio,(UINT32)&pECSFmt->ECSBlockWin,tblSize.w,tblSize.h,pECSFmt->header.Ecs_bit);
            DBG_IND("ECS (SensorMode = %d) finish!!\r\n",SectionInfo.SenModeInfo[SenModeCnt].SenMode);

            //chk calibration rst OK or NG
            if (ECS_Over_Spec == TRUE)
            {
                pTag->CAL_STATUS = _CAL_OverFlow;
                gCalIQData.Cal_Status.ECS_STATUS = _CAL_OverFlow;

                buf_cnt = snprintf(NULL, 0, "MODE %d COLOR SHADING OVERFLOW", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt, "MODE %d COLOR SHADING OVERFLOW", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                DBG_ERR("COLOR SHADING OVERFLOW\r\n");

                CalStrColor = UICOL_RED;
            }
            else
            {
                pTag->CAL_STATUS = _CAL_OK;
                gCalIQData.Cal_Status.ECS_STATUS = _CAL_OK;
                buf_cnt = snprintf(NULL, 0, "MODE %d COLOR SHADING CAL OK", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt, "MODE %d COLOR SHADING CAL OK", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                DBG_ERR("COLOR SHADING CAL OK\r\n");

                CalStrColor = UICOL_GREEN;

#if AE_TimeOut
                if (aeRst.status == CAL_AE_TIMEOUT)
                {
                    DBG_WRN("ECS  AE timeout\r\n");
                    pTag->CAL_STATUS = _CAL_AE_TIMEOUT;
                    gCalIQData.Cal_Status.ECS_STATUS = _CAL_AE_TIMEOUT;
                }
#endif
            }
            pTag->TagDataLen = sizeof(CAL_ECS_HEADER) + sizeof(UINT32)*tblSize.w*tblSize.h;
            pTag->TagID = (UINT32)GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin);

            if (CalDbg[CALDEBUG_RESULT_DATA] == 1)
            {
                CalApi()->fpSaveRAW((UINT32)&pTag->TagID,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->CAL_STATUS,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagDataLen,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagData,1,pTag->TagDataLen);
            }

            CAL_RW()->fpAddTag(CalSectionItem,pTag);

            //release buf
            CalApi()->fpBufEvent()->fpUnInit(buf.Id);

#if DetailOSD //show calibration result on OSD
            if (pTag->CAL_STATUS != _CAL_OK)
            {
                buf_cnt = snprintf(NULL, 0, "ECS FAIL (mode %d)", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "ECS FAIL (mode %d)", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
            else
            {
                buf_cnt = snprintf(NULL, 0, "ECS PASS (mode %d)", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "ECS PASS (mode %d)", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
#endif

            //set adjust result information for debug
            if (aeRst.status == CAL_AE_TIMEOUT)
                CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = TRUE;
            else
                CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = FALSE;

            CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum        = aeRst.convergeLum;
            CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO        = aeRst.uiISO[0];
            CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase    = aeRst.uiISOBase;
            CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT       = aeRst.uiExpT[0];
            CalDebugInfor->CalDebugECS->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio      = aeRst.uiRatio[0];
            SetAdustRst(Id, fpUIcb, CalDebugInfor);

            //next sensor mode
            SenModeCnt++;
            if (SenModeCnt > MaxSenModeNum)
            {
                DBG_ERR("sensor mode number is out of range\r\n");
                break;
            }
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt++;
    }

    //resume sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIERESUME,NULL,NULL);

    //stop all
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(CalSectionItem);

#if DetailOSD //show calibration result on OSD
    if(fpUIcb)
        fpUIcb(UICB_WAITKEY,NULL,UICOL_GREEN);
#endif

    return E_OK;
}

static ER AWB_Adjust(IPL_PROC_ID Id, fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    UINT32 j;
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(Id);
    CAL_AE_RST aeRst = {0};
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor = {0};
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    //#NT#2016/06/07#Silvia Wu -begin
    //#NT# fixed coverity warning
    //CAL_AWB_FMT pAwbFmt1;
    //CAL_AWB_FMT* pAwbFmt = &pAwbFmt1;
    CAL_AWB_FMT* pAwbFmt;
    //#NT#2016/06/07#Silvia Wu -end
    UINT32 GS_Rg = 0x100, GS_Bg = 0x100, Gain_Threshold;
    UINT32 rg_sum,bg_sum;
    CAL_SECMAP CalSectionItem;
    Cal_SECTION SectionInfo= {0};
    CAL_AWB_INFO CalInfo1 = {0};
    CAL_AWB_INFO* CalInfo = &CalInfo1;
    CTRL_PTR CtrlInfo = {0};
    CalBuf buf;
    UINT32 TempExpT;
    UINT32 TempRG = 0, TempBG = 0;
    UINT32 buf_cnt = 0;
    static CHAR m_CalMsg[100];
    UINT32 MaxSenModeCnt = 0;
    UINT32 SenModeCnt = 0;
    ER rt = E_OK;

    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_AWB_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_AWB_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_AWB_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_AWB_4;
    }
    else
    {
        CalSectionItem = SEC_AWB_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    CAL_RW()->fpDeleteSection(CalSectionItem);

    /*
        get current Section (AWB) information
    */
    UINT32 SectionCnt = 0;
    while (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
    {
        if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == CalSectionItem)
        {
            SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
            CalInfo = CalSectionInfo[Id].CalInfo->AwbInfor;
            break;
        }
        else
        {
            SectionCnt++;
            if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
            {
                SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
                DBG_ERR("section is not in the current IPL\r\n");
            }
            if (SectionCnt > MaxSectionNum)
            {
                DBG_ERR("section is out of range\r\n");
                return E_SYS;
            }
        }
    }

    // stop all pipeline
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //allocate section buffer
    dmaRst = CfgSectionBuf(CalSectionItem,DmaAddr,DmaSize);
    if(dmaRst.erCode != E_OK)
        return E_SYS;
    else
    {
        DmaAddr = dmaRst.RemainAddr;
        DmaSize = dmaRst.RemainSize;
    }

    //shift current dram address (shift size: 1 tag size), temporal buf
    TagTempAddr = DmaAddr;
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_AWB_FMT);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize >= TagSize)
    {
        DmaAddr += TagSize;
        DmaSize -= TagSize;
    }
    else
    {
        DBG_ERR("section buffer overflow\r\n");
        return E_SYS;
    }

    /*
        init buffer
        -> start run all pipeline (with sie only flag) to re-set sie output address
        -> pause sie
        -> release buffer
    */
    // ipp buf init
    //buf.Id = Id;
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    buf.Id = bufID;
    buf.StartAddr = DmaAddr;
    buf.Size = DmaSize;
    CalApi()->fpBufEvent()->fpInit(buf);

    SenModeCnt = 0;
    UINT32 maxSize = 0;
    UINT32 senModeSize = 0;
    UINT32 maxSizeSenMode = 0;
    while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
    {
        rt = Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

         //coverity warning
        if (rt != E_OK || Drv_Sensor.Mode == NULL)
           return E_SYS;

        senModeSize = Drv_Sensor.Mode->TransHD[0].DataSize * Drv_Sensor.Mode->TransVD[0].DataSize;
        if (maxSize < senModeSize)
        {
            maxSize = senModeSize;
            maxSizeSenMode = SenModeCnt;
        }
        SenModeCnt++;
    }

    // start run
    Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[maxSizeSenMode].SenMode, &Drv_Sensor);
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;
    CtrlInfo.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[maxSizeSenMode]);
    CtrlInfo.fpRdyImg = NULL;
    CalApi()->fpSetMode(Id,CAL_IPL_ALLRUN,&CtrlInfo,Cal_CtrlInit()->fpCalStation_CtrlInit);

    // pause sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

    //release buf
    CalApi()->fpBufEvent()->fpUnInit(buf.Id);

    //set debug information for SetAdjustRst()
    CAL_DEBUG_INFO *CalDebugInfor = &CalDebugInfor1;
    CAL_DEBUG_INFO_AWB* CalDebugAWB = &CalDebugAWB1;
    CalDebugInfor->CalDebugDP = NULL;
    CalDebugInfor->CalDebugISO = NULL;
    CalDebugInfor->CalDebugECS = NULL;
    CalDebugInfor->CalDebugAWB = CalDebugAWB;

    //clean OSD to show calibration result on OSD
#if DetailOSD
    if(fpUIcb)
        fpUIcb(UICB_CLEAN,(CHAR*)m_CalMsg,UICOL_RED);
#endif

    /*
        calibration for per sensor mode
    */
    UINT32 ConditionCnt = 0;
    while (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
#if 0
        switch (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin)
        {
            case CAL_CONDITION_HIGH_CT:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright & high color temperature", Id);
                break;
            case CAL_CONDITION_LOW_CT:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright & low color temperature", Id);
                break;
            case CAL_CONDITION_DEFAULT:
            default:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright", Id);
                break;
        }

        if(fpUIcb)
            fpUIcb(UICB_NONE,(CHAR*)m_CalMsg,UICOL_GREEN);
#endif
        SenModeCnt = 0;
        while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            //ipp buf init
            buf.Id = bufID;
            buf.StartAddr = DmaAddr;
            buf.Size = DmaSize;
            CalApi()->fpBufEvent()->fpInit(buf);

            //init sensor info
            CtrlPtr.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[SenModeCnt]);
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);
            CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;

            //config target sensor mode
            IPC_WaitVD(Id,TRUE); //IPC_WaitVD(SensorID,TRUE);
            Sensor_ChgMode(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
            IPC_WaitVD(Id,FALSE); //IPC_WaitVD(SensorID,FALSE);

            //get sensor info
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

            //repeat and get average R/B gain
            rg_sum = 0;
            bg_sum = 0;

            pAwbFmt = (CAL_AWB_FMT*)&pTag->TagData;
            pAwbFmt->BitSel = CalInfo->awb_bit;

            if (CalInfo->RepeatTime == 0)
            {
                DBG_ERR("Repeat Time minimum should be set to 1\r\n");
                CalInfo->RepeatTime = 1;
            }


            BOOL temp_AE_TIMEOUT = FALSE;
            for (j = 0; j < CalInfo->RepeatTime; j++)
            {
                //chg sie to capture mode
                CalApi()->fpSetMode(Id,CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpAWB_CtrlInit);
                Bayer = CalApi()->fpGetRawInfo(Id);

                //cal 3a converage
                //ae
                //aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

                TempExpT = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0];
                SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0]*100/Drv_Sensor.Mode->biningRatio;

                aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

                if (aeRst.status == CAL_AE_TIMEOUT)
                {
                    temp_AE_TIMEOUT = TRUE;
                }

                SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = TempExpT;

                //pause sie
                CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

                //run alg
                CalApi()->fpAlgEvent()->fpAWB_AVG(&Bayer.BaseInfo,&(pAwbFmt->Rg),&(pAwbFmt->Bg),pAwbFmt->BitSel);

                DBG_IND("AWB Rst(SensorMode = %d): Rgain = %d,Bgain = %d\r\n",SectionInfo.SenModeInfo[SenModeCnt].SenMode,(pAwbFmt->Rg), (pAwbFmt->Bg));
                rg_sum += pAwbFmt->Rg;
                bg_sum += pAwbFmt->Bg;
            }

            pAwbFmt->Rg = rg_sum/CalInfo->RepeatTime;
            pAwbFmt->Bg = bg_sum/CalInfo->RepeatTime;

            // save raw
            if (CalDbg[CALDEBUG_RAW] == 1)
            {
                CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
            }

            //chk calibration rst OK or NG
            pTag->CAL_STATUS = _CAL_OK;

            pTag->TagID = (UINT32)GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin);
            pTag->TagDataLen = sizeof(CAL_AWB_FMT);

            gAwbFmt.Rg = pAwbFmt->Rg;
            gAwbFmt.Bg = pAwbFmt->Bg;

            Gain_Threshold = CalInfo->Threshold;
#if 0   //support update awb threshold by text file
            UINT32 AWB_THR_Size = 2;
            UB AWB_THR_Addr[2];
            UINT32 TempGain_Threshold;

            sprintf(sFileName, "A:\\AWB_THR.txt", 0);
            pFileHdl = FileSys_OpenFile(sFileName, FST_OPEN_READ);

            if (pFileHdl)
            {
                FileSys_ReadFile(pFileHdl, (UINT8 *)AWB_THR_Addr, &AWB_THR_Size, 0, NULL);
                TempGain_Threshold =(AWB_THR_Addr[0]-0x30)*10+(AWB_THR_Addr[1]-0x30);

                DBG_ERR("Modify AWB GS param Threshold: %d\r\n",TempGain_Threshold);

                if (TempGain_Threshold > 0 && TempGain_Threshold < 100)
                {
                    Gain_Threshold = TempGain_Threshold;
                }

                FileSys_CloseFile(pFileHdl);
            }
#endif
#if 0   //Input GS value from text file
            UINT32 AWB_GS_Size = 7;
            UB AWB_GS_Addr[7];

            sprintf(sFileName, "A:\\AWB_GSgain.txt", 0);
            pFileHdl = FileSys_OpenFile(sFileName, FST_OPEN_READ);

            if (pFileHdl)
            {
                FileSys_ReadFile(pFileHdl, (UINT8 *)AWB_GS_Addr, &AWB_GS_Size, 0, NULL);
                GS_Rg =(AWB_GS_Addr[0]-0x30)*100+(AWB_GS_Addr[1]-0x30)*10+(AWB_GS_Addr[2]-0x30);
                GS_Bg =(AWB_GS_Addr[4]-0x30)*100+(AWB_GS_Addr[5]-0x30)*10+(AWB_GS_Addr[6]-0x30);

                DBG_ERR("Modify AWB GS param GS R: %d,GS B: %d\r\n",GS_Rg,GS_Bg);

                if (GS_Rg > 0 && GS_Rg < 1023)
                {
                    gAwbGSFmt.Rg= GS_Rg;
                }

                if (GS_Bg > 0 && GS_Bg < 1023)
                {
                    gAwbGSFmt.Bg= GS_Bg;
                }

                FileSys_CloseFile(pFileHdl);
            }
#endif

            if (gAwbGSFmt.Rg != 0 && gAwbGSFmt.Bg != 0)
            {
                GS_Rg = gAwbGSFmt.Rg;
                GS_Bg = gAwbGSFmt.Bg;

                DBG_ERR("GS %d,%d,%d\r\n",GS_Rg,GS_Bg,Gain_Threshold);

                if ((((100*abs(GS_Rg - pAwbFmt->Rg))/GS_Rg) < Gain_Threshold) && (((100*abs(GS_Bg - pAwbFmt->Bg))/GS_Bg) < Gain_Threshold))
                {

#if AE_TimeOut
                    if (temp_AE_TIMEOUT == TRUE) //if (aeRst.status == CAL_AE_TIMEOUT)
                    {
                        DBG_WRN("AWB  AE timeout\r\n");
                        pTag->CAL_STATUS = _CAL_AE_TIMEOUT;
                        gCalIQData.Cal_Status.AWB_STATUS = _CAL_AE_TIMEOUT;
                    }
                    else
                    {
                        buf_cnt = snprintf(NULL, 0, "AWB: PASS(GS:Rg %d Bg %d)",GS_Rg,GS_Bg);
                        snprintf((char*)m_CalMsg, buf_cnt, "AWB: PASS(GS:Rg %d Bg %d)",GS_Rg,GS_Bg);
                        DBG_ERR("AWB  PASS\r\n");

                        gCalIQData.Cal_Status.AWB_STATUS = _CAL_OK;
                        CalStrColor = UICOL_GREEN;
                    }
#else
                    buf_cnt = snprintf(NULL, 0, "AWB: PASS(GS:Rg %d Bg %d)",GS_Rg,GS_Bg);
                    snprintf((char*)m_CalMsg, buf_cnt, "AWB: PASS(GS:Rg %d Bg %d)",GS_Rg,GS_Bg);
                    DBG_ERR("AWB  PASS\r\n");

                    gCalIQData.Cal_Status.AWB_STATUS = _CAL_OK;
                    CalStrColor = UICOL_GREEN;
#endif
                }
                else
                {
                    buf_cnt = snprintf(NULL, 0, "AWB: NG(GS:Rg %d Bg %d)",GS_Rg,GS_Bg);
                    snprintf((char*)m_CalMsg, buf_cnt, "AWB: NG(GS:Rg %d Bg %d)",GS_Rg,GS_Bg);
                    DBG_ERR("AWB  NG\r\n");
                    gCalIQData.Cal_Status.AWB_STATUS = _CAL_NONE;
                    pTag->CAL_STATUS = _CAL_NONE;
                    CalStrColor = UICOL_RED;
                }

                TempRG = pAwbFmt->Rg;
                TempBG = pAwbFmt->Bg;

                pAwbFmt->Rg = (pAwbFmt->Rg*100)/GS_Rg;
                pAwbFmt->Bg = (pAwbFmt->Bg*100)/GS_Bg;
            }
            else
            {
                pAwbFmt->Rg = 100;
                pAwbFmt->Bg = 100;

                buf_cnt = snprintf(NULL, 0, "AWB NG - NO GS");
                snprintf((char*)m_CalMsg, buf_cnt, "AWB NG - NO GS");
                DBG_ERR("AWB NG - NO GS\r\n");

                gCalIQData.Cal_Status.AWB_STATUS = _CAL_NONE;
                pTag->CAL_STATUS = _CAL_NONE;
                CalStrColor = UICOL_RED;
            }

            if (CalDbg[CALDEBUG_RESULT_DATA] == 1)
            {
                CalApi()->fpSaveRAW((UINT32)&pTag->TagID,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->CAL_STATUS,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagDataLen,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagData,1,pTag->TagDataLen);
            }

            //write tag
            CAL_RW()->fpAddTag(CalSectionItem,pTag);

            //release buf
            CalApi()->fpBufEvent()->fpUnInit(buf.Id);

            DBG_IND("AWB_GS_Rg = %d, AWB_GS_Bg = %d, Rg = %d, Bg = %d\r\n",GS_Rg,GS_Bg,pAwbFmt->Rg,pAwbFmt->Bg);


#if DetailOSD //show calibration result on OSD
            if (pTag->CAL_STATUS != _CAL_OK)
            {
                buf_cnt = snprintf(NULL, 0, "AWB FAIL %d %d(mode %d)", pAwbFmt->Rg, pAwbFmt->Bg, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "AWB FAIL %d %d(mode %d)", pAwbFmt->Rg, pAwbFmt->Bg, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
            else
            {
                buf_cnt = snprintf(NULL, 0, "AWB PASS %d %d(mode %d)", pAwbFmt->Rg, pAwbFmt->Bg, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "AWB PASS %d %d(mode %d)", pAwbFmt->Rg, pAwbFmt->Bg, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
#endif

            //set adjust result information for debug
            if (temp_AE_TIMEOUT == TRUE)//(aeRst.status == CAL_AE_TIMEOUT)
                CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = TRUE;
            else
                CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = FALSE;

            CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum        = aeRst.convergeLum;
            CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO        = aeRst.uiISO[0];
            CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase    = aeRst.uiISOBase;
            CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT       = aeRst.uiExpT[0];
            CalDebugInfor->CalDebugAWB->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio      = aeRst.uiRatio[0];
            SetAdustRst(Id, fpUIcb, CalDebugInfor);

            //next sensor mode
            SenModeCnt++;
            if (SenModeCnt > MaxSenModeNum)
            {
                DBG_ERR("sensor mode number is out of range\r\n");
                break;
            }
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt++;
    }

    //resume sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIERESUME,NULL,NULL);

    //stop all
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(CalSectionItem);

#if DetailOSD //show calibration result on OSD
    if(fpUIcb)
        fpUIcb(UICB_WAITKEY,NULL,UICOL_GREEN);
#endif

    return E_OK;
}

#define TestRawInput DISABLE //test raw input from sd card instead of cap raw
#if TestRawInput
#define temptRawH 1188
#define temptRawV 1188
static char   temptRawpath[40]= "A:\\test_1188x1188.RAW";
static UINT32 tempRawbuf[temptRawH*temptRawV];
static IPL_RAW_BIT tempRawBit = IPL_RAW_BIT_8;
static IPL_RAW_PIX tumpRawSRPix = IPL_RAW_PIX_R;
#endif

static ER LENSCEN_Adjust(IPL_PROC_ID Id, fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(Id);
    CAL_AE_RST aeRst = {0};
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor = {0};
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    CAL_LENSCEN_FMT* pLENSCENFmt;
    CAL_SECMAP CalSectionItem;
    Cal_SECTION SectionInfo = {0};
    CAL_LENSCEN_INFO CalInfo1 = {0};
    CAL_LENSCEN_INFO* CalInfo = &CalInfo1;
    CTRL_PTR CtrlInfo = {0};
    CalBuf buf;
    UINT32 TempExpT;
#if DetailOSD
    UINT32 buf_cnt = 0;
    static CHAR m_CalMsg[100];
#endif
    UINT32 MaxSenModeCnt = 0;
    UINT32 SenModeCnt = 0;
    iCoordinate CenterSft = {0};
    iCoordinate RealCenter = {0};
    UINT32 Reliability = 0;
    ER rt = E_OK;

    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_LENSCEN_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_LENSCEN_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_LENSCEN_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_LENSCEN_4;
    }
    else
    {
        CalSectionItem = SEC_LENSCEN_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    CAL_RW()->fpDeleteSection(CalSectionItem);

    /*
        get current Section (Lens center) information
    */
    UINT32 SectionCnt = 0;
    while (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
    {
        if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == CalSectionItem)
        {
            SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
            CalInfo = CalSectionInfo[Id].CalInfo->LensCenInfor;
            break;
        }
        else
        {
            SectionCnt++;
            if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
            {
                SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
                DBG_ERR("section is not in the current IPL\r\n");
            }
            if (SectionCnt > MaxSectionNum)
            {
                DBG_ERR("section is out of range\r\n");
                return E_SYS;
            }
        }
    }

    // stop all pipeline
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    // allocate section buffer
    dmaRst = CfgSectionBuf(CalSectionItem,DmaAddr,DmaSize);
    if(dmaRst.erCode != E_OK)
        return E_SYS;
    else
    {
        DmaAddr = dmaRst.RemainAddr;
        DmaSize = dmaRst.RemainSize;
    }

    // shift current dram address (shift size: 1 tag size), temporal buf
    TagTempAddr = DmaAddr;
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_LENSCEN_FMT);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize >= TagSize)
    {
        DmaSize -= TagSize;
        DmaAddr += TagSize;
    }
    else
    {
        DBG_ERR("section buffer overflow\r\n");
        return E_SYS;
    }

    /*
        init buffer
        -> start run all pipeline (with sie only flag) to re-set sie output address
        -> pause sie
        -> release buffer
    */
    // ipp buf init
    //buf.Id = Id;
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    buf.Id = bufID;
    buf.StartAddr = DmaAddr;
    buf.Size = DmaSize;
    CalApi()->fpBufEvent()->fpInit(buf);

    SenModeCnt = 0;
    UINT32 maxSize = 0;
    UINT32 senModeSize = 0;
    UINT32 maxSizeSenMode = 0;
    while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
    {
        rt = Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

        //coverity warning
        if (rt != E_OK || Drv_Sensor.Mode == NULL)
           return E_SYS;

        senModeSize = Drv_Sensor.Mode->TransHD[0].DataSize * Drv_Sensor.Mode->TransVD[0].DataSize;
        if (maxSize < senModeSize)
        {
            maxSize = senModeSize;
            maxSizeSenMode = SenModeCnt;
        }
        SenModeCnt++;
    }

    // start run
    Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[maxSizeSenMode].SenMode, &Drv_Sensor);
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;
    CtrlInfo.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[maxSizeSenMode]);
    CtrlInfo.fpRdyImg = NULL;
    CalApi()->fpSetMode(Id,CAL_IPL_ALLRUN,&CtrlInfo,Cal_CtrlInit()->fpCalStation_CtrlInit);

    // pause sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

    //release buf
    CalApi()->fpBufEvent()->fpUnInit(buf.Id);

    //set debug information for SetAdjustRst()
    CAL_DEBUG_INFO *CalDebugInfor = &CalDebugInfor1;
    CAL_DEBUG_INFO_LENSCEN* CalDebugLENSCEN = &CalDebugLENSCEN1;
    CalDebugInfor->CalDebugDP = NULL;
    CalDebugInfor->CalDebugISO = NULL;
    CalDebugInfor->CalDebugECS = NULL;
    CalDebugInfor->CalDebugAWB = NULL;
    CalDebugInfor->CalDebugLENSCEN = CalDebugLENSCEN;

    //clean OSD to show calibration result on OSD
#if DetailOSD
    if(fpUIcb)
        fpUIcb(UICB_CLEAN,(CHAR*)m_CalMsg,UICOL_RED);
#endif

    /*
        calibration for per sensor mode
    */
    UINT32 ConditionCnt = 0;
    while (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
        SenModeCnt = 0;
        while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            //ipp buf init
            buf.Id = bufID;
            buf.StartAddr = DmaAddr;
            buf.Size = DmaSize;
            CalApi()->fpBufEvent()->fpInit(buf);

            //init sensor info
            CtrlPtr.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[SenModeCnt]);
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);
            CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;

            //config target sensor mode
            IPC_WaitVD(Id,TRUE);
            Sensor_ChgMode(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
            IPC_WaitVD(Id,TRUE);

            //get sensor info
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

            //chg sie to capture mode
            CalApi()->fpSetMode(Id,CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpLENSCEN_CtrlInit);
            Bayer = CalApi()->fpGetRawInfo(Id);

            //cal 3a converage
            TempExpT = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0];
            SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0]*100/Drv_Sensor.Mode->biningRatio;
            aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);
            SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = TempExpT;

            //pause sie
            CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

            //run alg
            pLENSCENFmt = (CAL_LENSCEN_FMT*)&pTag->TagData;

            //save raw
            if (CalDbg[CALDEBUG_RAW] == 1)
            {
                CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
            }



#if TestRawInput
            FST_FILE    filehdl;
            UINT32 RawSize;

            filehdl = FileSys_OpenFile(temptRawpath,FST_OPEN_READ);
            if(NULL == filehdl)
            {
                DBG_ERR("open %s fail\r\n",temptRawpath);
                return E_SYS;
            }
            FileSys_ReadFile(filehdl, (UINT8*)tempRawbuf, &RawSize, 0, NULL);

            Bayer.BaseInfo.Bit = tempRawBit;
            Bayer.BaseInfo.Height = temptRawH;
            Bayer.BaseInfo.Width = temptRawV;
            Bayer.BaseInfo.LineOffset = temptRawV;
            Bayer.BaseInfo.PixelAddr = (UINT32)(&tempRawbuf[0]);
            Bayer.BaseInfo.STPix = tumpRawSRPix;
#endif

            Reliability = CalApi()->fpAlgEvent()->fpLenCenter(&Bayer.BaseInfo, Bayer.BaseInfo.Width/2, Bayer.BaseInfo.Height/2, CalInfo->ScanRadius, CalInfo->AvgCnt, CalInfo->EdgeThr, CalInfo->ValidRadius, &RealCenter);
            DBG_IND("center %d %d ScanRadius %d AvgCnt %d EdgeThr %d ValidRadius %d\r\n"
            , Bayer.BaseInfo.Width/2, Bayer.BaseInfo.Height/2
            , CalInfo->ScanRadius, CalInfo->AvgCnt
            , CalInfo->EdgeThr, CalInfo->ValidRadius);

            CenterSft.x = RealCenter.x - Bayer.BaseInfo.Width/2;
            CenterSft.y = RealCenter.y - Bayer.BaseInfo.Height/2;

            DBG_IND("real center %d %d center shift %d %d reliability %d\r\n"
                , RealCenter.x, RealCenter.y
                , CenterSft.x,  CenterSft.y, Reliability);

            //chk calibration rst OK or NG
            if (Reliability < CalInfo->MinReliability)
            {
                DBG_WRN("low reliability (%d < %d)\r\n", Reliability, CalInfo->MinReliability);
                pTag->CAL_STATUS = _CAL_UnderFlow;
            }
            else
            {
                pTag->CAL_STATUS = _CAL_OK;
#if AE_TimeOut
#if TestRawInput // TestRawInput not ae converge
#else
                if (aeRst.status == CAL_AE_TIMEOUT)
                {
                    DBG_WRN("LENSCEN AE timeout\r\n");
                    pTag->CAL_STATUS = _CAL_AE_TIMEOUT;
                }
#endif
#endif
            }
            pTag->TagID = (UINT32)GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin);
            pLENSCENFmt->ActStr_X= Drv_Sensor.Mode->TransHD[0].DataStart;
            pLENSCENFmt->ActStr_Y= Drv_Sensor.Mode->TransVD[0].DataStart;
            pLENSCENFmt->CenterSft_X= CenterSft.x;
            pLENSCENFmt->CenterSft_Y= CenterSft.y;
            pLENSCENFmt->Reliability = Reliability;

            if (CalDbg[CALDEBUG_RESULT_DATA] == 1)
            {
                CalApi()->fpSaveRAW((UINT32)&pTag->TagID,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->CAL_STATUS,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagDataLen,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagData,1,pTag->TagDataLen);
            }

            CAL_RW()->fpAddTag(CalSectionItem,pTag);

            //release buf
            CalApi()->fpBufEvent()->fpUnInit(buf.Id);

#if DetailOSD //show calibration result on OSD
            if (pTag->CAL_STATUS != _CAL_OK)
            {
                buf_cnt = snprintf(NULL, 0, "LENSCEN FAIL (mode %d)", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "LENSCEN FAIL (mode %d)", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
            else
            {
                buf_cnt = snprintf(NULL, 0, "LENSCEN PASS (mode %d)", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "LENSCEN PASS (mode %d)", SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
#endif
            //set adjust result information for debug
            if (aeRst.status == CAL_AE_TIMEOUT)
                CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = TRUE;
            else
                CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].AeTimeout = FALSE;

            CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].Lum        = aeRst.convergeLum;
            CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].ISO        = aeRst.uiISO[0];
            CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].ISOBase    = aeRst.uiISOBase;
            CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].ExpT       = aeRst.uiExpT[0];
            CalDebugInfor->CalDebugLENSCEN->CalAeRst[MaxSenModeCnt + SenModeCnt].Ratio      = aeRst.uiRatio[0];
            SetAdustRst(Id, fpUIcb, CalDebugInfor);
            //next sensor mode
            SenModeCnt++;
            if (SenModeCnt > MaxSenModeNum)
            {
                DBG_ERR("sensor mode number is out of range\r\n");
                break;
            }
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt++;
    }

    //resume sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIERESUME,NULL,NULL);

    //stop all
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(CalSectionItem);

#if DetailOSD //show calibration result on OSD
    if(fpUIcb)
        fpUIcb(UICB_WAITKEY,NULL,UICOL_GREEN);
#endif

    return E_OK;
}


static ER AWBGS_Adjust(IPL_PROC_ID Id, fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    return E_OK;
#if 0
    UINT32 j;
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(Id);
    CAL_AE_RST aeRst = {0};
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    CAL_AWB_FMT* pAwbFmt;
    UINT32 size = 7;
    UINT32 rg_sum,bg_sum;
    CAL_SECMAP CalSectionItem;
    Cal_SECTION SectionInfo = {0};
    CAL_AWB_INFO CalInfo1 = {0};
    CAL_AWB_INFO* CalInfo = &CalInfo1;
    CTRL_PTR CtrlInfo = {0};
    CalBuf buf;
    UINT32 TempExpT;

    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_AWB_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_AWB_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_AWB_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_AWB_4;
    }
    else
    {
        CalSectionItem = SEC_AWB_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    CAL_RW()->fpDeleteSection(CalSectionItem);

    /*
        get current Section (AWB) information
    */
    UINT32 SectionCnt = 0;
    while (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
    {
        if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == CalSectionItem)
        {
            SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
            CalInfo = CalSectionInfo[Id].CalInfo->AwbInfor;
            break;
        }
        else
        {
            SectionCnt++;
            if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
            {
                SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
                DBG_ERR("section is not in the current IPL\r\n");
            }
            if (SectionCnt > MaxSectionNum)
            {
                DBG_ERR("section is out of range\r\n");
                return E_SYS;
            }
        }
    }

    sprintf((char*)m_CalMsg, "sensor %d Set to Bright", Id);
    if(fpUIcb)
        fpUIcb(UICB_WAITKEY,(CHAR*)m_CalMsg,UICOL_GREEN);

    // stop all pipeline
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //allocate section buffer
    dmaRst = CfgSectionBuf(CalSectionItem,DmaAddr,DmaSize);
    if(dmaRst.erCode != E_OK)
        return E_SYS;
    else
    {
        DmaAddr = dmaRst.RemainAddr;
        DmaSize = dmaRst.RemainSize;
    }

    //shift current dram address (shift size: 1 tag size), temporal buf
    TagTempAddr = DmaAddr;
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_AWB_FMT);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize >= TagSize)
    {
        DmaAddr += TagSize;
        DmaSize -= TagSize;
    }
    else
    {
        DBG_ERR("section buffer overflow\r\n");
        return E_SYS;
    }

    /*
        init buffer
        -> start run all pipeline (with sie only flag) to re-set sie output address
        -> pause sie
        -> release buffer
    */
    // ipp buf init
    //buf.Id = Id;
    Cal_BufID bufID;
    switch(Id)
    {
        case IPL_ID_1:
           bufID = BUF_ID_1;
           break;
        case IPL_ID_2:
           bufID = BUF_ID_2;
           break;
        case IPL_ID_3:
           bufID = BUF_ID_3;
           break;
        case IPL_ID_4:
           bufID = BUF_ID_4;
           break;
        default:
           bufID = BUF_ID_1;
           break;
    }
    buf.Id = bufID;
    buf.StartAddr = DmaAddr;
    buf.Size = DmaSize;
    CalApi()->fpBufEvent()->fpInit(buf);

    // start run
    Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[0].SenMode, &Drv_Sensor);
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;
    CtrlInfo.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[0]);
    CtrlInfo.fpRdyImg = NULL;
    CalApi()->fpSetMode(Id,CAL_IPL_ALLRUN,&CtrlInfo,Cal_CtrlInit()->fpCalStation_CtrlInit);

    // pause sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

    //release buf
    CalApi()->fpBufEvent()->fpUnInit(buf.Id);

    /*
        calibration for per sensor mode
    */
    UINT32 ConditionCnt = 0;
    while (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
        switch (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin)
        {
            case CAL_CONDITION_HIGH_CT:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright & high color temperature", Id);
                break;
            case CAL_CONDITION_LOW_CT:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright & low color temperature", Id);
                break;
            case CAL_CONDITION_DEFAULT:
            default:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright", Id);
                break;
        }

        if(fpUIcb)
            fpUIcb(UICB_NONE,(CHAR*)m_CalMsg,UICOL_GREEN);

        UINT32 SenModeCnt = 0;
        while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            //ipp buf init
            buf.Id = bufID;
            buf.StartAddr = DmaAddr;
            buf.Size = DmaSize;
            CalApi()->fpBufEvent()->fpInit(buf);

            //init sensor info
            CtrlPtr.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[SenModeCnt]);
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);
            CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;

            //config target sensor mode
            IPC_WaitVD(SensorID,TRUE);
            Sensor_ChgMode(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
            IPC_WaitVD(SensorID,FALSE);

            //get sensor info
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

            //repeat and get average R/B gain
            rg_sum = 0;
            bg_sum = 0;
            pAwbFmt = (CAL_AWB_FMT*)&pTag->TagData;
            pAwbFmt->BitSel = CalInfo->awb_bit;

            if (CalInfo->RepeatTime == 0)
            {
                DBG_ERR("Repeat Time minimum should be set to 1\r\n");
                CalInfo->RepeatTime = 1;
            }

            for (j = 0; j < CalInfo->RepeatTime; j++)
            {
                //chg sie to capture mode
                CalApi()->fpSetMode(Id,CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpAWB_CtrlInit);
                Bayer = CalApi()->fpGetRawInfo(Id);

                //cal 3a converage
                //ae
                //aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

                TempExpT = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0];
                SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0]*100/Drv_Sensor.Mode->biningRatio;

                aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

                SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = TempExpT;

                //pause sie
                CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

                //run alg
                CalApi()->fpAlgEvent()->fpAWB_AVG(&Bayer.BaseInfo,&(pAwbFmt->Rg),&(pAwbFmt->Bg),pAwbFmt->BitSel);
                DBG_IND("AWB Rst(SensorMode = %d): Rgain = %d,Bgain = %d\r\n",SectionInfo.SenModeInfo[SenModeCnt].SenMode,(pAwbFmt->Rg), (pAwbFmt->Bg));

                rg_sum += pAwbFmt->Rg;
                bg_sum += pAwbFmt->Bg;
            }
            pAwbFmt->Rg = rg_sum/CalInfo->RepeatTime;
            pAwbFmt->Bg = bg_sum/CalInfo->RepeatTime;

            //save raw
            if (CalDbg[CALDEBUG_RAW] == 1)
            {
                CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
            }

            //chk calibration rst OK or NG
            pTag->CAL_STATUS = _CAL_OK;

            pTag->TagID = (UINT32)GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin);
            pTag->TagDataLen = sizeof(CAL_AWB_FMT);

            //write tag
            CAL_RW()->fpAddTag(CalSectionItem,pTag);

            gAwbGSFmt.Rg = pAwbFmt->Rg;
            gAwbGSFmt.Bg = pAwbFmt->Bg;

            gAwbFmt.Rg = pAwbFmt->Rg;
            gAwbFmt.Bg = pAwbFmt->Bg;

            //Write AWB GS gain to SD card
            sprintf(sFileName, "A:\\AWBGS.txt");
            pFileHdl = FileSys_OpenFile(sFileName,FST_CREATE_ALWAYS|FST_OPEN_WRITE);

            if (pFileHdl)
            {
                sprintf((char*)CalStringBuffer, "%d %d",gAwbGSFmt.Rg,gAwbGSFmt.Bg);
                FileSys_WriteFile(pFileHdl, (UINT8*)CalStringBuffer, &size, 0, NULL);
                FileSys_CloseFile(pFileHdl);

                sprintf((char*)m_CalMsg, "AWB GS Rg %d",pAwbFmt->Rg);
                DBG_ERR("AWB GS Rg %d\r\n",pAwbFmt->Rg);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);

                sprintf((char*)m_CalMsg, "AWB GS Bg %d",pAwbFmt->Bg);
                DBG_ERR("AWB GS Bg %d\r\n",pAwbFmt->Bg);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);

                FileSys_CloseFile(pFileHdl);
            }
            else
            {
                sprintf((char*)m_CalMsg, "AWBGS Save Fail");
                DBG_ERR("AWBGS Save Fail\r\n");
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }

            pAwbFmt->Rg = 100;
            pAwbFmt->Bg = 100;

            if (CalDbg[CALDEBUG_RESULT_DATA] == 1)
            {
                CalApi()->fpSaveRAW((UINT32)&pTag->TagID,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->CAL_STATUS,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagDataLen,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagData,1,pTag->TagDataLen);
            }

            //write tag
            CAL_RW()->fpAddTag(CalSectionItem,pTag);

            //release buf
            CalApi()->fpBufEvent()->fpUnInit(buf.Id);

            //next sensor mode
            SenModeCnt++;
            if (SenModeCnt > MaxSenModeNum)
            {
                DBG_ERR("sensor mode number is out of range\r\n");
                break;
            }
        }
        ConditionCnt++;

        if(fpUIcb)
            fpUIcb(UICB_PROCESSING,NULL,UICOL_GREEN);
    }

    //resume sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIERESUME,NULL,NULL);

    //stop all
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(CalSectionItem);

#if 0
{
    pCAL_TAG pTag;
    CAL_AWB_FMT* pFmt;
    CAL_RW()->fpSectionRead(SEC_AWB,DmaAddr,DmaSize);
    pTag = CAL_RW()->fpReadTag(SEC_AWB, GetTagID(SEC_AWB, SensorMode_AWB[++i][++SensorID].SenMode));
    pFmt = (CAL_AWB_FMT*)&pTag->TagData;
    DBG_IND("CHK: %d,%d,%d\r\n",pFmt->Rg,pFmt->Bg,pFmt->BitSel);
}
#endif
    if(fpUIcb)
        fpUIcb(UICB_END,NULL,UICOL_GREEN);
    return E_OK;
#endif
}

static ER DP_Adjust_Dark(IPL_PROC_ID Id, fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(Id);
    CAL_AE_RST aeRst = {0};
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor = {0};
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    DPC_ALG_INFO DP_Info;
    UINT32 DP_Status_Chk = _CAL_DP_B_OK;
    CAL_SECMAP CalSectionItem;
    Cal_SECTION SectionInfo = {0};
    CAL_DPC_THRES CalInfo1 = {0};
    CAL_DPC_THRES* CalInfo = &CalInfo1;
    CTRL_PTR CtrlInfo = {0};
    CalBuf buf;
    UINT32 TempExpT;
    UINT32 buf_cnt = 0;
    static CHAR m_CalMsg[100];
    UINT32 MaxSenModeCnt = 0;
    UINT32 ConditionCnt = 0;
    UINT32 SenModeCnt = 0;
    ER rt = E_OK;

    // find max (DP limit cnt) to set dram size
    UINT32 maxDPLimitCnt = 0;
    UINT32 SumDPLimitCnt = 0;
    for (SenModeCnt=0; SenModeCnt<MaxSenModeNum; SenModeCnt++)
    {
        if (DPLimitCnt[Id][SenModeCnt] > maxDPLimitCnt)
        {
            maxDPLimitCnt = DPLimitCnt[Id][SenModeCnt];
        }
        SumDPLimitCnt = SumDPLimitCnt + DPLimitCnt[Id][SenModeCnt];
    }

    pTag = (pCAL_TAG)DmaAddr;
    //pTag->TagDataLen = sizeof(CAL_DP_FMT) + DP_MAX_CNT*sizeof(UINT32);
    pTag->TagDataLen = sizeof(CAL_DP_FMT) + maxDPLimitCnt*sizeof(UINT32);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize < TagSize)
    {
        DBG_ERR("buffer overflow\r\n");
        return E_SYS;
    }
    /*
        get current item ID
    */
    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_DP_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_DP_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_DP_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_DP_4;
    }
    else
    {
        CalSectionItem = SEC_DP_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    /*
        get current Section (DP) information
    */
    UINT32 SectionCnt = 0;
    while (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
    {
        if ((CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == CalSectionItem)
            &&(CalSectionInfo[Id].CalSecInfo[SectionCnt].bDP == FALSE))
        {
            SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
            CalInfo = CalSectionInfo[Id].CalInfo->DPDarkTH;
            break;
        }
        else
        {
            SectionCnt++;
            if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
            {
                SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
                DBG_ERR("section is not in the current IPL\r\n");
            }
            if (SectionCnt > MaxSectionNum)
            {
                DBG_ERR("section is out of range\r\n");
                return E_SYS;
            }
        }
    }

    if ((SectionInfo.SecSize) != (SumDPLimitCnt*4 + 0x100))
    {
        DBG_WRN("Section size %d & DPLimitCnt not match %d\r\n", SectionInfo.SecSize, SumDPLimitCnt*4 + 0x100);
        CalSectionInfo[Id].CalSecInfo[SectionCnt].SecSize = (SumDPLimitCnt*4 + 0x100);
    }

    ConditionCnt = 0;
    while (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
        SenModeCnt = 0;
        while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            pTag = CAL_RW()->fpReadTag(CalSectionItem, GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin));

            if(pTag != NULL)
            {
                if (pTag->CAL_STATUS != _CAL_DP_B_OK)
                {
                    DP_Status_Chk = _CAL_NONE;
                }
            }
            else
            {
                DP_Status_Chk = _CAL_NONE;
            }

            SenModeCnt++;
        }
        ConditionCnt++;
    }

    // stop all pipeline
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    if (DP_Status_Chk != _CAL_DP_B_OK)
    {
        CAL_RW()->fpDeleteSection(CalSectionItem);

        //allocate section buffer
        dmaRst = CfgSectionBuf(CalSectionItem,DmaAddr,DmaSize);
        if(dmaRst.erCode != E_OK)
            return E_SYS;
        else
        {
            DmaAddr = dmaRst.RemainAddr;
            DmaSize = dmaRst.RemainSize;
        }
    }
    TagTempAddr = DmaAddr;

    //shift current dram address (shift size: 1 tag size), temporal buf
    pTag = (pCAL_TAG)TagTempAddr;
    //pTag->TagDataLen = sizeof(CAL_DP_FMT) + DP_MAX_CNT*sizeof(UINT32);
    pTag->TagDataLen = sizeof(CAL_DP_FMT) + maxDPLimitCnt*sizeof(UINT32);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize >= TagSize)
    {
        DmaSize -= TagSize;
        DmaAddr += TagSize;
    }
    else
    {
        DBG_ERR("buffer overflow\r\n");
        return E_SYS;
    }

    /*
        init buffer
        -> start run all pipeline (with sie only flag) to re-set sie output address
        -> pause sie
        -> release buffer
    */
    // ipp buf init
    //buf.Id = Id;
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    buf.Id = bufID;
    buf.StartAddr = DmaAddr;
    buf.Size = DmaSize;
    CalApi()->fpBufEvent()->fpInit(buf);

    SenModeCnt = 0;
    UINT32 maxSize = 0;
    UINT32 senModeSize = 0;
    UINT32 maxSizeSenMode = 0;
    while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
    {
        rt = Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

         //coverity warning
        if (rt != E_OK || Drv_Sensor.Mode == NULL)
           return E_SYS;

        senModeSize = Drv_Sensor.Mode->TransHD[0].DataSize * Drv_Sensor.Mode->TransVD[0].DataSize;
        if (maxSize < senModeSize)
        {
            maxSize = senModeSize;
            maxSizeSenMode = SenModeCnt;
        }
        SenModeCnt++;
    }

    // start run
    Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[maxSizeSenMode].SenMode, &Drv_Sensor);
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;
    CtrlInfo.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[maxSizeSenMode]);
    CtrlInfo.fpRdyImg = NULL;
    CalApi()->fpSetMode(Id,CAL_IPL_ALLRUN,&CtrlInfo,Cal_CtrlInit()->fpCalStation_CtrlInit);

    // pause sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

    //release buf
    CalApi()->fpBufEvent()->fpUnInit(buf.Id);

    //set debug information for SetAdjustRst()
    CAL_DEBUG_INFO *CalDebugInfor = &CalDebugInfor1;
    CAL_DEBUG_INFO_DP* CalDebugDP = &CalDebugDP1;
    CalDebugInfor->CalDebugDP = CalDebugDP;
    CalDebugInfor->CalDebugISO = NULL;
    CalDebugInfor->CalDebugECS = NULL;
    CalDebugInfor->CalDebugAWB = NULL;

    //clean OSD to show calibration result on OSD
#if DetailOSD
    if(fpUIcb)
        fpUIcb(UICB_CLEAN,(CHAR*)m_CalMsg,UICOL_RED);
#endif

    /*
        calibration for per sensor mode
    */
    //#NT#2016/06/07#Silvia Wu -begin
    //#NT# fixed coverity warning
    //CAL_DP_FMT pDPFmt1;
    //CAL_DP_FMT* pDPFmt = &pDPFmt1;
    CAL_DP_FMT* pDPFmt;
    //#NT#2016/06/07#Silvia Wu -end
    CAL_DPC_HEADER Dark_Cnt = {0};

    ConditionCnt = 0;
    while (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
#if 0
        switch (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin)
        {
            case CAL_CONDITION_DEFAULT:
            default:
                sprintf((char*)m_CalMsg, "sensor %d Set to Bright", Id);
                break;
        }

        if(fpUIcb)
            fpUIcb(UICB_NONE,(CHAR*)m_CalMsg,UICOL_GREEN);
#endif
        UINT32 SenModeCnt = 0;
        while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            BOOL bDpOverFlow = FALSE;

            if (DP_Status_Chk == _CAL_DP_B_OK)
            {
                //read bright dp result
                pTag = CAL_RW()->fpReadTag(CalSectionItem, GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin));

                if(pTag == NULL || pTag->CAL_STATUS != _CAL_DP_B_OK)
                {
                    if (pTag != NULL)
                        DBG_ERR("DP tag read fail %x!!\r\n",pTag->CAL_STATUS);
                    else
                        DBG_ERR("DP tag read fail !!\r\n");

                    return E_SYS;
                }
                else
                {
                    DBG_IND("read DP bright data\r\n");
                }

                CopyTag(TagTempAddr,pTag);
                pTag = (pCAL_TAG)TagTempAddr;
            }

            //ipp buf init
            buf.Id = bufID;
            buf.StartAddr = DmaAddr;
            buf.Size = DmaSize;
            CalApi()->fpBufEvent()->fpInit(buf);

            //init sensor info
            CtrlPtr.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[SenModeCnt]);
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);
            CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;

            //config target sensor mode
            IPC_WaitVD(Id,TRUE); //IPC_WaitVD(SensorID,TRUE);
            Sensor_ChgMode(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
            IPC_WaitVD(Id,TRUE); //IPC_WaitVD(SensorID,TRUE);

            //get sensor info
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

            //chg sie to capture mode
            CalApi()->fpSetMode(Id,CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpDP_CtrlInit);
            Bayer = CalApi()->fpGetRawInfo(Id);

            //cal 3a converage
            //ae
            //aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

            // coverity warning
            if (Drv_Sensor.Mode==NULL)
                return E_SYS;

            TempExpT = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0];
            SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0]*100/Drv_Sensor.Mode->biningRatio;

            aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

            SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = TempExpT;

            //pause sie
            CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

            //save raw image
            if (CalDbg[CALDEBUG_RAW] == 1)
            {
                CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
            }

            //run alg
            pDPFmt = (CAL_DP_FMT*)&pTag->TagData;
            if (DP_Status_Chk != _CAL_DP_B_OK)
            {
                pDPFmt->DPCnt.PixelCnt = 0;
            }

            DP_Info.DPC.Bayer = Bayer.BaseInfo;
            DP_Info.DPC.DPAddr = (UINT32)&pDPFmt->DP_Pool;
            DP_Info.DPC.BayField = 1;//cmos
            DP_Info.DPC.BayAct_Str = Bayer.ActStart;
            DP_Info.DPC.BayCrop_Str = Bayer.CropStart;
            DP_Info.DPC.Count = pDPFmt->DPCnt.PixelCnt;
            //DP_Info.DPC.MaxCount = DP_MAX_CNT;
            DP_Info.DPC.MaxCount = DPLimitCnt[SensorID][SenModeCnt];
            DP_Info.DPC.Threshold = CalInfo->Threshold;
            DP_Info.DPC.Block_X = CalInfo->Block_X;
            DP_Info.DPC.Block_Y = CalInfo->Block_Y;
            DP_Info.SkipXL = 0;
            DP_Info.SkipXR = 0;
            DP_Info.SkipYD = 0;
            DP_Info.SkipYU = 0;

            Dark_Cnt.PixelCnt = CalApi()->fpAlgEvent()->fpDPC_Bayer(&DP_Info,&bDpOverFlow);
            pDPFmt->DPCnt.PixelCnt += Dark_Cnt.PixelCnt;
            DBG_IND("DP Rst(SensorMode = %d): DP cnt= %d\r\n",SectionInfo.SenModeInfo[SenModeCnt].SenMode,pDPFmt->DPCnt.PixelCnt);

            if(bDpOverFlow)
            {
                pTag->CAL_STATUS = _CAL_OverFlow;
                buf_cnt = snprintf(NULL, 0, "DP overflow");
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP overflow");

            }
            else
            {
                if (DP_Status_Chk == _CAL_DP_B_OK)
                {
                    pTag->CAL_STATUS = _CAL_OK;
                }
                else
                {
                    pTag->CAL_STATUS = _CAL_DP_D_OK;
                }
#if AE_TimeOut
                if (aeRst.status == CAL_AE_TIMEOUT)
                {
                    DBG_WRN("DP dark  AE timeout\r\n");
                    pTag->CAL_STATUS = _CAL_AE_TIMEOUT;
                }
#endif
            }

            if (pTag->CAL_STATUS == _CAL_OK)
            {
                if ( pDPFmt->DPCnt.PixelCnt != 0)
                {
                    //sort Bright & Dark DP.
                    DBG_IND("sort & arrange DP format\r\n");
                    CalApi()->fpAlgEvent()->fpDPC_Sort(&pDPFmt->DP_Pool,pDPFmt->DPCnt.PixelCnt);
                }
                pTag->TagDataLen = sizeof(CAL_DPC_HEADER) + CalApi()->fpAlgEvent()->fpDPC_ArgFmt(&pDPFmt->DP_Pool,pDPFmt->DPCnt.PixelCnt,&pDPFmt->DPCnt.LineCnt);
            }
            else
            {
                pTag->TagDataLen = sizeof(CAL_DPC_HEADER) + sizeof(UINT32)*pDPFmt->DPCnt.PixelCnt;
            }

            pTag->TagID = (UINT32)GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin);

            DBG_IND("DP Rst(SensorMode = %d): DP cnt= %d, DATALen = %d\r\n",SectionInfo.SenModeInfo[SenModeCnt].SenMode,pDPFmt->DPCnt.PixelCnt,pTag->TagDataLen);

            if (CalDbg[CALDEBUG_RESULT_DATA] == 1)
            {
                CalApi()->fpSaveRAW((UINT32)&pTag->TagID,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->CAL_STATUS,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagDataLen,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagData,1,pTag->TagDataLen);
            }

            //write tag
            CAL_RW()->fpAddTag(CalSectionItem,pTag);

            //release buf
            CalApi()->fpBufEvent()->fpUnInit(buf.Id);

#if DetailOSD //show calibration result on OSD

            if (pTag->CAL_STATUS == _CAL_OK)
            {
                buf_cnt = snprintf(NULL, 0, "DP PASS %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP PASS %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
            else if (pTag->CAL_STATUS == _CAL_DP_D_OK)
            {
                buf_cnt = snprintf(NULL, 0, "DP(DARK) PASS %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP(DARK) PASS %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
            else
            {
                buf_cnt = snprintf(NULL, 0, "DP(DARK) FAIL %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP(DARK) FAIL %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
#endif

            //set adjust result information for debug
            if (aeRst.status == CAL_AE_TIMEOUT)
                CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].AeTimeout = TRUE;
            else
                CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].AeTimeout = FALSE;

            CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].Lum        = aeRst.convergeLum;
            CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].ISO        = aeRst.uiISO[0];
            CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].ISOBase    = aeRst.uiISOBase;
            CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].ExpT       = aeRst.uiExpT[0];
            CalDebugInfor->CalDebugDP->CalAeRstDark[MaxSenModeCnt + SenModeCnt].Ratio      = aeRst.uiRatio[0];
            SetAdustRst(Id, fpUIcb, CalDebugInfor);

            //next sensor mode
            SenModeCnt++;
            if (SenModeCnt > MaxSenModeNum)
            {
                DBG_ERR("sensor mode number is out of range\r\n");
                break;
            }
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt++;
    }

    //resume sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIERESUME,NULL,NULL);

    //stop all
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //if (Cal_Status == _CAL_DP_D_OK || Cal_Status == _CAL_OK)
    {
        //write pstore
        CAL_RW()->fpSectionWrite(CalSectionItem);
    }

#if DetailOSD //show calibration result on OSD
    if(fpUIcb)
        fpUIcb(UICB_WAITKEY,NULL,UICOL_GREEN);
#endif

    return E_OK;
}

static ER DP_Adjust_Bright(IPL_PROC_ID Id, fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(Id);
    CAL_AE_RST aeRst = {0};
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor = {0};
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    DPC_ALG_INFO DP_Info;
    UINT32 DP_Status_Chk = _CAL_DP_D_OK;
    CAL_SECMAP CalSectionItem;
    Cal_SECTION SectionInfo = {0};
    CAL_DPC_THRES CalInfo1 = {0};
    CAL_DPC_THRES* CalInfo = &CalInfo1;
    CTRL_PTR CtrlInfo = {0};
    CalBuf buf;
    UINT32 TempExpT;
    UINT32 buf_cnt = 0;
    static CHAR m_CalMsg[100];
    UINT32 MaxSenModeCnt = 0;
    UINT32 ConditionCnt = 0;
    UINT32 SenModeCnt = 0;
    ER rt = E_OK;


    // find max (DP limit cnt) to set dram size
    UINT32 maxDPLimitCnt = 0;
    UINT32 SumDPLimitCnt = 0;
    for (SenModeCnt=0; SenModeCnt<MaxSenModeNum; SenModeCnt++)
    {
        if (DPLimitCnt[Id][SenModeCnt] > maxDPLimitCnt)
        {
            maxDPLimitCnt = DPLimitCnt[Id][SenModeCnt];
        }
        SumDPLimitCnt = SumDPLimitCnt + DPLimitCnt[Id][SenModeCnt];
    }

    pTag = (pCAL_TAG)DmaAddr;
    //pTag->TagDataLen = sizeof(CAL_DP_FMT) + DP_MAX_CNT*sizeof(UINT32);
    pTag->TagDataLen = sizeof(CAL_DP_FMT) + maxDPLimitCnt*sizeof(UINT32);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize < TagSize)
    {
        DBG_ERR("buffer overflow\r\n");
        return E_SYS;
    }

    /*
        get current item ID
    */
    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_DP_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_DP_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_DP_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_DP_4;
    }
    else
    {
        CalSectionItem = SEC_DP_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    /*
        get current Section (DP) information
    */
    UINT32 SectionCnt = 0;
    while (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
    {
        if ((CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == CalSectionItem)
            &&(CalSectionInfo[Id].CalSecInfo[SectionCnt].bDP == TRUE))
        {
            SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
            CalInfo = CalSectionInfo[Id].CalInfo->DPBrightTH;
            break;
        }
        else
        {
            SectionCnt++;
            if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
            {
                SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
                DBG_ERR("section is not in the current IPL\r\n");
            }
            if (SectionCnt > MaxSectionNum)
            {
                DBG_ERR("section is out of range\r\n");
                return E_SYS;
            }
        }
    }

    if ((SectionInfo.SecSize) != (SumDPLimitCnt*4 + 0x100))
    {
        DBG_WRN("Section size %d & DPLimitCnt not match %d\r\n", SectionInfo.SecSize, SumDPLimitCnt*4 + 0x100);
        CalSectionInfo[Id].CalSecInfo[SectionCnt].SecSize = (SumDPLimitCnt*4 + 0x100);
    }


    ConditionCnt = 0;
#if DP_Bright_only
#else
    while (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
        SenModeCnt = 0;
        while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            pTag = CAL_RW()->fpReadTag(CalSectionItem, GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin));
            if(pTag != NULL)
            {
                if (pTag->CAL_STATUS != _CAL_DP_D_OK)
                {
                    DP_Status_Chk = _CAL_NONE;
                }
            }
            else
            {
                DP_Status_Chk = _CAL_NONE;
            }

            SenModeCnt++;
        }
        ConditionCnt++;
    }
#endif

    // stop all pipeline
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

#if DP_Bright_only
#else
    if (DP_Status_Chk != _CAL_DP_D_OK)
#endif
    {
        CAL_RW()->fpDeleteSection(CalSectionItem);

        //allocate section buffer
        dmaRst = CfgSectionBuf(CalSectionItem,DmaAddr,DmaSize);
        if(dmaRst.erCode != E_OK)
            return E_SYS;
        else
        {
            DmaAddr = dmaRst.RemainAddr;
            DmaSize = dmaRst.RemainSize;
        }
    }

    TagTempAddr = DmaAddr;

    //shift current dram address (shift size: 1 tag size), temporal buf
    pTag = (pCAL_TAG)TagTempAddr;
    //pTag->TagDataLen = sizeof(CAL_DP_FMT) + DP_MAX_CNT*sizeof(UINT32);
    pTag->TagDataLen = sizeof(CAL_DP_FMT) + maxDPLimitCnt*sizeof(UINT32);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize >= TagSize)
    {
        DmaSize -= TagSize;
        DmaAddr += TagSize;
    }
    else
    {
        DBG_ERR("buffer overflow\r\n");
        return E_SYS;
    }

    /*
        init buffer
        -> start run all pipeline (with sie only flag) to re-set sie output address
        -> pause sie
        -> release buffer
    */
    // ipp buf init
    //buf.Id = Id;
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    buf.Id = bufID;
    buf.StartAddr = DmaAddr;
    buf.Size = DmaSize;
    CalApi()->fpBufEvent()->fpInit(buf);



    SenModeCnt = 0;
    UINT32 maxSize = 0;
    UINT32 senModeSize = 0;
    UINT32 maxSizeSenMode = 0;
    while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
    {
        rt = Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

         //coverity warning
        if (rt != E_OK || Drv_Sensor.Mode == NULL)
           return E_SYS;

        senModeSize = Drv_Sensor.Mode->TransHD[0].DataSize * Drv_Sensor.Mode->TransVD[0].DataSize;
        if (maxSize < senModeSize)
        {
            maxSize = senModeSize;
            maxSizeSenMode = SenModeCnt;
        }
        SenModeCnt++;
    }

    // start run
    Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[maxSizeSenMode].SenMode, &Drv_Sensor);
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;
    CtrlInfo.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[maxSizeSenMode]);
    CtrlInfo.fpRdyImg = NULL;
    CalApi()->fpSetMode(Id,CAL_IPL_ALLRUN,&CtrlInfo,Cal_CtrlInit()->fpCalStation_CtrlInit);

    // pause sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

    //release buf
    CalApi()->fpBufEvent()->fpUnInit(buf.Id);

    //set debug information for SetAdjustRst()
    CAL_DEBUG_INFO *CalDebugInfor = &CalDebugInfor1;
    CAL_DEBUG_INFO_DP* CalDebugDP = &CalDebugDP1;
    CalDebugInfor->CalDebugDP = CalDebugDP;
    CalDebugInfor->CalDebugISO = NULL;
    CalDebugInfor->CalDebugECS = NULL;
    CalDebugInfor->CalDebugAWB = NULL;

    //clean OSD to show calibration result on OSD
#if DetailOSD
    if(fpUIcb)
        fpUIcb(UICB_CLEAN,(CHAR*)m_CalMsg,UICOL_RED);
#endif

    /*
        calibration for per sensor mode
    */
    //#NT#2016/06/07#Silvia Wu -begin
    //#NT# fixed coverity warning
    //CAL_DP_FMT pDPFmt1;
    //CAL_DP_FMT* pDPFmt = &pDPFmt1;
    CAL_DP_FMT* pDPFmt;
    //#NT#2016/06/07#Silvia Wu -end
    CAL_DPC_HEADER Bright_Cnt = {0};
    ConditionCnt = 0;
    while (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin != CAL_CONDITION_END)
    {
#if 0
        switch (SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin)
        {
            case CAL_CONDITION_DEFAULT:
            default:
                sprintf((char*)m_CalMsg, "sensor %d Set to Dark", Id);
                break;
        }

        if(fpUIcb)
            fpUIcb(UICB_NONE,(CHAR*)m_CalMsg,UICOL_GREEN);
#endif
        /*UINT32 */SenModeCnt = 0;
        while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
        {
            BOOL bDpOverFlow = FALSE;
            //CalBuf buf;
#if DP_Bright_only
#else
            if (DP_Status_Chk == _CAL_DP_D_OK)
            {
                //read bright dp result
                pTag = CAL_RW()->fpReadTag(CalSectionItem, GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin));

                if(pTag == NULL || pTag->CAL_STATUS != _CAL_DP_D_OK)
                {
                    if (pTag != NULL)
                        DBG_ERR("DP tag read fail %x!!\r\n",pTag->CAL_STATUS);
                    else
                        DBG_ERR("DP tag read fail !!\r\n");
                    return E_SYS;
                }
                else
                {
                    DBG_IND("read DP dark data\r\n");
                }

                CopyTag(TagTempAddr,pTag);
                pTag = (pCAL_TAG)TagTempAddr;
            }
#endif

            //ipp buf init
            buf.Id = bufID;
            buf.StartAddr = DmaAddr;
            buf.Size = DmaSize;
            CalApi()->fpBufEvent()->fpInit(buf);

            //init sensor info
            CtrlPtr.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[SenModeCnt]);
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);
            CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;

            //config target sensor mode
            IPC_WaitVD(Id,TRUE); //IPC_WaitVD(SensorID,TRUE);
            Sensor_ChgMode(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
            IPC_WaitVD(Id,TRUE); //IPC_WaitVD(SensorID,TRUE);

            //get sensor info
            Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

            //chg sie to capture mode
            CalApi()->fpSetMode(Id,CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpDP_CtrlInit);
            Bayer = CalApi()->fpGetRawInfo(Id);

            //cal 3a converage
            //ae
            //aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

            // coverity warning
            if (Drv_Sensor.Mode==NULL)
                return E_SYS;

            TempExpT = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0];
            SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0]*100/Drv_Sensor.Mode->biningRatio;

            aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

            if (SectionInfo.CalAeInfoCon->CalAeInfo.uiTarget + 2 < aeRst.convergeLum)
            {
                DBG_ERR("DP bright  AE timeout\r\n");
                aeRst.status = CAL_AE_TIMEOUT;
            }

            SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = TempExpT;

            //pause sie
            CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

            //save raw image
            if (CalDbg[CALDEBUG_RAW] == 1)
            {
                CalApi()->fpSaveRAW(Bayer.BaseInfo.PixelAddr,Bayer.BaseInfo.LineOffset,Bayer.BaseInfo.Height);
            }

            //run alg
            pDPFmt = (CAL_DP_FMT*)&pTag->TagData;
#if DP_Bright_only
#else
            if (DP_Status_Chk != _CAL_DP_D_OK)
#endif
            {
                pDPFmt->DPCnt.PixelCnt = 0;
            }

            DP_Info.DPC.Bayer = Bayer.BaseInfo;
            DP_Info.DPC.DPAddr = (UINT32)&pDPFmt->DP_Pool;
            DP_Info.DPC.BayField = 1;//cmos
            DP_Info.DPC.BayAct_Str = Bayer.ActStart;
            DP_Info.DPC.BayCrop_Str = Bayer.CropStart;
            DP_Info.DPC.Count = pDPFmt->DPCnt.PixelCnt;
            //DP_Info.DPC.MaxCount = DP_MAX_CNT;
            DP_Info.DPC.MaxCount = DPLimitCnt[SensorID][SenModeCnt];
            DP_Info.DPC.Threshold = CalInfo->Threshold;
            DP_Info.DPC.Block_X = CalInfo->Block_X;
            DP_Info.DPC.Block_Y = CalInfo->Block_Y;
            DP_Info.SkipXL = 0;
            DP_Info.SkipXR = 0;
            DP_Info.SkipYD = 0;
            DP_Info.SkipYU = 0;

            Bright_Cnt.PixelCnt = CalApi()->fpAlgEvent()->fpDPC_Bayer(&DP_Info,&bDpOverFlow);

            pDPFmt->DPCnt.PixelCnt += Bright_Cnt.PixelCnt;
            DBG_IND("DP Rst(SensorMode = %d): DP cnt= %d\r\n",SectionInfo.SenModeInfo[SenModeCnt].SenMode,pDPFmt->DPCnt.PixelCnt);

            if(bDpOverFlow)
            {
                pTag->CAL_STATUS = _CAL_OverFlow;
                buf_cnt = snprintf(NULL, 0, "DP overflow");
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP overflow");
            }
            else
            {
                if (DP_Status_Chk == _CAL_DP_D_OK)
                {
                    pTag->CAL_STATUS = _CAL_OK;
                }
                else
                {
#if DP_Bright_only
#else
                    pTag->CAL_STATUS = _CAL_DP_B_OK;
#endif
                }
#if AE_TimeOut
                if (aeRst.status == CAL_AE_TIMEOUT)
                {
                    DBG_WRN("DP bright  AE timeout\r\n");
                    pTag->CAL_STATUS = _CAL_AE_TIMEOUT;
                }
#endif
            }

            if (pTag->CAL_STATUS == _CAL_OK)
            {
                if ( pDPFmt->DPCnt.PixelCnt != 0)
                {
                    //sort Bright & Dark DP.
                    DBG_IND("sort & arrange DP format\r\n");
                    CalApi()->fpAlgEvent()->fpDPC_Sort(&pDPFmt->DP_Pool,pDPFmt->DPCnt.PixelCnt);
                }
                pTag->TagDataLen = sizeof(CAL_DPC_HEADER) + CalApi()->fpAlgEvent()->fpDPC_ArgFmt(&pDPFmt->DP_Pool,pDPFmt->DPCnt.PixelCnt,&pDPFmt->DPCnt.LineCnt);
            }
            else
            {
                pTag->TagDataLen = sizeof(CAL_DPC_HEADER) + sizeof(UINT32)*pDPFmt->DPCnt.PixelCnt;
            }
            pTag->TagID = (UINT32)GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode, SectionInfo.CalAeInfoCon[ConditionCnt].CalConditin);

            DBG_IND("DP Rst(SensorMode = %d): DP cnt= %d, DATALen = %d\r\n",SectionInfo.SenModeInfo[SenModeCnt].SenMode,pDPFmt->DPCnt.PixelCnt,pTag->TagDataLen);

            if (CalDbg[CALDEBUG_RESULT_DATA] == 1)
            {
                CalApi()->fpSaveRAW((UINT32)&pTag->TagID,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->CAL_STATUS,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagDataLen,1,sizeof(UINT32));
                CalApi()->fpSaveRAW((UINT32)&pTag->TagData,1,pTag->TagDataLen);
            }

            //write tag
            CAL_RW()->fpAddTag(CalSectionItem,pTag);

            //release buf
            CalApi()->fpBufEvent()->fpUnInit(buf.Id);

#if DetailOSD //show calibration result on OSD
            /*CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
            CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
            GetAdjustRst(Id, NULL, CalDebugInfor_DNP);*/
            if (pTag->CAL_STATUS == _CAL_OK)
            {
                buf_cnt = snprintf(NULL, 0, "DP PASS %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP PASS %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
            else if (pTag->CAL_STATUS == _CAL_DP_B_OK)
            {
                buf_cnt = snprintf(NULL, 0, "DP(BRIGHT) PASS %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP(BRIGHT) PASS %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
            else
            {
                buf_cnt = snprintf(NULL, 0, "DP(BRIGHT) FAIL %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP(BRIGHT) FAIL %d(mode %d)", pDPFmt->DPCnt.PixelCnt, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
                if(fpUIcb)
                    fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
#endif

            //set adjust result information for debug
            if (aeRst.status == CAL_AE_TIMEOUT)
                CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].AeTimeout = TRUE;
            else
                CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].AeTimeout = FALSE;

            CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].Lum        = aeRst.convergeLum;
            CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].ISO        = aeRst.uiISO[0];
            CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].ISOBase    = aeRst.uiISOBase;
            CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].ExpT       = aeRst.uiExpT[0];
            CalDebugInfor->CalDebugDP->CalAeRstBright[MaxSenModeCnt + SenModeCnt].Ratio      = aeRst.uiRatio[0];
            SetAdustRst(Id, NULL, CalDebugInfor);


            //next sensor mode
            SenModeCnt++;
            if (SenModeCnt > MaxSenModeNum)
            {
                DBG_ERR("sensor mode number is out of range\r\n");
                break;
            }
        }
        if (ConditionCnt == 0)
        {
            MaxSenModeCnt = SenModeCnt;
        }
        ConditionCnt++;
    }

    //resume sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIERESUME,NULL,NULL);

    //stop all
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //if (Cal_Status == _CAL_DP_B_OK || Cal_Status == _CAL_OK)
    {
        //write pstore
        CAL_RW()->fpSectionWrite(CalSectionItem);
    }

#if DetailOSD //show calibration result on OSD
    if(fpUIcb)
        fpUIcb(UICB_WAITKEY,NULL,UICOL_GREEN);
#endif

    return E_OK;
}


static ER DP_Adjust(IPL_PROC_ID Id, fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    #if 0
    DP_Adjust_Dark(Id,fpUIcb,DmaAddr,DmaSize);

    DP_Adjust_Bright(Id,fpUIcb,DmaAddr,DmaSize);
    #endif

    return E_OK;
}

static ER Mshutter_Adjust(IPL_PROC_ID Id, fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
#if 0
    if(fpUIcb)
        fpUIcb(UICB_START,NULL,UICOL_GREEN);

    DBG_ERR("CAL: Mshutter_Adjust: not ready yet\r\n");

    if(fpUIcb)
        fpUIcb(UICB_END,(CHAR*)m_CalMsg,UICOL_GREEN);
#endif
    return E_OK;
}

static ER AE_Check(IPL_PROC_ID Id, fpUI_cb fpUIcb,UINT32 DmaAddr,UINT32 DmaSize)
{
    #if 0
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(Id);
    CAL_AE_RST aeRst = {0};
    CTRL_PTR CtrlPtr = {0};
    SENSOR_INFO Drv_Sensor;
    IPL_RAW_IMG Bayer;
    CAL_DMA_RST dmaRst;
    UINT32 TagTempAddr;
    pCAL_TAG pTag;
    UINT32 TagSize;
    CAL_AECHK_FMT* pAECHKFmt;
    CAL_SECMAP CalSectionItem;
    Cal_SECTION SectionInfo = {0};
    CTRL_PTR CtrlInfo = {0};
    CalBuf buf;

    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_AECHK_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_AECHK_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_AECHK_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_AECHK_4;
    }
    else
    {
        CalSectionItem = SEC_AECHK_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    CAL_RW()->fpDeleteSection(CalSectionItem);

    /*
        get current Section (AE Check) information
    */
    UINT32 SectionCnt = 0;
    while (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
    {
        if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == CalSectionItem)
        {
            SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
            break;
        }
        else
        {
            SectionCnt++;
            if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
            {
                SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
                DBG_ERR("section is not in the current IPL\r\n");
            }
            if (SectionCnt > MaxSectionNum)
            {
                DBG_ERR("section is out of range\r\n");
                return E_SYS;
            }
        }
    }

    sprintf((char*)m_CalMsg, "Set to LV15");
    if(fpUIcb)
        fpUIcb(UICB_START,(CHAR*)m_CalMsg,UICOL_GREEN);

    // stop all pipeline
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //allocate section buffer
    dmaRst = CfgSectionBuf(CalSectionItem,DmaAddr,DmaSize);
    if(dmaRst.erCode != E_OK)
        return E_SYS;
    else
    {
        DmaAddr = dmaRst.RemainAddr;
        DmaSize = dmaRst.RemainSize;
    }

    //shift current dram address (shift size: 1 tag size), temporal buf
    TagTempAddr = DmaAddr;
    pTag = (pCAL_TAG)TagTempAddr;
    pTag->TagDataLen = sizeof(CAL_AECHK_FMT);
    TagSize = (TAG_TypeSize + pTag->TagDataLen);

    if(DmaSize >= TagSize)
    {
        DmaAddr += TagSize;
        DmaSize -= TagSize;
    }
    else
    {
        DBG_ERR("section buffer overflow\r\n");
        return E_SYS;
    }

    /*
        init buffer
        -> start run all pipeline (with sie only flag) to re-set sie output address
        -> pause sie
        -> release buffer
    */
    // ipp buf init
    //buf.Id = Id;
    Cal_BufID bufID;
    switch(Id)
    {
        case IPL_ID_1:
           bufID = BUF_ID_1;
           break;
        case IPL_ID_2:
           bufID = BUF_ID_2;
           break;
        case IPL_ID_3:
           bufID = BUF_ID_3;
           break;
        case IPL_ID_4:
           bufID = BUF_ID_4;
           break;
        default:
           bufID = BUF_ID_1;
           break;
    }
    buf.Id = bufID;
    buf.StartAddr = DmaAddr;
    buf.Size = DmaSize;
    //DBG_ERR("ECS init addr=0x%x\r\n",buf.StartAddr);
    CalApi()->fpBufEvent()->fpInit(buf);
    // start run
    Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[0].SenMode, &Drv_Sensor);
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;
    CtrlInfo.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[0]);
    CtrlInfo.fpRdyImg = NULL;
    CalApi()->fpSetMode(Id,CAL_IPL_ALLRUN,&CtrlInfo,Cal_CtrlInit()->fpCalStation_CtrlInit);
    // pause sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);
    //release buf
    CalApi()->fpBufEvent()->fpUnInit(buf.Id);

    UINT32 SenModeCnt = 0;
    while (SectionInfo.SenModeInfo[SenModeCnt].SenMode != SENSOR_MODE_UNKNOWN)
    {
        CalBuf buf;
        //lock 3a,fd...etc.
        //AAA_SWITCH(TRUE,SensorID);
        //ipp buf init
        buf.Id = bufID;
        buf.StartAddr = DmaAddr;
        buf.Size = DmaSize;
        CalApi()->fpBufEvent()->fpInit(buf);
        //init sensor info
        CtrlPtr.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[SenModeCnt]);
        Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);
        CtrlPtr.pDrv_Sensor = (void*)&Drv_Sensor;
        //config target sensor mode
        IPC_WaitVD(TRUE,SensorID);
        Sensor_ChgMode(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode);
        IPC_WaitVD(TRUE,SensorID);
        //get sensor info
        Sensor_GetStatus(SensorID, SectionInfo.SenModeInfo[SenModeCnt].SenMode, &Drv_Sensor);

        //chg sie to capture mode
        CalApi()->fpSetMode(Id,CAL_IPL_CAP,&CtrlPtr,Cal_CtrlInit()->fpISO_CtrlInit);
        Bayer = CalApi()->fpGetRawInfo(Id);

        //cal 3a converage
        //ae
        aeRst = CalApi()->fp3AEvent()->fpAEStable(&Bayer.BaseInfo,SectionInfo.AeInfor,Id);

        //pause sie
        CalApi()->fpSetMode(Id,CAL_IPL_SIESTOP,NULL,NULL);

        //run alg
        pAECHKFmt = (CAL_AECHK_FMT*)&pTag->TagData;
        pAECHKFmt->ChkStatus = aeRst.status;

        pTag->CAL_STATUS = _CAL_OK;

        if(aeRst.status == CAL_AE_STABLE)
        {
            gCalIQData.Cal_Status.AECHK_STATUS = _CAL_OK;
            sprintf((char*)m_CalMsg, "AE CHECK OK");
            DBG_ERR("AE CHECK  PASS\r\n");

            CalStrColor = UICOL_GREEN;
        }
        else
        {
            pTag->CAL_STATUS = _CAL_NONE;
            gCalIQData.Cal_Status.AECHK_STATUS = _CAL_NONE;
            sprintf((char*)m_CalMsg, "AE CHECK NG");
            DBG_ERR("AE CHECK  NG\r\n");
            CalStrColor = UICOL_RED;
        }

        pTag->TagDataLen = sizeof(CAL_AECHK_FMT);
        pTag->TagID = (UINT32)GetTagID(CalSectionItem, SectionInfo.SenModeInfo[SenModeCnt].SenMode);

        //write tag
        CAL_RW()->fpAddTag(CalSectionItem,pTag);

        //release buf
        CalApi()->fpBufEvent()->fpUnInit(buf.Id);

        if(fpUIcb)
            fpUIcb(UICB_PROCESSING,(CHAR*)m_CalMsg,CalStrColor);

        //next sensor mode
        SenModeCnt++;
        if (SenModeCnt > MaxSenModeNum)
        {
            DBG_ERR("sensor mode number is out of range\r\n");
            break;
        }
    }

    //resume sie
    CalApi()->fpSetMode(Id,CAL_IPL_SIERESUME,NULL,NULL);

    //stop all
    CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);

    //write pstore
    CAL_RW()->fpSectionWrite(CalSectionItem);

    if(fpUIcb)
        fpUIcb(UICB_END,NULL,CalStrColor);
    #endif
    return E_OK;
}

/**
    CAL debug level read from SD card
*/

ER CAL_SetDebugLevel (CalDebugIdx idx, UINT32 value)
{
    if (idx >= CALDEBUG_MAX_NUM)
    {
        DBG_ERR("idx %d > max %d\r\n", idx, CALDEBUG_MAX_NUM);
        return E_SYS;
    }

    CalDbg[idx] = value;

    return E_OK;
}

UINT32 CAL_GetDebugLevel (CalDebugIdx idx)
{
    if (idx >= CALDEBUG_MAX_NUM)
    {
        DBG_ERR("idx %d > max %d\r\n", idx, CALDEBUG_MAX_NUM);
        return E_SYS;
    }

    return CalDbg[idx];
}

CAL_ITEM_OBJ m_CalItem = {
    ISO_Adjust,
    AWB_Adjust,
    AWBGS_Adjust,
    Mshutter_Adjust,
    ECS_Adjust,
    DP_Adjust,
    DP_Adjust_Bright,
    DP_Adjust_Dark,
    AE_Check,
    GetAdjustRst,
    CAL_GetDebugLevel,
    NULL,
    NULL,
    LENSCEN_Adjust,
};

static INT32 fs_getline(CHAR **lineptr, UINT32 offset ,UINT32 filesize)
{
    char   *ptr = *lineptr;
    INT32  templen=0;

    if (offset >= filesize)
        return -1;
    while (offset+templen < filesize)
    {
        //DBG_MSG("0x%x ",*ptr);
        if (*ptr == '\n' || *ptr == 0)
        {
            //DBG_MSG("0x%x ",*ptr);
            return templen+1;
        }
        ptr++;
        templen++;
    }
    //DBG_MSG("\r\n");
    return templen;
}


BOOL CAL_ReadDebugConfig_OV4689_IMX322LQJ_EVB_FF(void)
{
    // read config from A:\\eng.cfg
    char        path[20]= "A:\\eng.cfg";
    FST_FILE    filehdl;
    CHAR        tempbuf[200];
    UINT32      Size = sizeof(tempbuf);
    UINT32      fileSize;
    char        *line = NULL;
    UINT32      len = 0;
    INT32       read, offset;
    char        keyTempstr[30];



    filehdl = FileSys_OpenFile(path,FST_OPEN_READ);
    if(NULL == filehdl)
    {
        DBG_ERR("open %s fail\r\n",path);
        return FALSE;
    }
    fileSize = FileSys_GetFileLen(path);
    FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);
    FileSys_ReadFile(filehdl, (UINT8*)tempbuf, &Size, 0, NULL);

    line = tempbuf;
    offset = 0;
    while ((read = fs_getline(&line, offset ,fileSize)) != -1)
    {
        if (line[read - 1] == '\n')
        {
            line[read - 1] = 0;
            read-=1;
        }
        //DBG_IND("fileSize = %d, read = %d,offset = %d, line=0x%x\r\n",fileSize,read,offset,line);
        //DBG_IND("%s\r\n",line);

        //cal_debug_info
        //len = sprintf(keyTempstr,"cal_debug_info=");
        len = snprintf(keyTempstr, sizeof(keyTempstr),"cal_debug_info=");

        if (strncmp(keyTempstr,line,len) ==0)
        {
            int cal_debug_info = 0;
            sscanf_s(line+len,"%d",&cal_debug_info);
            DBG_IND("cal_debug_info %d\r\n", cal_debug_info);
            CAL_SetDebugLevel (CALDEBUG_INFO, cal_debug_info);
            line+=read;
            offset+=read;
            continue;
        }

        //cal_debug_raw
        //len = sprintf(keyTempstr,"cal_debug_raw=");
        len = snprintf(keyTempstr, sizeof(keyTempstr),"cal_debug_raw=");

        if (strncmp(keyTempstr,line,len) ==0)
        {
            int cal_debug_raw = 0;
            sscanf_s(line+len,"%d",&cal_debug_raw);
            DBG_IND("cal_debug_raw %d\r\n", cal_debug_raw);
            CAL_SetDebugLevel (CALDEBUG_RAW, cal_debug_raw);
            line+=read;
            offset+=read;
            continue;
        }

        //cal_debug_data
        //len = sprintf(keyTempstr,"cal_debug_data=");
        len = snprintf(keyTempstr, sizeof(keyTempstr),"cal_debug_data=");

        if (strncmp(keyTempstr,line,len) ==0)
        {
            int cal_debug_data = 0;
            sscanf_s(line+len,"%d",&cal_debug_data);
            DBG_IND("cal_debug_data %d\r\n", cal_debug_data);
            CAL_SetDebugLevel (CALDEBUG_RESULT_DATA, cal_debug_data);
            line+=read;
            offset+=read;
            continue;
        }

        //cal_debug_osdwaitkey
        //len = sprintf(keyTempstr,"cal_debug_osdwaitkey=");
        len = snprintf(keyTempstr, sizeof(keyTempstr),"cal_debug_osdwaitkey=");

        if (strncmp(keyTempstr,line,len) ==0)
        {
            int cal_debug_osdwaitkey = 0;
            sscanf_s(line+len,"%d",&cal_debug_osdwaitkey);
            DBG_IND("cal_debug_osdwaitkey %d\r\n", cal_debug_osdwaitkey);
            CAL_SetDebugLevel (CALDEBUG_OSD_WAITKEY, cal_debug_osdwaitkey);
            line+=read;
            offset+=read;
            continue;
        }

        //cal_debug_prvstart
        //len = sprintf(keyTempstr,"cal_debug_prvstart=");
        len = snprintf(keyTempstr, sizeof(keyTempstr),"cal_debug_prvstart=");

        if (strncmp(keyTempstr,line,len) ==0)
        {
            int cal_debug_prvstart = 0;
            sscanf_s(line+len,"%d",&cal_debug_prvstart);
            DBG_IND("cal_debug_prvstart %d\r\n", cal_debug_prvstart);
            CAL_SetDebugLevel (CALDEBUG_PRV_START, cal_debug_prvstart);
            line+=read;
            offset+=read;
            continue;
        }


        //cal_debug_prvend
        //len = sprintf(keyTempstr,"cal_debug_prvend=");
        len = snprintf(keyTempstr, sizeof(keyTempstr),"cal_debug_prvend=");

        if (strncmp(keyTempstr,line,len) ==0)
        {
            int cal_debug_prvend = 0;
            sscanf_s(line+len,"%d",&cal_debug_prvend);
            DBG_IND("cal_debug_prvend %d\r\n", cal_debug_prvend);
            CAL_SetDebugLevel (CALDEBUG_PRV_END, cal_debug_prvend);
            line+=read;
            offset+=read;
            continue;
        }

        line+=read;
        offset+=read;

    }
    FileSys_CloseFile(filehdl);
    return TRUE;
}


pCAL_ITEM_OBJ CAL_ITEM_OV4689_IMX322LQJ_EVB_FF(IPL_PROC_ID Id)
{
    //release buf used by set2preview first
    //CalApi()->fpBufEvent()->fpUnInit(Id);
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    CalApi()->fpBufEvent()->fpUnInit(bufID);

    return &m_CalItem;
}

static void Cal_Display_cfg(UINT32 ImgIdx,IPL_YCC_IMG_INFO* path1,IPL_YCC_IMG_INFO* path2,IPL_YCC_IMG_INFO* path3,IPL_YCC_IMG_INFO* path4,fpCallBack fp_CB, IPL_PROC_ID IPL_ID)
{
    DISPLAYER_PARAM LyrParam;
    DISP_OBJ* pDispObj=disp_getDisplayObject(DISP_1);

    static UINT32 cnt = 0;

    pDispObj->open();

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_ENABLE.bEn = TRUE;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_ENABLE, &LyrParam);

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_MODE.BufFormat = DISPBUFFORMAT_YUV420PACK;
    LyrParam.SEL.SET_MODE.BufMode = DISPBUFMODE_AUTO_HANDSHAKE; //don't care
    LyrParam.SEL.SET_MODE.BufNumber = DISPBUFNUM_1; //don't care
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_MODE,&LyrParam);

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_WINSIZE.iWinOfsX = 0;
    LyrParam.SEL.SET_WINSIZE.iWinOfsY = 0;
    LyrParam.SEL.SET_WINSIZE.uiWinWidth = path2->Ch[0].Width;
    LyrParam.SEL.SET_WINSIZE.uiWinHeight = path2->Ch[0].Height;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_WINSIZE,&LyrParam);

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_BUFSIZE.uiBufWidth = path2->Ch[0].Width;
    LyrParam.SEL.SET_BUFSIZE.uiBufHeight = path2->Ch[0].Height;
    LyrParam.SEL.SET_BUFSIZE.uiBufLineOfs = path2->Ch[0].LineOfs;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_BUFSIZE,&LyrParam);

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_VDOBUFADDR.BufSel = DISPBUFADR_0;
    LyrParam.SEL.SET_VDOBUFADDR.uiAddrY0 = path2->PixelAddr[0];
    LyrParam.SEL.SET_VDOBUFADDR.uiAddrCb0 = path2->PixelAddr[1];
    LyrParam.SEL.SET_VDOBUFADDR.uiAddrCr0 = path2->PixelAddr[2];
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_VDOBUFADDR,&LyrParam);

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_ACTBUF.ActiveBuf = DISPACTBUF_0;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_ACTBUF, &LyrParam);

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_OUTDIR.BufOutDir = DISPOUTDIR_NORMAL;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_OUTDIR, &LyrParam);

    memset(&LyrParam,0,sizeof(DISPLAYER_PARAM));
    LyrParam.SEL.SET_BUFXY.uiBufOfsX = 0;
    LyrParam.SEL.SET_BUFXY.uiBufOfsY = 0;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1,DISPLAYER_OP_SET_BUFXY,&LyrParam);

    pDispObj->load(FALSE);

    //IME output addr no PPB

    cnt++;
}

static void SetAwbGain(IPL_PROC_ID Id, CAL_AWB_RST awbrst, CCRange BitSel)
{
    //set awb stable result (Rgain, Bgain)
    //AWB_SetColorGain(Id, awbrst.Rgain, 256, awbrst.Bgain);

    IFE_SUBFUNC*    Ife_sub = &Ife_sub1;
    IPL_HAL_GROUP*   Grp = &Grp1;
    UINT32* pWeight = &pWeight1[0];
    IFE_FILT* pFilter = &pFilter1;
    Spatial_Filter* Spatial =  &Spatial1;
    Range_Filter* Range = &Range1;
    RangeCFGA* A = &A11;
    RangeCFGB* B = &B11;
    UINT32* A_pThres = &A_pThres1[0];
    UINT32* A_pThLut = &A_pThLut1[0];
    UINT32* B_pThres = &B_pThres1[0];
    UINT32* B_pThLut = &B_pThLut1[0];

    Ipc_IQ.EngSubF.pIfe_sub = Ife_sub;
    Ipc_IQ.pGrp = Grp;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter = pFilter;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Spatial = Spatial;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Spatial->pWeight = pWeight;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Spatial->sOnlyEn = 1;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range = Range;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range->A = A;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range->B = B;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range->A->pThres = A_pThres;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range->A->pThLut = A_pThLut;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range->B->pThres = B_pThres;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range->B->pThLut = B_pThLut;
    Ipc_IQ.EngSubF.pIfe_sub->CGain.CG_Mask.GainMask = GainMask;
    Ipc_IQ.Idx = 1;
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Spatial->pWeight = &IFE_spatial[0];
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range->A->pThres = &IFE_A_pThres[0];
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range->A->pThLut = &IFE_A_pThLut[0];
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range->B->pThres = &IFE_B_pThres[0];
    Ipc_IQ.EngSubF.pIfe_sub->pFilter->Range->B->pThLut = &IFE_B_pThLut[0];

    CAL_IQS_CGain_PRV[0] = awbrst.Rgain;
    CAL_IQS_CGain_PRV[1] = 256;
    CAL_IQS_CGain_PRV[2] = 256;
    CAL_IQS_CGain_PRV[3] = awbrst.Bgain;
    Ipc_IQ.EngSubF.pIfe_sub->CGain.pCGain = &CAL_IQS_CGain_PRV[0];
    Ipc_IQ.EngSubF.pIfe_sub->CGain.IfeGainSel = BitSel;
    Ipc_IQ.EngSubF.pIfe_sub->ifeFuncEn |=  (FilterEn     |  ///< enable bilateral filter
                                            CgainEn     |  ///< enable color gain
                                            0);

    Ipc_IQ.pGrp->ifeUpdate |= (IFE_CGAIN       |       ///< update color gain
                               IFE_SUBFEN       |      ///< update func enable/disable
                               IFE_FILTER       |       ///< update noise reduction filter
                               IFE_CGEFFECT       |       ///< update color gain output inverse/mask
                            0);


    IPL_CTRLRunTimeChg_IQ(Id, IPC_IFE_IQparam,(void*)&Ipc_IQ);
}

/*
    this preview have no calibration result.
*/
static BOOL m_CalPrv[IPL_ID_MAX_NUM] = {FALSE, FALSE, FALSE, FALSE};
ER Set2Preview_OV4689_IMX322LQJ_EVB_FF(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize)
{
    CalBuf buf = {0};
    SENSOR_INFO Drv_Sensor;
    CTRL_PTR CtrlInfo = {0};
    ER ErrorCode;//ER ErrorCode = E_OK;
    IPL_RAW_IMG BayerInfo;
    CAL_SECMAP CalSectionItem;
    Cal_SECTION SectionInfo = {0};
    UINT32 SensorID = IPL_UTI_CONV2_SEN_ID(Id);
    CCRange BitSel;
    CAL_AWB_RST awbrst;
    IPL_PROC_ID iplID;
    Cal_BufID bufID;

    // get ipl number from project
    IPL_Num_OV4689_IMX322LQJ = IplNum;

    if (Id == IPL_ID_1)
    {
        CalSectionItem = PRV_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = PRV_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = PRV_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = PRV_4;
    }
    else
    {
        CalSectionItem = PRV_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    //get current Section (preview) information
    UINT32 SectionCnt = 0;
    while (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem != IPL_Section_End)
    {
        if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == CalSectionItem)
        {
            SectionInfo = CalSectionInfo[Id].CalSecInfo[SectionCnt];
            break;
        }
        else
        {
            SectionCnt++;
            if (CalSectionInfo[Id].CalSecInfo[SectionCnt].CalItem == IPL_Section_End)
            {
                DBG_ERR("section is not in the current IPL\r\n");
            }
            if (SectionCnt > MaxSenModeNum)
            {
                DBG_ERR("section number is out of range\r\n");
                break;
            }
        }
    }

    //stop IPP & uninit buffer previous preview sensor ID
    for (iplID = 0; iplID < IPL_ID_MAX_NUM; iplID++)
    {
        if (m_CalPrv[iplID] == TRUE)
        {
            CalApi()->fpSetMode(iplID,CAL_IPL_ALLSTOP,NULL,NULL);
            m_CalPrv[iplID] = FALSE;

            //CalApi()->fpBufEvent()->fpUnInit(iplID);
            bufID = CalApi()->fpBufEvent()->fpGetCalBufID(iplID);

            CalApi()->fpBufEvent()->fpUnInit(bufID);
        }
    }

    //set preview sensor mode information
    IPL_AlgSetUIInfo(Id, IPL_SEL_PRVSENMODE, SectionInfo.SenModeInfo->SenMode);

    //init buf for preview only
    //buf.Id = Id;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    buf.Id = bufID;
    buf.StartAddr = BufAddr;
    buf.Size = BufSize;
    ErrorCode = CalApi()->fpBufEvent()->fpInit(buf);
    if (ErrorCode!=E_OK)
        return ErrorCode;

    //set sensor mode
    Sensor_ChgMode(SensorID,IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE));
    Sensor_GetStatus(SensorID, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &Drv_Sensor);

    CtrlInfo.fpRdyImg = Cal_Display_cfg;
    CtrlInfo.pDrv_Sensor = (void*)&Drv_Sensor;

    //get calibration info mapping to current sensor mode
    CtrlInfo.pCal_Sensor = (void*)&(SectionInfo.SenModeInfo[0]);

    //start ipp
    ErrorCode = CalApi()->fpSetMode(Id,CAL_IPL_PRV,&CtrlInfo,Cal_CtrlInit()->fpPrv_CtrlInit);
    m_CalPrv[Id] = TRUE;

    //AE
    BayerInfo = CalApi()->fpGetRawInfo(Id);
    BitSel = CalSectionInfo[Id].CalInfo->AwbInfor->awb_bit;
    //CalApi()->fp3AEvent()->fpAEStable(&BayerInfo.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);

    UINT32 TempExpT;
    TempExpT = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0];
    SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0]*100/Drv_Sensor.Mode->biningRatio;
    CalApi()->fp3AEvent()->fpAEStable(&BayerInfo.BaseInfo,SectionInfo.CalAeInfoCon->CalAeInfo,Id);
    SectionInfo.CalAeInfoCon->CalAeInfo.uiExpT[0] = TempExpT;

    //AWB
    awbrst = CalApi()->fp3AEvent()->fpAWBStable(&BayerInfo.BaseInfo, BitSel);
    SetAwbGain(Id, awbrst, BitSel);

    return ErrorCode;
}


