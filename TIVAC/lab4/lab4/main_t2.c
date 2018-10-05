#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

// duty cycle divisor for 75% duty
double DUTY_75 = 1.3333;

uint32_t ui32Period;

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

	// disable GPIO lock for SW2
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;

	// enable TIMER0 and set it to periodic configuration
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

	// enable TIMER1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    //enable the GPIO peripheral and configure the pins connected to the switch as inputs.
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    //enables a specific event within the GPIO to generate an interrupt.
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0);
    //sets interrupt to rising edge on GPIO
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_INT_PIN_0, GPIO_RISING_EDGE);
    //enables the specific vector associated with GPIOF.
    IntEnable(INT_GPIOF);

	/*
	 * compute period
	 * PERIOD: (CLK / Hz) / (DUTY FRACTION.)
	 *
	 * recall 50% => 2, 20% duty => 5
	 * 75% = 3/4 of CLK so 15MHz at 2Hz
	 *
	 * (40MHz / 2Hz) / 1.3333) = 15000000
	 */
	ui32Period = ((SysCtlClockGet() / 2) / DUTY_75);
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

	// interrupt for TIMER0
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_BOTH);

	// interrupt for TIMER1
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    // allows interrupts to operate
	IntMasterEnable();

	// enable TIMER0
	TimerEnable(TIMER0_BASE, TIMER_A);

	// run program indefinitely
	while(1)
	{
	}
}

// timer0, default LED blinking protocol
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

// portF, SW2 button protocol
void PortFPin0IntHandler(void)
{
    // disable GPIOF interrupt
    IntDisable(INT_GPIOF);

    // clear interrupt flag
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_0);

    // force set LED to 0 as a reset
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    // turn on LED again
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);

    // disable TIMER0
    IntDisable(INT_TIMER0A);

    // assign TIMER1 to a 1.5s long value
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() * 1.5);

    // enable the timer
    TimerEnable(TIMER1_BASE, TIMER_A);

    // enable TIMER1 interrupt
    IntEnable(INT_TIMER1A);

    // program will enter Timer1IntHandler function after 1.5s
}

// timer1, LED stays on until it enters this interrupt
void Timer1IntHandler(void)
{
    // stop TIMER1
    TimerDisable(TIMER1_BASE, TIMER_A);

    // Clear the timer interrupt
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    // force set the LED to off
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    // reenable GPIO and TIMER0 interrupts
    IntEnable(INT_GPIOF);
    IntEnable(INT_TIMER0A);

    // turn off TIMER1 until button is pressed again
    IntDisable(INT_TIMER1A);

}
