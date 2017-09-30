#include "Sensor.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "ae_AR0230CS_ALTM_int.h"

#define __MODULE__ AE_ALTM
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#if (ALTM_VERSION == ALTM_VER_1)
void ae_AR0230CS_ALTM_Proc(void)
{
    /*[0715][ONsemi][Joyce]*/
    /*------------------------ALTM tuning------------------------*/
    //Tuning by sensor
    SENSOR_CMD Lmin, Lmax, LogYAvg, LDarkFloor, ALTM_PowerOffset;
    UINT32 /*det1, det2,*/ lmin, PowerOffset_start, PowerOffset_stop, PowerOffset;
    double altmW;

    //sensor register define
    ALTM_PowerOffset.uiAddr = 0x2412;
    Lmin.uiAddr = 0x242A;
    Lmax.uiAddr = 0x242E;
    LogYAvg.uiAddr = 0x2432;
    LDarkFloor.uiAddr = 0x243C;


    Sensor_ReadReg(SENSOR_ID_1, &Lmin);
    Sensor_ReadReg(SENSOR_ID_1, &Lmax);
    Sensor_ReadReg(SENSOR_ID_1, &LogYAvg);

    lmin = Lmin.uiData[0];

    if (Lmax.uiData[0] - Lmin.uiData[0] > 1024 ) //non-uniform
    {
        if (Lmin.uiData[0] > 1024)
            lmin = LDarkFloor.uiData[0] = 512;
        else if (Lmin.uiData[0] > 512)
            lmin = LDarkFloor.uiData[0] = 128;
        else if (Lmin.uiData[0] > 32)
            lmin = LDarkFloor.uiData[0] = 0;
        else
            LDarkFloor.uiData[0] = 65535;
    }
    else
        LDarkFloor.uiData[0] = 65535;

    if (Lmax.uiData[0] > 62258) //95%
    {
        altmW = 1.0;
    }
    else if (Lmax.uiData[0] < 4095)
    {
        altmW = (double)Lmax.uiData[0] / (12285.0);
    }
    else
    {
        altmW = (1.0/3.0) + (2.0/3.0)*((double)Lmax.uiData[0] - 4095.0) / 58163.0;
    }

    PowerOffset_start   = 21 ;
    PowerOffset_stop    = 45 ;
    PowerOffset = (UINT32)((double)PowerOffset_start + (double)(PowerOffset_stop - PowerOffset_start) * altmW);

    if (PowerOffset > 45)
        PowerOffset = 45;

    ALTM_PowerOffset.uiData[0]= PowerOffset;

    AE_waitTrig2Event();
    //lvds_waitInterrupt(LVDS_INTERRUPT_FRMEND);

    Sensor_WriteReg(SENSOR_ID_1, &LDarkFloor);
    Sensor_WriteReg(SENSOR_ID_1, &ALTM_PowerOffset);
}
#elif (ALTM_VERSION == ALTM_VER_2)
/*--------------------------------------------------------*/

extern void AR0230_ALTM_FW_Control(void);
extern double clamp(double n, double min, double max);
extern void AR0230_SensorRegisterInit(void);
extern void AR0230_GetSensorRegisterValue(void);
extern void AR0230_GetSensorRigister(double Lr_16, double Lbar_offset_16, double Lbar_gain_16, UINT32 ALTM_damper_16, double power_offset_16);
extern void AR0230_SetSensorRigister(void);
extern int ALTM_setK1(int Lmax, int Lmin, double BV);
extern double ALTM_loGamma(int AvgLogY, double BV);
extern double BV_cal(int coarse_integration, int AvgLogY, int AGC, int HCG, int LLPCK, int MCLK, int pll_m, int pll_div, int vt_pix_div, int vt_sys_div);
extern double BV_cal_new(double EV_data, int AvgLogY);
extern double EV_data (int coarse_integration,int AGC, int HCG, int LLPCK);

//ALTM_FW Control-Joyce
/*
//Static registers
AvgLogY = R0x2432
Lmin = 0x242A
Lmax = 0x242E
//exposure registers
coarse_integration = 0x3012
AGC = 0x3060
HCG = 0x3100
LLPCK = 0x300C
//PLL registers
pll_m = 0x3030
pll_div = 0x302E
vt_pix_div = 0x302A
vt_sys_div = 0x302C
//ALTM_control registers
Lr = 0x2404
Lbar_offset = 0x240C
Lbar_gain = 0x2408
power_gain = 0x2410
power_offset = 0x2412
altm_min_factor = 0x2438
altm_max_factor = 0x243A
altm_dark_floor = 0x243C
altm_brightness_floor = 0x243C
damper = 0x2440
ALTM_HW_Contrl = 0x2400
*/

#define altm_damping_fast 63
#define altm_damping_med 15
#define altm_damping_slow 4
#define altm_min_factor 1
#define altm_max_factor 1.125
#define altm_dark_floor 63353
#define altm_brightness_floor 256
#define altm_hi_gamma 50
#define altm_BV_offset -7.5
#define PCLK 37.125
#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) > (b) ? (a) : (b))
//#define clamp(n, min, max) ((n) < (min) ? (min) : (n) > (max) ? (max) : (n))


INT32 k0 = 128;
INT32 k1 = -1;
INT32 frame_cunt = 0;

double avg_logY_pre, avg_logY_cur, Lmin_control_pre, Lmin_control_cur, Lmax_control_pre, Lmax_control_cur;

SENSOR_CMD Integration, AGC, HCG, LLPCK, pll_m, pll_div, vt_pix_div, vt_sys_div;
SENSOR_CMD IntegrationNext, AGCNext, HCGNext;
SENSOR_CMD Lmin_stat, Lmax_stat, AvgLogY_stat, Lr, Lbar_offset, Lbar_gain, ALTM_power_offset,ALTM_power_gain, ALTM_damper, ALTM_HW_control, ALTM_2DMC_control;
SENSOR_CMD ALTM_min_factor, ALTM_max_factor, ALTM_dark_floor, ALTM_brightness_floor;

//control_1: init and get sensor register value, disable ALTM HW control
void AR0230_ALTM_FW_Control_1(void)
{
    AR0230_SensorRegisterInit();
    AR0230_GetSensorRegisterValue();
    ALTM_HW_control.uiData[0] = 0;
    Sensor_WriteReg(SENSOR_ID_1, &ALTM_HW_control);
}

void AR0230_ALTM_FW_Control_2(void)
{
    IntegrationNext.uiAddr = 0x3012;
    AGCNext.uiAddr = 0x3060;
    HCGNext.uiAddr = 0x3100;
    Sensor_ReadReg(SENSOR_ID_1, &IntegrationNext);
    Sensor_ReadReg(SENSOR_ID_1, &AGCNext);
    Sensor_ReadReg(SENSOR_ID_1, &HCGNext);
}

void AR0230_ALTM_FW_Control_3(void)
{
    double E_delta_log, E_delta,E_cur, E_next, BV, La, Lr_16, Lbar_offset_16, Lbar_gain_16, power_offset_16;
    UINT32 damper;

    frame_cunt += 1;

    if (frame_cunt > 1)
    {
        avg_logY_pre = avg_logY_cur;
        Lmin_control_pre = Lmin_control_cur;
        Lmax_control_pre = Lmax_control_cur;
    }
    avg_logY_cur = (double)AvgLogY_stat.uiData[0] / 4096.0;
    Lmin_control_cur = Lmin_stat.uiData[0]*1.0;
    Lmax_control_cur = Lmax_stat.uiData[0]*1.0;


    E_cur = EV_data((int)Integration.uiData[0], (int)AGC.uiData[0], (int)HCG.uiData[0], (int)LLPCK.uiData[0]);
    E_next = EV_data((int)IntegrationNext.uiData[0], (int)AGCNext.uiData[0], (int)HCGNext.uiData[0], (int)LLPCK.uiData[0]);



    E_delta_log = log2(E_next/E_cur);
    E_delta = pow(2, E_delta_log);
    avg_logY_cur = floor(avg_logY_cur + E_delta_log);


    Lmax_control_cur = clamp(Lmax_control_cur*E_delta*altm_max_factor, altm_brightness_floor, 65535.0);
    Lmin_control_cur = clamp(Lmin_control_cur*E_delta*altm_min_factor - altm_dark_floor, 0.0, Lmax_control_cur);

    /* Damper Control */
    if (abs(E_delta_log) > 0.5)
        damper = altm_damping_fast;
    else if (abs(E_delta_log) < 0.125)
        damper = altm_damping_slow;
    else
        damper = altm_damping_med;


    if (frame_cunt < 2)
        return;

    Lmin_control_cur = (Lmin_control_pre * (64 - damper) + (Lmin_control_cur * damper)) / 64.0;
    Lmax_control_cur = (Lmax_control_pre * (64 - damper) + (Lmax_control_cur * damper)) / 64.0;
    avg_logY_cur = (avg_logY_pre*(64-damper)+(avg_logY_cur*damper))/64.0;



    BV = BV_cal_new(E_cur, (int)AvgLogY_stat.uiData[0]);

    ALTM_setK1(Lmax_stat.uiData[0], Lmin_stat.uiData[0], BV);

    La= pow(2, avg_logY_cur);
    Lr_16 = (double)(k1*k0)/ (k0+(Lmax_control_cur - Lmin_control_cur)/La);
    Lbar_offset_16 = log2(Lmin_control_cur + Lr_16)-1.0;
    Lbar_gain_16 = 1/(log2(Lmax_control_cur + Lr_16) - (Lbar_offset_16+1.0));

    power_offset_16 = ALTM_loGamma(AvgLogY_stat.uiData[0], BV);

    //DBG_DUMP("2. Lr_16:%f, Lbar_offset_16:%f, Lbar_gain_16:%f, power_offset_16:%f\n", Lr_16, Lbar_offset_16, Lbar_gain_16, power_offset_16);

    AR0230_GetSensorRigister(Lr_16, Lbar_offset_16, Lbar_gain_16, damper, power_offset_16);
#if 0
    extern int UI_ForceShowString(CHAR * pStr, PURECT pRect, BOOL bClear);
    CHAR msg[100];
    URECT rect = {10, 30, 640, 20};
    snprintf((char *)msg, sizeof(msg), "%4f %d  %4f",BV, k1, Lr_16);
    UI_ForceShowString(msg, &rect, 1);
#endif
    return;
}

double clamp(double n, double min, double max)
{
    if (n<= min)
        return min;
    else if (n> max)
        return max;
    else
        return n;
}

void AR0230_SensorRegisterInit()
{
    Integration.uiAddr = 0x3012;
    AGC.uiAddr = 0x3060;
    HCG.uiAddr = 0x3100;
    LLPCK.uiAddr = 0x300C;
    Lmin_stat.uiAddr = 0x242A;
    Lmax_stat.uiAddr = 0x242E;
    AvgLogY_stat.uiAddr = 0x2432;
    Lr.uiAddr = 0x2404;
    Lbar_offset.uiAddr = 0x240C;
    Lbar_gain.uiAddr = 0x2408;
    ALTM_power_gain.uiAddr = 0x2410;
    ALTM_power_offset.uiAddr = 0x2412;
    ALTM_damper.uiAddr = 0x2440;
    ALTM_HW_control.uiAddr = 0x2400;
    ALTM_2DMC_control.uiAddr = 0x3198;

}

void AR0230_GetSensorRegisterValue()
{
    Sensor_ReadReg(SENSOR_ID_1, &Integration);
    Sensor_ReadReg(SENSOR_ID_1, &AGC);
    Sensor_ReadReg(SENSOR_ID_1, &HCG);
    Sensor_ReadReg(SENSOR_ID_1, &LLPCK);
    Sensor_ReadReg(SENSOR_ID_1, &Lmin_stat);
    Sensor_ReadReg(SENSOR_ID_1, &Lmax_stat);
    Sensor_ReadReg(SENSOR_ID_1, &AvgLogY_stat);
}

void AR0230_GetSensorRigister(double Lr_16, double Lbar_offset_16, double Lbar_gain_16, UINT32 ALTM_damper_16, double power_offset_16)
{
    Lr.uiData[0] = (UINT32)(ceil(Lr_16));
    Lbar_offset.uiData[0]  = (UINT32)(ceil(Lbar_offset_16*4096.0));
    Lbar_gain.uiData[0]  = (UINT32)(ceil(Lbar_gain_16*4096.0));
    ALTM_damper.uiData[0]  = ALTM_damper_16;
    ALTM_power_offset.uiData[0] = (UINT32)(ceil(power_offset_16));
    ALTM_power_gain.uiData[0] = altm_hi_gamma - ALTM_power_offset.uiData[0];
}

void AR0230_SetSensorRigister(void)
{
    Sensor_WriteReg(SENSOR_ID_1, &Lr);
    Sensor_WriteReg(SENSOR_ID_1, &Lbar_offset);
    Sensor_WriteReg(SENSOR_ID_1, &Lbar_gain);
    Sensor_WriteReg(SENSOR_ID_1, &ALTM_damper);
    Sensor_WriteReg(SENSOR_ID_1, &ALTM_power_offset);
    Sensor_WriteReg(SENSOR_ID_1, &ALTM_power_gain);
}


int ALTM_setK1(int Lmax, int Lmin, double BV)
{
    UINT32 k1_max, k1_min, k1_slope, k1a, k1_damp;
    double BV_start1, BV_end1, BV_start2, BV_end2;
    double r, w1, w2;

    k1_max = 2048;
    k1_min = 512;
    k1_slope = 175;
    k1a = 0;
    k1_damp = 20;
    BV_end1 = 8.5;//1000 lux
    BV_start1 = 7.6;//500 lux
    BV_end2 = 4.3;//50 lux
    BV_start2 = -1.0;//1 lux

    r= pow(2, log2(Lmax- Lmin)-(AvgLogY_stat.uiData[0]/4096.0)/2.0);
    k1a = clamp(k1_slope*r, k1_min, k1_max);
    w1 = clamp((double)(BV_end1 - BV)/(double)(BV_end1-BV_start1),0,1);
    w2 = clamp((double)(BV_end2 - BV)/(double)(BV_end2-BV_start2),0,1);

    if (BV <= BV_start2 || BV >= BV_end1)
        k1 = k1_max;
    else if (BV <BV_end1 && BV >= BV_start1)
        k1 = (1-w1) * k1_max + w1*k1a;
    else if (BV < BV_end2 || BV >= BV_start2)
        k1 = (1-w2)*k1a + w2* k1_min;
    else
        k1 = k1a;

    return k1;
}

double ALTM_loGamma(int AvgLogY, double BV)
{
    double BV_dark = 2.0; //10 lux
    double BV_bright = 8.5; //1000 lux
    double w1 = 0.0;
    double Power_offset;
    UINT32 offset_start = 21;
    UINT32 offset_end = 42;

    if (BV <= BV_dark)
        w1 = 1;
    else if (BV < BV_bright)
        w1 = (double)(BV_bright - BV) / (double)(BV_bright - BV_dark);
    else
        w1 = 0;

    Power_offset = (UINT32)(floor((double)offset_start +(offset_end - offset_start)* (1.0-w1)));


    return Power_offset;
}

double BV_cal(int coarse_integration, int AvgLogY, int AGC, int HCG, int LLPCK, int MCLK, int pll_m, int pll_div, int vt_pix_div, int vt_sys_div)
{
    double integration, conversion_gain, coarse_gain, fine_gain, gain_time;
    double gain_metric_log, integration_metric_log, exposure_metric_log;
    double BV_1;

    //////////////////////PCLK = (double)MCLK * pll_m / (double)(pll_div*vt_pix_div*vt_sys_div);

    conversion_gain = pow(2.7, (double)HCG/4.0);
    coarse_gain = (AGC - AGC%16)/16;
    fine_gain = AGC - coarse_gain *16;
    gain_time = pow(2.0, (double)coarse_gain) * (32.0/(double)(32-fine_gain))*conversion_gain;

    integration = coarse_integration * LLPCK;

    gain_metric_log = log2(gain_time);

    integration_metric_log = log2(integration);
    exposure_metric_log = log2(PCLK) - gain_metric_log - integration_metric_log;

    BV_1 = ((double)AvgLogY/4096.0) + exposure_metric_log;

    return BV_1;
}

double BV_cal_new(double EV_data, int AvgLogY)
{
    double pclk;
    double EV_metric_log, exposure_metric_log;
    double BV_1;

    pclk = 37.125;

    EV_metric_log = log2(EV_data);

    exposure_metric_log = log2(pclk) - EV_metric_log;

    BV_1 = ((double)AvgLogY/4096.0) + exposure_metric_log + altm_BV_offset;

    return BV_1;
}


double EV_data (int coarse_integration,int AGC, int HCG, int LLPCK)
{
    double conversion_gain, coarse_gain, fine_gain, gain_time, EV_data;


    conversion_gain = pow(2.7, (double)HCG/4.0);
    coarse_gain = (AGC - AGC%16)/16;
    fine_gain = AGC - coarse_gain *16;
    gain_time = pow(2.0, (double)coarse_gain) * (32.0/(double)(32-fine_gain))*conversion_gain;

    EV_data = coarse_integration * LLPCK * gain_time;

    return EV_data;
}

#endif
