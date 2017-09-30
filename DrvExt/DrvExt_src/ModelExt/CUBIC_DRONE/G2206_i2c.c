//////////////////////////////////////////


#include "Type.h"
#include "Debug.h"
#include "i2c.h"
#include "G2206_i2c.h"
#include "Utility.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

/*

Base on DEMO circuit layout (provided by MS), ChargeIC use:
    I2C_SDA / P_GPIO[0]*   PWR_SDA
    I2C_SCL / P_GPIO[1]*   PWR_SCL

Top:
    PIN_I2C_CFG_CH1 = 0x01,             ///< Enable channel 1 (on PGPIO[0..1])
    PIN_I2C_CFG_CH1_2ND_PINMUX = 0x02,  ///< Enable 2nd pinmux of channel 1 (on PGPIO[57..58])
    PIN_I2C_CFG_CH2 = 0x10,             ///< Enable channel 2 (on PGPIO[28..29])
    PIN_I2C_CFG_CH2_2ND_PINMUX = 0x20,  ///< Enable 2nd pinmux of channel 2 (on SGPIO[26..27])

>> This ChargeIC use "I2C CH1 first pinmux" = PIN_I2C_CFG_CH1

PinmuxCfg.c
>>  {PIN_FUNC_I2C,              PIN_I2C_CFG_CH1},

Driver:
    call i2c_open()
*/

I2C_SESSION hChargeIC = I2C_TOTAL_SESSION;

#define I2C_CHARGE_IC   ENABLE

// Software reset charger IC.
I2C_STS ChargeIC_Reset(void)
{
    return I2C_STS_OK;
}

BOOL ChargeIC_Open(void)
{
#if (I2C_CHARGE_IC==ENABLE)
    I2C_STS erReturn = I2C_STS_OK;
    erReturn=i2c_open(&hChargeIC);
    if(erReturn != I2C_STS_OK)
    {
        DBG_ERR("Error open I2C driver!!\r\n");
    }
    i2c_setConfig(hChargeIC, I2C_CONFIG_ID_PINMUX, I2C_PINMUX_1ST); //select to control I2C 1st device
    i2c_setConfig(hChargeIC, I2C_CONFIG_ID_MODE, I2C_MODE_MASTER);
    i2c_setConfig(hChargeIC, I2C_CONFIG_ID_BUSCLOCK, I2C_BUS_CLOCK_200KHZ); //G2206 datasheet says: up to 400KHZ

    return TRUE;
#else
    return FALSE;
#endif
}

BOOL ChargeIC_Close(void)
{
#if  (I2C_CHARGE_IC==ENABLE)
    if(hChargeIC == I2C_TOTAL_SESSION)
    {
        DBG_ERR("Not open I2C driver yet!!\r\n");
        return FALSE;
    }
    i2c_close(hChargeIC);
    return TRUE;
#else
    return FALSE;
#endif
}

#if  (I2C_CHARGE_IC==ENABLE)
static I2C_STS ChargeIC_Receive(UINT32 *value, BOOL bNACK, BOOL bStop)
{
    I2C_DATA I2cData;
    I2C_BYTE I2cByte[1];
    I2C_STS ret;

    I2cData.VersionInfo = DRV_VER_96650;
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

static I2C_STS ChargeIC_Transmit(UINT32 value, BOOL bStart, BOOL bStop)
{
    I2C_DATA I2cData;
    I2C_BYTE I2cByte[1];
    I2C_STS ret;

    DBG_IND("i2c write = %02x\r\n", value);
    I2cData.VersionInfo = DRV_VER_96650;
    I2cData.ByteCount = I2C_BYTE_CNT_1;
    I2cData.pByte = I2cByte;
    I2cByte[0].uiValue = value & 0xff;
    I2cByte[0].Param = I2C_BYTE_PARAM_NONE;
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

#if 1
//A6 b7..4: 0010 -> CH5, 1.2V
//A6 b3..0: 1000 -> LDO7, 1.8V
//A3 b3..0: 1000 -> LDO8, 1.8V
UINT32 cmd_ChargeIC_init[]=
{
0x03,   0xF8,
//0x05,   0xCD, // Fine tune 1.1V voltage
0x06,   0x28,
0xff00, 1,
0xffff, 0, //end of cmd
};
#endif

UINT32 cmd_Charge_en[]=  //charge start
{
#if 1
0xff02, 0x0,  //read [addr] to temp
0xff03, ~CHARGE_EN_MASK, //temp and with [data], then or with param
0xff04, 0x0,  //write temp to [addr]
0xffff, 0,    //end of cmd
#else
0x7,    0x42, //clear 0x08 is for start charge
0x8,    0x14, //100mA = 0x10, 500mA = 0x14
0x9,    0x36, //disable INT = 0x36, enable INT = 0x34
0xffff, 0, //end of cmd
#endif
};

#if 0
UINT32 cmd_Charge_stop[]=  //charge stop
{
0x7,    0x42|0x08, //0x08 is for stop charge
0xffff, 0, //end of cmd
};
#endif


UINT32 cmd_Charge_setCurrent[]=  //set charge input current
{
0xff02, 0x06,  //read [addr] to temp
0xff03, ~INPUT_CURRENT_LIMIT_MASK, //temp and with [data], then or with param
0xff04, 0x06,  //write temp to [addr]
0xffff, 0,    //end of cmd
};


UINT32 cmd_Charge_geten[]=  //charge get en
{
0xff01, 0x00, //read return value at 0x00
0xffff, 0, //end of cmd
};
UINT32 cmd_Charge_getstat[]=  //charge get state
{
0xff01, 0x02, //read return value at 0x0a
0xffff, 0, //end of cmd
};

UINT32 cmd_Charge_getstat2[]=  //charge get status2 reg.
{
0xff01, 0x04, //read return value at 0x02
0xffff, 0, //end of cmd
};

UINT32 cmd_ChargeIC_read[]=  //read [data] to ret
{
0xff06, 0, //read [data] to ret
0xffff, 0, //end of cmd
};

UINT32 cmd_ChargeIC_write[]=  //write [data] with ret
{
0xff07, 0, //write [data] with ret
0xffff, 0, //end of cmd
};
// A2 b7: 1 -> CH5 enable
UINT32 cmd_Charge_en_1_2V[]=  //Enable 1.2V for CMOS sensor
{
0xff02, 0x02,  //read [addr] to temp
0xff03, ~0x80, //temp and with [data], then or with param
0xff04, 0x02,  //write temp to [addr]
0xffff, 0,    //end of cmd
};
// A2 b6: 1 -> Ch7 enable
// A2 b5: 1 -> LDO8 enable
UINT32 cmd_Charge_en_1_8V[]=  //Enable 1.8V for CMOS sensor
{
0xff02, 0x02,  //read [addr] to temp
0xff03, ~0x60, //temp and with [data], then or with param
0xff04, 0x02,  //write temp to [addr]
0xffff, 0,    //end of cmd
};


BOOL ChargeIC_DumpAll(void)
{
#if(I2C_CHARGE_IC==ENABLE)
    int i;
    UINT32 reg[0x0c];

    if(hChargeIC == I2C_TOTAL_SESSION)
    {
        DBG_ERR("Not open I2C driver yet!!\r\n");
        return FALSE;
    }
    i2c_lock(hChargeIC);
    UINT32 retv = 0;
    ChargeIC_Transmit(CHARGEIC_WRITE_ADDRESS, 1, 0);
    ChargeIC_Transmit(0, 0, 0); //reg
    ChargeIC_Transmit(CHARGEIC_READ_ADDRESS, 1, 0);
    ChargeIC_Receive(&retv, 0, 0); reg[0x00] = retv;//data
    ChargeIC_Receive(&retv, 0, 0); reg[0x01] = retv;//data
    ChargeIC_Receive(&retv, 0, 0); reg[0x02] = retv;//data
    ChargeIC_Receive(&retv, 0, 0); reg[0x03] = retv;//data
    ChargeIC_Receive(&retv, 0, 0); reg[0x04] = retv;//data
    ChargeIC_Receive(&retv, 0, 0); reg[0x05] = retv;//data
    ChargeIC_Receive(&retv, 0, 0); reg[0x06] = retv;//data
    ChargeIC_Receive(&retv, 0, 0); reg[0x07] = retv;//data
    ChargeIC_Receive(&retv, 0, 0); reg[0x08] = retv;//data
    ChargeIC_Receive(&retv, 0, 0); reg[0x09] = retv;//data
    ChargeIC_Receive(&retv, 1, 1); reg[0x0a] = retv;//data
    i2c_unlock(hChargeIC);
    for(i=0x00;i<=0x0a;i++)
    {
    debug_msg("ChargeIC[0x%02x]=0x%02x\r\n", i, reg[i]);
    }

    return TRUE;
#else
    return FALSE;
#endif
}


BOOL ChargeIC_DoCmd(const UINT32* pCmdList, UINT32 param, UINT32* ret)
{
#if(I2C_CHARGE_IC==ENABLE)
    const UINT32*     pcmd;
    UINT32      temp = 0;
    BOOL bQuit = FALSE;

    if(hChargeIC == I2C_TOTAL_SESSION)
    {
        DBG_ERR("Not open I2C driver yet!!\r\n");
        return FALSE;
    }
    pcmd = pCmdList;

    i2c_lock(hChargeIC);
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
            UINT32 retv = 0;
            ChargeIC_Transmit(CHARGEIC_WRITE_ADDRESS, 1, 0);
            ChargeIC_Transmit(pcmd[1], 0, 0); //reg
            ChargeIC_Transmit(CHARGEIC_READ_ADDRESS, 1, 0);
            ChargeIC_Receive(&retv, 1, 1); //data
            DBG_IND("Read reg =0x%x, value =0x%x\r\n",pcmd[1],retv);
            if(ret!=0)
                ret[0]=retv;
        }
        else if(pcmd[0] == 0xff02) //read to temp
        {
            ChargeIC_Transmit(CHARGEIC_WRITE_ADDRESS, 1, 0);
            ChargeIC_Transmit(pcmd[1], 0, 0); //reg
            ChargeIC_Transmit(CHARGEIC_READ_ADDRESS, 1, 0);
            ChargeIC_Receive(&temp, 1, 1); //data
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
            ChargeIC_Transmit(CHARGEIC_WRITE_ADDRESS, 1, 0);
            ChargeIC_Transmit(pcmd[1], 0, 0); //reg
            ChargeIC_Transmit(temp, 0, 1); //data
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
            if(param <= 0x0A)
            {
                ChargeIC_Transmit(CHARGEIC_WRITE_ADDRESS, 1, 0);
                ChargeIC_Transmit(param, 0, 0); //reg
                ChargeIC_Transmit(CHARGEIC_READ_ADDRESS, 1, 0);
                ChargeIC_Receive(&retv, 1, 1); //data
            }
            if(ret!=0)
                ret[0]=retv;
        }
        else if(pcmd[0] == 0xff07) //write [data] with ret
        {
            UINT32 setv = 0;
            if(ret!=0)
                setv = ret[0];
            if(param <= 0x0A)
            {
            ChargeIC_Transmit(CHARGEIC_WRITE_ADDRESS, 1, 0);
            ChargeIC_Transmit(param, 0, 0); //reg
            ChargeIC_Transmit(setv, 0, 1); //data
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
            ChargeIC_Transmit(CHARGEIC_WRITE_ADDRESS, 1, 0);
            ChargeIC_Transmit(pcmd[0], 0, 0); //reg
            ChargeIC_Transmit(pcmd[1], 0, 1); //data
            DBG_IND("Write reg =0x%x, value =0x%x\r\n",pcmd[0],pcmd[1]);
        }
        pcmd+=2;
    }
    i2c_unlock(hChargeIC);

    return TRUE;
#else
    return FALSE;
#endif
}


