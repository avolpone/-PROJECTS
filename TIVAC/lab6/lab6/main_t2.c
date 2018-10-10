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
    volatile uint32_t DUTY_R = 0;

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
    ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);

    // set period to 1000 in order to allow gradual change of color
    ui32Period = 1000;

    ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Period);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, DUTY_R);

    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);

    ROM_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT|PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);

    while(1)
    {
        // keep looping while R LED is <90%
        while (DUTY_R < 900)
        {
            // increment duty by 0.1
            DUTY_R += 1;

            // if R LED reaches 90% duty, reset to 10%
            if(DUTY_R > 900)
                DUTY_R = 100;

            // LED change delay
            SysCtlDelay(50000);

            // apply duty to R LED
            ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, DUTY_R);
        }

        // reset R LED to 10%
        DUTY_R = 100;
    }
}
