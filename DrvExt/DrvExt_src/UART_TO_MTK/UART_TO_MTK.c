/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPS.c
    @ingroup

    @brief      Excute command via UART interface.
                Excute command and output message via UART interface.

    @note       Nothing.

    @version    V1.00.000
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Debug.h"
#include "SysKer.h"
#include "kernel.h"
#include "uart.h"
#include "UART_TO_MTK.h"
#include "../../../Project/DemoKit/SrcCode/NVTUserCommand.h"
/* add begin by ZMD, 2016-08-24, 原因: */
#include "../../../Project/DemoKit/SrcCode/PrjCfg_Comm_Head.h"/*ZMD*/
#include "IOCfg.h"
/* add end by ZMD, 2016-08-24 */
#include "DxCommon.h"/*ZMD*/
#include "DxDisplay.h"/*ZMD*/
//#include "../../../Project/DemoKit/SrcCode/System/SysCommon.h"
#include "../../../Project/DemoKit/SrcCode/System/SysMain.h"
//@{
#define THIS_DBGLVL         1       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          UART_TO_MTK
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[buf]"
#include "DebugModule.h"
//#include "Perf.h"



#if 1

#if ( _UART_TO_MTK_RECEIVE_FROM_ ==  RECEIVE_FROM_UART3)
#define MTK_UART_OPEN   		uart3_open
#define MTK_UART_CLOSE   		uart3_close
#define MTK_UART_IS_OPEN   		uart3_isOpened
#define MTK_UART_PUT_CHAR   	uart3_putChar
#define MTK_UART_GET_CHAR   	uart3_getChar
#define MTK_UART_PUT_STRING   	uart3_putString
#define MTK_UART_GET_STRING   	uart3_getString
#define MTK_UART_SET_CONFIG   	uart3_setConfig
#define MTK_UART_WRITE   		uart3_write
#define MTK_UART_READ   		uart3_read
#define MTK_UART_INIT  			uart3_init
#endif

typedef unsigned long long  U64;     ///< Unsigned 64 bits data type
typedef signed long long    S64;      ///< Signed 64 bits data type
typedef unsigned long       U32;     ///< Unsigned 32 bits data type
typedef signed long         S32;      ///< Signed 32 bits data type
typedef unsigned short      U16;     ///< Unsigned 16 bits data type
typedef signed short        S16;      ///< Signed 16 bits data type
typedef unsigned char       U8;      ///< Unsigned 8 bits data type
typedef signed char         S8;       ///< Signed 8 bits data type



#define MTK_MESSAGE_POOL_SIZE 30

#define DISP_DEV_WIDTH     864
#define DISP_DEV_HIGH     480

#define DISP_UI_WIDTH     864//768
#define DISP_UI_HIGH     480//240

typedef struct _MTK_663_FRAME_STRUCT_
{
    U8 is_used;
    U8 cmd;
    U8 seq;
    U8 ver;
    U8 data_len;
    U8 *data;
} MTK_663_FRAME_STRUCT;



typedef struct _MTK_GPS_INFO_STRUCT_
{
    U32 lat;
    U32 lng;
    U8 speed;
    U8 N_S;//
    U8 W_E;//
} MTK_GPS_INFO_STRUCT;

#define TRANSMIT_OVERTIME     2000 // 2000~30000   //UNIT ms
MTK_GPS_INFO_STRUCT g_mtk_gps= {0};
UINT8 is_xomode_working = 0;

//MTK_663_FRAME_STRUCT  MTK_node_pool[MTK_MESSAGE_POOL_SIZE]= {0};
//#define MTK_MALLOC(size)
//#define MTK_FREE(buff)
//extern UINT32 POOL_ID_APP;
UINT8 guiMTKMenuSetting[MENU_SETTING_MAX];//={3,1,1,0,1,0,0};
BOOL gbMTKHeatBeatEn=TRUE;
static UINT32 guiMTKSpeed;

//BOOL gbReverseGearDetFlag = FALSE;
static int gbBackSensorMode = 0;//0:Normal ,  1: ReverseGear,  2:  BackMirror
BOOL gbLockTPEvent=FALSE;
#define TOUCH_INTERVAL_TIME 1000
#if(FW_VERSION_NUM_FUNC==ENABLE)
BOOL FwVersion_Seting_state=FWVERSION_SETING_STATE_OFF;
#endif


#if defined(YQCONFIG_PARKING_MODE_SUPPORT)
//pgl 20161201 modify,  if power on by gSensor(ParkingMinitor),  don't send any messages except for Gensor Message
extern BOOL ParkingCaptrueFlag;
#endif

void MTKComposeCMDRspFrame(UINT8 frame_id, UINT8 cmd,UINT8 * data, UINT8 data_len)
{

    UINT8 data_sum =0;
    UINT8 check_sum =0;
    UINT32 j;

    if(is_xomode_working!=0)
    {
        return;
    }

#if defined(YQCONFIG_PARKING_MODE_SUPPORT)
//pgl 20161201 modify,  if power on by gSensor(ParkingMinitor),  don't send any messages except for Gensor Message
    if(ParkingCaptrueFlag&&cmd!=CMD_GSENSOR_TRIG)
    {
        return;
    }
#endif
    data_sum = 0;
    for(j = 0; j<data_len; j++)
    {
        data_sum+=(U8)(data[j]);
        //debug_msg("MTKComposeCMDRspFrame, data:%x..\r\n", data[j]);
    }

    check_sum = (U8)(frame_id + VERSION_CHAR + cmd + data_len + data_sum);

    //debug_msg("MTKComposeCMDRspFrame:%d----cmd=%x,  check_sum=%x\r\n", frame_id,cmd, check_sum);

    MTK_UART_PUT_CHAR(START_CHAR1);
    MTK_UART_PUT_CHAR(START_CHAR2);
    MTK_UART_PUT_CHAR(frame_id);
    MTK_UART_PUT_CHAR(VERSION_CHAR);
    MTK_UART_PUT_CHAR(cmd);
    MTK_UART_PUT_CHAR((CHAR)data_len);
    MTK_UART_WRITE((UINT8*)data,(U8)data_len);
    MTK_UART_PUT_CHAR(check_sum);
    MTK_UART_PUT_CHAR(STOP_CHAR);

}

#endif


#define PRI_UART_TO_MTKRECEIVE            10
#define STKSIZE_UART_TO_MTKRECEIVE        6144//64KB stack size

UINT32  UART_TO_MTKRECEIVE_ID = 0;
UINT32  FLG_ID_UART_TO_MTK = 0;

// global variables for parsing NMEA code
//CHAR    UART_TO_MTKtype[6], UART_TO_MTKRemainStr[NMEA_SENTENCE_SIZE], UART_TO_MTKPostPtr[NMEA_SENTENCE_SIZE + 20], *UART_TO_MTKpPostPrt;
//UINT32  UART_TO_MTKPostPtrLen;
//UINT32  UART_TO_MTKCheckSum;

//BOOL Uart_UpKey_Flg 	= FALSE;
//BOOL Uart_DownKey_Flg 	= FALSE;
//BOOL Uart_ModeKey_Flg 	= FALSE;
//BOOL Uart_MenuKey_Flg 	= FALSE;
//BOOL Uart_OKKey_Flg 	= FALSE;



extern void UART_TO_MTKRecTsk(void);

//void UART_TO_MTKRec_TransformData(CHAR *NMEA_str, UINT32 Len, CHAR *pPostPrt, UINT32 *PostPtrLen);
//BOOL UART_TO_MTKRec_CheckSum(CHAR *NMEA_str, UINT32 Len, UINT32 CheckSum);
//BOOL UART_TO_MTKRec_GetUTCDate(UINT32 *Year, UINT32 *Month, UINT32 *Day);
//BOOL UART_TO_MTKRec_GetUTCTime(UINT32 *Hour, UINT32 *Minute, UINT32 *Second);
BOOL XmodemCheckBackSensorBackMirrorDet(void);

#define MTK_START_CHAR1 0xFF
#define MTK_START_CHAR2 0xAA


/* Xmodem control */
#define SOH 0x01
#define EOT 0x04  //sender stop transmit, then receiver response ACK 
#define ACK 0x06  //received success, pls send next frame
#define NAK 0x15  // need resend
#define CAN 0x18 //receiver want to stop transmit,  he will send CAN to sender

#define BLOCK_SIZE 1024
#define FRAME_SIZE (BLOCK_SIZE+5)//128+5
//#define ONE_SECOND 60000
#define COPY_DATA_LEN 2048


typedef enum
{
    XMODEM_IDLE_MODE=0,
    XMODEM_IDLE_TRANSMITING_MODE=1,
    XMODEM_IDLE_ENDING_REQUEST=2,

} XMODEM_WORKING_MODE;




void UART_TO_MTK_InstallID(void)
{
    OS_CONFIG_TASK(UART_TO_MTKRECEIVE_ID, PRI_UART_TO_MTKRECEIVE, STKSIZE_UART_TO_MTKRECEIVE, UART_TO_MTKRecTsk);
    OS_CONFIG_FLAG(FLG_ID_UART_TO_MTK);
    debug_msg("UART_TO_MTK_InstallID----\r\n");
}

static BOOL bUART_TO_MTKRecOpened = FALSE;
ER UART_TO_MTKRec_Open(UART_BAUDRATE BaudRate)
{
    if (bUART_TO_MTKRecOpened)
        return E_SYS;


    #if (_UART_TO_MTK_RECEIVE_FROM_ == RECEIVE_FROM_UART2)
    debug_msg("\r\nUART: use uart 2\r\n");
    #elif  (_UART_TO_MTK_RECEIVE_FROM_ == RECEIVE_FROM_UART3)
    debug_msg("\r\nUART: use uart 3\r\n");
    #endif


    if (MTK_UART_OPEN() == E_OK)
    {
        DBG_DUMP("\r\nUART: Open success\r\n");
        MTK_UART_INIT(BaudRate, UART_LEN_L8_S1, UART_PARITY_NONE);
    }
    else
    {
        DBG_ERR("UART: open fail!\r\n");
        return E_SYS;
    }


    DBG_DUMP("Start UART_TO_MTK receive task\r\n");
    sta_tsk(UART_TO_MTKRECEIVE_ID,0);

    loc_cpu();
    bUART_TO_MTKRecOpened = TRUE;
    unl_cpu();

    Perf_Open();//Jacky lan@20160922
    return E_OK;
}

ER UART_TO_MTKRec_Close(void)
{

    if (!bUART_TO_MTKRecOpened)
        return E_SYS;

    #if (_UART_TO_MTK_RECEIVE_FROM_ == RECEIVE_FROM_UART2)
    debug_msg("\r\nUART: close uart 2\r\n");
    #elif  (_UART_TO_MTK_RECEIVE_FROM_ == RECEIVE_FROM_UART3)
    debug_msg("\r\nUART: close uart 3\r\n");
    #endif

    #if (_UART_TO_MTK_RECEIVE_FROM_ == RECEIVE_FROM_UART2)
    if (MTK_UART_CLOSE() == E_OK)
        DBG_DUMP("UART: close success\r\n");
    else
        DBG_ERR("UART: close fail!\r\n");
    #endif

    DBG_DUMP("Terminate GPS receive task\r\n");

    ter_tsk(UART_TO_MTKRECEIVE_ID);

    loc_cpu();
    bUART_TO_MTKRecOpened = FALSE;
    unl_cpu();

    return E_OK;
}

void UART_TO_MTKRecTsk(void)
{
    UINT8 data_sum;
    UINT32  uiLen;
    UINT32  uiDVRLen = 0x01;
    UINT16  CheckSentenceType1, CheckSentenceType2;
    UINT32  CheckSentenceType3;
    CHAR    RecSentence[MTK_SENTENCE_SIZE];
    CHAR    RecSentence_read[MTK_SENTENCE_SIZE];
    CHAR    RecSentence_write[MTK_SENTENCE_SIZE] = {0xFF,0xAA};
    CHAR    DVRDataSet[MTK_SENTENCE_SIZE] = {0x00};
    CHAR    *pSentenceData;
    int    i, j, k;
    // BOOL isReverse = FALSE;
    static CHAR TouchStatus;
    CHAR    CmdChar;
    CHAR    FrameID;
    CHAR	VersionChar;
    CHAR    StopChar;
    UINT32	CheckSumChar;
    UINT32	CheckSum;

    UINT32    DVRCheckSum = 0x00;

    UINT32  DataTemp;

    static CHAR dataTemp;
    static UINT32 command;
    static UINT32 Step = 0;

    UINT32 Touch_X,Touch_Y;
    UINT32 X_Hi,X_Low,Y_Hi,Y_Low;
    UINT32 Touch_Position;

    CHAR   Touch_UP,Touch_Down;

    static UINT32 year,month,day;
    static UINT32 hour,min,second;
    static BOOL gbUpdateGpsTimeFlag=FALSE;
    U8 data = 0;
    U8 dataTmp[16];
    UINT32 capture_sensor_select;
    UINT32    currentTick=0;
    static UINT32 lastTick=0;
    static BOOL TPPressed=FALSE;
    kent_tsk();

    clr_flg(FLG_ID_UART_TO_MTK, UART_TO_MTK_FLAG_DATAVALID);

    while(1)
    {
        /* uiLen = NMEA_SENTENCE_SIZE;
          #if (_UART_TO_MTK_RECEIVE_FROM_ == RECEIVE_FROM_UART2)
          if (uart2_getString(RecSentence, &uiLen) == E_PAR)
          #elif (_UART_TO_MTK_RECEIVE_FROM_ == RECEIVE_FROM_UART3)
          if (uart3_getString(RecSentence, &uiLen) == E_PAR)
          #else
          if (uart_getString(RecSentence, &uiLen) == E_PAR)
          #endif*/

        Step = 0;
        CheckSum = 0;
        MTK_UART_GET_CHAR(RecSentence_read);
        //debug_msg("is_xmomdem--%d,     read[0]=%x \r\n",is_xomode_working,  RecSentence_read[0] );
        if(is_xomode_working!=0)
        {
            if(RecSentence_read[0] == ACK)
            {
                if(is_xomode_working==1)
                {
                    XmodemStartSendNextFrameToMTK();
                }

                if(is_xomode_working ==2)
                {
                    is_xomode_working = 0;
                    //transmit file finis..
                    Ux_PostEvent(NVTEVT_MTK_TRANSMIT_FINISH, 0);
                }
            }

            if(RecSentence_read[0] == NAK)
            {
                XmodemStartResendFrameToMTK();
            }


            if(RecSentence_read[0] == CAN)
            {
                is_xomode_working = 0;
                XmodemResetSendToMTK();
            }


        }
        else
        {
            if(*RecSentence_read == MTK_START_CHAR1)
            {
                Step = 1;
            }
            if(Step == 1)
            {
                MTK_UART_GET_CHAR(RecSentence_read);
                if(*RecSentence_read == MTK_START_CHAR2)
                {
                    MTK_UART_GET_CHAR(&dataTemp);
                    FrameID = dataTemp;
                    Step = 2;
                }
            }
            if(Step == 2)
            {
                MTK_UART_GET_CHAR(&dataTemp);
                VersionChar = dataTemp;
                Step = 3;
            }
            if(Step == 3)
            {
                MTK_UART_GET_CHAR(&dataTemp);
                CmdChar = dataTemp;
                Step = 4;
            }
            if(Step == 4)
            {
                MTK_UART_GET_CHAR(&dataTemp);
                uiLen = dataTemp;
                Step = 5;
            }
            if(Step == 5)
            {
                MTK_UART_READ(RecSentence_read,uiLen+2);
                CheckSumChar = RecSentence_read[uiLen];
                StopChar = RecSentence_read[uiLen+1];
                Step = 6;
            }
            if(Step == 6)
            {
                for(i=0; i < uiLen; i++)
                {
                    CheckSum += RecSentence_read[i];
                }
                CheckSum += (FrameID + VersionChar + CmdChar + uiLen);
                CheckSum &= 0xff;

//                debug_msg("FrameID = %x\r\n",FrameID);
//                debug_msg("VersionChar = %x\r\n",VersionChar);
//                debug_msg("CmdChar = %x\r\n",CmdChar);
//                debug_msg("uiLen = %x\r\n",uiLen);
//                debug_msg("CheckSumChar = %x\r\n",CheckSumChar);
//                debug_msg("StopChar = %x\r\n",StopChar);

                if((CheckSum == CheckSumChar) && (StopChar == STOP_CHAR))
                {
                    //debug_msg("RX Data OK   cmd=%02x \r\n",  CmdChar);
                    switch(CmdChar)
                    {
                    case CMD_TOUCH:
                       // debug_msg("RX Data OK   cmd=%02x \r\n",  CmdChar);
                        TouchStatus = RecSentence_read[0];
                        #if defined(YQCONFIG_TOUCH_TO_MTK_PLAYSOUND_OPTION)
                        {
                            if(TouchStatus==1)//Pressed status
                            {                                
                                currentTick= Perf_GetCurrent();
                            }
                        }
                        #else
                        {
                            if(TouchStatus==0)//Pressed status
                            {
                                currentTick= Perf_GetCurrent();
                            }
                        }
                        #endif
                        X_Hi=RecSentence_read[1];
                        X_Low=RecSentence_read[2];
                        Y_Hi=RecSentence_read[3];
                        Y_Low=RecSentence_read[4];
                        Touch_X=((X_Hi<<8)|X_Low)*DISP_UI_WIDTH/DISP_DEV_WIDTH;
                        Touch_Y=((Y_Hi<<8)|Y_Low)*DISP_UI_HIGH/DISP_DEV_HIGH;

                        Touch_Position=((Touch_X&0xFFFF)<<16)|Touch_Y;

#if 0
                        Touch panel: 34603196 (528,188), Fram ID=18,TouchStatus=1----0--0--0
 Touch panel 3
 [0-0=0]Touch panel: 34603196 (528,188), Fram ID=18,TouchStatus=1,gbLockTPEvent=0
 #endif
                         debug_msg("Touch panel: %d (%d,%d), Fram ID=%d,TouchStatus=%d----%d--%d--%d\r\n ",Touch_Position,Touch_X,Touch_Y,FrameID,TouchStatus,\
                                   XmodemGetMTKReverseGearDet(),XmodemCheckBackSensorBackMirrorDet(),gbLockTPEvent);
                        if(XmodemGetMTKReverseGearDet())//gbReverseGearDetFlag == FALSE)/*倒车模式*/
                        {
                            // debug_msg("Touch panel 1\r\n ");
                            //if is reverse mode, forbid touch event
                            //Ux_PostEvent(NVTEVT_TOUCHPANEL_KEY, 3,TouchStatus,Touch_Position,FrameID);
                        }
                        else if(XmodemCheckBackSensorBackMirrorDet())/*屏保模式退出*/
                        {
                            // debug_msg("Touch panel 2\r\n ");
                            if(TouchStatus==1)/*触摸2次，可能有问题*/
                            {
                                UINT8 uiResqData[16]= {0};
                                XmodemSetBackSensorMode(SENSORMODE_NONE);//后摄正常模式
                                uiResqData[0]=0x00;
                                MTKComposeCMDRspFrame(FrameID, CMD_TOUCH,(UINT8 *)&uiResqData, 1);
                            }
                        }
                        else
                        {
                            debug_msg("Touch panel 3\r\n ");
                            debug_msg("[%d-%d=%d]Touch panel: %d (%d,%d), \r\nFram ID=%d,TouchStatus=%d,gbLockTPEvent=%d\r\n ",currentTick,lastTick,(currentTick-lastTick)/1000,Touch_Position,Touch_X,Touch_Y,FrameID,TouchStatus,gbLockTPEvent);
                            if(gbLockTPEvent==FALSE)
                            {
                                 CHKPNT;
                                #if defined(YQCONFIG_TOUCH_TO_MTK_PLAYSOUND_OPTION)
                                {         
                                 CHKPNT;                                    
                                    if((currentTick-lastTick)/1000>TOUCH_INTERVAL_TIME)//
                                    {
                                       CHKPNT;
                                        lastTick=currentTick;
                                        CHKPNT;
                                        // TPPressed=TRUE;
                                        Ux_PostEvent(NVTEVT_TOUCHPANEL_KEY, 3,TouchStatus,Touch_Position,FrameID);
                                    }
                                    else
                                    {
                                 CHKPNT;                                        
                                        debug_msg("Ingore touch event..\r\n");
                                    }
                                }
                                #else
                                {
                                 CHKPNT;                                    
                                    if(TouchStatus==0)//Pressed status
                                    {
                                 CHKPNT;                                        
                                        if((currentTick-lastTick)/1000>TOUCH_INTERVAL_TIME)//
                                        {
                                 CHKPNT;                                            
                                            lastTick=currentTick;
                                            CHKPNT;
                                            TPPressed=TRUE;
                                            Ux_PostEvent(NVTEVT_TOUCHPANEL_KEY, 3,TouchStatus,Touch_Position,FrameID);
                                        }
                                        else
                                        {
                                 CHKPNT;                                            
                                            debug_msg("Ingore touch event..\r\n");
                                        }
                                    }
                                    else// released status
                                    {
                                 CHKPNT;                                        
                                        if(TPPressed==TRUE)
                                        {
                                            TPPressed=FALSE;
                                            CHKPNT;
                                            Ux_PostEvent(NVTEVT_TOUCHPANEL_KEY, 3,TouchStatus,Touch_Position,FrameID);
                                        }
                                    }
                                }
                                #endif
                            }
                            else
                            {
                                debug_msg("TP Event is lock by project...\r\n");
                            }
                        }
                        break;

                    case CMD_TIME:

                        year=RecSentence_read[0]+2000;
                        month=RecSentence_read[1];
                        day=RecSentence_read[2];
                        hour=RecSentence_read[3];
                        min=RecSentence_read[4];
                        second=RecSentence_read[5];
                        debug_msg("year=%d,   month=%d, day=%d, hour=%d,  min=%d,  second=%d \r\n", year, month, day, hour, min, second);
                        //if(gbUpdateGpsTimeFlag==FALSE)//pgl_modify
                        {
                            rtc_setDate(year, month, day);
                            rtc_setTime(hour, min, second);
                            rtc_triggerCSET();
                            //	gbUpdateGpsTimeFlag=TRUE;//pgl_modify
                        }
                        {
                            /*
                            result:  0--success， 1----fail
                            */
                            U8 data = 0;
                            MTKComposeCMDRspFrame(FrameID, CmdChar,&data, 1);
                        }


                        break;

                    case CMD_GPS:
                        //bit0: 0--north, 1--south
                        if(RecSentence_read[0]&0x01)
                        {
                            //south
                            g_mtk_gps.N_S = 'S';
                        }
                        else
                        {
                            //north
                            g_mtk_gps.N_S = 'N';
                        }

                        //bit1: 0--east,  1---west
                        if(RecSentence_read[0]&0x02)
                        {
                            //west
                            g_mtk_gps.W_E = 'W';
                        }
                        else
                        {
                            //east
                            g_mtk_gps.W_E = 'E';
                        }

                        DataTemp = (UINT32)RecSentence_read[1];
                        DataTemp |=(UINT32)(RecSentence_read[2]<<8);
                        DataTemp |=(UINT32)(RecSentence_read[3]<<16);
                        DataTemp |=(UINT32)(RecSentence_read[4]<<24);
                        g_mtk_gps.lng = DataTemp;


                        DataTemp = (UINT32)RecSentence_read[5];
                        DataTemp |=(UINT32)(RecSentence_read[6]<<8);
                        DataTemp |=(UINT32)(RecSentence_read[7]<<16);
                        DataTemp |=(UINT32)(RecSentence_read[8]<<24);
                        g_mtk_gps.lat = DataTemp;
                        g_mtk_gps.speed = RecSentence_read[9];

                        guiMTKSpeed=(UINT32)g_mtk_gps.speed;
                        //guiMTKSpeed=70;

                        //debug_msg("GPS info is  lat=%d,%c,  lng=%d,%c,  speed=%d,%d\r\n", g_mtk_gps.lat, g_mtk_gps.N_S, g_mtk_gps.lng, g_mtk_gps.W_E,g_mtk_gps.speed,guiMTKSpeed);

                        if(gbMTKHeatBeatEn==TRUE)
                        {
                            Ux_PostEvent(NVTEVT_MTK_GPSINFO, 1,FrameID);
                        }

                        break;

                    case CMD_SETTING:

                        /*
                        result:  0--success， 1----fail
                        */
                        //U8 data = 0;
                        //if(RecSentence_read[0]>=2)
                        //{
                        //	RecSentence_read[0]+=2;// menu option offset for NT96663 movie resolution
                        //}
                        memcpy(guiMTKMenuSetting, RecSentence_read, MENU_SETTING_MAX);

                        //debug_msg("-->CMD_SETTING: ");
                        //for(i=0;i<MENU_SETTING_MAX;i++)
                        //{
                        //	debug_msg(" %d, ",RecSentence_read[i]);
                        //}
                        //debug_msg("\r\n");

                        Ux_PostEvent(NVTEVT_MTK_MENU_SETTING, 1,FrameID);
                        break;

                    case CMD_ADAS_SWITCH:
                        CHKPNT;
                        Ux_PostEvent(NVTEVT_MTK_ADAS_SWITCH, 2,FrameID, RecSentence_read[0]);
                        break;
                    case CMD_MOVIE_START:
                        debug_msg("start to movie record..\r\n");
                        //data=0;
                        Ux_PostEvent(NVTEVT_MTK_STARTREC, 1,FrameID);
                        //MTKComposeCMDRspFrame(FrameID, CMD_MOVIE_START,&data, 1);
                        break;
                    case CMD_MOVIE_STOP:
                        debug_msg("stop to movie record..\r\n");
                        //data=0;
                        Ux_PostEvent(NVTEVT_MTK_STOPREC, 1,FrameID);
                        //MTKComposeCMDRspFrame(FrameID, CMD_MOVIE_STOP,&data, 1);
                        break;
                    case CMD_MOVIE_FORMAT:
                        debug_msg("format DVR ext TF card..\r\n");
                        //data=0;
                        //MTKComposeCMDRspFrame(FrameID, CMD_MOVIE_FORMAT,&data, 1);
                        Ux_PostEvent(NVTEVT_MTK_FORMATCARD, 1,FrameID);
                        break;
                    case CMD_FILE:
                        is_xomode_working = 1;
                        //XmodeStartSendFirstToMTK();
                        capture_sensor_select=RecSentence_read[0];
                        debug_msg("wechat capture the %d sensor ..\r\n",capture_sensor_select);

                        Ux_PostEvent(NVTEVT_KEY_CUSTOM2, 2,FrameID,capture_sensor_select);
                        break;
                    case CMD_DAOCHE:
                        debug_msg("reverse gear detecded..%d\r\n",RecSentence_read[0]);
                        #if defined(YQCONFIG_COMB_DET_BACKCAR_BY_GPIO)||defined(YQCONFIG_COMB_DET_BACKCAR_BY_UART)
                        //extern UINT8 MapLcdSwitchFlag;

                        // extern UINT32 System_GetConnectSensor(void);

                        if ( RecSentence_read[0] == 2)/*屏保*/
                        {
                            if((System_GetConnectSensor() & SENSOR_2)== SENSOR_2)
                            {
                                XmodemSetBackSensorMode(SENSORMODE_MIRROR);
                            }
                        }
                        /* modify begin by ZMD, 2017-02-15 new version management*/
                        #if defined(YQCONFIG_COMB_DET_BACKCAR_BY_GPIO)

                        #elif defined(YQCONFIG_COMB_DET_BACKCAR_BY_UART)
                        else if(RecSentence_read[0] == 1)/*倒车*/
                        {
                            if((System_GetConnectSensor() & SENSOR_2)== SENSOR_2)
                            {
                                XmodemSetBackSensorMode(SENSORMODE_REVERSE);
                            }
                        }
                        else /*正常模式*/
                        {
                            XmodemSetBackSensorMode(SENSORMODE_NONE);
                        }
                        #else
                        #endif
                        /* modify end by ZMD, 2017-02-15 */

                        #endif

                        break;
                    case CMD_CAPTURE:
                        Ux_PostEvent(NVTEVT_MTK_CAPTURE, 1,FrameID);
                        break;
                    case CMD_CHANGE_DIAPLAYMODE:
                        debug_msg("CHANGE_DIAPLAYMODE..%d\r\n",RecSentence_read[0]);
                        extern int PRIMARY_MODE_PLAYBACK;

                        if (System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_PLAYBACK)
                        {
                            data=1;
                            MTKComposeCMDRspFrame(FrameID, CMD_CHANGE_DIAPLAYMODE,&data, 1);
                        }
                        else  if(XmodemCheckBackSensorNoramlDet())
                        {
                            Ux_PostEvent(NVTEVT_MTK_CHANGE_DIAPLAYMODE, 1,RecSentence_read[0]);
                            data=0;
                            MTKComposeCMDRspFrame(FrameID, CMD_CHANGE_DIAPLAYMODE,&data, 1);
                        }
                        else if(XmodemGetMTKReverseGearDet())
                        {
                            data=2;
                            MTKComposeCMDRspFrame(FrameID, CMD_CHANGE_DIAPLAYMODE,&data, 1);
                        }
                        else if(XmodemCheckBackSensorBackMirrorDet())
                        {
                            data=3;
                            MTKComposeCMDRspFrame(FrameID, CMD_CHANGE_DIAPLAYMODE,&data, 1);
                        }
                        break;




                    case CMD_SWITCH_MODE:
                        debug_msg("CHANGE_MODE..%d\r\n",RecSentence_read[0]);
                        extern int PRIMARY_MODE_USBMSDC;
                        if (System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_USBMSDC)
                        {
                            Ux_PostEvent(NVTEVT_MTK_CHANGE_MODE, 1,RecSentence_read[0]);
                            data=0;
                            MTKComposeCMDRspFrame(FrameID, CMD_SWITCH_MODE,&data, 1);
                        }
                        else
                        {
                            data=1;
                            MTKComposeCMDRspFrame(FrameID, CMD_SWITCH_MODE,&data, 1);
                        }
                        break;
		//add by qiuhan on 20171030 for   The lens position calibration
		    case CMD_LENS_POS_CAL_ON:
			    debug_msg("=======CMD_LENS_POS_CAL_ON===========\r\n");//,RecSentence_read);
			    Ux_PostEvent(NVTEVT_MTK_LENS_POS_CAL_OPEN, 1, FrameID);
                         break;
						 
		    case CMD_LENS_POS_CAL_OFF:
			    debug_msg("=======CMD_LENS_POS_CAL_OFF===========\r\n");
			    Ux_PostEvent(NVTEVT_MTK_LENS_POS_CAL_CLOSE, 1, FrameID);
                         break;
		//add end
//add by qiuhan on 20171028 for U15 begin
		    case CMD_OTG_TO_MTK://add by qiuhan on 20170815
                        debug_msg("=======CMD_OTG_TO_MTK=========%s===\r\n",RecSentence_read);
			/*  if (System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_USBMSDC)
                        {
                            debug_msg("==============PRIMARY_MODE_USBMSDC=============\r\n");
				Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_USBMSDC);
                            data=1;
                            MTKComposeCMDRspFrame(FrameID, CMD_OTG_TO_MTK,&data, 1);
                        }*/
                        Ux_PostEvent(NVTEVT_MTK_OTG_TO_MTK, 1, FrameID);
			  // data=1;
                      //      MTKComposeCMDRspFrame(FrameID, CMD_OTG_TO_MTK,&data, 1);
			   
			   break;
		    case CMD_OTG_TO_NTK:
                        debug_msg("=======CMD_OTG_TO_NTK=========%s===\r\n",RecSentence_read);
		//	   extern int PRIMARY_MODE_USBMSDC;
                        if (System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_USBMSDC)
                        {
                            debug_msg("==============PRIMARY_MODE_USBMSDC=============\r\n");
				Ux_PostEvent(NVTEVT_MTK_CHANGE_MODE, 1,RecSentence_read[0]);
                            data=0;
                            MTKComposeCMDRspFrame(FrameID, CMD_OTG_TO_NTK,&data, 1);
                        }
			   break;
//add end
                    case CMD_SHARP_TURN_VEDIO:
				 debug_msg("======= CMD_SHARP_TURN_VEDIO========%s====\r\n",RecSentence_read);
				 Ux_PostEvent(NVTEVT_SHARP_TURN_VEDIO, 1, FrameID);
				 break;
		      case CMD_RUSH_VEDIO:
				 debug_msg("======= CMD_RUSH_VEDIO========%s====\r\n",RecSentence_read);
				 Ux_PostEvent(NVTEVT_RUSH_VEDIO, 1, FrameID);
				 break;
		      case CMD_NASTY_BRAKE_VEDIO:
				 debug_msg("======= CMD_NASTY_BRAKE_VEDIO========%s====\r\n",RecSentence_read);
				 Ux_PostEvent(NVTEVT_NASTY_BRAKE_VEDIO, 1, FrameID);
				 break;
                    case CMD_EMR_VEDIO:
				 debug_msg("=======CMD_EMR_VEDIO=========%s===\r\n",RecSentence_read);
				 Ux_PostEvent(NVTEVT_MTK_EMR_VIDEO, 1, FrameID);
				 break;
                        #if(FW_VERSION_NUM_FUNC==ENABLE)
                    case CMD_FWVERSION:
                        debug_msg("CMD_FWVERSION_STATE_OK\r\n");
                        if(FwVersion_Seting_state==FWVERSION_SETING_STATE_ON)
                        {
                            FwVersion_Seting_state=FWVERSION_SETING_STATE_OK;
                        }
                        break;
                        #endif

                    default:
                        break;
                    }
                }
            }
        }
    }
}




#if 1
////////////////////////////////////
//add x-modem function [start]
//2015-10-22
////////////////////////////////////

/*
#define  UINT32 U32
#define  UINT8 U8
#define  UINT64 U64
*/

#define LIANYONG_663

//refrence:  http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=22525389&id=160174

typedef struct _STRUCT_XMODEM_FRAME_
{
    unsigned char blkNum;
    unsigned char blkNumRev;
    unsigned char dataBlk[BLOCK_SIZE];
    unsigned char checkSum;
} STRUCT_XMODEM_FRAM;

typedef struct
{
    int u16PageNums;
    int u16NandPageCnt;
    unsigned char u8Data[COPY_DATA_LEN];
} STRUCT_COPY_DATA;

int XmodemWriteChar(char c)
{

    MTK_UART_PUT_CHAR(c);

//   return write(mTtyfd,ch,1);

}

int XmodemWriteData(unsigned char *data, int data_len)
{

    MTK_UART_WRITE((UINT8*)data,(UINT32)data_len);
    debug_msg("XmodemWriteData----len=%d\r\n", data_len);
//XmodemRun(data);
//    return write(mTtyfd,ch,1);

}

#if 0
//void XmodemRun(char * buf);
static unsigned char CheckSum(STRUCT_XMODEM_FRAM *oneFrame)
{
    unsigned char i;
    unsigned char dataSum = 0x00;
    for(i = 0; i < BLOCK_SIZE; ++i)
    {
        dataSum += oneFrame->dataBlk[i];
    }
    return (dataSum & 0xff);
}
static unsigned char RecFrame(STRUCT_XMODEM_FRAM* oneFrame,char *buf)
{
    unsigned char returnVal;
    unsigned char i;
    unsigned char data_len;
    oneFrame->blkNum = buf[1];
    oneFrame->blkNumRev = buf[2];
    data_len = buf[3];

    if( oneFrame->blkNumRev != 255 - (oneFrame->blkNum & 0xFF))
        returnVal = 1;
    else
    {
        for(i = 0; i < BLOCK_SIZE; ++i)
        {
            oneFrame->dataBlk[i] = buf[i+(FRAME_SIZE-BLOCK_SIZE-1)];
        }
        oneFrame->checkSum = buf[FRAME_SIZE-1];
        if(oneFrame->checkSum == CheckSum(oneFrame))
        {
            returnVal = 0;
            #ifdef LIANYONG_663
            debug_msg("xmodem rev data ok.\r\n");
            #else
            kal_prompt_trace(MOD_CC, "xmodem rev data ok.\r\n");
            #endif
        }
        else
            returnVal = 2;
    }
    return (returnVal);
}
#endif


//typdef byte  (unsigned char)

unsigned char frame_buff[FRAME_SIZE]= {0};

unsigned char* XmodemPackageFrame(unsigned char data[],unsigned data_len,  unsigned char curr)
{
    int data_sum = 0x00;
    int len;
    int i;

    frame_buff[0]=SOH;
    frame_buff[1]=curr;
    frame_buff[2]=(unsigned char)(255-(frame_buff[1]&0xFF));
    frame_buff[3]=(unsigned char)data_len;
    len = data_len;
    //memset((void*)data, 0,sizeof(data_len));
    memcpy((void*)(frame_buff+4), data,data_len);


    if(data_len<BLOCK_SIZE)
    {
        memset((void*)(frame_buff+data_len+4), 0x1A, (BLOCK_SIZE-data_len));
        /*
                    for(i = 0 ; i<(BLOCK_SIZE-data_len); i++){
                        frame_buff[data_len+4+i] = (unsigned char)0x1A;
                    }
        */

    }

    for(i= 0;  i<BLOCK_SIZE; ++i)
    {
        data_sum +=frame_buff[(FRAME_SIZE-BLOCK_SIZE-1)+i];
    }

    frame_buff[BLOCK_SIZE+(FRAME_SIZE-BLOCK_SIZE-1)] = (unsigned char)data_sum;

    return frame_buff;
}


////////////////////////////////////
//add x-modem function [end]
////////////////////////////////////
//Include\Lib\DevStrg\FileSys
//DrvExt\DrvExt_src\UART_TO_MTK


#include "../../../Include/Lib/DevStrg/FileSys/FileSysTsk.h"

UINT32 curr_file_to_mtk_frame =0xffff;
UINT32 total_frame_to_mtk =0;
char frame_buff_for_mtk[BLOCK_SIZE]= {0};
UINT32 photo_data_address;
UINT32 photo_data_size;

static char srcImgName[64];
//#define   PGL_FILE_NAME   "e:\\abc.jpg"
int XmodemGetFileTotalFrame(UINT32 Addr, UINT32 Size)//(char *pPath)
{
    UINT64 tmp = 0;
    UINT64 filesize;
    FST_FILE filehdl;

    //UINT8 filename[255] = {0};
    //FS_HANDLE fhandle = 0;
    UINT32 file_len;
    #if 0
    kal_wsprintf((unsigned short*)filename, PGL_FILE_NAME);

    fhandle = FS_Open((const WCHAR*)filename, FS_READ_WRITE);

    if(fhandle < 0)
    {
        FS_Close(fhandle);
        //open file error;
        return -1;
    }

    if (FS_NO_ERROR != FS_GetFileSize(fhandle, &filesize))
    {
        FS_Close(fhandle);

        //get file error
        return -2;

    }

    FS_Close(fhandle);

    #else
    #if 0
    strcpy(srcImgName, pPath);
    filehdl =  FileSys_OpenFile(srcImgName, FST_OPEN_READ);
    if(filehdl==NULL)
    {
        debug_msg("XmodemGetFileTotalFrame === - open file fail: %s\r\n",srcImgName);
        total_frame_to_mtk = 0;
        FileSys_CloseFile(filehdl);
        return total_frame_to_mtk;
    }

    if(FST_STA_OK == FileSys_SeekFile(filehdl, 0, FST_SEEK_END))
    {
        debug_msg("XmodemGetFileTotalFrame :  seek file success\r\n");
        file_len = FileSys_TellFile(filehdl);
    }
    FileSys_CloseFile(filehdl);
    if((file_len>1000*1024)||(file_len<=0))
    {
        debug_msg("XmodemGetFileTotalFrame :  file length is too large or error..%d\r\n",file_len);
        return 0;
    }
    debug_msg("XmodemGetFileTotalFrame Length: %d.\r\n",file_len);
    #else
    file_len=Size;
    if((file_len>1000*1024)||(file_len<=0))
    {
        debug_msg("XmodemGetFileTotalFrame :  file length is too large or error..%d\r\n",file_len);
        return 0;
    }
    #endif
    #endif
    tmp = file_len%((UINT64)BLOCK_SIZE);
    total_frame_to_mtk = (UINT32)(file_len/((UINT64)BLOCK_SIZE));
    //debug_msg("XmodemGetFileDataByIndex---tmp=%llu\r\n"+ tmp);

    if(tmp>((UINT64)0))
    {
        debug_msg("XmodemGetFileDataByIndex------\r\n");
        total_frame_to_mtk++;
    }

    //debug_msg("XmodemGetFileTotalFrame :  tell file_len = %llu,  totalFrame=%d\r\n",file_len, total_frame_to_mtk);

    return total_frame_to_mtk;

}

int XmodemGetFileDataByIndex(UINT8 * buff, UINT32 buff_size, UINT32 index)
{
    //char srcImgName[200]={0};
    UINT32 file_len;

    int tmp =0;
    UINT8 filename[255] = {0};
    UINT32 w_ret = 0;
    //memcpy(&save_qqt_setting , &g_qqt_msg_context , sizeof(g_qqt_msg_context));
    UINT32 start_pos, end_pos;

    #if 0

    FST_FILE fhandle = FileSys_OpenFile(srcImgName, FST_OPEN_READ);

    if(fhandle==NULL)
    {
        //debug_msg("XmodemGetFileDataByIndex  - open file fail: %s\r\n",srcImgName);
        total_frame_to_mtk = 0;
        FileSys_CloseFile(fhandle);
        return total_frame_to_mtk;
    }


    if(FST_STA_OK == FileSys_SeekFile(fhandle, 0, FST_SEEK_END))
    {
        FileSys_SeekFile(fhandle, 0, FST_SEEK_END);
    }


    file_len = FileSys_TellFile(fhandle);

    //file_len = 109;//FileSys_GetFileLen(srcImgName);


    //kal_prompt_trace(MOD_CC, "FileLens = %d\r\n",FileLens);

    /*
    	tmp = file_len%BLOCK_SIZE;

    	total_frame_to_mtk = file_len/BLOCK_SIZE;

    	if(tmp>0)
    	{
    		total_frame_to_mtk++;
    	}
    */


    start_pos = curr_file_to_mtk_frame*BLOCK_SIZE;
    end_pos = (curr_file_to_mtk_frame+1)*BLOCK_SIZE;


    //debug_msg("file_len = %llu,   start=%d, end=%d\r\n",file_len, start_pos, end_pos);

    if(start_pos>=(UINT32)file_len)
    {
        debug_msg("XmodemGetFileDataByIndex---return -3\r\n");
        FileSys_CloseFile(fhandle);
        return -3;
    }

    if(end_pos<=((UINT32)file_len))
    {
        //debug_msg("XmodemGetFileDataByIndex---end_pos<=file_len\r\n");
        FileSys_SeekFile(fhandle, start_pos, FST_SEEK_SET);
        FileSys_ReadFile(fhandle, (void*)buff, &buff_size, 0,NULL);
        FileSys_CloseFile(fhandle);
        //debug_msg("XmodemGetFileDataByIndex---1--buff_size=%d\r\n", buff_size);
        w_ret = buff_size;
    }
    else
    {
        //debug_msg("XmodemGetFileDataByIndex---end_pos>file_len\r\n");
        FileSys_SeekFile(fhandle, start_pos, FST_SEEK_SET);
        FileSys_ReadFile(fhandle, (void*)buff, &buff_size, 0,NULL);
        FileSys_CloseFile(fhandle);
        //debug_msg("XmodemGetFileDataByIndex--2---buff_size=%d\r\n", buff_size);

        w_ret = (((UINT32)file_len)-start_pos);
    }

    return w_ret;
    #else

    /*
    	FST_FILE fhandle = FileSys_OpenFile(srcImgName, FST_OPEN_READ);

    	if(fhandle==NULL)
    	{
    		//debug_msg("XmodemGetFileDataByIndex  - open file fail: %s\r\n",srcImgName);
    		total_frame_to_mtk = 0;
    		FileSys_CloseFile(fhandle);
    		return total_frame_to_mtk;
    	}


            if(FST_STA_OK == FileSys_SeekFile(fhandle, 0, FST_SEEK_END))
    	{
    	        FileSys_SeekFile(fhandle, 0, FST_SEEK_END);
    	 }

    */
    file_len = photo_data_size;

    //file_len = 109;//FileSys_GetFileLen(srcImgName);


    //kal_prompt_trace(MOD_CC, "FileLens = %d\r\n",FileLens);

    /*
    	tmp = file_len%BLOCK_SIZE;

    	total_frame_to_mtk = file_len/BLOCK_SIZE;

    	if(tmp>0)
    	{
    		total_frame_to_mtk++;
    	}
    */


    start_pos = curr_file_to_mtk_frame*BLOCK_SIZE;
    end_pos = (curr_file_to_mtk_frame+1)*BLOCK_SIZE;


    //debug_msg("file_len = %llu,   start=%d, end=%d\r\n",file_len, start_pos, end_pos);

    if(start_pos>=(UINT32)file_len)
    {
        //debug_msg("XmodemGetFileDataByIndex---return -3\r\n");
        //FileSys_CloseFile(fhandle);
        return -3;
    }

    if(end_pos<=((UINT32)file_len))
    {
        //debug_msg("XmodemGetFileDataByIndex---end_pos<=file_len\r\n");
        //FileSys_SeekFile(fhandle, start_pos, FST_SEEK_SET);
        //FileSys_ReadFile(fhandle, (void*)buff, &buff_size, 0,NULL);
        //FileSys_CloseFile(fhandle);
        memcpy((void*)buff, (photo_data_address+start_pos), buff_size);
        debug_msg("XmodemGetFileDataByIndex---1--buff_size=%d\r\n", buff_size);
        w_ret = buff_size;
    }
    else
    {
        //debug_msg("XmodemGetFileDataByIndex---end_pos>file_len\r\n");
        //FileSys_SeekFile(fhandle, start_pos, FST_SEEK_SET);
        //FileSys_ReadFile(fhandle, (void*)buff, &buff_size, 0,NULL);
        //FileSys_CloseFile(fhandle);
        memcpy((void*)buff, (photo_data_address+start_pos), (file_len-start_pos));

        debug_msg("XmodemGetFileDataByIndex--2---buff_size=%d\r\n", (file_len-start_pos));

        w_ret = (((UINT32)file_len)-start_pos);
    }

    return w_ret;
    #endif

    //debug_msg("FileSys_ReadFile = %s\r\n",buff);

}

void XmodemSendOneFrameToMTK(void)
{
    int i = 0;
    int len =0;
    unsigned char * data= NULL;

    debug_msg("XmodemSendOneFrameToMTK---total=%d, curr_index=%d\r\n", total_frame_to_mtk, curr_file_to_mtk_frame);

    if(curr_file_to_mtk_frame>=total_frame_to_mtk)
    {
        curr_file_to_mtk_frame = 0;
        total_frame_to_mtk = 0;
        is_xomode_working  = 0;
        //finish send EOT to MTK,  mtk will send ACK, if receive ACK, then finish Xmodem mode(XmodemWriteChar=0)
        //debug_msg("write EOT to MTK--1--\r\n");
        XmodemWriteChar(EOT);
        is_xomode_working = 2;
        return;
    }

    memset((void*)frame_buff_for_mtk,0,sizeof(frame_buff_for_mtk));

    len = XmodemGetFileDataByIndex((UINT8*)frame_buff_for_mtk,sizeof(frame_buff_for_mtk), curr_file_to_mtk_frame);

    if(len>=0)
    {
        int i;
        int checksum =0;

        for(i = 0;  i<BLOCK_SIZE; i++)
        {
            checksum +=frame_buff_for_mtk[i];
        }
        data = XmodemPackageFrame(frame_buff_for_mtk, len ,curr_file_to_mtk_frame);
        XmodemWriteData(data,FRAME_SIZE);
    }
    else
    {
        //debug_msg("write EOT to MTK--2--\r\n");
        XmodemWriteChar(EOT);
        is_xomode_working = 2;
    }



}


/*

		curr_file_to_mtk_frame++;
		send_one_frame_to_mtk();


		if(RecSentence_read[0] == ACK)
		{
			curr_file_to_mtk_frame++;
			send_one_frame_to_mtk();
		}

		if(RecSentence_read[0] == NAK)
		{
			send_one_frame_to_mtk();
		}
*/


void XmodemResetSendToMTK(void)
{

    debug_msg("XmodemResetSendToMTK\r\n");
    curr_file_to_mtk_frame = 0;
    total_frame_to_mtk = 0;
    memset((void*)frame_buff_for_mtk,0,sizeof(frame_buff_for_mtk));
    is_xomode_working = 0;
}

void XmodeStartSendFirstToMTK(UINT32 Addr, UINT32 Size)//(char *pPath)
{
    curr_file_to_mtk_frame = 0;
    photo_data_address=Addr;
    photo_data_size=Size;
    if(XmodemGetFileTotalFrame(Addr, Size)>0)
    {
        XmodemSendOneFrameToMTK();
    }
    else
    {
        //send fail
        XmodemResetSendToMTK();
        is_xomode_working = 0;
    }
}
void XmodemStartSendNextFrameToMTK(void)
{

    curr_file_to_mtk_frame++ ;
    XmodemSendOneFrameToMTK();
}
void XmodemStartResendFrameToMTK(void)
{
    XmodemSendOneFrameToMTK();
}

#endif


BOOL XmodemGetMTKMenuSetting(CHAR *Setting)
{
    memcpy( Setting,(UINT8*)&guiMTKMenuSetting,MENU_SETTING_MAX);
    return TRUE;
}

void XmodemSetMTKHeartBeatRes(BOOL En)
{
    gbMTKHeatBeatEn=En;
}

BOOL XmodemGetSpeed(FLOAT *Speed)
{
    #if defined(YQCONFIG_HAVE_GPS_OR_NOT)
		    if((g_mtk_gps.lat!=0)&&(g_mtk_gps.lng!=0))
		    {
		        *Speed= (UINT32)guiMTKSpeed;
		        return TRUE;
		    }
		    else
		    {
		        *Speed= 0;
		        return FALSE;
		    }
    #else //test
		guiMTKSpeed =90;

	  //  debug_msg("\r\n**********XmodemGetSpeed  guiMTKSpeed = %f, %d**************\r\n",(FLOAT)guiMTKSpeed, guiMTKSpeed);
	    
	    *Speed= (FLOAT)guiMTKSpeed;
	 //   debug_msg("\r\n**********XmodemGetSpeed  g_CurSpeed = %f, %d, %u,%03d, km/h**************\r\n",*Speed, (UINT32)(*Speed),(UINT32)(*Speed),(UINT32)(*Speed));
	    
	    return TRUE;

    #endif
}
UINT8 Mirror_Reverse_flag=0;
void XmodemSetBackSensorMode(int mode)
{
    static UINT8 Mirror_Reverse_flag1=0;
    if(SENSORMODE_NONE==mode)
    {

        if(1==Mirror_Reverse_flag1)
        {
            Mirror_Reverse_flag1=0;
            UINT8 uiResqData[16]= {0};
            uiResqData[0]=0x00;
            MTKComposeCMDRspFrame(0, CMD_TOUCH,(UINT8 *)&uiResqData, 1);
        }
    }
    else if(SENSORMODE_REVERSE==mode)
    {
        if(XmodemCheckBackSensorBackMirrorDet())
        {
            Mirror_Reverse_flag1=1;
            Mirror_Reverse_flag=1;
        }
    }
    else if(SENSORMODE_MIRROR==mode)
    {
    }
    else
    {
    }
    gbBackSensorMode = mode;
}

BOOL XmodemCheckBackSensorMode(viod)
{
    static int BackSensorlasttMode=0;
    int BackSensorcurrentMode;

    BackSensorcurrentMode= gbBackSensorMode;
    if(BackSensorcurrentMode!=BackSensorlasttMode)
    {
        BackSensorlasttMode=BackSensorcurrentMode;
        return TRUE;
    }
    else
    {

        return FALSE;
    }

}

/*
void XmodemSetMTKReverseGearDet(BOOL En)
{

    //if(gbBackSensorMode==TRUE)
    if(En)
    {
        gbBackSensorMode = 1;
    }
    else
    {
        //reverse event should break screensaver, so we set  0(normal
        gbBackSensorMode = 0;//
    }
}
*/
BOOL XmodemGetMTKReverseGearDet(void)
{
    if(gbBackSensorMode==1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}


BOOL XmodemCheckBackSensorBackMirrorDet(void)
{
    if(gbBackSensorMode==2)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL XmodemCheckBackSensorNoramlDet(void)
{
    if(gbBackSensorMode!=1&&gbBackSensorMode!=2)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL XmodemGetTPEventIsLock()
{
    return gbLockTPEvent;
}
void XmodemSetTPEventLock(BOOL Lock)
{
    gbLockTPEvent=Lock;
}
