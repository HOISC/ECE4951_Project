
#ifndef _GLOBALSANDDEFINES_H_
#define _GLOBALSANDDEFINES_H_

/***********************
* Includes             *
***********************/
#include <stdint.h>
#include <stdbool.h>
/***********************
* Public Types         *
***********************/

/***********************
* Public Macros        *
***********************/


/***********************
* Public Defines       *
***********************/

//PORTF LED pins
#define PF2  (*((volatile uint32_t *)0x40025010))
#define PF3  (*((volatile uint32_t *)0x40025020))
#define PF4  (*((volatile uint32_t *)0x40025040))

#define receiver_address 0x17FF00F0

//UART data output rate in Hz
#define UARTDataOutRate 4

/***********************
* Public Constants     *
***********************/


/***********************
* Public Variables     *
***********************/

//Flag for user inputting a gain setting.
//False no new input.
//True new gain setting is being inputted.
extern bool g_InputFlag;

//Gain setting for HX711    When true the gain is high.
extern bool g_gain_is_high;

//Weight raw without filtering
extern int32_t g_Weight;

//Weight after filtering
extern int32_t g_WeightFiltered;



/***********************
* Public Functions     *
***********************/


#endif /* _GLOBALSANDDEFINES_H_ */

