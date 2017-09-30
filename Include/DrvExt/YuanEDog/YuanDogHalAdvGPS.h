#ifndef YUANDOGHALGPSADVGPS_H__
#define YUANDOGHALGPSADVGPS_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C"{
#endif
 
int YuanHalAdvGPSGetInfo(unsigned char *fixed, unsigned long *date, unsigned long *time, float *lat, float *lng, float *speed, float *bearing, unsigned char *satellites);

#ifdef __cplusplus
}
#endif

#endif //YUANDOGHALGPSADVGPS_H__