#include "YuanDogHalSound.h"
#include "YuanDogHalUtils.h"
#include "YuanDogHalLog.h"
#include <stdio.h>
#include <stdlib.h>

#include "Timer.h"
#include "ErrorNo.h"
#include "Audio.h"
#include "../../../Include/Lib/DevAud/GxSound/GxSound.h"
//#include "../../../Include/Lib/DevStrg/FileSys/FileSysTsk.h"
//#include "../../../Project/DemoKit/SrcCode/UIWnd/SPORTCAM/Resource/SoundData.h"
//#include "../../../Project/DemoKit/SrcCode/Kernel/syscfg.h"
#include "../../../Include/App/AppControl/AppControl.h"


static SOUND_ITEM s_SouldList[SOUND_LIST_LEN];
static int s_SouldListFront = 0;
static int s_SouldListRear = 0;


static unsigned long guiYuanEdogSoundTimerID = 0;

//HAL_REPLACE: Please add code here
static int sound_critial = 0;

#define YUAN_SOUND_INIT_CRITICAL()

#define YUAN_SOUND_ENTER_CRITICAL()	do {	\
										/*LOG_HAL_INFO("enter critical%d\r\n",sound_critial);*/	\
										while(sound_critial){LOG_HAL_INFO("critical waiting%d\r\n",sound_critial);};	\
										sound_critial++;			\
									}while(0);
#define YUAN_SOUND_EXIT_CRITICAL()	do {	\
										/*LOG_HAL_INFO("exit critical%d\r\n",sound_critial);	*/\
										if(sound_critial) sound_critial--;\
									}while(0);

typedef enum
{
	YUAN_HAL_SOUND_PLAY=1,
	YUAN_HAL_SOUND_STOP,
	YUAN_HAL_SOUND_PAUSE,
}YUAN_HAL_SOUND_STATUS;

////////////////////////////////////////////////////////////////
// static function for sound list queue.
static int SoundListIsEmpty(void);
static int SoundListIsFull(void);
static int SoundListPopRear(SOUND_ITEM *SoundItem);
static int SoundListPushFront(SOUND_ITEM *SoundItem);
static int SoundListPushRear(SOUND_ITEM *SoundItem);

static int YuanOSGetStatus();
static int YuanOSEnableSpeaker(int Enable);

////////////////////////////////////////////////////////////////
// static function for sound list queue.
static int SoundListIsEmpty(void)
{
	return (s_SouldListFront == s_SouldListRear);
}
static int SoundListIsFull(void)
{
	return (((s_SouldListFront + 1) % SOUND_LIST_LEN) == s_SouldListRear);
}
static int SoundListPopRear(SOUND_ITEM *SoundItem)
{
	if (SoundListIsEmpty())	return 0;
	
	YUAN_SOUND_ENTER_CRITICAL();
	s_SouldListRear = (s_SouldListRear + 1) % SOUND_LIST_LEN;
	if (SoundItem)	memcpy(SoundItem,s_SouldList+s_SouldListRear,sizeof(SOUND_ITEM));
	YUAN_SOUND_EXIT_CRITICAL();

	LOG_HAL_INFO("SoundListPopRear(%d,%d,%s)\r\n",s_SouldListRear,s_SouldListFront,SoundItem->FileName);

	return 1;
}
static int SoundListPushFront(SOUND_ITEM *SoundItem)
{
	if (SoundListIsFull())	return 0;

	YUAN_SOUND_ENTER_CRITICAL();
	s_SouldListFront = (s_SouldListFront + 1) % SOUND_LIST_LEN;
	memcpy(s_SouldList+s_SouldListFront,SoundItem,sizeof(SOUND_ITEM));
	YUAN_SOUND_EXIT_CRITICAL();

	LOG_HAL_INFO("SoundListPushFront(%d,%d,%s)\r\n",s_SouldListRear,s_SouldListFront,SoundItem->FileName);

	return 1;
}
static void SoundListRemovedKeypad()
{
	int i;

	for (i=0; i<SOUND_LIST_LEN; i++)
	{
		if (s_SouldList[i].Level == YUAN_SOUND_LEVEL_KEYPAD)
		{
			memset(s_SouldList[i].FileName,0,sizeof(s_SouldList[i].FileName));
		}
	}
}
static int SoundListPushRear(SOUND_ITEM *SoundItem)
{
	if (SoundListIsFull())	SoundListPopRear(NULL);

	YUAN_SOUND_ENTER_CRITICAL();
	if (SoundItem->Level == YUAN_SOUND_LEVEL_KEYPAD)	SoundListRemovedKeypad();
	memcpy(s_SouldList+s_SouldListRear,SoundItem,sizeof(SOUND_ITEM));
	s_SouldListRear = (s_SouldListRear + SOUND_LIST_LEN - 1) % SOUND_LIST_LEN;
	YUAN_SOUND_EXIT_CRITICAL();

	LOG_HAL_INFO("SoundListPushRear(%d,%d,%s)\r\n",s_SouldListRear,s_SouldListFront,SoundItem->FileName);
	return 1;
}
static int SoundListPush(const char* Title, int Length, uint16 Level)
{
	char *Pos = (char*)Title;
	SOUND_ITEM SoundItem;
	
	SoundItem.Type = YUAN_SOUND_WAV;
	SoundItem.Level = Level;

	if (Level == YUAN_SOUND_LEVEL_KEYPAD)
	{
		strcpy(SoundItem.FileName,Title);
		SoundListPushRear(&SoundItem);
	}
	
	while (*Pos && ((Pos - Title) < Length))
	{
		strcpy(SoundItem.FileName,Pos);

		//LOG_HAL_INFO("SoundListPush:%s\r\n",Pos);
		if (Level == YUAN_SOUND_LEVEL_NORMAL)			SoundListPushFront(&SoundItem);
		else if (Level == YUAN_SOUND_LEVEL_EDOG)		SoundListPushFront(&SoundItem);
		else											SoundListPushFront(&SoundItem);

		Pos += (strlen(Pos) + 1);
	}

}


void YuanSoundTimerHdl()
{
	//add code to polling sound status;
	//LOG_HAL_INFO("YuanSoundTimerHdl:%d\r\n",YuanOSGetStatus());
	YuanHalExecSound();
}

static int YuanOpenSoundTime()
{
	LOG_HAL_INFO("YuanOpenSoundTime:%d\r\n",guiYuanEdogSoundTimerID);
	if (guiYuanEdogSoundTimerID == 0)
	{
		if (timer_open((UINT *)&guiYuanEdogSoundTimerID, YuanSoundTimerHdl) != E_OK)
		{
		  
		}
		timer_cfg(guiYuanEdogSoundTimerID, 20*1000, TIMER_MODE_FREE_RUN | TIMER_MODE_ENABLE_INT, TIMER_STATE_PLAY);  
	}
	return 1;
}
static int YuanCloseSoundTime()
{
	LOG_HAL_INFO("YuanCloseSoundTime:%d\r\n",guiYuanEdogSoundTimerID);
	if (timer_close((UINT)guiYuanEdogSoundTimerID) != E_OK)
	{	  
		guiYuanEdogSoundTimerID = 0;
	}
	return 1;
}

static int YuanOSGetStatus()
{
	//HAL_REPLACE:Please add code here
	if(GxSound_IsPlaying()==TRUE)
	{
		return YUAN_HAL_SOUND_PLAY;
	}
	else
	{
		return YUAN_HAL_SOUND_STOP;
	}

}

static int YuanOSEnableSpeaker(int Enable)
{
	//HAL_REPLACE:Please add code here
}

///////////////////////////////////////////////////////////////
// Sound Thread
static int		s_SoundVolume = 3;


static void YuanHalPlaySoundList(SOUND_ITEM *pSoundItem);

////////////////////////////////////////////////////////////////
// global function
void YuanHalSoundInit(void)
{
	//YuanHalSoundSetVolume(6);
}
void YuanHalSoundClose(void)
{
}

void YuanHalSoundSetVolume(int Volume)
{
	s_SoundVolume = Volume;
	//HAL_REPLACE:Please add code here
}
int YuanHalSoundGetVolume(void)
{
	return s_SoundVolume;
}

int YuanHalExecSound(void)
{
	static char strTitle[32];
	SOUND_ITEM SoundItem;
    int ret;
		
	if (YUAN_HAL_SOUND_PLAY == YuanOSGetStatus())
	{
        //LOG_HAL_INFO("YuanHalExecSound:GxSound_IsPlaying playing\r\n");
		return 1;
	}
	if (TRUE == UISound_IsPlaying())
	{
		//LOG_HAL_INFO("YuanHalExecSound:UISound_IsPlaying playing\r\n");
		return 1;
	}	
	ret = SoundListPopRear(&SoundItem);

	if (ret)
	{
		YuanOSEnableSpeaker(1);
		//YuanHalSoundSetVolume(6);
		sprintf(strTitle,"B:\\data\\wav\\%s.WAV",SoundItem.FileName);
		//LOG_HAL_INFO("YuanHalExecSound:%d,%s\r\n",ret,strTitle);		
		UISound_SetPlaying(TRUE);
		Ux_PostEvent(NVTEVT_SOUND_EDOG_CB, 1, strTitle);
	}
	else
	{
		YuanOSEnableSpeaker(0);
	}
}

void YuanHalPlaySound(const char* Title, int Length, YUAN_SOUND_MODE mode,uint16 Level)
{
	if (mode == YUAN_SOUND_WAV)
	{
		//memcpy(SoudItem.FileName,Title,YUAN_MIN(Length,sizeof(SoudItem.FileName)));					
		
		SoundListPush(Title, Length, Level);
	}	
	//YuanOpenSoundTime();

	//YuanHalPlaySoundList(&SoudItem);
}
