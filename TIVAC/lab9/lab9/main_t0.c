#include <stdint.h>
#include <stdbool.h>
#include <math.h>               // lib for math functions
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"      // floating point lib
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

// define the constant for PI as a floating point number
#ifndef M_PI
#define M_PI                    3.14159265358979323846
#endif

// depth of the data buffer for samples
#define SERIES_LENGTH 100

// floating point array that holds the series data points
float gSeriesData[SERIES_LENGTH];

// the counter for computation
int32_t i32DataCount = 0;

int main(void)
{
    // radian variable
    float fRadians;

    // enable FP data onto stack, space is reserved
    FPULazyStackingEnable();

    // floating point unit is enabled
    FPUEnable();

    // ==================================================
    // Configure system clock
    // CLK = 50MHz = (400MHz PLL / (4 * 2))
    // --------------------------------------------------
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // define radian with respect to the series length
    fRadians = ((2 * M_PI) / SERIES_LENGTH);

    // continue to compute while counter is under depth size
    while(i32DataCount < SERIES_LENGTH)
    {
        // sin function of given radian value
        gSeriesData[i32DataCount] = sinf(fRadians * i32DataCount);

        // increment counter by 1
        i32DataCount++;
    }

    // loop here forever
    while(1)
    {
    }
}
