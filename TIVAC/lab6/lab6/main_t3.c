#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"


#define PWM_FREQUENCY 55

int main(void)
{
    volatile uint32_t ui32Period;
    volatile uint32_t ui32PWMClock;
    volatile uint32_t DUTY_R;
    volatile uint32_t DUTY_G;
    volatile uint32_t DUTY_B;

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);
    ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
    ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);

    ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    ui32Period = 100;

    ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Period);
    ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Period);

    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, DUTY_R);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, DUTY_B);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, DUTY_G);

    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    ROM_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT|PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);

    while(1)
    {
        // loop through R LED starting at 90% and ending at 10%
        for(DUTY_R = 90; DUTY_R >= 10; DUTY_R = DUTY_R - 1)
        {
            SysCtlDelay(1000000);

            // loop through B LED starting at 90% and ending at 10%
            for(DUTY_B = 90; DUTY_B >= 10; DUTY_B = DUTY_B - 1)
            {
                SysCtlDelay(1000000);

                // loop through G LED starting at 90% and ending at 10%
                for(DUTY_G = 90; DUTY_G >= 10; DUTY_G = DUTY_G - 1)
                {
                    SysCtlDelay(1000000);

                    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, DUTY_R);
                    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, DUTY_B);
                    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, DUTY_G);
                }
            }
        }

        // reset all RGB LEDs to 90% duty
        DUTY_R = 90;
        DUTY_G = 90;
        DUTY_B = 90;

    }
}
