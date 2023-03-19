

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/ssi.h"
#include "driverlib/pin_map.h"

#include "GlobalsAndDefines.h"

#include "MovAvgFilter.h"
#include "UARTcomm.h"

//#define PF2  (*((volatile uint32_t *)0x40025010))

void HX711SPISetup(void)
{
    //Enable SSIO and GPIOA
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI0)) {}
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {}

    //Configure GPIOA pins for SSI0
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinTypeSSI(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2);

    //Configure and enable SSI0, Clock speed and Freescale Mode
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_1, SSI_MODE_MASTER, 1000000, 16);
    SSIEnable(SSI0_BASE);

    //On the falling edge of SSI0RX from the HX711 data is ready
    //Generate an interrupt on the falling edge of SSI0RX
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);

    IntPrioritySet(INT_SSI0, 0x00); //Set the priority for receiving HX711 data to priority 0


    //Enable PORTA interrupt in the PORT and the NVIC
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_4);  //Enable SSI0RX interrupt in PORTA peripheral hardware
    IntEnable(INT_GPIOA);  //Enable PORTA interrupt in the NVIC
}

void GPIOPortA_Handler(void)
{
    uint32_t ui32Data, tempHigh, tempLow;
    int32_t raw24bitHX711value;

    //Get top 16 MSB's from HX711
    HWREG(SSI0_BASE + SSI_O_CR0) &= (0x0000FFF0);     //clear DSS, data size select, in CR0
    HWREG(SSI0_BASE + SSI_O_CR0) |= SSI_CR0_DSS_16;   //set DSS for 16 bits
    SSIDataPut(SSI0_BASE, 0x00);                      //start SPI transfer
    SSIDataGet(SSI0_BASE, &ui32Data);                 //get the 16 MSB's from SPI peripheral
    tempHigh = ui32Data << 16;                        //put the 16 MSB's into top 16 bits of tempHigh (bit loc 31-16)

    if (g_gain_is_high == true){
        //Get bottom 8 LSB's from HX711 and set gain to 128
        HWREG(SSI0_BASE + SSI_O_CR0) &= (0x0000FFF0);     //clear DSS, data size select, in CR0
        HWREG(SSI0_BASE + SSI_O_CR0) |= SSI_CR0_DSS_9;    //set DSS for 9 bits (16 + 9 = 25 = high gain)
        SSIDataPut(SSI0_BASE, 0x00);                      //start SPI transfer
        SSIDataGet(SSI0_BASE, &ui32Data);                 //get the 9 LSB's from SPI peripheral (bit loc 0 - 8)
        tempLow = ui32Data << 7;                          //move the 9 bits (8 - 0) to be next to the 16 bits (bit loc 15 - 7)
        tempLow = tempLow & 0x0000FF00;                   //Set all bits other than the 8 (bit loc 15 - 8) we need to 0
    }
    else{
        //Get bottom 8 LSB's from HX711 and set gain to 64
        HWREG(SSI0_BASE + SSI_O_CR0) &= (0x0000FFF0);     //clear DSS, data size select, in CR0
        HWREG(SSI0_BASE + SSI_O_CR0) |= SSI_CR0_DSS_11;   //set DSS for 11 bits (16 + 11 = 27 = low gain)
        SSIDataPut(SSI0_BASE, 0x00);                      //start SPI transfer
        SSIDataGet(SSI0_BASE, &ui32Data);                 //get the 11 MSB's from SPI peripheral (bit loc 0 - 8)
        tempLow = ui32Data << 5;                          //move the 11 bits (10 - 0) to be next to the 16 bits (bit loc 15 - 5)
        tempLow = tempLow & 0x0000FF00;                   //Set all bits other than the 8 (bit loc 15 - 8) we need to 0
    }
    //Put tempHigh and tempLow together and store in signed integer to build final 24 bit result
    raw24bitHX711value = tempHigh | tempLow;              //Combine the 16 MSB's and 8 LSB's.
    raw24bitHX711value = raw24bitHX711value >> 8;         //Move all 24 bits back to the beginning (bit loc 0 - 23)

    g_Weight = raw24bitHX711value;
    g_WeightFiltered = MovAvgFilter(g_Weight);


    //Clear the interrupt
    GPIOIntClear(GPIO_PORTA_BASE,GPIO_INT_PIN_4);
}

