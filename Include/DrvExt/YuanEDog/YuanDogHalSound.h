#ifndef YUANDOGHALSOUND_H__
#define YUANDOGHALSOUND_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
//	YUAN_SOUND_MP3,
	YUAN_SOUND_WAV,
	YUAN_SOUND_TTS
}YUAN_SOUND_MODE;

#define YUAN_SOUND_LEVEL_NORMAL		1
#define YUAN_SOUND_LEVEL_KEYPAD		2
#define YUAN_SOUND_LEVEL_EDOG		3
#define YUAN_SOUND_LEVEL_DIGTIAL	4
/////////////////////////////////////////////////////////////////////
// static function

#define SOUND_LIST_LEN		24
#define SOUND_FILENAME_LEN	128

typedef struct  
{
	uint16 Type;
	uint16 Level;
	char FileName[SOUND_FILENAME_LEN];
}SOUND_ITEM;

/////////////////////////////////////////////////////////////////////
// global function

//��ʼ������
//����:			��
//����ֵ:		��
void YuanHalSoundInit(void);

//�ر�����
//����:			��
//����ֵ:		��
void YuanHalSoundClose(void);

//�ر�����
//	Volume		������С(0~6)
//����ֵ:		��
void YuanHalSoundSetVolume(int Volume);


//�ر�����
//����:			��
//����ֵ:		
//	������С(0~6)	
int YuanHalSoundGetVolume(void);

//��������
//����:
//	Title			�����ļ��������б�
//	TitleLen		�����ļ��������б��ַ�����
//	NumTitle		�����ļ��������б���Ŀ
//	mode			YUAN_SOUND_WAV��PCM ��ʽ�� YUAN_SOUND_TTS: TTS����
//  level			�������ͻ����ȼ�
//����ֵ:			��
void YuanHalPlaySound(const char* Title, int Length, YUAN_SOUND_MODE mode,uint16 Level);

//��ѯ�����жϲ���
//����:				��
//����ֵ:			��
int YuanHalExecSound(void);

#ifdef __cplusplus
}
#endif

#endif