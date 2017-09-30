#include "UIAppPhoto.h"
#include "ImageUnit_Photo.h"

extern PHOTO_FUNC_INFO PhotoFuncInfo_dummy;

#if _FD_FUNC_
extern PHOTO_FUNC_INFO PhotoFuncInfo_fd;
#endif
#if MOVIE_FD_FUNC_
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvfd;
#endif
#if MOVIE_TSR_FUNC_
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvtsr;
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvtsrdsp;
#endif

#if ((_ADAS_FUNC_ == ENABLE)&&(_ADAS_DSP_FUNC_ ==ENABLE))
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvldfcdsp;
#endif

#if _SD_FUNC_
extern PHOTO_FUNC_INFO PhotoFuncInfo_sd;
#endif

//#NT#2015/09/17#JS Liu -begin
#if _CVHOST_EX_
extern PHOTO_FUNC_INFO MovieFuncInfo_CvHostEx;
#endif
//#NT#2015/09/17#JS Liu -end

//#NT#2016/05/31#Omega Yu -begin
#if _NVTCV_EX_
extern PHOTO_FUNC_INFO MovieFuncInfo_NvtCvEx;
#endif
//#NT#2016/05/31#Omega Yu -end

#if (_ADAS_FUNC_)
extern PHOTO_FUNC_INFO PhotoFuncInfo_adas;
extern PHOTO_FUNC_INFO PhotoFuncInfo_SnG;
extern PHOTO_FUNC_INFO PhotoFuncInfo_autoVP;
#endif
//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvot;
#endif
//#NT#2016/03/02#Lincy Lin -end

//#NT#2016/05/20#Yang Jin -begin
//#NT#Support Dis dsp function
#if MOVIE_DISDSP_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvdisdsp;
#endif
//#NT#2016/05/20#Yang Jin -end

//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FD_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvdddfd;
#endif
#if MOVIE_DDD_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvddd;
#endif
//#NT#2016/04/22#Nestor Yang -end
//#NT#2016/05/27#David Tsai -begin
//#NT#Support tampering detection and background capture function
//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
#if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
void Movie_BCLock(BOOL bLock);
void Movie_BCProcessRateSet(UINT32 ProcessRate);
UINT32 Movie_BCProcessRateGet(void);
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvbc;
#endif

#if MOVIE_TD_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvtd;
#endif
#if MOVIE_TWD_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvtwd;
#endif
#if MOVIE_TZD_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvtzd;
#endif
//#NT#2016/10/14#Yuzhe Bian -end
//#NT#2016/05/27#David Tsai -end

//#NT#2016/10/26#Omega Yu -begin
//#NT# Support scene change detection function
#if MOVIE_SCD_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvscd;
#endif
//#NT#2016/10/26#Omega Yu -end

//#if IPCAM_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_motion;
//#endif
extern PHOTO_FUNC_INFO PhotoFuncInfo_dis;

extern UINT32          Photo_getDualCamSetting(void);
//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
void Movie_FTGLock(BOOL bLock);
BOOL Movie_FTGGetLockSts(void);
void Movie_FTGProcessRateSet(UINT32 ProcessRate);
UINT32 Movie_FTGProcessRateGet(void);
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvftg;
extern PHOTO_FUNC_INFO PhotoFuncInfo_mvftgfd;
#endif
//#NT#2016/10/17#Bin Xiao -end
//#NT#2016/11/01#Charlie Chang -begin
//#NT# support motion detection start flag
void Movie_MDLock(BOOL enable);
//#NT#2016/11/01#Charlie Chang -end
