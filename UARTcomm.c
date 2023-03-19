

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "GlobalsAndDefines.h"



// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x7F
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F


//------------UART_InChar------------
// Input: none
// Output: ASCII code for key typed
char UART0_InChar(void){
    return((char)(UARTCharGet(UART0_BASE)));
}


//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART0_OutChar(char data){
    UARTCharPut(UART0_BASE, data);
}

void UART0_OutCRLF(void){
  UART0_OutChar(CR);
  UART0_OutChar(LF);
}


//Function to get user gain setting from the keyboard
//The function does not update the global vaiable until the user presses enter
//A flag is set if a gain is being inputted
//False no new input.
//True new gain is being inputted.
void GetUARTInputGainSetting()
{
    char character;
    static char gain;

    character = UART0_InChar();

    if (character == 'H'){
        UART0_OutChar(character);
        g_gain_is_high = true;
        gain = 'H';
        g_InputFlag = true;
    }
    else if (character == 'L'){
        UART0_OutChar(character);
        g_gain_is_high = false;
        gain = 'L';
        g_InputFlag = true;
    }
    else if (character == CR){
        UART0_OutCRLF();
        UARTprintf("Gain = %c \n", gain);
        g_InputFlag = false;
    }
    else{
        UART0_OutCRLF();
        UARTprintf("Enter H (high) or L (low) followed by enter key\n");
        g_InputFlag = true;
    }
}


//Function to send out data to the UART for display
void SendUARTDataOut(void)
{

    //If there is no user input send data out to the UART
    if (g_InputFlag == false)
    {
        UARTprintf(" %d   %d\n", g_Weight, g_WeightFiltered);
    }
}


