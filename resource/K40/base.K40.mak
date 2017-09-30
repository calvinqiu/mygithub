
#****************************************************************************************#
#  Segment: custom config Related
#****************************************************************************************#
YQCONFIG_AE_CUST = AE_IMX291M_IMX322LQJ_EVB_FF      
            							# Option1:AE_OV4689_IMX322LQJ_EVB_FF
            							# Option2:AE_SAMPLE   
            							
YQCONFIG_AWB_CUST = AWB_IMX291M_IMX322LQJ_EVB_FF      
            							# Option1:AWB_OV4689_IMX322LQJ_EVB_FF
            							# Option2:AWB_SAMPLE             							   							

YQCONFIG_IPP_CUST = IPL_IMX291M_IMX322LQJ_EVB_FF      
            							# Option1:IPL_OV4689_IMX322LQJ_EVB_FF
            							# Option2:IPL_FAKE  
            							
YQCONFIG_SENSOR_CUST = CMOS_IMX291M      
            							# Option1:CMOS_OV4689
            							# Option2:OFF  
            							
YQCONFIG_SENSOR_2_CUST = CMOS_IMX322LQJVX1      
            							# Option1:CMOS_IMX322LQJVX1
            							# Option2:OFF 

YQCONFIG_LCD_CUST = Disp_IFDSI_LCD1_XJ040WV1402C_R61408      
            							# Option1:Disp_IFDSI_LCD1_TWJ50IP008_ILI9806E
            							# Option2:Disp_VIRTUAL_LCD1_OFF

YQCONFIG_LCD_WIDTH_CUST = 800    
								# Option1:1280
								#Option2:864 
								
YQCONFIG_LCD_HEIGHT_CUST = 480
								# Option1:400
								# Option2:480
								
YQCONFIG_LCD_ROTATE_180_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no    
								
YQCONFIG_OSD_USE_ROTATE_BUFFER_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no   
								          							
YQCONFIG_VDO_USE_ROTATE_BUFFER_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no    

YQCONFIG_VDO2_USE_ROTATE_BUFFER_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no    

YQCONFIG_NETWORK_CUST = NETWORK_SDIO_EVB_WIFI      
            							# Option1:NETWORK_SDIO_EVB_WIFI
            							# Option2:NETWORK_NONE  
								
YQCONFIG_GSENSOR_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no 

YQCONFIG_GPIO_GSENSOR_POWERON_ANDROID_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no 
            							
YQCONFIG_COMB_LCDMODE = MIPI	    
            							# Option1:MIPI
            							# Option2:RGB
            							# Option3:PARALLEL 
								
YQCONFIG_COMB_SENSOR_ROTATE_OPTION = NONE	    
            							# Option1:NONE
            							# Option2:FLIP
            							# Option3:MIRROR 
            							# Option4:FLIP_MIRROR
	    
            							# Option1:FLIP_NONE
            							# Option2:FLIP_H
            							# Option3:FLIP_V 
            							# Option4:FLIP_H_V
								 
YQCONFIG_COMB_SENSOR_2_ROTATE_OPTION = FLIP	    
            							# Option1:NONE
            							# Option2:FLIP
            							# Option3:MIRROR            							           							
            							
YQCONFIG_COMB_UIFILETYPE_OPTION = TS	    
            							# Option1:TS
            							# Option2:MOV
            							# Option3:AVI            							           							
            							
YQCONFIG_CPU2_TYPE_CUST = CPU2_LINUX      
            							# Option1:CPU2_LINUX
            							# Option2:CPU2_ECO
                                        
YQCONFIG_STORAGE_CUST = Storage_SPINAND      
            							# Option1:Storage_SPINAND
            							# Option2:Storage_SPI

YQCONFIG_EMBMEM_BLK_SIZE_CUST = 0x20000      
            							# Option1:0x20000
            							# Option2:0x1000   

YQCONFIG_USB_TYPE_CUST = HOST                                          
            							# Option1:HOST
            							# Option2:DEVICE 

YQCONFIG_GPS_CUST = GPS_GENERAL                                          
            							# Option1:GPS_GENERAL
            							# Option2:GPS_NONE  

YQCONFIG_UI_STYLE_CUST = UI_STYLE_CARDV     
            							# Option1:UI_STYLE_SPORTCAM
            							# Option2:UI_STYLE_CARDV 
                                        
                                        
#****************************************************************************************#
#  Segment: Software Config Platform Related
#****************************************************************************************#
YQCONFIG_COMB_CUST_PROJECT_NAME = K40_BASE
								# YQCONFIG_COMB_CUST_PROJECT_NAME_K40_BASE
            							# Option1:K40_BASE
            							# Option2:DIDI_BASE
YQCONFIG_CUSTOM_VERSION_NAME = "K40.public"
            							# Option1:.AMX
            							# Option2:.public            							
								 
YQCONFIG_PARKING_MODE_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no  
								
YQCONFIG_CHECK_SENSOR_2_PLUGIN_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no 
								           							          							         							          							            							
YQCONFIG_ADAS_FUNC_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no  
								          							          							         							          							            							
YQCONFIG_LED_FUNCTION_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no  
								          							          							         							          							            							
 YQCONFIG_AUDIO_MIC_SWITCH_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no   
								 

 YQCONFIG_DEBUG_MESSAGE_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no   
  

YQCONFIG_CUSTOM_NAMING_RULE_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no   
                                        
YQCONFIG_COMB_PLAYMODE_OPTION = PLAYBACKMODE	    
            							# Option1:USBMSDCMODE
            							# Option2:PLAYBACKMODE    
            							

            							
YQCONFIG_COMB_PLAYMODE_ITEMS_ONE_PAGE_OPTION = SMALLSCREEN	    
            							# Option1:BIGSCREEN
            							# Option2:SMALLSCREEN  

YQCONFIG_POWERSOUND_FUNCTION_SUPPORT = yes     
            							# Option1:yes
            							# Option2:no 

YQCONFIG_SDHOTPLUG_FUNCTION_SUPPORT = yes
            							# Option1:yes
            							# Option2:no 
                                        
YQCONFIG_COMB_AUDIO_OUTPUT_PATH = SPK	    
            							# Option1:LINE
            							# Option2:SPK
            							# Option3:MONO                                         
								
YQCONFIG_COMB_RAM_SIZE = 2G 
            							# Option1:2G
            							# Option2:4G 
								
YQCONFIG_LCD_RESET_REVERSE_SUPPORT = yes            								       							          							
            							# Option1:yes
            							# Option2:no	

YQCONFIG_UCTRL_CUSTOM_SUPPORT = yes            								       							          							
            							# Option1:yes
            							# Option2:no 

YQCONFIG_ADC2_FOR_KPD_SUPPORT = yes  
            							# Option1:yes
            							# Option2:no

YQCONFIG_YUANEDOG_SUPPORT = yes   
            							# Option1:yes
            							# Option2:no 

YQCONFIG_SDCARD_SWITCH_DETECT_SUPPORT = yes   
            							# Option1:yes
            							# Option2:no  

YQCONFIG_POWER_KEY_SWITCH_LCD_TV_SUPPORT = yes   
            							# Option1:yes
            							# Option2:no     

YQCONFIG_BACKCAR_DET_SUPPORT = yes   
            							# Option1:yes
            							# Option2:no  

YQCONFIG_PWRKEY_LONGPRESS_PWROFF_SUPPORT = yes   
            							# Option1:yes
            							# Option2:no  

YQCONFIG_NTK2MCU_STATUS_SUPPORT = yes   
            							# Option1:yes
            							# Option2:no                                         
   