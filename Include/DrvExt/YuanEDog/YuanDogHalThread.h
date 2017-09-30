#ifndef YUANDOGHALTHREAD_H__
#define YUANDOGHALTHREAD_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*YuanHalThread)(void* lpParameter);
/////////////////////////////////////////////////////////////////////
// static function

/////////////////////////////////////////////////////////////////////
// global function

//创建线程函数
//参数:
//	pThreadFun		线程函数
//	lpParam			线程函数参数
//	ThreadID		线程ID
//返回值:
//	1: 成功； 0: 失败；
int YuanHalCreateThread(YuanHalThread pThreadFun, uint32 *lpParam,uint32 ThreadID);

//销毁线程函数
//参数:
//	ThreadID		线程ID
//返回值:			无
void YuanHalDestroyThread(uint32 ThreadID);


#ifdef __cplusplus
}
#endif

#endif