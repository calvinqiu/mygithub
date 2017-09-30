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

//�����̺߳���
//����:
//	pThreadFun		�̺߳���
//	lpParam			�̺߳�������
//	ThreadID		�߳�ID
//����ֵ:
//	1: �ɹ��� 0: ʧ�ܣ�
int YuanHalCreateThread(YuanHalThread pThreadFun, uint32 *lpParam,uint32 ThreadID);

//�����̺߳���
//����:
//	ThreadID		�߳�ID
//����ֵ:			��
void YuanHalDestroyThread(uint32 ThreadID);


#ifdef __cplusplus
}
#endif

#endif