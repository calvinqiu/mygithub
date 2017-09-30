/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       PrjCfg_Comm_Tail.h
    @ingroup

    @brief

    @note       This config genernate default config where isn't set in PrjCfg_MODEL.h
*/

#ifndef _PRJCFG_COMM_TAIL_H_
#define _PRJCFG_COMM_TAIL_H_

#ifndef ECOS_POWER_OFF_FLOW
#define ECOS_POWER_OFF_FLOW DISABLE
#endif

#ifndef PHOTO_MODE
#define PLAY_MODE                       ENABLE
#define PHOTO_MODE                      ENABLE
#define MOVIE_MODE                      ENABLE
#define USB_MODE                        ENABLE
#endif

#ifndef DISPLAY_DEFAULT_ATTACH_DEVICE
#define DISPLAY_DEFAULT_ATTACH_DEVICE   LCD1_MASK
#endif

//#NT#2016/06/21#Jeah Yen -begin
//#NT#Support display fast switch
#ifndef DISPLAY_FAST_SWITCH
#define DISPLAY_FAST_SWITCH             DISABLE //fast swicth display device
#endif
//#NT#2016/06/21#Jeah Yen -end

#ifndef DISPLAY_OSD2_FMT
#define DISPLAY_OSD2_FMT                DISP_PXLFMT_INDEX8 // 256 colors
#endif

#ifndef LCD_PREVIEW_VDO_FMT
#define LCD_PREVIEW_VDO_FMT             GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define LCD2_PREVIEW_VDO_FMT            GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define TV_PREVIEW_VDO_FMT              GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define HDMI_PREVIEW_VDO_FMT            GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#endif

#ifndef LINE_INSERT_FUNCTION
#define SPK_MASK                        0x01
#define LOUT_MASK                       0x02
#define LINE_INSERT_FUNCTION            DISABLE //Auto Detect - LINE cable insert (audio out)
#endif

#ifndef AUDIO_PLAY_FUNCTION
#define AUDIO_PLAY_FUNCTION             DISABLE //Auto Play
#endif

#ifndef USE_DPOF
#define USE_DPOF DISABLE
#endif

#ifndef ONVIF_FUNC
#define ONVIF_FUNC DISABLE
#endif

#ifndef BT_FUNC
#define BT_FUNC DISABLE
#endif

#ifndef MSDCVENDOR_UPDFW
#define MSDCVENDOR_UPDFW DISABLE
#endif

#ifndef UCTRL_APP_MOVIE_FEATURE_SETGET
#define UCTRL_APP_MOVIE_FEATURE_SETGET DISABLE
#endif

#ifndef IPCAM_FUNC
#define IPCAM_FUNC DISABLE
#endif

#ifndef MEDIA_VASTREAM
#define MEDIA_VASTREAM DISABLE
#endif

#ifndef DUALCAM_PIP_BEHIND_FLIP
#define DUALCAM_PIP_BEHIND_FLIP DISABLE
#endif

#ifndef YOUKU_SDK
#define YOUKU_SDK DISABLE
#endif

//#NT#2016/05/06#Jeah Yen -begin
//#NT#Support IOTP2P
#ifndef IOT_P2P_FUNC
#define IOT_P2P_FUNC                    DISABLE
#endif
//#NT#2016/05/06#Jeah Yen -end

//#NT#2016/07/14#Charlie Chang -begin
//#NT# Support 4g module
#ifndef NETWORK_4G_MODULE
#define NETWORK_4G_MODULE                      DISABLE
#endif
//#NT#2016/07/14#Charlie Chang -end
#ifndef FS_MULTI_STRG_FUNC
#define FS_MULTI_STRG_FUNC              DISABLE
#endif

//#NT#2016/03/07#KCHong -begin
//#NT#Low power timelapse function
#ifndef TIMELAPSE_LPR_FUNCTION
#define TIMELAPSE_LPR_FUNCTION DISABLE
#endif
//#NT#2016/03/07#KCHong -end

//#NT#2016/02/18#Lincy Lin -begin
//#NT#Support logfile function
#ifndef LOGFILE_FUNC
#define LOGFILE_FUNC DISABLE
#endif
//#NT#2016/02/18#Lincy Lin -end
//#NT#2016/02/22#Niven Cho -begin
//#NT#add IR_CUT_FUNC to enable IR CUT function.
#ifndef IR_CUT_FUNC
#define IR_CUT_FUNC DISABLE
#endif
//#NT#2016/02/22#Niven Cho -end

//#NT#2016/02/26#Niven Cho -begin
//#NT#eCos debug msg alway show on uITRON via IPC.
#ifndef ECOS_DBG_MSG_FORCE_UART1_DIRECT
#define ECOS_DBG_MSG_FORCE_UART1_DIRECT     DISABLE
#endif
//#NT#2016/02/26#Niven Cho -end
#ifndef USOCKET_CLIENT
#define USOCKET_CLIENT DISABLE
#endif

//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
#ifndef MOVIE_OT_FUNC
#define MOVIE_OT_FUNC                   DISABLE
#endif
#ifndef MOVIE_OT_DRAW_VIDEO
#define MOVIE_OT_DRAW_VIDEO             DISABLE
#endif
//#NT#2016/03/02#Lincy Lin -end

#ifndef MOVIE_DISDSP_FUNC
#define MOVIE_DISDSP_FUNC                   DISABLE
#endif
#ifndef MOVIE_DISDSP_DRAW_VIDEO
#define MOVIE_DISDSP_DRAW_VIDEO             DISABLE
#endif
//#NT#2016/03/04#Lincy Lin -begin
//#NT#Porting media 4.0 flow
#ifndef MEDIA_4_0
#define MEDIA_4_0                       DISABLE
#endif
//#NT#2016/03/04#Lincy Lin -end

//#NT#2016/03/16#Lincy Lin -begin
//#NT#Support HTTP liveview datastamp function
#ifndef HTTP_LIVEVIEW_DATASTAMP_FUNC
#define HTTP_LIVEVIEW_DATASTAMP_FUNC    DISABLE
#endif
//#NT#2016/03/16#Lincy Lin -end

//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#ifndef WIFI_UI_FLOW_VER
#define WIFI_UI_VER_1_0      0
#define WIFI_UI_VER_2_0      1
#define WIFI_UI_FLOW_VER     WIFI_UI_VER_1_0
#endif
//#NT#2016/03/23#Isiah Chang -end

//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (IPCAM_FUNC == ENABLE)
#define LOG_DBGINFO_IN_GPS_SECTION      DISABLE
#else
#define LOG_DBGINFO_IN_GPS_SECTION      ENABLE
#endif
//#NT#2016/06/28#KCHong -end

//#NT#2016/04/14#YongChang_Qui -begin
//#NT#Log For project unaware of WIFI_POWER_OFF_FLOW
#ifndef WIFI_POWER_OFF_FLOW
#define WIFI_POWER_OFF_FLOW             DISABLE
#endif
//#NT#2016/04/14#YongChang_Qui -end

//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#ifndef CURL_FUNC
#define CURL_FUNC                       DISABLE
#endif
//#NT#2016/04/21#Lincy Lin -end

//#NT#2016/04/25#KCHong -begin
//#NT#ADAS Function
#ifndef _ADAS_FUNC_
#define _ADAS_FUNC_                     DISABLE
#endif
//#NT#2016/04/25#KCHong -end

//#NT#2016/07/06#CCChao -begin
//#NT#TSR Function
#ifndef _TSR_FUNC_
#define _TSR_FUNC_                      DISABLE
#endif
#ifndef MOVIE_TSR_FUNC_
#define MOVIE_TSR_FUNC_                 DISABLE
#endif
#ifndef MOVIE_TSR_DRAW_VIDEO
#define MOVIE_TSR_DRAW_VIDEO            DISABLE
#endif
#ifndef TSR_MODEL_IN_PSTORE
#define TSR_MODEL_IN_PSTORE             DISABLE
#endif
//#NT#2016/07/06#CCChao -end

//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#ifndef _DDD_FUNC_
#define _DDD_FUNC_                      DISABLE
#endif

#ifndef MOVIE_DDD_FUNC
#define MOVIE_DDD_FUNC                  DISABLE
#endif

#ifndef MOVIE_DDD_FD_FUNC
#define MOVIE_DDD_FD_FUNC               DISABLE
#endif

#ifndef MOVIE_DDD_DRAW_VIDEO
#define MOVIE_DDD_DRAW_VIDEO            DISABLE
#endif
//#NT#2016/07/22#ML Cui -begin
//#NT# Support DDD Save log in GPS
#ifndef MOVIE_DDD_SAVE_LOG
#define MOVIE_DDD_SAVE_LOG              DISABLE
#endif
//#NT#2016/07/22#ML Cui -end
//#NT#2016/04/22#Nestor Yang -end

#ifndef DDD_MODEL_IN_PSTORE
#define DDD_MODEL_IN_PSTORE              DISABLE
#endif

//#NT#2016/04/27#Lincy Lin -begin
//#NT#Support userlog function
#ifndef USERLOG_FUNC
#define USERLOG_FUNC                    DISABLE
#endif
//#NT#2016/04/27#Lincy Lin -end
//#NT#2016/05/23#David Tsai -begin
//#NT# Support tampering detection and background calculation function
#ifndef MOVIE_BC_FUNC
#define MOVIE_BC_FUNC                   DISABLE
#endif

#ifndef MOVIE_TD_FUNC
#define MOVIE_TD_FUNC                   DISABLE
#endif

#ifndef MOVIE_TD_DRAW_VIDEO
#define MOVIE_TD_DRAW_VIDEO             DISABLE
#endif
//#NT#2016/05/23#David Tsai -end

//#NT#2016/08/06#XQ Liu -begin
//#NT#Support TD save log in GPS
#ifndef MOVIE_TD_SAVE_LOG
#define MOVIE_TD_SAVE_LOG               DISABLE
#endif
//#NT#2016/08/06#XQ Liu -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
#ifndef MOVIE_TWD_FUNC
#define MOVIE_TWD_FUNC                  DISABLE
#endif

#ifndef MOVIE_TWD_DRAW_VIDEO
#define MOVIE_TWD_DRAW_VIDEO            DISABLE
#endif

#ifndef MOVIE_TZD_FUNC
#define MOVIE_TZD_FUNC                  DISABLE
#endif

#ifndef MOVIE_TZD_DRAW_VIDEO
#define MOVIE_TZD_DRAW_VIDEO            DISABLE
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/10/26#Omega Yu -begin
//#NT# Support scene change detection function
 #ifndef MOVIE_SCD_FUNC
#define MOVIE_SCD_FUNC                  DISABLE
#endif
//#NT#2016/10/26#Omega Yu -end

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#ifndef MOVIE_FTG_FUNC
#define MOVIE_FTG_FUNC                  DISABLE
#endif

#ifndef MOVIE_FTG_DRAW_VIDEO
#define MOVIE_FTG_DRAW_VIDEO            DISABLE
#endif
//#NT#2016/10/17#Bin Xiao -end

//#NT#2016/05/24#Charlie Chang -begin
//#NT# for send high quality streaming (stream1)
#ifndef MOVIE_NET_USE_STREAM_1
#define MOVIE_NET_USE_STREAM_1                    DISABLE
#endif
//#NT#2016/05/24#Charlie Chang -end



//#NT#2016/05/23#HM Tseng -begin
//#NT#Support audio push function
#ifndef AUDIO_PUSH_FUNC
#define AUDIO_PUSH_FUNC                 DISABLE
#endif
//#NT#2016/05/23#HM Tseng -end

//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#ifndef UVC_MULTIMEDIA_FUNC
#define UVC_MULTIMEDIA_FUNC         DISABLE
#endif
//#NT#2016/05/31#Ben Wang -end

//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function
#ifndef MOVIE_FD_DRAW_VIDEO
#define MOVIE_FD_DRAW_VIDEO         DISABLE
#endif
//#NT#2016/06/08#Lincy Lin -end

//#NT#2016/06/17#Brain Yen -begin
//#NT#Pipview LR Function
#ifndef PIP_VIEW_LR
#define PIP_VIEW_LR          DISABLE   //(ENABLE: display 2 sensor by PipView in left, right view, side by side, DISABLE: normal pipview)
#endif
//#NT#2016/06/17#Brain Yen -end
//#NT#2016/07/07#David Tsai -begin
//#NT#Two stream mode for IP Cam
#ifndef TWO_STREAM_MODE
#define TWO_STREAM_MODE     DISABLE
#endif
//#NT#2016/07/07#David Tsai -end

//#NT#2016/07/12#Niven Cho -begin
//#NT#LINUX_LOAD_BY_UBOOT
#ifndef LINUX_LOAD_BY_UBOOT
#define LINUX_LOAD_BY_UBOOT             DISABLE
#endif
//#NT#2016/07/12#Niven Cho -end

//#NT#2016/07/19#Charlie Chang -begin
//#NT# force unplug RTSP unit
#ifndef FORCE_UNPLUG_RTSP
#define FORCE_UNPLUG_RTSP         DISABLE
#endif
//#NT#2016/07/19#Charlie  -end

//#NT#2016/08/03#Charlie Chang -begin
//#NT# support iqtool
#ifndef IQTOOL_FUNC
#define IQTOOL_FUNC          DISABLE
#endif
//#NT#2016/08/03#Charlie Chang -end

//#NT#2016/08/08#Lincy Lin -begin
//#NT#Reduce gxgfx buffer
#ifndef GXGFX_ADVANCED_STRING_FUNC
#define GXGFX_ADVANCED_STRING_FUNC      ENABLE
#endif
//#NT#2016/08/08#Lincy Lin -end
//#NT#2016/08/11#Brain Yen -begin
//#NT#save dual stream unit
#ifndef SAVE_DUAL_STREAM_FUNC
#define SAVE_DUAL_STREAM_FUNC           DISABLE
#endif
//#NT#2016/08/11#Brain Yen -end

//#NT#2016/08/12#Hideo Lin -begin
//#NT#For small size clone movie
#ifndef SMALL_CLONE_MOVIE
#define SMALL_CLONE_MOVIE               DISABLE
#endif
#ifndef CUSTOM_NAMING_RULE
#define CUSTOM_NAMING_RULE              DISABLE
#endif
//#NT#2016/08/12#Hideo Lin -end
//#NT#2016/08/19#Lincy Lin -begin
//#NT#Support linux cvhost function
#ifndef LINUX_CVHOST_FUNC
#define LINUX_CVHOST_FUNC               DISABLE
#endif
//#NT#2016/08/19#Lincy Lin -end

//#NT#2016/08/29#HM Tseng -begin
//#NT#For audio play testing mode
#ifndef AUDIO_PLAY_TEST
#define AUDIO_PLAY_TEST                 DISABLE
#endif
//#NT#2016/08/29#HM Tseng -end

//#NT#2016/09/19#Niven Cho -begin
//#NT#SD_HOTPLUG
#ifndef SDHOTPLUG_FUNCTION
#define SDHOTPLUG_FUNCTION              DISABLE
#endif
//#NT#2016/09/19#Niven Cho -end

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#ifndef _3DNROUT_FUNC
#define _3DNROUT_FUNC                   DISABLE
#endif
//#NT#2016/09/20#Bob Huang -end

//#NT#2016/11/18#Charlie Chang -begin
//#NT# support Face tracking granding snapshot
#ifndef FTG_SNAPSHOT_FUN
#define FTG_SNAPSHOT_FUN      DISABLE
#endif

#ifndef FTG_MAX_SNAPSHOT_NUM
#define FTG_MAX_SNAPSHOT_NUM      5
#endif

#ifndef FTG_OVERRALL_MAX_BUF_SIZE
#define FTG_OVERRALL_MAX_BUF_SIZE      250*1024
#endif

#ifndef FTG_FACE_MAX_BUF_SIZE
#define FTG_FACE_MAX_BUF_SIZE      100*1024
#endif

#ifndef FTG_MAX_SNAPSHOT_SIZE
#define FTG_MAX_SNAPSHOT_SIZE      1024*1024*25
#endif

#ifndef FTG_MAX_FACE_PIC_NUM
#define FTG_MAX_FACE_PIC_NUM      3
#endif

#ifndef FTG_SAVE_LANDMARK_TO_PSTORE
#define FTG_SAVE_LANDMARK_TO_PSTORE      DISABLE
#endif
//#NT#2016/11/18#Charlie Chang -end

#ifndef RTSP_AUDIO_ENABLE
#define RTSP_AUDIO_ENABLE      ENABLE
#endif

#ifndef AUDIO_CMD_UCTRL_ENABLE
#define AUDIO_CMD_UCTRL_ENABLE      ENABLE
#endif

#ifndef FACE_GRADE_TEST
#define FACE_GRADE_TEST           DISABLE
#endif

#ifndef IPCAM_UVC_FUNC
#define IPCAM_UVC_FUNC           DISABLE
#endif

//#NT#2016/12/06#Niven Cho -begin
//#NT#MULTI_DRIVE
#ifndef FS_DX_TYPE_DRIVE_A
#define FS_DX_TYPE_DRIVE_A DX_TYPE_CARD1
#endif

#ifndef FS_DX_TYPE_DRIVE_B
#define FS_DX_TYPE_DRIVE_B USER_DX_TYPE_EMBMEM_FAT
#endif
//#NT#2016/12/06#Niven Cho -end

//#NT#2016/12/14#Niven Cho -begin
//#NT#EMMC_AS_PSTORE
// only for (_CPU2_TYPE_==_CPU2_LINUX_ && _INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
#ifndef PST_DEV_ID
#define PST_DEV_ID 4 //used for gxstrg to mount linux emmc-pstore partition
#endif

#ifndef PST_FS_DRIVE
#define PST_FS_DRIVE "E:\\"
#endif
//#NT#2016/12/14#Niven Cho -end

//#NT#2016/12/22#HM Tseng -begin
//#NT# Support AEC function
#ifndef AUDIO_AEC_FUNC
#define AUDIO_AEC_FUNC                  DISABLE
#endif
//#NT#2016/12/22#HM Tseng -end

//#NT#2017/03/02#Lincy Lin -begin
//#NT#Support fileSize align function
#ifndef FILESIZE_ALIGN_FUNC
#define FILESIZE_ALIGN_FUNC             DISABLE
#endif
//#NT#2017/03/02#Lincy Lin -end

//#NT#2017/03/02#Niven Cho -begin
//#NT#EMMC_AS_PSTORE
#if (_CPU2_TYPE_==_CPU2_LINUX_ && _INTERSTORAGE_ == _INTERSTORAGE_EMMC_ && FS_MULTI_STRG_FUNC==DISABLE)
#error "FS_MULTI_STRG_FUNC must ENABLE for EMMC + LINUX"
#endif
//#NT#2017/03/02#Niven Cho -end

//#NT#2017/03/07#Niven Cho[117156] -begin
//#NT#FS_SWITCH_STRG_FUNC
#ifndef FS_SWITCH_STRG_FUNC
#define FS_SWITCH_STRG_FUNC DISABLE
#endif
#if (FS_SWITCH_STRG_FUNC && FS_MULTI_STRG_FUNC)
#error "FS_SWITCH_STRG_FUNC and FS_MULTI_STRG_FUNC cannot be both enabled."
#endif
#if (FS_SWITCH_STRG_FUNC && SDHOTPLUG_FUNCTION)
#error "FS_SWITCH_STRG_FUNC and SDHOTPLUG_FUNCTION cannot be both enabled."
#endif
//#NT#2017/03/07#Niven Cho[117156] -end

#ifndef FISHEYE_TYPE
#define FISHEYE_TYPE DISABLE
#endif

//#NT#2017/04/21#Niven Cho -begin
//#NT#[118423], UIFramework got -1 and then perform ker dump
#ifndef UIFRAMEWORK_HANG_DUMP_KER
#define UIFRAMEWORK_HANG_DUMP_KER		DISABLE
#endif
//#NT#2017/04/21#Niven Cho -end

#endif //_PRJCFG_COMM_TAIL_H_
