#ifndef MOVIE_MAPPING_H
#define MOVIE_MAPPING_H

typedef enum _MOVIE_REC_TYPE{
    MOVIE_REC_TYPE_UNKNOWN,
    MOVIE_REC_TYPE_FRONT,
    MOVIE_REC_TYPE_BEHIND,
    MOVIE_REC_TYPE_DUAL,
    MOVIE_REC_TYPE_CLONE,
}MOVIE_REC_TYPE;



extern UINT32   GetMovieSizeWidth(UINT32 uiIndex);
extern UINT32   GetMovieSizeHeight(UINT32 uiIndex);
extern UINT32   GetMovieFrameRate(UINT32 uiIndex);
extern UINT32   GetMovieTargetBitrate(UINT32 uiSize, UINT32 uiQuality);
extern void     SetMovieTargetBitrate(UINT32 uiType, UINT32 uiSize, UINT32 uiBitRate);
extern void     SetMovieTargetFrameRate(UINT32 uiType, UINT32 uiIndex, UINT32 uiFramRate);
#if (UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE)
extern void     SetMovieLiveViewFrameRate(UINT32 uiType, UINT32 uiIndex, UINT32 uiFrameRate);
#endif
extern UINT32   GetMovieDispAspectRatio(UINT32 uiIndex);
extern UINT32   GetMovieSizeRatio(UINT32 ubIndex);
extern UINT32   GetMovieAudioVolumeValue(UINT32 uiIndex);
extern UINT32   Get_MovieTimeLapseValue(UINT32 uhIndex);
//#NT#2015/03/18#Lily Kao -begin
//#NT#Add for UVAC to set parameters for MFK
extern UINT32   GetMovieTBRAspRatio(UINT32 width, UINT32 height, UINT32 fps, UINT32 *pTBR, UINT32 *pAspRatio);
//#NT#2015/03/18#Lily Kao -end

extern UINT32   GetMovieSizeWidth_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieSizeHeight_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieFrameRate_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieTargetBitrate_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieDispAspectRatio_2p(UINT32 uiPath, UINT32 uiIndex);
extern MOVIE_REC_TYPE GetMovieRecType_2p(UINT32 uiIndex);
extern UINT32   CheckWiFiMapTbl(void);

#endif
