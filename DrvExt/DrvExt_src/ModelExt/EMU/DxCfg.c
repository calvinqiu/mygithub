#include "DrvExt.h"
#include "IOInit.h"
#include "Dx.h"
#include "DxCommon.h"

//IO
/*
[PinmuxCfg.c]
pinmux

[GpioMapping.c]
cgpio x32 (Storage)
pgpio x64 (Peripheral)
sgpio x30 (SENSOR)
lgpio x28 (LCD)
dgpio x9 (Dedicated)
pwm x20
pwm_ccnt x3

[VoltageDet.c]
adc x8

[SerialComm.c]
i2c x2 x2
sif x1 x6
csi x2 x4
lvds x1 x10
spi x3
uart x2
*/

extern DX_OBJECT gDevEmbMem1;
extern DX_OBJECT gDevRamdisk0;
extern DX_OBJECT gDevCARD1;

#if (_ALL_IN_ONE_TYPE_ == _EMU_LINUX_)
extern DX_OBJECT gDevEmbMem0;
extern DX_OBJECT gDevEmbMem2;
extern DX_OBJECT gDevEmbMem3;
extern DX_OBJECT gDevEmbMem4;
extern DX_OBJECT gDevEmbMem5;
extern DX_OBJECT gDevEmbMem6;
#endif

DX_HANDLE Dx_GetObject(UINT32 DxClassType)  // Query device object
{
    UINT32 hDevice = 0;

    switch(DxClassType & DX_CLASS_MASK)
    {
    //external device
    case DX_CLASS_AUDIO_EXT:
        break;
    case DX_CLASS_SENSOR_EXT:
        break;
    case DX_CLASS_STORAGE_EXT:
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM1)
        {
            hDevice = (UINT32)(&gDevEmbMem1);
        }
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_RAMDISK0)
        {
            hDevice = (UINT32)(&gDevRamdisk0);
        }
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_CARD1)
        {
            hDevice = (UINT32)(&gDevCARD1);
        }
#if (_ALL_IN_ONE_TYPE_ == _EMU_LINUX_)
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM0)
        {
            hDevice = (UINT32)(&gDevEmbMem0);
        }
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM2)
        {
            hDevice = (UINT32)(&gDevEmbMem2);
        }
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM3)
        {
            hDevice = (UINT32)(&gDevEmbMem3);
        }
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM4)
        {
            hDevice = (UINT32)(&gDevEmbMem4);
        }
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM5)
        {
            hDevice = (UINT32)(&gDevEmbMem5);
        }
        else
        if((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM6)
        {
            hDevice = (UINT32)(&gDevEmbMem6);
        }
#endif
        break;
    case DX_CLASS_POWER_EXT:
        break;
    case DX_CLASS_INPUT_EXT:
        break;
    case DX_CLASS_STATUS_EXT:
        break;
    }

    if(hDevice == 0)
    {
        debug_msg("^R[Dx] GetObject %08x failed\r\n", DxClassType);
    }
    return (DX_HANDLE)hDevice;
}

void Dx_InitIO(void)  // Config IO for external device
{
    IO_InitIntDir();    //initial interrupt destination
    IO_InitPinmux();    //initial PINMUX config
    IO_InitGPIO();      //initial GPIO pin status
    IO_InitADC();       //initial ADC pin status
}

