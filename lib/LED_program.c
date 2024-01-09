/******************LED Program*******************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:17/08/2023.****************/
/************************************************/
#include "../inc/STD_TYPES.h"
#include "../inc/BIT_MATH.h"
#include "DIO_interface.h"
#include "USART_interface.h"
#include "LED_private.h"
#include "LED_config.h"
#include "LED_interface.h"
#include "FreeRTOS.h"
#include "task.h"

/*Global Variable To hold the received char or input from UART
mapping it to my array of structs index's */
u8 Global_u8ReceivedState = 0;
/*Creating array of structs named patterns
each struct can hold an array for the pattern sequence and the used delay for that pattern
you can check the struct in LED_interface.h File
user can configure the MAX_PATTERN_LENGTH from there Also its flexible and auto detects
the length on its own but you must provide the EOP flag at the end of your Pattern
user can add up to 255 patterns and its only limited by the Total Number of patterns variable.
you can change its type and put more patterns
if you have more than 8 LEDs adjust your pattern numbers accordingly
each number represent 8 bit binary or a port in my atmega32 */
LedPattern Patterns[] =
	{
		/*index 0 in my array of structs will contain the off signal default behaviour*/
		{1, {0, EOP}},

		/*index 1 in my array of structs contains flashing pattern with 500ms delay.*/
		{500, {255, 0, EOP}},

		/*index 2 in my array of structs contains moving left with 250ms delay.*/
		{250, {128, 64, 32, 16, 8, 4, 2, 1, EOP}},

		/*index 3 in my array of structs contains moving right with 250ms delay.*/
		{250, {1, 2, 4, 8, 16, 32, 64, 128, EOP}},

		/*index 4, Two LEDs converging with 300ms delay.*/
		{300, {24, 36, 66, 129, EOP}},

		/*index 5, Two LEDs diverging with 300ms delay.*/
		{250, {129, 66, 36, 24, EOP}},

		/*index 6, Ping Pong with 250ms delay.*/
		{300, {2, 8, 32, 128, 64, 16, 4, 1, EOP}},

		/*index 7, Snake effect with 300ms delay.*/
		{300, {224, 112, 56, 28, 14, 7, 131, 193, EOP}},

		/*index 8, converging and diverging with 300ms delay.*/
		{300, {129, 195, 231, 255, 255, 231, 195, EOP}}};

void LED_voidInit(void)
{
	/*Set Led Port direction as output*/
	DIO_voidSetPortDirection(LED_PORT, PORT_DIRECTION_OUTPUT);
}
/*Checking what does my UDR register in UART Holds it gets called on received data on interrupts*/
void LED_voidCheckState(void)
{
	Global_u8ReceivedState = (USART_u8ReceiveDataOnInterrupt() - '0');
}
/*My main Function that acts upon my global variable that holds what state
my lEDs should be animating.*/
void LED_voidActivatePattern(void *ptr)
{
	/*just a variable for future use if needed in case of a condition
	 *that tells me i can only show the patterns that is higher in the index and not lower.*/
	u8 static Local_u8ActiveLEDsState = 0;
	/*Iterator to go over the whole choosen pattern depends on how long is the pattern*/
	u8 static Local_u8Iterator = 0;
	/*Getting the total number of my patterns instead of hardcoding it*/
	u8 static const Local_u8TotalNoOfPatterns = sizeof(Patterns) / sizeof(Patterns[0]);

	while (1)
	{
		/*If user choose new pattern apply it under the next conditions
		 *it must be higher in the index number
		 *Pattern index must exist in my Pattern struct array
		 *user can reset by choosing the default off pattern which is index 0
		 *it also starts the new choosen pattern from the start by reseting everything to zero */
		if (VALID_LED_PATTERN)
		{
			Local_u8ActiveLEDsState = Global_u8ReceivedState;
		}
		/*for loop to display "turn on LEDs" based on the received pattern with the correct delay
		 *until you reach end of pattern (EOP) which is equals -1
		 *this is the original Approach which checked for change in patterns only when the pattern
		 *is completely displayed better for cpu usage but the requirements wants to check and change every 1ms*/
		for (Local_u8Iterator = 0; Patterns[Local_u8ActiveLEDsState].pattern[Local_u8Iterator] != EOP; Local_u8Iterator++)
		{
			DIO_voidSetPortValue(LED_PORT, Patterns[Local_u8ActiveLEDsState].pattern[Local_u8Iterator]);
			vTaskDelay(Patterns[Local_u8ActiveLEDsState].delay);
		}
	}
}