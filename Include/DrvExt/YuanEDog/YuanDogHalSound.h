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

//初始化声音
//参数:			无
//返回值:		无
void YuanHalSoundInit(void);

//关闭声音
//参数:			无
//返回值:		无
void YuanHalSoundClose(void);

//关闭声音
//	Volume		音量大小(0~6)
//返回值:		无
void YuanHalSoundSetVolume(int Volume);


//关闭声音
//参数:			无
//返回值:		
//	音量大小(0~6)	
int YuanHalSoundGetVolume(void);

//声音播放
//参数:
//	Title			播放文件或内容列表
//	TitleLen		播放文件或内容列表字符长度
//	NumTitle		播放文件或内容列表数目
//	mode			YUAN_SOUND_WAV：PCM 格式； YUAN_SOUND_TTS: TTS播报
//  level			播放类型或优先级
//返回值:			无
void YuanHalPlaySound(const char* Title, int Length, YUAN_SOUND_MODE mode,uint16 Level);

//查询方法判断播放
//参数:				无
//返回值:			无
int YuanHalExecSound(void);

#ifdef __cplusplus
}
#endif

#endif