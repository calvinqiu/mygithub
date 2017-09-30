#include "i2c.h"

#define I2C_SLAVE_ADDRESS 0x12 //0010010
#define CHARGEIC_READ_ADDRESS (i2c_conv7bSARToData(I2C_SLAVE_ADDRESS) | 0x1)
#define CHARGEIC_WRITE_ADDRESS (i2c_conv7bSARToData(I2C_SLAVE_ADDRESS) | 0x0)

extern BOOL ChargeIC_Open(void);
extern BOOL ChargeIC_Close(void);
extern BOOL ChargeIC_DumpAll(void);
extern BOOL ChargeIC_DoCmd(const UINT32* pCmdList, UINT32 param, UINT32* ret);
extern I2C_STS ChargeIC_Reset(void); // Software reset charger IC.

extern UINT32 cmd_ChargeIC_init[];
extern UINT32 cmd_Charge_en[];
extern UINT32 cmd_Charge_setCurrent[];
extern UINT32 cmd_Charge_geten[];
extern UINT32 cmd_Charge_getstat[];
extern UINT32 cmd_Charge_getstat2[];
extern UINT32 cmd_ChargeIC_read[];
extern UINT32 cmd_ChargeIC_write[];
extern UINT32 cmd_Charge_en_1_2V[];
extern UINT32 cmd_Charge_en_1_8V[];

// ------ reg 0x00 ----------
// 1. charge en
#define CHARGE_EN_MASK            0x04
#define CHARGE_EN_OFF             0x00
#define CHARGE_EN_ON              0x04
// 2. charge time
#define CHARGE_TIME_MASK          0xF0
#define CHARGE_TIME_1HOUR         0x00
#define CHARGE_TIME_2HOUR         0x10
#define CHARGE_TIME_3HOUR         0x20
#define CHARGE_TIME_4HOUR         0x30
#define CHARGE_TIME_5HOUR         0x40
#define CHARGE_TIME_6HOUR         0x50
#define CHARGE_TIME_7HOUR         0x60
#define CHARGE_TIME_8HOUR         0x70
#define CHARGE_TIME_9HOUR         0x80
#define CHARGE_TIME_10HOUR        0x90
#define CHARGE_TIME_11HOUR        0xA0
#define CHARGE_TIME_12HOUR        0xB0
#define CHARGE_TIME_13HOUR        0xC0
#define CHARGE_TIME_14HOUR        0xD0
#define CHARGE_TIME_15HOUR        0xE0
#define CHARGE_TIME_16HOUR        0xF0
// 3. charge input current limit
#define INPUT_CURRENT_LIMIT_MASK  0xF0
#define INPUT_CURRENT_LIMIT_66mA  0x00
#define INPUT_CURRENT_LIMIT_300mA 0x40
#define INPUT_CURRENT_LIMIT_350mA 0x50
#define INPUT_CURRENT_LIMIT_400mA 0x60
#define INPUT_CURRENT_LIMIT_600mA 0x90
#define INPUT_CURRENT_LIMIT_700mA 0xA0
#define INPUT_CURRENT_LIMIT_800mA 0xB0
//#define INPUT_CURRENT_LIMIT_95mA  0x00
//#define INPUT_CURRENT_LIMIT_475mA 0x02
//#define INPUT_CURRENT_LIMIT_1A    0x01
//#define INPUT_CURRENT_LIMIT_1_5A  0x03


//getstat: return reg 0x02 INT_status
//#define CHARGE_STAT_MASK    0x1C
#define CHARGE_STAT_NOBATT  0x04
//#define CHARGE_STAT_FULL    0x08
#define CHARGE_POWER_GOOD   0x20

//getstat: return reg 0x04 status2
#define CHARGE_STAT_TS_FAULT       0x08
#define CHARGE_STAT_BAT_NEG        0x10
#define CHARGE_STAT_TS_FLAG_MASK   0xE0

#define BATT_TEMP_MASK      0xE0   // battery temperature mask
#define BATT_TEMP_UNDER_0   0xC0   // temperature < 0
#define BATT_TEMP_0_10      0x80   // temperature 0 ~ 10
#define BATT_TEMP_10_45     0x00   // temperature 10 ~ 45
#define BATT_TEMP_45_60     0x20   // temperature 45 ~ 60
#define BATT_TEMP_LARGE_60  0x60   // temperature > 60


