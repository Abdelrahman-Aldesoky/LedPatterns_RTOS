/*****************USART_PROGRAM******************/
/********Authors: Amira Magdy,Abdelrahman********/
/******************Ver.: 0.0.1.******************/
/****************Date:13/09/2023.****************/
/************************************************/

#include "../inc/STD_TYPES.h"
#include "../inc/BIT_MATH.h"
#include "DIO_interface.h"
#include "GIE_interface.h"
#include "USART_private.h"
#include "USART_config.h"
#include "USART_interface.h"

static void (*Global_ptrtofunc)(void)=NULL;

void USART_voidInit(void)
{
	GIE_voidEnable();
	UCSRB |= (1 << UCSRB_RXCIE) | (1 << UCSRB_RXEN) | (1 << UCSRB_TXEN); // Turn on the transmission reception ..
	// circuitry and receiver interrupt
	UCSRC |= (1 << UCSRC_URSEL) | (1 << UCSRC_UCSZ0) | (1 << UCSRC_UCSZ1); // Use 8-bit character sizes
	UBRRL = 51;															   // Load lower 8-bits of the baud rate value..
	// into the low byte of the UBRR register
	UBRRH = 0; // Load upper 8-bits of the baud rate value..
	// into the high byte of the UBRR register
	// Set Frame Format -> 8 data, 1 stop, No Parity
}

void USART_voidSend(u8 Copy_u8Data)
{
	// Wait until transmission Register Empty
	while ((GET_BIT(UCSRA, UCSRA_UDRE)) == 0X00)
		;

	// while((UCSRA&0x20) == 0x00);
	UDR = Copy_u8Data;
}

u8 USART_voidReceive(void)
{
	u8 ReceivedData = 0;
	// Wait until Reception Complete
	while ((GET_BIT(UCSRA, UCSRA_RXC)) == 0)
		;
	// while((UCSRA&0x80) == 0x00);
	ReceivedData = UDR;

	/* Clear Flag */
	SET_BIT(UCSRA, UCSRA_RXC);
	return ReceivedData;
}

u8 USART_voidSetCallBack(void (*Copy_ptrtofunc)(void))
{
	u8 Local_u8Errorstate = OK;
	if (Copy_ptrtofunc != NULL)
	{
		Global_ptrtofunc = Copy_ptrtofunc;
	}
	else
	{
		Local_u8Errorstate = NOK;
	}
	return Local_u8Errorstate;
}

void __vector_13(void) __attribute__((signal));

void __vector_13(void)
{
	if (Global_ptrtofunc != NULL)
	{
		Global_ptrtofunc();
	}
}