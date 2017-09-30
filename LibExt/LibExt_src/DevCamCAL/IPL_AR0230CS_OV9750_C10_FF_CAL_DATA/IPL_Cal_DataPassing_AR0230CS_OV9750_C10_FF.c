#include "IPL_Cal_DataPassing_AR0230CS_OV9750_C10_FF_Int.h"
#include "IPL_CAL_Debug.h"

#define TblSize 2

static UINT32 SecctionTbl_Tag[][TblSize] = {
    {SEC_AWB_1,   TAG_AWB},

    {SEC_ECS_1,   TAG_ECS},

    {SEC_DP_1,    TAG_DP},

    {SEC_ISO_1,   TAG_ISO},

    {SEC_MSHUT_1, TAG_MSHUT},

    {SEC_AECHK_1,   TAG_AECHK},

    {SEC_LENSCEN_1,   TAG_LENSCEN},

    {SEC_AWB_2,   TAG_AWB},

    {SEC_ECS_2,   TAG_ECS},

    {SEC_DP_2,    TAG_DP},

    {SEC_ISO_2,   TAG_ISO},

    {SEC_MSHUT_2, TAG_MSHUT},

    {SEC_AECHK_2,   TAG_AECHK},

    {SEC_LENSCEN_2,   TAG_LENSCEN},
};

CAL_TAGID GetTagID(CAL_SECMAP SecID,UINT32 Idx, UINT32 option)
{
    UINT32 i = 0;
    UINT32 loop = sizeof(SecctionTbl_Tag)/(sizeof(UINT32)*TblSize);

    CAL_CONDITION condition = option;
    SENSOR_MODE senMode = Idx;
    senMode = senMode<<(0X10);

    for(i=0;i<loop;i++)
    {
        if(SecctionTbl_Tag[i][0] == SecID)
            return (SecctionTbl_Tag[i][1] + senMode + condition);
    }

    DBG_ERR("no match\r\n");
    return TAG_NONE;
}

/**
    need to match with IPL_Cal_Setting.c
*/
UINT32 CalSenModeMap(IPL_PROC_ID Id, CAL_SECMAP SecID, UINT32 Input_SenMode)
{
    switch(SecID)
    {
        case SEC_AWB_1:
        case SEC_AWB_2:
        case SEC_AWB_3:
        case SEC_AWB_4:
        case SEC_ISO_1:
        case SEC_ISO_2:
        case SEC_ISO_3:
        case SEC_ISO_4:
        case SEC_ECS_1:
        case SEC_ECS_2:
        case SEC_ECS_3:
        case SEC_ECS_4:
        case SEC_DP_1:
        case SEC_DP_2:
        case SEC_DP_3:
        case SEC_DP_4:
        case SEC_LENSCEN_1:
        case SEC_LENSCEN_2:
        case SEC_LENSCEN_3:
        case SEC_LENSCEN_4:
        default:
            if (Id == IPL_ID_1)
                return SENSOR_MODE_1;
            else
                return SENSOR_MODE_3;
            break;
    }
}

#if 0
    - extern
#endif

/**
    need to match with IPL_Cal_DataPassing.h
*/
static CAL_SECMAP secMap[IPL_ID_MAX_NUM][Section_End] =
{{SEC_AWB_1, SEC_ECS_1, SEC_DP_1, SEC_MSHUT_1, SEC_ISO_1, SEC_AECHK_1, SEC_LENSCEN_1},
 {SEC_AWB_2, SEC_ECS_2, SEC_DP_2, SEC_MSHUT_2, SEC_ISO_2, SEC_AECHK_2, SEC_LENSCEN_2},
 {SEC_AWB_3, SEC_ECS_3, SEC_DP_3, SEC_MSHUT_3, SEC_ISO_3, SEC_AECHK_3, SEC_LENSCEN_3},
 {SEC_AWB_4, SEC_ECS_4, SEC_DP_4, SEC_MSHUT_4, SEC_ISO_4, SEC_AECHK_4, SEC_LENSCEN_4}};

/**
    need to match with IPL_Cal_Setting.c
*/
static CAL_CONDITION CalConditionMap(IPL_PROC_ID Id, CAL_DATA_INFO dataInfo)
{
    switch (dataInfo.StationID)
    {
        case SEC_AWB:
            switch (dataInfo.CalCondition[0])
            {
                case CAL_CONDITION_LOW_CT:
                case CAL_CONDITION_HIGH_CT:
                case CAL_CONDITION_DEFAULT:
                default:
                    return (dataInfo.CalCondition[0]);
                    break;
            }
            break;

        case SEC_ECS:
            switch (dataInfo.CalCondition[0])
            {
                case CAL_CONDITION_ZOOM0:
                case CAL_CONDITION_ZOOM1:
                case CAL_CONDITION_ZOOM2:
                case CAL_CONDITION_ZOOM3:
                case CAL_CONDITION_DEFAULT:
                default:
                    return (dataInfo.CalCondition[0]);
                    break;
            }
            break;

        case SEC_DP:
            return (dataInfo.CalCondition[0]);
            break;

        case SEC_ISO:
            return (dataInfo.CalCondition[0]);
            break;

        default:
            return CAL_CONDITION_DEFAULT;
            break;
    }
}

CAL_TAG* GetCalData(IPL_PROC_ID Id, CAL_DATA_INFO dataInfo)
{
    CAL_SECMAP_GETDATA StationID = dataInfo.StationID;
    SENSOR_MODE Input_SenMode = dataInfo.SenMode;
    UINT32 Cal_SenMode;
    CAL_SECMAP SecID;
    CAL_CONDITION condition;//CAL_CONDITION condition = CAL_CONDITION_DEFAULT;

    SecID = secMap[Id][StationID];
    Cal_SenMode = CalSenModeMap(Id, SecID, Input_SenMode);
    condition = CalConditionMap(Id, dataInfo);

    return CAL_RW()->fpReadTag(SecID,GetTagID(SecID, Cal_SenMode, condition));
}
