/*
*******************************************************************************
 Board

 Board initialization
*******************************************************************************
*/

#ifndef BOARD_PIC18_H
#define BOARD_PIC18_H

#include <xc.h>

// OSC

#define _XTAL_FREQ 64000000UL

// LCD

#define LCD_RS_LAT      LATBbits.LATB3
#define LCD_RS_TRIS     TRISBbits.TRISB3

#define LCD_E_LAT       LATBbits.LATB1
#define LCD_E_TRIS      TRISBbits.TRISB1

#define LCD_D4_LAT      LATDbits.LATD6
#define LCD_D4_TRIS     TRISDbits.TRISD6

#define LCD_D5_LAT      LATDbits.LATD5
#define LCD_D5_TRIS     TRISDbits.TRISD5

#define LCD_D6_LAT      LATDbits.LATD4
#define LCD_D6_TRIS     TRISDbits.TRISD4

#define LCD_D7_LAT      LATCbits.LATC7
#define LCD_D7_TRIS     TRISCbits.TRISC7

// buttons

#define BUTTON_A_PORT   PORTDbits.RD0
#define BUTTON_A_TRIS   TRISDbits.TRISD0
#define BUTTON_A_WPU    WPUDbits.WPUD0

#define BUTTON_B_PORT   PORTDbits.RD1
#define BUTTON_B_TRIS   TRISDbits.TRISD1
#define BUTTON_B_WPU    WPUDbits.WPUD1

#endif
