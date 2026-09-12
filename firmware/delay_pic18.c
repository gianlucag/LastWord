/*
*******************************************************************************
 Delay

 Implementation for the PIC18F47K42
*******************************************************************************
*/

#include "delay.h"
#include "board.h"
#include "board_pic18.h"

#ifndef _XTAL_FREQ
#error "_XTAL_FREQ must be defined"
#endif

void delay_us(uint16_t us)
{
    while (us--)
    {
        __delay_us(1);
    }
}

void delay_ms(uint16_t ms)
{
    while (ms--)
    {
        __delay_ms(1);
    }
}
