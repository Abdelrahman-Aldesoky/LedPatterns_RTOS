/******************LED Program*******************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:17/08/2023.****************/
/************************************************/
#define __DELAY_BACKWARD_COMPATIBLE__
#include "../inc/STD_TYPES.h"
#include "../inc/BIT_MATH.h"
#include "DIO_interface.h"
#include "LED_private.h"
#include "LED_config.h"
#include "LED_interface.h"
#include "USART_interface.h"
#include "USART_private.h"
#include "FreeRTOS.h"
#include "task.h"
#include <util/delay.h>

//global variable for the received number from usart
u8 CurrentLedState = 0;
// array of structs for my patterns and delay for each one
LedPattern Patterns[] =
	{
		{{255, 0, 255, 0, 255, 0, 255, 0}, 500},		 // 0 Pattern array then delay
		{{128, 64, 32, 16, 8, 4, 2, 1}, 250},			 // 1
		{{1, 2, 4, 8, 16, 32, 64, 128}, 250},			 // 2
		{{129, 66, 36, 24, 129, 66, 36, 24}, 300},		 // 3
		{{24, 36, 66, 129, 24, 36, 66, 129}, 300},		 // 4
		{{2, 8, 32, 128, 64, 16, 4, 1}, 250},			 // 5
		{{224, 112, 56, 28, 14, 7, 131, 193}, 300},		 // 6
		{{129, 195, 231, 255, 255, 231, 195, 129}, 300}, // 7
};

void LED_voidInit(void)
{
	/*Set Led Port direction as output*/
	DIO_voidSetPortDirection(PORTA_REG, PORT_DIRECTION_OUTPUT);
}

void LED_voidCheckState(void)
{
	CurrentLedState = (UDR - '0');
}

void LED_voidActivatePattern(void *ptr)
{
	while (1)
	{
		u8 static state = 0;
		// if(state<=CurrentLedState)
		// {
		if (CurrentLedState < 8 && CurrentLedState >= 0)
		{
			state = CurrentLedState;
		}
		// }
		for (u8 i = 0; i < 8; i++)
		{
			DIO_voidSetPortValue(LED_PORT, Patterns[state].pattern[i]);
			_delay_ms(Patterns[state].delay);
		}
	}
	vTaskDelay(5);
}
