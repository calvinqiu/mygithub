
#include "PrjCfg.h"
#if (AUDIO_PLAY_FUNCTION == ENABLE)
#include "WavStudioTsk.h"
#include "NetAudQue.h"
#include "Utility.h"

#include "VControl.h" //for Ux_SendEvent()
#include "AppControl.h" //for NVTEVT_AUDIO_INPUT

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          NetAudQue
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

UINT32 g_AudioQueue_Addr = 0;
UINT32 g_AudioQueue_Size = 0;
UINT32 g_AudioQueue_Head = 0;
UINT32 g_AudioQueue_Len = 0;
BOOL g_AudioQueue_Feeded = FALSE;

static void NetAudQue_Lock(void)
{
    wai_sem(NAUDQUE_SEM_ID);
}
static void NetAudQue_Unlock(void)
{
    sig_sem(NAUDQUE_SEM_ID);
}

void NetAudQue_Start(UINT32 DevID, PWAVSTUD_AUD_INFO pCodec, UINT32 Addr, UINT32 Size)
{
    DBG_MSG("^CAudQue Start - begin\r\n");
    DBG_MSG("^CAudQue buffer addr =%08x, size = %08x\r\n", Addr, Size);
    g_AudioQueue_Addr = Addr;
    g_AudioQueue_Size = Size>>1; //unit: SampleCount
    g_AudioQueue_Head = 0;
    g_AudioQueue_Len = 0;
    g_AudioQueue_Feeded = FALSE;
    NetAudQue_Unlock();
    DBG_MSG("^CAudQue Start - end\r\n");
}

void NetAudQue_Stop(UINT32 DevID)
{
    DBG_MSG("^CAudQue Stop - begin\r\n");
    NetAudQue_Lock();
    g_AudioQueue_Addr = 0;
    g_AudioQueue_Size = 0; //unit: SampleCount
    g_AudioQueue_Head = 0;
    g_AudioQueue_Len = 0;
    g_AudioQueue_Feeded = FALSE;
    DBG_MSG("^CAudQue Stop - end\r\n");
}

BOOL NetAudQue_Push(UINT32 DevID, UINT32 Addr, UINT32 SampleCount)
{
    UINT32 head = 0;
    UINT32 len = 0;
    UINT32 tail = 0;
    UINT32 new_tail = 0;
    UINT32 pre_copy = 0;
    NetAudQue_Lock();
    head = g_AudioQueue_Head;
    len = g_AudioQueue_Len;
    //check buffer full?
    if((g_AudioQueue_Len + SampleCount)>g_AudioQueue_Size)
    {
        DBG_MSG("^CAudQue [%08x/%08x] Push %08x: ", g_AudioQueue_Len, g_AudioQueue_Size, (SampleCount<<1));
        DBG_MSG("^RAudQue full! refuse this push.\r\n");
        NetAudQue_Unlock();
        return FALSE;
    }
    tail=head+len;if(tail>=g_AudioQueue_Size)tail-=g_AudioQueue_Size;
    new_tail=tail+SampleCount;if(new_tail>=g_AudioQueue_Size){new_tail-=g_AudioQueue_Size;pre_copy=g_AudioQueue_Size-(tail);}
    if(pre_copy > 0)
    {
        UINT32 addr1,size1;
        UINT32 addr2,size2;
        addr1 = (g_AudioQueue_Addr+(tail<<1));
        size1 = (pre_copy<<1);
        addr2 = (g_AudioQueue_Addr+(0));
        size2 = (SampleCount<<1)-(pre_copy<<1);
        hwmem_open();
        hwmem_memcpy((g_AudioQueue_Addr+(tail<<1)), (Addr), (pre_copy<<1));
        hwmem_memcpy((g_AudioQueue_Addr+(0)), (Addr+(pre_copy<<1)), (SampleCount<<1)-(pre_copy<<1));
        hwmem_close();
        g_AudioQueue_Len += SampleCount;
        DBG_MSG("^CAudQue [%08x/%08x] Push %08x: %08x~%08x %08x~%08x\r\n", g_AudioQueue_Len, g_AudioQueue_Size, (SampleCount<<1), addr1, addr1+size1, addr2, addr2+size2);
    }
    else
    {
        UINT32 addr1,size1;
        addr1 = (g_AudioQueue_Addr+(tail<<1));
        size1 = (SampleCount<<1);
        hwmem_open();
        hwmem_memcpy((g_AudioQueue_Addr+(tail<<1)), (Addr), (SampleCount<<1));
        hwmem_close();
        g_AudioQueue_Len += SampleCount;
        DBG_MSG("^CAudQue [%08x/%08x] Push %08x: %08x~%08x\r\n", g_AudioQueue_Len, g_AudioQueue_Size, (SampleCount<<1), addr1, addr1+size1);
    }
    NetAudQue_Unlock();
    return TRUE;
}

BOOL NetAudQue_PullCB(PAUDIO_BUF_QUEUE pAudBufQue, UINT32 id)
{
    UINT32 Addr = 0;
    UINT32 SampleCount = 0;
    UINT32 head = 0;
    UINT32 len = 0;
    UINT32 tail = 0;
    UINT32 new_head = 0;
    UINT32 pre_copy = 0;
    NetAudQue_Lock();
    Addr = pAudBufQue->uiAddress;
    SampleCount = (pAudBufQue->uiSize>>1);
    head = g_AudioQueue_Head;
    len = g_AudioQueue_Len;
    //DBG_MSG("^GAudPlay CB - addr=0x%x,size=0x%x,validsize=0x%x\r\n", pAudBufQue->uiAddress, pAudBufQue->uiSize, pAudBufQue->uiValidSize);
    //check buffer empty?
    if((g_AudioQueue_Len)<SampleCount)
    {
        //DBG_ERR("almost empty! ignore for this pull.\r\n");
        //WavStudio_Unlock();
        //return FALSE;
        DBG_MSG("^GAudQue [%08x/%08x] Pull %08x: ", g_AudioQueue_Len, g_AudioQueue_Size, (SampleCount<<1));
        DBG_MSG("^YAudQue empty! fill zero for pull.\r\n");
        hwmem_open();
        hwmem_memset((Addr), 0, (SampleCount<<1));
        hwmem_close();
        if(g_AudioQueue_Feeded)
        {
            g_AudioQueue_Feeded = FALSE;
            Ux_SendEvent(0, NVTEVT_AUDIO_INPUT, 1, g_AudioQueue_Feeded); //notify: audio input = off
        }
        NetAudQue_Unlock();
        return TRUE;
    }
    else
    {
        if(!g_AudioQueue_Feeded)
        {
            g_AudioQueue_Feeded = TRUE;
            Ux_SendEvent(0, NVTEVT_AUDIO_INPUT, 1, g_AudioQueue_Feeded); //notify: audio input = on
        }
    }
    tail=head+len;if(tail>=g_AudioQueue_Size)tail-=g_AudioQueue_Size;
    new_head=head+SampleCount;if(new_head>=g_AudioQueue_Size){new_head-=g_AudioQueue_Size;pre_copy=g_AudioQueue_Size-(head);}
    if(pre_copy > 0)
    {
        UINT32 addr1,size1;
        UINT32 addr2,size2;
        addr1 = (g_AudioQueue_Addr+(head<<1));
        size1 = (pre_copy<<1);
        addr2 = (g_AudioQueue_Addr+(0));
        size2 = (SampleCount<<1)-(pre_copy<<1);
        hwmem_open();
        hwmem_memcpy((Addr), (g_AudioQueue_Addr+(head<<1)), (pre_copy<<1));
        hwmem_memcpy((Addr+(pre_copy<<1)), (g_AudioQueue_Addr+(0)), (SampleCount<<1)-(pre_copy<<1));
        hwmem_close();
        g_AudioQueue_Head = new_head;
        g_AudioQueue_Len -= SampleCount;
        DBG_MSG("^GAudQue [%08x/%08x] Pull %08x: %08x~%08x %08x~%08x\r\n", g_AudioQueue_Len, g_AudioQueue_Size, (SampleCount<<1), addr1, addr1+size1, addr2, addr2+size2);
    }
    else
    {
        UINT32 addr1,size1;
        addr1 = (g_AudioQueue_Addr+(head<<1));
        size1 = (SampleCount<<1);
        hwmem_open();
        hwmem_memcpy((Addr), (g_AudioQueue_Addr+(head<<1)), (SampleCount<<1));
        hwmem_close();
        g_AudioQueue_Head = new_head;
        g_AudioQueue_Len -= SampleCount;
        DBG_MSG("^GAudQue [%08x/%08x] Pull %08x: %08x~%08x\r\n", g_AudioQueue_Len, g_AudioQueue_Size, (SampleCount<<1), addr1, addr1+size1);
    }
    NetAudQue_Unlock();
    return TRUE;
}

#endif

