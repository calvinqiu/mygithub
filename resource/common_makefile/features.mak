#****************************************************************************************#
#  Segment: custom config Related
#****************************************************************************************#

YQCONFIG_AE_CUST = AE_OV4689_IMX322LQJ_EVB_FF      
            							# Option1:AE_OV4689_IMX322LQJ_EVB_FF
            							# Option2:AE_SAMPLE   
            							
YQCONFIG_AWB_CUST = AWB_OV4689_IMX322LQJ_EVB_FF      
            							# Option1:AWB_OV4689_IMX322LQJ_EVB_FF
            							# Option2:AWB_SAMPLE             							   							

YQCONFIG_IPP_CUST = IPL_OV4689_IMX322LQJ_EVB_FF      
            							# Option1:IPL_OV4689_IMX322LQJ_EVB_FF
            							# Option2:IPL_FAKE  
            							
YQCONFIG_SENSOR_CUST = CMOS_OV4689      
            							# Option1:CMOS_OV4689
            							# Option2:OFF  
            							
YQCONFIG_SENSOR_2_CUST = CMOS_IMX322LQJVX1      
            							# Option1:CMOS_IMX322LQJVX1
            							# Option2:OFF 

YQCONFIG_LCD_CUST = Disp_IFDSI_LCD1_TWJ50IP008_ILI9806E      
            							# Option1:Disp_IFDSI_LCD1_TWJ50IP008_ILI9806E
            							# Option2:OFF

YQCONFIG_LCD_WIDTH_CUST = 1280    
								# Option1:1280
								#Option2:864 
								
YQCONFIG_LCD_HEIGHT_CUST = 400
								# Option1:400
								# Option2:480

YQCONFIG_LCD_ROTATE_180_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no    
								
YQCONFIG_OSD_USE_ROTATE_BUFFER_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no   
								          							
YQCONFIG_VDO_USE_ROTATE_BUFFER_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no    

YQCONFIG_VDO2_USE_ROTATE_BUFFER_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no    

YQCONFIG_NETWORK_CUST = NETWORK_NONE      
            							# Option1:NETWORK_SDIO_EVB_WIFI
            							# Option2:NETWORK_NONE             							

YQCONFIG_GSENSOR_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no 

YQCONFIG_GPIO_GSENSOR_POWERON_ANDROID_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no 

YQCONFIG_GYRO_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no  

YQCONFIG_GPS_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no 
YQCONFIG_WIFI_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no 
            							
YQCONFIG_COMB_LCDMODE = NONE	    
            							# Option1:MIPI
            							# Option2:RGB
            							# Option3:PARALLEL 
            							
YQCONFIG_COMB_SENSOR_ROTATE_OPTION = NONE	    
            							# Option1:NONE
            							# Option2:FLIP
            							# Option3:MIRROR   

YQCONFIG_COMB_SENSOR_DISPLAY_FLIP_OPTION = FLIP_NONE	    
            							# Option1:FLIP_NONE
            							# Option2:FLIP_H
            							# Option3:FLIP_V 
            							# Option4:FLIP_H_V
            							
YQCONFIG_COMB_SENSOR_2_ROTATE_OPTION = NONE	    
            							# Option1:NONE
            							# Option2:FLIP
            							# Option3:MIRROR            							           							
            							           							
YQCONFIG_COMB_UIFILETYPE_OPTION = TS	    
            							# Option1:TS
            							# Option2:MOV
            							# Option3:AVI   

YQCONFIG_CPU2_TYPE_CUST = CPU2_ECOS      
            							# Option1:CPU2_LINUX
            							# Option2:CPU2_ECOS 

YQCONFIG_STORAGE_CUST = Storage_SPI      
            							# Option1:Storage_SPINAND
            							# Option2:Storage_SPI  

YQCONFIG_EMBMEM_BLK_SIZE_CUST = 0x1000      
            							# Option1:0x20000
            							# Option2:0x1000   

YQCONFIG_USB_TYPE_CUST = DEVICE                                          
            							# Option1:HOST
            							# Option2:DEVICE    

YQCONFIG_GPS_CUST = GPS_NONE                                          
            							# Option1:GPS_GENERAL
            							# Option2:GPS_NONE 

YQCONFIG_UI_STYLE_CUST = UI_STYLE_SPORTCAM     
            							# Option1:UI_STYLE_SPORTCAM
            							# Option2:UI_STYLE_CARDV                                        
#****************************************************************************************#
#  Segment: Software Config Platform Related
#****************************************************************************************#
YQCONFIG_USE_FIX_PROJECT_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no
								
YQCONFIG_CUSTOM_VERSION_NAME = ".public"
            							# Option1:.AMX
            							# Option2:.public            							
       							
YQCONFIG_ANDROID_SYSTEM_SUPPORT = no
								# Option1:yes
            							# Option2:no            							
            							
YQCONFIG_DRIVE_EVENT_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no
            							
YQCONFIG_UART_TO_MTK_SUPPORT = no
								# Option1:yes
            							# Option2:no     
            							
YQCONFIG_UART_TO_MCU_SUPPORT = no
								# Option1:yes
            							# Option2:no            							         							
YQCONFIG_SDHOTPLUG_FUNCTION_SUPPORT = no
								# Option1:yes
            							# Option2:no            							         							

YQCONFIG_POWERLOGO_FUNCTION_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no

YQCONFIG_POWERSOUND_FUNCTION_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no 

YQCONFIG_PWRLEVEL_FUNCTION_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no 
								
YQCONFIG_USB1_TYPE_SUPPORT = DEVICE	    
            							# Option1:DEVICE
            							# Option2:HOST 
           							
YQCONFIG_LONG_STARTUP_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no  

YQCONFIG_MOVIE_AUTOREC_ACPLUG_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no             							

YQCONFIG_COMB_AUDIO_OUTPUT_PATH = LINE	    
            							# Option1:LINE
            							# Option2:SPK
            							# Option3:MONO            							             							

YQCONFIG_COMB_MOVIE_LIVEVIEW = RTSP_LIVEVIEW	    
            							# Option1:RTSP_LIVEVIEW
            							# Option2:HTTP_LIVEVIEW   
            							
YQCONFIG_COMB_DET_BACKCAR_BY = NONE
								# Option1:GPIO
            							# Option2:UART
            							# Option3:NONE 
YQCONFIG_COMB_DET_LCD_SWITCH_BY = NONE
								# Option1:GPIO
            							# Option2:UART
            							# Option3:NONE 
YQCONFIG_PARKING_MODE_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no   
YQCONFIG_CHECK_SENSOR_2_PLUGIN_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no             							         							            							
YQCONFIG_ADAS_FUNC_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no            							          							         							          							            							
YQCONFIG_LED_FUNCTION_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no   
YQCONFIG_AUDIO_MIC_SWITCH_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no   
YQCONFIG_TP_EVENT_LOCK_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no   
YQCONFIG_DEBUG_MESSAGE_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no   
YQCONFIG_PIP_VIEW_LR_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no    
								         							          							         							          							            							
YQCONFIG_CUSTOM_NAMING_RULE_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no    
YQCONFIG_COMB_PLAYMODE_OPTION = PLAYBACKMODE	    
            							# Option1:USBMSDCMODE
            							# Option2:PLAYBACKMODE    
YQCONFIG_TOUCH_LOCKFILE_OPTION = no	    
            							# Option1:yes
            							# Option2:no 
            							
YQCONFIG_TOUCH_AUDIOSWITCH_OPTION = no	    
            							# Option1:yes
            							# Option2:no  
            							
YQCONFIG_COMB_PLAYMODE_ITEMS_ONE_PAGE_OPTION = BIGSCREEN	    
            							# Option1:BIGSCREEN
            							# Option2:SMALLSCREEN    

YQCONFIG_COMB_RAM_SIZE = 2G 
            							# Option1:2G
            							# Option2:4G  

YQCONFIG_LCD_RESET_REVERSE_SUPPORT = no            								       							          							
            							# Option1:yes
            							# Option2:no   

YQCONFIG_UCTRL_CUSTOM_SUPPORT = no            								       							          							
            							# Option1:yes
            							# Option2:no  

YQCONFIG_ADC2_FOR_KPD_SUPPORT = no  
            							# Option1:yes
            							# Option2:no 
								
YQCONFIG_YUANEDOG_SUPPORT = no   
            							# Option1:yes
            							# Option2:no	

YQCONFIG_SDCARD_SWITCH_DETECT_SUPPORT = no   
            							# Option1:yes
            							# Option2:no  

YQCONFIG_POWER_KEY_SWITCH_LCD_TV_SUPPORT = no   
            							# Option1:yes
            							# Option2:no   

YQCONFIG_BACKCAR_DET_SUPPORT = no   
            							# Option1:yes
            							# Option2:no 

YQCONFIG_PWRKEY_LONGPRESS_PWROFF_SUPPORT = no   
            							# Option1:yes
            							# Option2:no   

YQCONFIG_NTK2MCU_STATUS_SUPPORT = no   
            							# Option1:yes
            							# Option2:no                                        
