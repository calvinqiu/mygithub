#include "IntDirCfg.h"
#include "interrupt.h"

const INT_DST  uiIntDirTbl =
{
    //MIPS1 interrupt destination
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_TIMER    = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SIE      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SIE2     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SIE3     = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SIE4     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_IPE      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_IME      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_DCE      = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_IFE      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_IFE2     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_DIS      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_FDE      = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_RDE      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_RHE      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_DRE      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_DAI      = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_H264     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_JPEG     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_GRAPHIC  = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_GRAPHIC2 = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_AFFINE   = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_ISE      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_TGE      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_TSMUX    = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_GPIO     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_REMOTE   = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_PWM      = FALSE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_USB      = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_USB2     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_NAND     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SDIO     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SDIO2    = FALSE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SDIO3    = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_DMA      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_ETHERNET = FALSE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SPI      = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SPI2     = FALSE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SPI3     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SPI4     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SPI5     = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SIF      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_I2C      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_I2C2     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_UART     = TRUE,

#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_UART2    = FALSE,
#else
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_UART2    = TRUE,
#endif
    //#NT#2016/03/02#David Tsai -begin
    //#NT#Assign UART3/4 to Linux application
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_UART3    = FALSE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_UART4    = FALSE,
    //#NT#2016/03/02#David Tsai -end
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_ADC      = TRUE,


    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_IDE      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_IDE2     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_DSI      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_MI       = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_HDMI     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_VX1      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_LVDS     = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_LVDS2    = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_RTC      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_WDT      = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_CG       = TRUE,
    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_CC       = TRUE,

    .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_I2C3     = TRUE,

    //MIPS2 interrupt destination
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_TIMER    = TRUE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SIE      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SIE2     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SIE3     = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SIE4     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_IPE      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_IME      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_DCE      = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_IFE      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_IFE2     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_DIS      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_FDE      = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_RDE      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_RHE      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_DRE      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_DAI      = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_H264     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_JPEG     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_GRAPHIC  = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_GRAPHIC2 = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_AFFINE   = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_ISE      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_TGE      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_TSMUX    = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_GPIO     = TRUE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_REMOTE   = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_PWM      = TRUE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_USB      = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_USB2     = TRUE,
#if (_CPU2_TYPE_ == _CPU2_LINUX_ && (_INTERSTORAGE_ == _INTERSTORAGE_SPINAND_ || _INTERSTORAGE_ == _INTERSTORAGE_SPI_))
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_NAND     = TRUE,
#else
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_NAND     = FALSE,
#endif

#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SDIO     = TRUE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SDIO2    = TRUE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SDIO3    = TRUE,
#else
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SDIO     = FALSE,
#if (_CPU2_TYPE_ == _CPU2_ECOS_)
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SDIO2    = TRUE,
#else
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SDIO2    = FALSE,
#endif
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SDIO3    = FALSE,
#endif
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_DMA      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_ETHERNET = TRUE,
#if (_CPU2_TYPE_ == _CPU2_LINUX_ && (_INTERSTORAGE_ == _INTERSTORAGE_SPINAND_ || _INTERSTORAGE_ == _INTERSTORAGE_SPI_))
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SPI      = TRUE,
#else
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SPI      = FALSE,
#endif

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SPI2     = TRUE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SPI3     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SPI4     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SPI5     = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_SIF      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_I2C      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_I2C2     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_UART     = FALSE,

#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_UART2    = TRUE,
#else
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_UART2    = FALSE,
#endif

    //#NT#2016/03/02#David Tsai -begin
    //#NT#Assign UART3/4 to Linux application
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_UART3    = TRUE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_UART4    = TRUE,
    //#NT#2016/03/02#David Tsai -end
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_ADC      = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_IDE      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_IDE2     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_DSI      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_MI       = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_HDMI     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_VX1      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_LVDS     = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_LVDS2    = FALSE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_RTC      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_WDT      = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_CG       = FALSE,
    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_CC       = TRUE,

    .int_id_dst[CC_CORE_MIPS2].Bit.bInt_ID_I2C3     = FALSE,

    //DSP interrupt destination
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_TIMER      = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SIE        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SIE2       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SIE3       = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SIE4       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IPE        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IME        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DCE        = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IFE        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IFE2       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DIS        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_FDE        = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_RDE        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_RHE        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DRE        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DAI        = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_H264       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_JPEG       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_GRAPHIC    = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_GRAPHIC2   = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_AFFINE     = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_ISE        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_TGE        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_TSMUX      = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_GPIO       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_REMOTE     = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_PWM        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_USB        = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_USB2       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_NAND       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SDIO       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SDIO2      = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SDIO3      = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DMA        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_ETHERNET   = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SPI        = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SPI2       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SPI3       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SPI4       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SPI5       = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SIF        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_I2C        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_I2C2       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_UART       = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_UART2      = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_UART3      = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_UART4      = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_ADC        = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IDE        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IDE2       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DSI        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_MI         = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_HDMI       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_VX1        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_LVDS       = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_LVDS2      = FALSE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_RTC        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_WDT        = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_CG         = FALSE,
    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_CC         = TRUE,

    .int_id_dst[CC_CORE_DSP].Bit.bInt_ID_I2C3       = FALSE,

};

void IO_InitIntDir(void)
{
    int_setConfig(INT_CONFIG_ID_INT_DESTINATION, (UINT32)&uiIntDirTbl);
}


