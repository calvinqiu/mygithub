/**
    ADAS dsp detection library.

    @file       adas_dsp_lib.h
    @ingroup    mILibADASDSP

    Copyright  ALPHAVISON . 2017.  All rights reserved.
*/
#ifndef _ADAS_DSP_LIB_H
#define _ADAS_DSP_LIB_H




/**
    
*/
typedef enum
{
    FCWS_FAILURE_TRUE   = 1,      ///<  in failure mode
    FCWS_FAILURE_FALSE  = 0        ///<  Not in failure mode
} FCWS_FAILURE_STATUS;


//@{
/**
    LDWS Dsp Result
*/
typedef struct _DSP_LDWS_RESULT_INFO
{
    LDWS_FAILURE_STATUS      Failure;             ///<  Failure mode
    LDWS_DEPARTURE_DIR       DepartureDir;        ///<  departure direction
    LDWS_DEPARTURE_DIR       DepartureDirVoice;   ///<  departure direction
    LDWS_AUTOVP_INFO         AutoVpParms;         ///<  auto-vp adjustmnet info
    UINT32                   uiLx1;          ///<  pt1's x coordinate of left lane
    UINT32                   uiLy1;          ///<  pt1's y coordinate of left lane
    UINT32                   uiLx2;          ///<  pt2's x coordinate of left lane
    UINT32                   uiLy2;          ///<  pt2's y coordinate of left lane
    UINT32                   uiRx1;          ///<  pt1's x coordinate of right lane
    UINT32                   uiRy1;          ///<  pt1's y coordinate of right lane
    UINT32                   uiRx2;          ///<  pt2's x coordinate of right lane
    UINT32                   uiRy2;          ///<  pt2's y coordinate of right lane
} DSP_LDWS_RESULT_INFO;


//@{
/**
    FCWS Dsp Result
*/
typedef struct _DSP_FCWS_RESULT_INFO
{
	FCWS_FAILURE_STATUS    Failure;           ///<  Failure mode
    UINT32                 uiDist;            ///<  distance to front car
    URECT                  uiTargetRect;      ///<  target rect
    BOOL                   FCSoundWarning;    ///<  sound warning flag
 	 
} DSP_FCWS_RESULT_INFO;

//@{
/**
    ADAS Dsp Result
*/
typedef struct _ADAS_DSP_RESULT_INFO
{
    DSP_LDWS_RESULT_INFO      LdwsDspRsltInfo;  ///<  LDWS   result
    DSP_FCWS_RESULT_INFO      FcwsDspRsltInfo;  ///<  FCWS   result
} ADAS_DSP_RESULT_INFO;

/**
    AV_DumpRltToADASDspBuf
*/
extern INT32 AV_DumpRltToADASDspBuf(UINT32 GpsSpeed);

/**
    Init ADAS Dsp engine
*/
extern VOID AV_ADASDSP_Init(MEM_RANGE *buf);
/**
    
*/
extern INT32 AV_ADASDSP_Process(void);

/**
    
*/
extern UINT32 AV_ADASDSP_CalcBuffSize(VOID); 

/**
   
*/
extern INT32 ADASDsp_GetLdFcRslt(ADAS_DSP_RESULT_INFO *pADASDspResult, URECT* pTargetCoord);
/**
   
*/
extern BOOL ADASDsp_GetAppsStatus(void);


#endif
