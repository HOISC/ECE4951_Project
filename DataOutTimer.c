

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"

#include "GlobalsAndDefines.h"
#include "UARTcomm.h"
#include "CANcomm.h"


//Timing for data output
void Timer2A_Handler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    //Output data to the UART
    SendUARTDataOut();

    //Output data to over CAN
    send_24bitdata_CAN(g_Weight);
}

/***************************************************************
* TIMER2A is used for setting the data output rate to the UART *
***************************************************************/
void DataOutTimerSetup(void)
{
    uint32_t DataOutPeriod;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);

    //We want a 3Hz interrupt, so the timer period (counts) is: System Clock / 3 = Counts = 80MHz / 3
    DataOutPeriod = SysCtlClockGet() / 3;
    TimerLoadSet(TIMER2_BASE, TIMER_A, DataOutPeriod - 1);

    IntPrioritySet(INT_TIMER2A, 0x20); // Priority 1

    IntEnable(INT_TIMER2A);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER2_BASE, TIMER_A);
}







