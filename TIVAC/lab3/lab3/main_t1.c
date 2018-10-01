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
         * ((0.425) / ( (1/CLK) * 3) = 56.7x10^5
         *
         * So close approximation gives
         * 40MHz / 7 = 57.1x10^5
         */
        SysCtlDelay(SysCtlClockGet() / 7);

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);

        // delay to turn off LEDs
        SysCtlDelay(SysCtlClockGet() / 7);

        // cycle through R, G, and B pin data
        if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
    }
}
