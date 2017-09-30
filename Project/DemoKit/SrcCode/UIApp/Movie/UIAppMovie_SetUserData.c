////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppMovie.h"
#include "MovieInterface.h"
#include "MovieUdtaVendor.h"
#include "IQS_Utility.h"
#include "ImageApp_CamMovie.h"
//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif
//#NT#2016/06/28#KCHong -end
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppMovie
#define __DBGLVL__          1 //0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

extern GPSDATA    gpsdata;

void Movie_SetUserData(void)
{
    #if (SBS_VIEW_FUNC == ENABLE)
    UINT32  uiPoolAddr;
    #else
    UINT32  uiUserDataAddr, uiUserDataSize = 0;
    #endif

    // Disable end-char 'A' of file name.
    MovRec_EnableEndChar4SingleRec(FALSE);
    MovRec_EnableEndChar4MultiRec(TRUE);

    //#NT#2016/11/18#KS Hung -begin
    //#NT#For Side By Side calibration from APP
    #if (SBS_VIEW_FUNC == ENABLE)
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_SBS_CAL);
    MovRec_SetUserData((UINT32)uiPoolAddr, SysGetFlag(FL_SBS_CAL_PARAM_SIZE));
    #else
    //#NT#2013/05/15#Calvin Chang#Add customer's user data to MOV/MP4 file format -begin
    if(MovieUdta_MakeVendorUserData(&uiUserDataAddr, &uiUserDataSize))
    {
        // Enable custom user data
        MovRec_ChangeParameter(MOVREC_RECPARAM_EN_CUSTOMUDATA, TRUE, 0, 0);
        // Set user data
        MovRec_SetUserData((UINT32)uiUserDataAddr, uiUserDataSize);
    }
    //#NT#2013/05/15#Calvin Chang -end
    #endif
    //#NT#2016/11/18#KS Hung -end

    // for GPS data recording test
    MovRec_ChangeParameter(MOVREC_RECPARAM_GPSON, TRUE, 0, 0);

//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    UINT32 GPSReadyBufAddr = 0;
    UINT32 LogSize = mp4log_GetTotalLogSize() * 1024;
    LogSize = LogSize?LogSize:1024;
    MovRec_SetGpsBlkSize(LogSize);
    if(mp4log_ToggleBuf(&GPSReadyBufAddr, NULL) == E_OK)
    {
        MovRec_SetGPSData(GPSReadyBufAddr, (LogSize - 16));
    }
    else
    {
        DBG_ERR("No GPS buffer!!!");
    }
#else
    MovRec_SetGPSData((UINT32)&gpsdata, sizeof(GPSDATA));
#endif
//#NT#2016/06/28#KCHong -end
}

