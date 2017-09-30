#ifndef MOVIE_MAPPING_H
#define MOVIE_MAPPING_H

#define MOVIE_MAX_CHANNEL  3

typedef enum _MOVIE_REC_TYPE{
    MOVIE_REC_TYPE_UNKNOWN,
    MOVIE_REC_TYPE_FRONT,
    MOVIE_REC_TYPE_BEHIND,
    MOVIE_REC_TYPE_DUAL,
    MOVIE_REC_TYPE_CLONE,
}MOVIE_REC_TYPE;

//bit rate control type
typedef enum _MOVIE_BRC_TYPE{
    MOVIE_BRC_TYPE_NONE,
    MOVIE_BRC_TYPE_CBR,
    MOVIE_BRC_TYPE_VBR,
    MOVIE_BRC_TYPE_MAX
}MOVIE_BRC_TYPE;

typedef struct {
    BOOL   bCropEnable;
    UINT32 uiCoordinateX;
    UINT32 uiCoordinateY;
} MOVIE_ENCODE_CROP;




extern UINT32   GetMovieSizeWidth(UINT32 uiIndex);
extern UINT32   GetMovieSizeHeight(UINT32 uiIndex);
extern UINT32   GetMovieFrameRate(UINT32 uiIndex);
extern UINT32   GetMovieTargetBitrate(UINT32 uiSize, UINT32 uiQuality);
extern void     SetMovieTargetBitrate(UINT32 uiPath, UINT32 uiSize, UINT32 uiBitRate);
#if (UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE)
extern void     SetMovieLiveViewFrameRate(UINT32 uiPath, UINT32 uiIndex, UINT32 uiFrameRate);
#endif

extern UINT32   GetMovieGOP_2p(UINT32 uiPath);
extern void     SetMovieGOP_2p(UINT32 uiPath, UINT32 gop);
extern UINT32   GetMovieSVCT_2p(UINT32 uiPath);
extern void     SetMovieSVCT_2p(UINT32 uiPath, UINT32 svct);
extern void     SetMovieVidEncFmt_2p(UINT32 uiPath, UINT32 vidEncFmt);
extern UINT32   GetMovieVidEncFmt_2p(UINT32 uiPath);
extern void     SetMovieSizeIdx_2p(UINT32 uiPath, UINT32 uiSizeIndex);
extern UINT32   GetMovieSizeIdx_2p(UINT32 uiPath);
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
extern UINT32   GetMovieSizeRatio_2p(UINT32 uiPath);
extern MOVIE_REC_TYPE GetMovieRecType_2p(UINT32 uiIndex);
extern void     SetMovieCrop_2p(UINT32 uiPath, MOVIE_ENCODE_CROP *crop);
extern MOVIE_ENCODE_CROP* GetMovieCrop_2p(UINT32 uiPath);
extern VOID             SetMovieBRC_2p(UINT32 uiPath, MOVIE_BRC_TYPE BrcType, VOID *pBrcInfo);
extern MOVIE_BRC_TYPE   GetMovieBRC_2p(UINT32 uiPath, UINT32 *pBrcInfoAddr);
extern UINT32   CheckWiFiMapTbl(void);

#endif
