
#ifndef _GSENSOR_COMBO_H
#define _GSENSOR_COMBO_H

#include "GSensor.h"
typedef enum
{
    GSENSOR_NONE    = 0,
    GSENSOR_DRAMD07,
    GSENSOR_GMA301,
    GSENSOR_DA380,
}GSENSOR_MAKER;

typedef struct
{
     BOOL         (*open)(void);
     BOOL         (*close)(void);
     BOOL         (*GetStatus)(Gsensor_Data *GS_Data);
     BOOL         (*ParkingMode)(void);
     BOOL         (*CrashMode)(void);
     void         (*SetSensitivity)(GSENSOR_SENSITIVITY GSensorSensitivity);
	void         (*ClearCrashMode)(void);
	void         (*SetInterrupt)(BOOL En);	 
}GSENSOR_OBJ, *PGSENSOR_OBJ;


extern PGSENSOR_OBJ GSensor_GMA301_getGSensorObj(void);
extern PGSENSOR_OBJ GSensor_DRAMD07_getGSensorObj(void);
extern PGSENSOR_OBJ GSensor_DA380_getGSensorObj(void);
extern PGSENSOR_OBJ GSensor_NONE_getGSensorObj(void);
extern BOOL   GSensor_SetParkingMode(void);
extern GSENSOR_MAKER GSensor_GetGsensorMaker();
extern BOOL Gsensor_GetCrashMode();
#endif

