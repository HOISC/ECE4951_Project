

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"


static int32_t fifo[16];        //array to hold FIFO data (global)

//FIFO function for moving average filter
int32_t push_front_pop_back(int32_t push_value)
{
    static uint32_t current_fifo_index = 0;     //FIFO Index (Private Global)
    int32_t popValue;

    popValue = fifo[current_fifo_index];        //Pop Data off FIFO
    fifo[current_fifo_index] = push_value;      //Push Data to FIFO

    if (current_fifo_index == 15) //wrap index to the beginning of FIFO if we reach the end of the FIFO
    {
        current_fifo_index = 0;
    }
    else
    {
        current_fifo_index++;
    }

    return popValue;
}

//Recursive implementation of moving average filter
int32_t MovAvgFilter(int32_t input_value)
{

    static int32_t oldSum = 0;
    static int32_t newSum;
    int32_t poppedValue;

    poppedValue = push_front_pop_back(input_value);

    newSum = oldSum + input_value - poppedValue;

    oldSum = newSum;

    return (newSum / 16);
}

