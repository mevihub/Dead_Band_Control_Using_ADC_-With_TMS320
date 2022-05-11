/*
 * main_2.c
 *
 *  Created on: 13-Apr-2022
 *      Author: Devilal
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "F2837xD_device.h"
#include "F28x_Project.h"
#include "F2837xD_Examples.h"

#include "driverlib.h"
#include "device.h"
#define EX_ADC_RESOLUTION 12

void PinMux_init_1();
void initEPWM1();
void ConfigADC(uint32_t ADC_BASE);
void initADC_SOC(void);
unsigned int EPWM_TIMER_TBPRD = 5000;
uint16_t Adc_Result_1;
float32 Scale;


void main(void)
{

    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();
    PinMux_init_1();



    ConfigADC(ADCA_BASE);
    initADC_SOC();
    initEPWM1();

    while(1)
    {

        ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER0);
        // Wait for ADCA to complete, then acknowledge flag
        while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false)
        {

        }
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
        Adc_Result_1 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);

        Scale = Adc_Result_1/4.096;

        EPwm1Regs.CMPA.bit.CMPA = Scale;    // Set compare A value
//        initEPWM1();
//        DEVICE_DELAY_US(10000);
    }

}

void PinMux_init_1()
{
    EALLOW;
    //EPWM1 -> myEPWM1 Pinmux
    GpioCtrlRegs.GPAPUD.bit.GPIO0=0; //
    GpioCtrlRegs.GPAPUD.bit.GPIO1=0;

    GpioCtrlRegs.GPAMUX1.bit.GPIO0=1; // 0=GPIO,  1=EPWM1A,  2=Resv,  3=Resv
    GpioCtrlRegs.GPAMUX1.bit.GPIO1=1; // 0=GPIO,  1=EPWM1A,  2=Resv,  3=Resv

    EDIS;

}


void initEPWM1()
{



        EALLOW;
        CpuSysRegs.PCLKCR0.bit.TBCLKSYNC=0;
        EDIS;

        // Set Compare values

//        // ------------ EPWM1A ------------------                            BUCK //

        EPwm1Regs.TBPRD = 1000;       // Set timer period 801 TBCLKs
        EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
        EPwm2Regs.TBCTR = 0x0000;
//        EPwm1Regs.CMPA.bit.CMPA = Scale;    // Set compare A value

        EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
        EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
        EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
        EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
        EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;


        EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;           // load on CTR = Zero
        EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


//        EPwm1Regs.AQCTLA.bit.PRD = AQ_SET;
//        EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
//        EPwm1Regs.AQCTLB.bit.PRD = AQ_SET;
//        EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;

        EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;
        EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
        EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;
        EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;


       EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
       EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
       EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;

       EPwm1Regs.DBRED.all = 100;
       EPwm1Regs.DBFED.all = 100;




    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC=1;
    EDIS;

}


void ConfigADC(uint32_t ADC_BASE)
{
    EALLOW;

    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);

#if(EX_ADC_RESOLUTION == 12)
    {
        ADC_setMode(ADC_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    }
#elif(EX_ADC_RESOLUTION == 16)
    {
      ADC_setMode(ADCA_BASE, ADC_RESOLUTION_16BIT, ADC_MODE_DIFFERENTIAL);
    }
#endif
    ADC_setInterruptPulseMode(ADC_BASE, ADC_PULSE_END_OF_CONV);
    ADC_enableConverter(ADC_BASE);
    DEVICE_DELAY_US(1000);
    EDIS;
}


void initADC_SOC(void)
{
#if(EX_ADC_RESOLUTION == 12)
    {
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0, 15);
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN0, 15);
    }
#elif(EX_ADC_RESOLUTION == 16)
    {
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0,64);
    }
#endif
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

}
