#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"

int main(void)
{
    uint32_t ui32ADC0Value[4];
    volatile uint32_t ui32TempAvg;
    volatile uint32_t ui32TempValueC;
    volatile uint32_t ui32TempValueF;

    // ==================================================
    // Configure system clock
    // CLK = 40MHz = (400MHz PLL / (5 * 2))
    // --------------------------------------------------
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    // enable periphery for GPIO_F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // GPIO output enable for LEDs
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // enable ADC0, set oversampling to 64
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ROM_ADCHardwareOversampleConfigure(ADC0_BASE, 64);

    // configure ADC sequencer to 2 and assign the ADC value types
    ROM_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 3, ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
    ROM_ADCSequenceEnable(ADC0_BASE, 2);

    while(1)
    {
        // clear ADC interrupt and set processor trigger to sequence 2
        ROM_ADCIntClear(ADC0_BASE, 2);
        ROM_ADCProcessorTrigger(ADC0_BASE, 2);

       // while busy, keep looping
       while(!ROM_ADCIntStatus(ADC0_BASE, 2, false))
       {}

       // grab ADC data and compute the avg value and temps for F and C
       ROM_ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC0Value);
       ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
       ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
       ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;

       // turn blue LED if tempF is > 66, else keep it off
       if(ui32TempValueF > 66)
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x04);
       else
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
    }
}

// timer1 int
void Timer1IntHandler(void)
{
}
