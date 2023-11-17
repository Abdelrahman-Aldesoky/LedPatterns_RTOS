/*****************LED Interface******************/
/**********Author: Abdelrahman Mohamed.**********/
/********************Ver.: 01********************/
/****************Date:17/08/2023.****************/
/************************************************/

#ifndef LED_INTERFACE_H_
#define LED_INTERFACE_H_

typedef struct
{
    u8 pattern[8];
    u16 delay;
} LedPattern;

void LED_voidInit(void);
void LED_voidCheckState(void);
void LED_voidActivatePattern(void *ptr);

#endif