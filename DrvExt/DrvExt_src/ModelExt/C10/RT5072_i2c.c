//////////////////////////////////////////


#include "Type.h"
#include "Debug.h"
#include "i2c.h"
#include "RT5072_i2c.h"
#include "Utility.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

/*

Base on DEMO circuit layout (provided by MS), PwrIC use:
    I2C_SDA / P_GPIO[0]*   PWR_SDA
    I2C_SCL / P_GPIO[1]*   PWR_SCL

Top:
    PIN_I2C_CFG_CH1 = 0x01,             ///< Enable channel 1 (on PGPIO[0..1])
    PIN_I2C_CFG_CH1_2ND_PINMUX = 0x02,  ///< Enable 2nd pinmux of channel 1 (on PGPIO[57..58])
    PIN_I2C_CFG_CH2 = 0x10,             ///< Enable channel 2 (on PGPIO[28..29])
    PIN_I2C_CFG_CH2_2ND_PINMUX = 0x20,  ///< Enable 2nd pinmux of channel 2 (on SGPIO[26..27])

>> This PwrIC use "I2C CH1 first pinmux" = PIN_I2C_CFG_CH1

PinmuxCfg.c
>>  {PIN_FUNC_I2C,              PIN_I2C_CFG_CH1},

Driver:
    call i2c_open()
*/

I2C_SESSION hPwrIC = I2C_TOTAL_SESSION;

#define I2C_POWER_IC   ENABLE

BOOL PwrIC_Open(void)
{
#if (I2C_POWER_IC==ENABLE)
    I2C_STS erReturn = I2C_STS_OK;
    erReturn=i2c_open(&hPwrIC);
    if(erReturn != I2C_STS_OK)
    {
        DBG_ERR("Error open I2C driver!!\r\n");
        return FALSE;
    }
    i2c_setConfig(hPwrIC, I2C_CONFIG_ID_PINMUX, I2C_PINMUX_1ST); //select to control I2C 1st device
    i2c_setConfig(hPwrIC, I2C_CONFIG_ID_MODE, I2C_MODE_MASTER);
    i2c_setConfig(hPwrIC, I2C_CONFIG_ID_BUSCLOCK, I2C_BUS_CLOCK_200KHZ);
    return TRUE;
#else
    return FALSE;
#endif
}

BOOL PwrIC_Close(void)
{
#if  (I2C_POWER_IC==ENABLE)
    if(hPwrIC == I2C_TOTAL_SESSION)
    {
        DBG_ERR("Not open I2C driver yet!!\r\n");
        return FALSE;
    }
    i2c_close(hPwrIC);
    return TRUE;
#else
    return FALSE;
#endif
}

#if  (I2C_POWER_IC==ENABLE)
static I2C_STS PwrIC_Receive(UINT32 *value, BOOL bNACK, BOOL bStop)
{
    I2C_DATA I2cData;
    I2C_BYTE I2cByte[1];
    I2C_STS ret;

    I2cData.VersionInfo = DRV_VER_96660;
    I2cData.ByteCount = I2C_BYTE_CNT_1;
    I2cData.pByte = I2cByte;
    I2cByte[0].Param = I2C_BYTE_PARAM_NONE;
    if ( bNACK == TRUE )
        I2cByte[0].Param |= I2C_BYTE_PARAM_NACK;
    if ( bStop == TRUE )
        I2cByte[0].Param |= I2C_BYTE_PARAM_STOP;
    ret = i2c_receive(&I2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }
    *value = I2cByte[0].uiValue;
    DBG_IND("i2c read = %02x\r\n", *value);
    return ret;
}

static I2C_STS PwrIC_Transmit(UINT32 value, BOOL bStart, BOOL bStop)
{
    I2C_DATA I2cData;
    I2C_BYTE I2cByte[1];
    I2C_STS ret;

    DBG_IND("i2c write = %02x\r\n", value);
    I2cData.VersionInfo = DRV_VER_96660;
    I2cData.ByteCount = I2C_BYTE_CNT_1;
    I2cData.pByte = I2cByte;

    I2cByte[0].uiValue = value & 0xff;
    I2cByte[0].Param   = I2C_BYTE_PARAM_NONE;
    if ( bStart == TRUE )
        I2cByte[0].Param |= I2C_BYTE_PARAM_START;
    if ( bStop == TRUE )
        I2cByte[0].Param |= I2C_BYTE_PARAM_STOP;
    ret = i2c_transmit(&I2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %d!!\r\n", ret);
    }
    return ret;
}
#endif

/*
Power IC 硬體 設定

CH1    5V         HDMI/Speaker
CH2    VCC3.3V    System
CH3    VCC1.15V   System
CH4    VCC1.5V    System
CH5    VCC1.8V    IR
CH6    VCC1.8V    Sensor
CH7    Back Light LED
CH8    VCC2.8     Sensor

*/

/*
當DSP Power_EN送給 Power IC 之後，Power 開啟順序為CH1 --> CH3 --> CH4 ---> CH2, 此為外部硬體已設定完成，軟體不需設定。

Power IC Init
write
0x4     0xA4      CH1 5V, CH2 3.3V
0x6     0x88      CH5 1.8V, CH6 1.8V
0x3     0xFA      CH8 2.8V
0x2     0x0F      開LCD 背光      Bit[4:0]這為控制LCD的亮度
0x2               Bit[7:5]分別控制 CH5, CH6, CH8的 Enable
0x2     0x8F      開啟IR Power


Sensor Open
write
0x2     AF        開啟Sensor 2.8V
Delay   5ms
0x2     EF        開啟Sensro 1.8V

Sensor Close
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
write
0x2    0xA0  關CH6
Delay 5mS
0x2    0x20  關閉CH5
Delay 5mS
0x2    0x0   關閉CH8

LCD backlight level
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//將0x0的Bit 5設為1
read
0x0    v
write
0x0    v|0x20
write
//由0x02 的Bit0~Bit 4控制，亮度總共有32階
0x2    0x01
0x2    0x02
  :
0x2    0x1f

Start Charge
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
write
0x07    0x42
0x08    0x14
0x09    0x36
read
0xA     預設是0 (要確認看是否有認何錯誤發生?)

Stop Charge
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
write
0x07    0x4A

*/

/*
NOTE: before send "cmd_PwrIC_init"
      need check default value:
PwrIC[0x00]=0x91 ok
PwrIC[0x01]=0x00 ok
PwrIC[0x02]=0x00 ok
PwrIC[0x03]=0xfc ok
PwrIC[0x04]=0x54 ok
PwrIC[0x05]=0xcc ok
PwrIC[0x06]=0xc4 ok
PwrIC[0x07]=0x42 ok
PwrIC[0x08]=0x14 ok
PwrIC[0x09]=0x34 ok
PwrIC[0x0a]=0x00 ok
PwrIC[0x0b]=0x02 ok
*/


UINT32 cmd_PwrIC_init[]=
{
0x7,    0x43,      // current limit maximum 1.5ma
0x4,    0xA4,      // CH1 5V, CH2 3.3V
0x6,    0x08,      // CH5 1.8V, CH6 1.8V
0x3,    0xFC,      // CH8 2.8V
//0x2,    0x0F,      // 開LCD 背光      Bit[4:0]這為控制LCD的亮度
//0x2               // Bit[7:5]分別控制 CH5, CH6, CH8的 Enable
0x2,    0x80,      // 開啟IR Power
0xff00, 0x0A,      // Delay 10 ms
0xffff, 0x00,      //end of cmd
};
UINT32 cmd_Sensor_open[]=
{
//0x2,    0xAF,       // 開啟Sensor 2.8V
//0x2,    0x6F,       // 開啟Sensor 2.8V
0xff00, 0x05,       // Delay 5 ms
//0x2,    0xEF,       // 開啟Sensro 1.8V
0xff02, 0x2,        //read [addr] to temp
0xff03, 0x1F,       //temp and with [data], then or with param // 開啟Sensro 1.8V
0xff04, 0x2,        //write temp to [addr]
0xffff, 0x00,       //end of cmd
};
UINT32 cmd_Sensor_close[]=
{
/*
0x2,    0xA0,
0xff00, 5, //delay 5 ms
0x2,    0x20,
0xff00, 5, //delay 5 ms
0x2,    0x00,
*/
0xff03, 0x0, //temp and with [data], then or with param
0xff08, 0xA0,  //temp or with [data]
0xff04, 0x2, //write temp to [addr]
0xff00, 5, //delay 5 ms
0xff03, 0x0, //temp and with [data], then or with param
0xff08, 0x20,  //temp or with [data]
0xff04, 0x2, //write temp to [addr]
0xff00, 5, //delay 5 ms
0xff03, 0x0, //temp and with [data], then or with param
0xff08, 0x00,  //temp or with [data]
0xff04, 0x2, //write temp to [addr]

0xffff, 0, //end of cmd
};
UINT32 cmd_LCD_setbl[]=  //set brightness level
{
0xff02, 0x2, //read [addr] to temp
0xff03, 0xe0, //temp and with [data], then or with param
0xff04, 0x2, //write temp to [addr]
0xffff, 0, //end of cmd
};
UINT32 cmd_LCD_getbl[]=  //get brightness level
{
0xff02, 0x2, //read [addr] to temp
0xff03, 0x1f, //temp and with [data], then or with param
0xff05, 0, //set return value = temp
0xffff, 0, //end of cmd
};

UINT32 cmd_Charge_en[]=  //charge start
{
#if 1
0xff02, 0x7,  //read [addr] to temp
0xff03, ~CHARGE_EN_MASK, //temp and with [data], then or with param
0xff04, 0x7,  //write temp to [addr]
0xffff, 0,    //end of cmd
#else
0x7,    0x42, //clear 0x08 is for start charge
0x8,    0x14, //100mA = 0x10, 500mA = 0x14
0x9,    0x36, //disable INT = 0x36, enable INT = 0x34
0xffff, 0, //end of cmd
#endif
};

/*
UINT32 cmd_Charge_stop[]=  //charge stop
{
0x7,    0x42|0x08, //0x08 is for stop charge
0xffff, 0, //end of cmd
};
*/

UINT32 cmd_Charge_setCurrent[]=  //set charge input current
{
0xff02, 0x7,  //read [addr] to temp
0xff03, ~INPUT_CURRENT_LIMIT_MASK, //temp and with [data], then or with param
0xff04, 0x7,  //write temp to [addr]
0xffff, 0,    //end of cmd
};


UINT32 cmd_Charge_geten[]=  //charge get en
{
0xff01, 0x07, //read return value at 0x07
0xffff, 0, //end of cmd
};
UINT32 cmd_Charge_getstat[]=  //charge get state
{
0xff01, 0x0A, //read return value at 0x0a
0xffff, 0, //end of cmd
};

UINT32 cmd_PwrIC_read[]=  //read [data] to ret
{
0xff06, 0, //read [data] to ret
0xffff, 0, //end of cmd
};
UINT32 cmd_PwrIC_write[]=  //write [data] with ret
{
0xff07, 0, //write [data] with ret
0xffff, 0, //end of cmd
};

BOOL PwrIC_DumpAll(void)
{
#if(I2C_POWER_IC==ENABLE)
    UINT32 retv;
    int i;
    UINT32 reg[0x0c];

    if(hPwrIC == I2C_TOTAL_SESSION)
    {
        DBG_ERR("Not open I2C driver yet!!\r\n");
        return FALSE;
    }
    i2c_lock(hPwrIC);

    PwrIC_Transmit(PWRIC_WRITE_ADDRESS, 1, 0);
    PwrIC_Transmit(0, 0, 0); //reg
    PwrIC_Transmit(PWRIC_READ_ADDRESS, 1, 0);
    PwrIC_Receive(&retv, 0, 0); reg[0x00] = retv;//data
    PwrIC_Receive(&retv, 0, 0); reg[0x01] = retv;//data
    PwrIC_Receive(&retv, 0, 0); reg[0x02] = retv;//data
    PwrIC_Receive(&retv, 0, 0); reg[0x03] = retv;//data
    PwrIC_Receive(&retv, 0, 0); reg[0x04] = retv;//data
    PwrIC_Receive(&retv, 0, 0); reg[0x05] = retv;//data
    PwrIC_Receive(&retv, 0, 0); reg[0x06] = retv;//data
    PwrIC_Receive(&retv, 0, 0); reg[0x07] = retv;//data
    PwrIC_Receive(&retv, 0, 0); reg[0x08] = retv;//data
    PwrIC_Receive(&retv, 0, 0); reg[0x09] = retv;//data
    PwrIC_Receive(&retv, 0, 0); reg[0x0a] = retv;//data
    PwrIC_Receive(&retv, 1, 1); reg[0x0b] = retv;//data
    i2c_unlock(hPwrIC);

    for(i=0x00;i<=0x0b;i++)
    {
        debug_msg("PwrIC[0x%02x]=0x%02x\r\n", i, reg[i]);
    }

    return TRUE;
#else
    return FALSE;
#endif
}


BOOL PwrIC_DoCmd(const UINT32* pCmdList, UINT32 param, UINT32* ret)
{
#if(I2C_POWER_IC==ENABLE)
    const UINT32*     pcmd;
    UINT32      temp = 0;
    BOOL bQuit = FALSE;

    if(hPwrIC == I2C_TOTAL_SESSION)
    {
        DBG_ERR("Not open I2C driver yet!!\r\n");
        return FALSE;
    }

    pcmd = pCmdList;

    i2c_lock(hPwrIC);
    while(!bQuit) //delay
    {
        if(pcmd[0] == 0xffff) //quit
        {
            bQuit = TRUE;
        }
        else if(pcmd[0] == 0xff00)
        {
            DBG_IND("---delay(%d)\r\n", pcmd[1]);
            Delay_DelayMs(pcmd[1]); //time
        }
        else if(pcmd[0] == 0xff01) //read to ret
        {
            UINT32 retv;
            PwrIC_Transmit(PWRIC_WRITE_ADDRESS, 1, 0);
            PwrIC_Transmit(pcmd[1], 0, 0); //reg
            PwrIC_Transmit(PWRIC_READ_ADDRESS, 1, 0);
            PwrIC_Receive(&retv, 1, 1); //data
            DBG_IND("Read reg =0x%x, value =0x%x\r\n",pcmd[1],retv);
            if(ret!=0)
                ret[0]=retv;
        }
        else if(pcmd[0] == 0xff02) //read to temp
        {
            PwrIC_Transmit(PWRIC_WRITE_ADDRESS, 1, 0);
            PwrIC_Transmit(pcmd[1], 0, 0); //reg
            PwrIC_Transmit(PWRIC_READ_ADDRESS, 1, 0);
            PwrIC_Receive(&temp, 1, 1); //data
            DBG_IND("Read reg =0x%x, value =0x%x\r\n",pcmd[1],temp);
        }
        else if(pcmd[0] == 0xff03) //temp and data, or param
        {
            DBG_IND("temp =0x%x,pcmd[1] = 0x%x, param=0x%x\r\n",temp,pcmd[1],param);
            temp &= pcmd[1];
            temp |= param;
            DBG_IND("temp =0x%x\r\n",temp);
        }
        else if(pcmd[0] == 0xff04) //write temp
        {
            PwrIC_Transmit(PWRIC_WRITE_ADDRESS, 1, 0);
            PwrIC_Transmit(pcmd[1], 0, 0); //reg
            PwrIC_Transmit(temp, 0, 1); //data
            DBG_IND("Write reg =0x%x, value =0x%x\r\n",pcmd[1],temp);
        }
        else if(pcmd[0] == 0xff05) //return temp
        {
            if(ret!=0)
                ret[0]=temp;
        }
        else if(pcmd[0] == 0xff06) //read [data] to ret
        {
            UINT32 retv = 0;
            if(param <= 0x0B)
            {
                PwrIC_Transmit(PWRIC_WRITE_ADDRESS, 1, 0);
                PwrIC_Transmit(param, 0, 0); //reg
                PwrIC_Transmit(PWRIC_READ_ADDRESS, 1, 0);
                PwrIC_Receive(&retv, 1, 1); //data
            }
            if(ret!=0)
                ret[0]=retv;
        }
        else if(pcmd[0] == 0xff07) //write [data] with ret
        {
            UINT32 setv = 0;
            if(ret!=0)
                setv = ret[0];
            if(param <= 0x0B)
            {
                PwrIC_Transmit(PWRIC_WRITE_ADDRESS, 1, 0);
                PwrIC_Transmit(param, 0, 0); //reg
                PwrIC_Transmit(setv, 0, 1); //data
                DBG_IND("Write reg =0x%x, value =0x%x\r\n",param,setv);
            }
        }
        else if(pcmd[0] == 0xff08) //temp or with [data]
        {
            temp |= pcmd[1];
            DBG_IND("temp =0x%x\r\n",temp);
        }
        else
        {
            PwrIC_Transmit(PWRIC_WRITE_ADDRESS, 1, 0);
            PwrIC_Transmit(pcmd[0], 0, 0); //reg
            PwrIC_Transmit(pcmd[1], 0, 1); //data
            DBG_IND("Write reg =0x%x, value =0x%x\r\n",pcmd[0],pcmd[1]);
        }
        pcmd+=2;
    }

    i2c_unlock(hPwrIC);

    return TRUE;
#else
    return FALSE;
#endif
}


