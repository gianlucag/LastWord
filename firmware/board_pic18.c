/*
*******************************************************************************
 Board

 PIC18F47K42 implementation
*******************************************************************************
*/

#include "board.h"
#include "board_pic18.h"

// clock

#pragma config FEXTOSC = OFF          // use internal oscillator
#pragma config RSTOSC  = HFINTOSC_64MHZ // power-up with 64 MHz HFINTOSC
#pragma config CLKOUTEN = OFF         // disable clock output pin
#pragma config CSWEN    = OFF         // allow runtime clock switching
#pragma config FCMEN    = OFF         // disable fail-safe clock monitor

// reset and brown-out

#pragma config MCLRE    = EXTMCLR     // MCLR pin enabled
#pragma config LPBOREN  = OFF         // disable low-power brown-out reset
#pragma config BOREN    = ON          // enable brown-out reset
#pragma config BORV     = VBOR_2P45   // brown-out threshold = 2.45 V

// miscellaneous

#pragma config ZCD      = OFF         // disable zero-cross detect
#pragma config PPS1WAY  = OFF         // PPS can be reconfigured multiple times
#pragma config STVREN   = ON          // reset on stack overflow/underflow

// watchdog

#pragma config WDTE     = OFF         // disable watchdog timer
#pragma config WDTCCS   = SC          // software-controlled watchdog clock


void board_init(void)
{
	// clock
    OSCFRQ = 0x08; // HFINTOSC = 64 MHz

	// disable all analog inputs

    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;

	// set GPIO directions

    LCD_RS_TRIS = 0;
    LCD_E_TRIS  = 0;

    LCD_D4_TRIS = 0;
    LCD_D5_TRIS = 0;
    LCD_D6_TRIS = 0;
    LCD_D7_TRIS = 0;

    BUTTON_A_TRIS = 1;
    BUTTON_B_TRIS = 1;

	// GPIOs state

    LCD_RS_LAT = 0;
    LCD_E_LAT  = 0;

    LCD_D4_LAT = 0;
    LCD_D5_LAT = 0;
    LCD_D6_LAT = 0;
    LCD_D7_LAT = 0;

	// enable internal pull ups

    BUTTON_A_WPU = 1;
    BUTTON_B_WPU = 1;

    WPUB = 0;

	// disable all interrupts
	INTCON0bits.GIE = 0;
}
