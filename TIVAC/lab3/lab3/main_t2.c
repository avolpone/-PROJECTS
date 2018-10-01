#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// stores LED pin data
uint8_t ui8PinData;

// LED array for asserting specific color configurations
// R, G, B, RG, RB, GB, RGB
uint8_t array[7]={2,8,4,10,6,12,14};

// index counter
uint8_t i = 0;

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
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, ui8PinData);

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

        // assign pin values through predefined array for LED colors
        ui8PinData = array[i];

        // iterate through LED configuration array
        i++;

        // at the 7th index location, reset LED counter
        if(i == 7)
            i=0;
    }

}
