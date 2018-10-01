#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"


int main(void)
{
    double DUTY_75 = 1.33333;
	uint32_t ui32Period;

    // ==================================================
    // Configure system clock
    // CLK = 40MHz = (400MHz PLL / (5 * 2))
    // --------------------------------------------------
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // enable periphery for GPIO_F and set pins 1 through 3 as output
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	// enable TIMER_0 and set it to periodic configuration
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    //enable the GPIO peripheral and configure the pins connected to the switch as inputs.
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
    //enables a specific event within the GPIO to generate an interrupt.
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_0);
    //sets interrupt to rising edge on GPIO
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_INT_PIN_0, GPIO_RISING_EDGE);
    //enables the specific vector associated with GPIOF.
    IntEnable(INT_GPIOE);

	/*
	 * compute period
	 * PERIOD: (CLK / Hz) / (DUTY FRACTION.)
	 *
	 * recall 50% => 2, 20% duty => 5
	 * 75% = 3/4 of CLK so 15MHz at 2Hz
	 *
	 * (40MHz / 2Hz) / 1.3333) = 15000000
	 */
	ui32Period = (SysCtlClockGet() / 2) / DUTY_75;
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

	// interrupt for TIMER0
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();

	// enable TIMER0
	TimerEnable(TIMER0_BASE, TIMER_A);

	while(1)
	{
	}
}

void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	// write back the opposite state
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
	}
}

void PortEPin0IntHandler(void)
{
    // Clear the GPIO interrupt
    GPIOIntClear(GPIO_PORTE_BASE, GPIO_INT_PIN_0);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
    {
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
    else
    {
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
    }
}
