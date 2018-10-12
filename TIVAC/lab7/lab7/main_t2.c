#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

// new header files for timer interrupt and gpio
#include "inc/tm4c123gh6pm.h"

// new header files for ADC
#include "driverlib/adc.h"
#include "driverlib/rom.h"

#include <stdio.h>
#include <stdlib.h>

uint32_t ui32ADC0Value[4];
volatile uint32_t ui32TempAvg;
volatile uint32_t ui32TempValueC;
volatile uint32_t ui32TempValueF;

char UART_TempValueF[24];

void UARTStrPut(char*);

void UARTIntHandler(void)
{
    uint32_t ui32Status;

    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status

    UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

    while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
    {
        // if capital R is pressed, turn on RED LED
        if(UARTCharGet(UART0_BASE) == 'R')
        {
            UARTStrPut("R");
            UARTStrPut("\r\n");

            UARTStrPut("Red LED: ON");

           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_1);

           UARTStrPut("\r\n");
        }

        // if capital R is pressed, turn off RED LED
        if(UARTCharGet(UART0_BASE) == 'r')
        {
            UARTStrPut("r");
            UARTStrPut("\r\n");

            UARTStrPut("Red LED: OFF");

           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, !GPIO_PIN_1);

           UARTStrPut("\r\n");
        }

        // if capital G is pressed, turn on GREEN LED
        if(UARTCharGet(UART0_BASE) == 'G')
        {
            UARTStrPut("G");
            UARTStrPut("\r\n");

            UARTStrPut("Green LED: ON");

           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_3);

           UARTStrPut("\r\n");
        }

        // if capital G is pressed, turn off GREEN LED
        if(UARTCharGet(UART0_BASE) == 'g')
        {
            UARTStrPut("g");
            UARTStrPut("\r\n");

            UARTStrPut("Green LED: OFF");

           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, !GPIO_PIN_3);

           UARTStrPut("\r\n");
        }

        // if capital B is pressed, turn on BLUE LED
        if(UARTCharGet(UART0_BASE) == 'B')
        {
            UARTStrPut("B");
            UARTStrPut("\r\n");

            UARTStrPut("Blue LED: ON");

           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_2);

           UARTStrPut("\r\n");
        }

        // if capital B is pressed, turn off BLUE LED
        if(UARTCharGet(UART0_BASE) == 'b')
        {
            UARTStrPut("b");
            UARTStrPut("\r\n");

            UARTStrPut("Blue LED: OFF");

           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, !GPIO_PIN_2);

           UARTStrPut("\r\n");
        }

        // if capital T is pressed
        if(UARTCharGet(UART0_BASE) == 'T')
        {
            UARTStrPut("T");
            UARTStrPut("\r\n");

            // indicate temperature command
            UARTStrPut("Temperature: ");

            // clear ADC interrupt and set processor trigger to sequence 2
            ADCIntClear(ADC0_BASE, 2);
            ADCProcessorTrigger(ADC0_BASE, 2);

            // while busy, keep looping
            while(!ROM_ADCIntStatus(ADC0_BASE, 2, false))
            {}

            // grab ADC data and compute the avg value and temps for F and C
            ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC0Value);
            ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
            ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
            ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;

            // convert integer to string
            itoa(ui32TempValueF, UART_TempValueF);

            // print temperature value
            UARTStrPut(UART_TempValueF);
            UARTStrPut("\r\n");
        }
    }
}

int main(void) {
	
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIO port for LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //enable pin for LED PF2

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    IntMasterEnable(); //enable processor interrupts
    IntEnable(INT_UART0); //enable the UART interrupt
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts

    // clear screen and display header
    UARTStrPut("\e[1;1H\e[2J");
    UARTStrPut("CPE 403 - Lab 7, Task 1 \r\n\r\n");

    while (1)
    {
    	//if (UARTCharsAvail(UART0_BASE)) UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));
    }

}

// custom made print string function
void UARTStrPut(char *str)
{
   while(*str)
      UARTCharPut(UART0_BASE, *str++);
}

// custom made integer to string function
char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

// timer1
void Timer1IntHandler(void)
{
}
