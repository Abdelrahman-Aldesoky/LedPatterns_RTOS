/******************LED Private*******************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:17/08/2023.****************/
/************************************************/

#ifndef LED_PRIVATE_H_
#define LED_PRIVATE_H_

#define RED_LED_IS_ON DIO_u8GetPinValue(RED_LED_PORT, RED_LED_PIN)
#define YELLOW_LED_IS_ON DIO_u8GetPinValue(YELLOW_LED_PORT, YELLOW_LED_PIN)
#define GREEN_LED_IS_ON DIO_u8GetPinValue(GREEN_LED_PORT, GREEN_LED_PIN)

#endif
