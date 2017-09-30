////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppMovie.h"
#include "MovieInterface.h"
#include "UIMovieMapping.h"
#include "FileSysTsk.h"
#include "ImageApp_NetMovie.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppMovie
#define __DBGLVL__          1 //0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
extern BOOL     gb3DNROut;
#endif
//#NT#2016/09/20#Bob Huang -end

static UINT32 MovExe_GetMaxRecSec(void)
{
    UINT32 sec;

    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI) // wifi mode
    {
        if (ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_WRITE_COUNT) == 2)
        {
            sec = MovRec_GetFreeRecSecByPath(1, 1);
        }
        else
        {
            sec = MovRec_GetFreeRecSecByPath(1, 0);
        }
    }
    else
    {
        if(GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_DUAL
        || GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_CLONE)
        {
            sec = MovRec_GetFreeRecSecByPath(1, 1);
        }
        else
        {
            sec = MovRec_GetFreeRecSecByPath(1, 0);
        }
    }

    return sec;
}

UINT32 MovieExe_GetMaxRecSec(void)
{
    UINT32 sec;
    MovRec_SetDisk2SecRestriction(MEDIAREC_RESTR_INDEXBUF, 0);
    MovRec_SetDisk2SecRestriction(MEDIAREC_RESTR_FSLIMIT, 0);
    //#NT#2016/09/20#Bob Huang -begin
    //#NT#Support HDMI Display with 3DNR Out
    #if (_3DNROUT_FUNC == ENABLE)
    if(gb3DNROut)
    {
        sec = MovRec_GetFreeRecSecByPath(1, 0);
    }
    else
    #endif
    //#NT#2016/09/20#Bob Huang -end
    {
        //sec = MovRec_Disk2SecondWithType(MOVREC_COUNTTYPE_FS, 0);
        sec = MovExe_GetMaxRecSec();
    }
    DBG_IND("max sec %d\r\n", sec);
    return sec ;
}

UINT32 MovieExe_GetFreeRecSec(void)
{
    UINT32 sec;

    #if 0
    MEDIAREC_CALCSEC_SETTING setting;

    setting.nowFSFreeSpace = FileSys_GetDiskInfo(FST_INFO_FREE_SPACE);
    setting.vidfps = GetMovieFrameRate(UI_GetData(FL_MOVIE_SIZE));//30;
    setting.vidTBR = GetMovieTargetBitrate(UI_GetData(FL_MOVIE_SIZE), 0);//0x180000;
    setting.audSampleRate = 32000;
    setting.audCodec = MOVAUDENC_PCM;
    setting.audChs = 1;
    setting.clustersize = FileSys_GetDiskInfo(FST_INFO_CLUSTER_SIZE);
    setting.gpson = 1;
    setting.nidxon= 1;

    sec = SMediaRec_CalcSecBySetting(&setting);
    //sec = MovRec_CalcSecBySetting(&setting); //TBD
    DBG_IND("max sec %d\r\n", retV);
    #else
    sec = MovExe_GetMaxRecSec();
    #endif

    return sec ;
}

