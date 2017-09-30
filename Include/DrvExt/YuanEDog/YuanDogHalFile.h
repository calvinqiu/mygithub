#ifndef YUANDOGHALFILE_H__
#define YUANDOGHALFILE_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C"{
#endif 

/////////////////////////////////////////////////////////////////////
// global function

/////////////////////////////////////////////////////////////////////
// config file function	
//�������ļ�
//����:				��
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalConfigFileOpen(void);

//�ر������ļ�
//����:				��
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalConfigFileClose(void);

//�������ļ��ж�ȡһ���ı�
//����:
//	pBuf			�ַ�������ָ��
//	Count			�ַ������鳤��
//����ֵ:
//	�����ַ���ָ�룻
char*	YuanHalConfigFileGets(char* pBuf,uint Count);

/////////////////////////////////////////////////////////////////////
// A-GPS file function	

//����A-GPS�ļ����ݣ��ļ�����
//����:				��
//����ֵ:			��
void	YuanHalaGpsDestory(void);

//����A-GPS����
//����:
//	pBuf			����ָ��
//	nBytes			���ݳ���
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalaGpsFileSet(void* pBuf, uint nBytes);

//ȡA-GPS���ݿ�����
//����:
//	pBuf			����ָ��
//	nBytes			���ݳ���
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalaGpsFileGet(uint nPosition, void* pBuf, uint nBytes);
/////////////////////////////////////////////////////////////////////
// e-dog file(db/index) function	
//��ʼ�����ӹ����ݿ��ļ�
//����:				��
//����ֵ:			
//	1: �ɹ���	0: ʧ�ܣ���
int	YuanHalDogDBInit(void);

//���ٵ��ӹ����ݿ��ļ����ݣ��ļ�����
//����:				��
//����ֵ:			��
void	YuanHalDogDBDestory(void);

//��ȡ���ӹ����ݿⳤ��
//����:				��
//����ֵ:			
//	���ص��ӹ����ݿⳤ��
uint	YuanHalDogDBLength(void);

//������ӹ����ݿ�ͷ
//����:				��
//����ֵ:			
//	���ص��ӹ����ݿⳤ��
uint	YuanHalPackageDogDBHead(void);

//д���ӹ����ݿ�����
//����:
//	nPosition		����λ�ã�ƫ������
//	pBuf			����ָ��
//	nBytes			���ݳ���
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalDogDBSet(uint nPosition, uint8* pBuf, uint nBytes);

//ȡ���ӹ����ݿ�����
//����:
//	nPosition		����λ�ã�ƫ������
//	pBuf			����ָ��
//	nBytes			���ݳ���
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalDogDBGet(uint nPosition, void* pBuf, uint nBytes);

//��ʼ�����ӹ����ݿ������ļ�
//����:				��
//����ֵ:			
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalDogIndexInit(void);

//���ٵ��ӹ����ݿ������ļ����ݣ��ļ�����
//����:				��
//����ֵ:			��
void	YuanHalDogIndexDestory(void);

//��ȡ���ӹ����ݿ������ļ�����
//����:				��
//����ֵ:			
//	���ص��ӹ����ݿ������ļ�����
uint	YuanHalDogIndexLength(void);

//д���ӹ����ݿ������ļ�����
//����:
//	nPosition		����λ�ã�ƫ������
//	pBuf			����ָ��
//	nBytes			���ݳ���
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalDogIndexSet(uint nPosition, uint8* pBuf, uint nBytes);

//ȡ���ӹ����ݿ������ļ�����
//����:
//	nPosition		����λ�ã�ƫ������
//	pBuf			����ָ��
//	nBytes			���ݳ���
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int		YuanHalDogIndexGet(uint nPosition, void* pBuf, uint nBytes);

/////////////////////////////////////////////////////////////////////
// NVRM file function	
//дNVRM����
//����:
//	nPosition		����λ�ã�ƫ������
//	pBuf			����ָ��
//	nBytes			���ݳ���
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int		YuanHalNVRMSet(uint nPosition, uint8* pBuf, uint nBytes);

//��NVRM����
//����:
//	nPosition		����λ�ã�ƫ������
//	pBuf			����ָ��
//	nBytes			���ݳ���
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int		YuanHalNVRMGet(uint nPosition, uint8* pBuf, uint nBytes);

//��ȡNVRM�ļ�����
//����:				��
//����ֵ:			
//	���ص��ӹ����ݿ������ļ�����
uint	YuanHalNVRMLength(void);

/////////////////////////////////////////////////////////////////////
// YBT file function	
//�������ļ�
//����:				��
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int		YuanYBTDBFileOpen(const char* FileName);

//��YBT�ļ��ж�ȡһ���ı�
//����:
//	pBuf			�ַ�������ָ��
//	Count			�ַ������鳤��
//����ֵ:
//	�����ַ���ָ�룻
char*	YuanYBTDBFileGets(char* pBuf,uint Count);

//�ر�YBT�ļ�
//����:				��
//����ֵ:
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanYBTDBFileClose(void);


//��ȡ���ӹ��������ݰ�
//����:				
//	start				���ӹ���������
//	buffer				���ӹ�����ָ��
//	buffer_size			���ӹ����ݳ��ȣ�256�����ݣ�
//����ֵ
//  ��ȡ�����ֽ���
unsigned int YuanHalDogDBLoadEx(unsigned int start, void *buffer, unsigned int buffer_len);

//���ص��ӹ�����������
//����:
//	ver				���ӹ����ݰ汾��
//	url				�������ӹ��������ӵ�ַ
//	onCompleted		���ؽ����ص�����
//	onError			����ʧ�ܻص�����
//	data			���ؽ����ص���������
//����ֵ:
//	0: �ɹ���	-1: ʧ�ܣ�
int YuanHalDogDBUpdate(int ver, const char *url, void (*onCompleted)(int ver, void *), void (*onError)(int ver, void *), void *data);

//��ʼ�ϴ��ļ���������
//������
// url				�ϴ���ַ
// imei				�豸IMEI
// filename			�ļ���,����NULLʱ��Ĭ����չ��Ϊjpg, ��������ֵʱʹ�ø�ֵ��Ϊ�����ļ����ļ���������ַ���
//					����filename="photo.png",�������ϴ�����ļ���Ϊ "xxxxxxphoto.png",����xxxxxxΪ���������ɵ�����
// size				�ļ�����
// onUploadStatus	�ϴ�ִ��״̬��codeΪ״̬�룬���£�msgΪ������Ϣ��idΪ��¼��ţ�urlΪ���յ�·��
// handle			�ص������Ĳ���
//����ֵ:
//	ָ���ϴ������һ��ָ�룬��YuanHalDogUploadPut�еĵ�һ������

enum {
	YUANDOG_UPLOAD_OK			=0,		//�ɹ�
	YUANDOG_UPLOAD_COMPLETED	=1,		//�ϴ�����
	YUANDOG_UPLOAD_BUFFERED		=2,		//��������д
	YUANDOG_UPLOAD_ERR			=-1,	//����
	YUANDOG_UPLOAD_PENDING		=-2		//��������
};

void* YuanHalDogUploadStart(const char *url,
						  const char *imei,
						  const char *filename,
						  int size,
						  void (*onUploadStatus)(int code, const char *msg, int id, const char *url, void *handle),
						  void *handle);
//�����ϴ�������
//������
//	handle���ϴ������ָ�룬��YuanHalDogUploadStart���صõ�
//	buf����Ҫ�ϴ�������ָ��
//	size����Ҫ�ϴ����ֽ���
//����ֵ��
//	YUANDOG_UPLOAD_ERR
//	YUANDOG_UPLOAD_PENDING
//	>0 ʵ�ʻ�����ֽ���
int YuanHalDogUploadPut(void *handle, void *buf, int size);
#ifdef __cplusplus
}
#endif

#endif