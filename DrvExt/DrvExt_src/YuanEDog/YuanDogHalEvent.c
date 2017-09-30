#include <stdlib.h>
#include <stdio.h>
#include "YuanDogHalEvent.h"
#include "YuanDogHalUtils.h"
#include "YuanDogHalLog.h"
#include "../../../Include/Common/type.h"
#include "../../../Include/Common/Kernel/Flag.h"
#include "../../../Include/Common/Kernel/consts.h"
//#include <Flag.h>

#define YUANHAL_EVENT_LEN		20

//#define UCTRL_YUANDOG_FLG_ID	1

//UINT32 UCTRL_YUANDOG_FLG_ID = 0;
extern unsigned long  FLG_ID_YUAN_DOG;

// Flag
#define FLGYUANDOG_IDLE          FLGPTN_BIT(0)
#define FLGYUANDOG_UPDATE        FLGPTN_BIT(1)

/////////////////////////////////////////////////////////////////////
// static variable
static YUANHAL_EVENT	YuanHalEventQueue[YUANHAL_EVENT_LEN]={0};
static int				YuanHalEventQueueRear=0,YuanHalEventQueueFront=0;

void YuanHalEventInit(void)
{
}
int YuanHalSetEvent(YUANHAL_EVENT *pEvent)
{
	if (((YuanHalEventQueueFront+1) % YUANHAL_EVENT_LEN) != YuanHalEventQueueRear)
	{
		YuanHalEventQueueFront = (YuanHalEventQueueFront+1) % YUANHAL_EVENT_LEN;

		memcpy(YuanHalEventQueue+YuanHalEventQueueFront,pEvent,sizeof(YUANHAL_EVENT));

		//HAL_REPLACE:Please add code here
		set_flg(FLG_ID_YUAN_DOG, FLGYUANDOG_UPDATE);
		//LOG_HAL_INFO("YuanHalSetEvent:%d\r\n",pEvent->EventCode);

		return 1;
	}
	return 0;
}
int YuanHalGetEvent(YUANHAL_EVENT *pEvent)
{
	if (YuanHalEventQueueFront != YuanHalEventQueueRear && pEvent)
	{
		YuanHalEventQueueRear = (YuanHalEventQueueRear+1) % YUANHAL_EVENT_LEN;

		memcpy(pEvent,YuanHalEventQueue+YuanHalEventQueueRear,sizeof(YUANHAL_EVENT));
		
		//LOG_HAL_INFO("YuanHalGetEvent:%d\r\n",pEvent->EventCode);

		//if (pEvent->EventCode != YUANHAL_EVENT_TICKET) LOG_HAL_INFO("YuanHalGetEvent:%d\r\n",pEvent->EventCode);
		return 1;
	}
	return 0;
}

int  YuanHalWaitForEvent(uint32 milliseconds)
{	
	FLGPTN  FlgPtn;  
	//uint32 dwWaitResult; 

	
    set_flg(FLG_ID_YUAN_DOG, FLGYUANDOG_IDLE);
    wai_flg(&FlgPtn, FLG_ID_YUAN_DOG, FLGYUANDOG_UPDATE, TWF_ORW|TWF_CLR);
    clr_flg(FLG_ID_YUAN_DOG, FLGYUANDOG_IDLE);
	
	//LOG_HAL_INFO("YuanHalWaitForEvent:%d\r\n",FlgPtn);

    if (FlgPtn & FLGYUANDOG_UPDATE)
    {
        return YUANHAL_EVENT_SET;
    }
    else 
    {
        return YUANHAL_EVENT_OTHER;
    }

}
