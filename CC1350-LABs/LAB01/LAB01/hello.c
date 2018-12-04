/*
 *  ======== hello.c ========
 */

/* XDC Module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Module Headers */
#include <ti/sysbios/BIOS.h>

/* Example/Board Header files */
#include "Board.h"

/*
 *  ======== main ========
 */
int main()
{
    /* Call driver init functions */
    Board_initGeneral();

    System_printf("hello world\n");

    /*
     *  normal BIOS programs, would call BIOS_start() to enable interrupts
     *  and start the scheduler and kick BIOS into gear.  But, this program
     *  is a simple sanity test and calls BIOS_exit() instead.
     */
    BIOS_exit(0);  /* terminates program and dumps SysMin output */
    return(0);
}
