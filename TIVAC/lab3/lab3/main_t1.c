#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// stores LED pin data
uint8_t ui8PinData=2;

int main(void)
{
    // constant for clock delay
    double DIVISOR = 7.059;

    // ==================================================
    // Configure system clock
    // CLK = 40MHz = (400MHz PLL / (5 * 2))
    // --------------------------------------------------
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // enable periphery for GPIO_F and set pins 1 through 3 as output
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    while(1)
    {
        // set LED's on depending on pin data
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);

        /*
         * DELAY: ((seconds) / ( (1/CLK) * 3)
         *
         * For 0.425 seconds
         * ((0.425) / ( (1/40MHz) * 3) = 56.7x10^5
         *
         * So close approximation gives
         * 40MHz / x = 56.7x10^5
         * x = 7.059 => DIVISOR
         */
        SysCtlDelay(SysCtlClockGet() / DIVISOR);

        // debugging variable to confirm timing
        uint32_t SYS_DELAY = SysCtlClockGet() / DIVISOR;

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);

        // delay to turn off LEDs
        SysCtlDelay(SysCtlClockGet() / DIVISOR);

        // cycle through R, G, and B pin data
        if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
    }
}
