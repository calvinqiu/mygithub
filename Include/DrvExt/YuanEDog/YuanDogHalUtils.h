#ifndef YUANDOGHALUTILS_H__
#define YUANDOGHALUTILS_H__
#include <assert.h>
#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

//ASSERT ����
//����:
//	arg				ASSERT ����
//����ֵ:			��
#ifdef WIN32
#define YuanHalAssert(arg) assert(arg);
#else
extern void YuanHalAssert(uint32 arg);
#endif

#ifdef __cplusplus
}
#endif

#endif