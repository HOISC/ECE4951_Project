

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"

#include "GetWeightHX711SPI.h"
#include "CANcomm.h"
#include "DataOutTimer.h"
#include "UARTSetup.h"



void Initialization(void)
{

    /***********************
    * Bus Clock            *
    ***********************/
    //Set system clock to 80MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);



    /***********************
    * PortF                *
    ***********************/
    //Enable PORTF
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  //Enable clock on GPIO PORTF
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //Sets pins 1,2,3 as outputs



    /********************************
    *Strain Gauge Project Peripherals*
    ********************************/
    HX711SPISetup();
    DataOutTimerSetup();
    CANSetup();
    UART0Setup();



    /*************************
    * Master Interrupt Enable*
    *************************/
    IntMasterEnable();
}



