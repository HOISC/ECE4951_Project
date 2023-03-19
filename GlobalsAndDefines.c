
/***********************
* Includes             *
***********************/
#include <stdint.h>
#include <stdbool.h>


/***********************
* Type Definitions     *
***********************/

/***********************
* Macros               *
***********************/

/***********************
* Defines              *
***********************/

/***********************
* Constants            *
***********************/

/***********************
* Calibrations         *
***********************/

/***********************
* Global Variables     *
***********************/

//Flag for user inputting a gain setting.
//False no new input.
//True new gain setting is being inputted.
bool g_InputFlag;

//Gain setting for HX711    When true the gain is high.
bool g_gain_is_high = true;

//Weight raw without filtering
int32_t g_Weight = 0;

//Weight after filtering
int32_t g_WeightFiltered = 0;




/***********************
* File Scope Variables *
***********************/

/***********************
* Function Prototypes  *
***********************/


/***********************
* Function Definitions *
***********************/
