#ifndef YUANDOGHALINTERFACE_H__
#define YUANDOGHALINTERFACE_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C"{
#endif 

/////////////////////////////////////////////////////////////////////
// static function
#define YUAN_IMEI_LEN	15
#define YUAN_IMSI_LEN	18
#define YUAN_ICCID_LEN	20

/////////////////////////////////////////////////////////////////////
// global function

/////////////////////////////////////////////////////////////////////
// interface file function	
//��ȡIMEI��
	//����:				
	//	IMEI		IMEI number��
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalGetIMEI(char* IMEI);

//����IMEI��
	//����:						
	//	IMEI		IMEI number��
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalSetIMEI(char* IMEI);

//��ȡICCID��
//����:				
//	ICCID		ICCID number��
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int YuanHalGetICCID(char *ICCID);

//��ȡIMSI��
//����:				
//	IMSI		IMSI number��
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int YuanHalGetIMSI(char *IMSI);

//��ȡ�汾��
//����:				
//	HWVer			Ӳ���汾�� - BCD�룬etc��v1.01 = 101��
//	MTKVer			����汾�ţ�
//	EDogVer			���ӹ����ݰ汾�ţ�
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalGetVer(uint16 *HWVer, uint16* MTKVer, uint16* EDogVer);


#ifdef __cplusplus
}
#endif

#endif