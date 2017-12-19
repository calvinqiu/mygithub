
#****************************************************************************************#
#  Segment: custom config Related
#****************************************************************************************#
YQCONFIG_AE_CUST = AE_AR0238CSP_EVB_FF      
            							# Option1:AE_OV4689_IMX322LQJ_EVB_FF
            							# Option2:AE_SAMPLE   
            							
YQCONFIG_AWB_CUST = AWB_AR0238CSP_EVB_FF     
            							# Option1:AWB_OV4689_IMX322LQJ_EVB_FF
            							# Option2:AWB_SAMPLE             							   							

YQCONFIG_IPP_CUST = IPL_AR0238CSP_EVB_FF
            							#IPL_IMX291M_IMX322LQJ_EVB_FF      
            							# Option1:IPL_OV4689_IMX322LQJ_EVB_FF
            							# Option2:IPL_FAKE  
            							
YQCONFIG_SENSOR_CUST = CMOS_AR0238CSP      
            							# Option1:CMOS_OV4689
            							# Option2:OFF  
            							
YQCONFIG_SENSOR_2_CUST = OFF      
            							# Option1:CMOS_IMX322LQJVX1
            							# Option2:OFF 

YQCONFIG_LCD_CUST = Disp_IFDSI_LCD1_XJ040WV1402C_R61408      
            							# Option1:Disp_IFDSI_LCD1_TWJ50IP008_ILI9806E
            							# Option2:OFF
YQCONFIG_LCD_WIDTH_CUST = 800    
								# Option1:1280
								#Option2:864 
								
YQCONFIG_LCD_HEIGHT_CUST = 480
								# Option1:400
								# Option2:480
								
YQCONFIG_LCD_ROTATE_180_SUPPORT = no	    
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

YQCONFIG_NETWORK_CUST = NETWORK_NONE      
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
            							# Option4:MP4
            							

#****************************************************************************************#
#  Segment: Software Config Platform Related
#****************************************************************************************#
YQCONFIG_COMB_CUST_PROJECT_NAME = U15_BASE
								# YQCONFIG_COMB_CUST_PROJECT_NAME_K40_BASE
            							# Option1:U08_BASE
            							# Option2:U15_BASE
YQCONFIG_CUSTOM_VERSION_NAME = "U15.public"
            							# Option1:.AMX
            							# Option2:.public            							

            							
YQCONFIG_ANDROID_SYSTEM_SUPPORT = yes
								# Option1:yes
            							# Option2:no            							
YQCONFIG_UART_TO_MTK_SUPPORT = yes
								# Option1:yes
            							# Option2:no
            							
YQCONFIG_UART_TO_MCU_SUPPORT = no
								# Option1:yes
            							# Option2:no  

            							
YQCONFIG_COMB_DET_BACKCAR_BY = NONE
								# Option1:GPIO
            							# Option2:UART
            							# Option3:NONE 
								
YQCONFIG_COMB_DET_LCD_SWITCH_BY = GPIO
								# Option1:GPIO
            							# Option2:UART
            							# Option3:NONE  
								 
YQCONFIG_PARKING_MODE_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no  
								
YQCONFIG_CHECK_SENSOR_2_PLUGIN_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no 
								           							          							         							          							            							
YQCONFIG_ADAS_FUNC_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no  
								          							          							         							          							            							
YQCONFIG_LED_FUNCTION_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no  
								          							          							         							          							            							
 YQCONFIG_AUDIO_MIC_SWITCH_SUPPORT = yes	    
            							# Option1:yes
            							# Option2:no   
								
 YQCONFIG_TP_EVENT_LOCK_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no   

 YQCONFIG_DEBUG_MESSAGE_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no   

YQCONFIG_PIP_VIEW_LR_SUPPORT = yes 
            							# Option1:yes
            							# Option2:no    

YQCONFIG_CUSTOM_NAMING_RULE_SUPPORT = no	    
            							# Option1:yes
            							# Option2:no    
YQCONFIG_COMB_PLAYMODE_OPTION = USBMSDCMODE	    
            							# Option1:USBMSDCMODE
            							# Option2:PLAYBACKMODE    
            							
YQCONFIG_TOUCH_LOCKFILE_OPTION = yes	    
            							# Option1:yes
            							# Option2:no  
YQCONFIG_TOUCH_SETUP_OPTION = yes	    
            							# Option1:yes
            							# Option2:no 
            							
YQCONFIG_COMB_PLAYMODE_ITEMS_ONE_PAGE_OPTION = BIGSCREEN	    
            							# Option1:BIGSCREEN
            							# Option2:SMALLSCREEN   

YQCONFIG_TOUCH_TO_MTK_PLAYSOUND_OPTION = yes
            							# Option1:yes
            							# Option2:no    
YQCONFIG_DATEPRINT_SUPPORT = yes
                                                        # Option1:yes
            							# Option2:no
YQCONFIG_HAVE_GPS_OR_NOT=yes
                                                        #Option1:yes
                                                        #Option2:no
YQCONFIG_FCWS_DISTANCE_SHOW_ON_CAR=no
                                                        #Option1:yes
                                                        #Option2:no
YQCONFIG_CHANGE_NEW_LCD_OR_NOT=no
                                                        #Option1:yes
                                                        #Option2:no
YQCONFIG_START_RECORDING_DELETE_OLD_FILE=yes
                                                         #Option1:yes
                                                         #Option2:no
