/*
*******************************************************************************
 LCD 2x16 interface

 Emulator implementation
 Console output
*******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include "lcd.h"

static char line1[17];
static char line2[17];

static void lcd_draw(void)
{
	// clear screen
    printf("\033[2J");

	// move cursor back
    printf("\033[H");

    printf("+----------------+\n");
    printf("|%-16s|\n", line1);
    printf("|%-16s|\n", line2);
    printf("+----------------+\n");

    fflush(stdout);
}

void lcd_init(void)
{
    lcd_clear();
}

void lcd_clear(void)
{
    memset(line1, ' ', 16);
    memset(line2, ' ', 16);
    line1[16] = '\0';
    line2[16] = '\0';
    lcd_draw();
}

void lcd_write_line1(const char *text)
{
    memset(line1, ' ', 16);
    strncpy(line1, text, 16);
    line1[16] = '\0';
    lcd_draw();
}

void lcd_write_line2(const char *text)
{
    memset(line2, ' ', 16);
    strncpy(line2, text, 16);
    line2[16] = '\0';
    lcd_draw();
}
