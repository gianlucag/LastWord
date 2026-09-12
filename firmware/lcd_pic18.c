/*
*******************************************************************************
 LCD 2x16 interface

 Implementation for the PIC18F47K42
 LCD module is the HD44780 in 4-bit mode
*******************************************************************************
*/

#include <string.h>
#include "lcd.h"
#include "delay.h"
#include "board.h"
#include "board_pic18.h"

static char line1[17];
static char line2[17];

static void lcd_write_nibble(uint8_t nibble)
{
    LCD_D4_LAT = (nibble >> 0) & 1;
    LCD_D5_LAT = (nibble >> 1) & 1;
    LCD_D6_LAT = (nibble >> 2) & 1;
    LCD_D7_LAT = (nibble >> 3) & 1;

    LCD_E_LAT = 1;
    delay_us(1);

    LCD_E_LAT = 0;
    delay_us(50);
}

static void lcd_write_byte(uint8_t value, uint8_t rs)
{
    LCD_RS_LAT = rs;
    lcd_write_nibble(value >> 4);
    lcd_write_nibble(value & 0x0F);
}

static void lcd_command(uint8_t command)
{
    lcd_write_byte(command, 0);

    switch (command)
    {
        case 0x01:  // clear display
        case 0x02:  // return home
            delay_ms(2);
            break;

        default:
            delay_us(50);
            break;
    }
}

static void lcd_data(uint8_t data)
{
    lcd_write_byte(data, 1);
    delay_us(50);
}

static void lcd_goto(uint8_t row, uint8_t col)
{
    uint8_t address;

    switch (row)
    {
        case 0:
            address = 0x00;
            break;

        case 1:
            address = 0x40;
            break;

        default:
            return;
    }

    lcd_command(0x80 | (address + col));
}

static void lcd_draw(void)
{
    uint8_t i;

    lcd_goto(0, 0);
    for (i = 0; i < 16; i++) lcd_data(line1[i]);

    lcd_goto(1, 0);
    for (i = 0; i < 16; i++) lcd_data(line2[i]);
}

void lcd_init(void)
{
    LCD_RS_TRIS = 0;
    LCD_E_TRIS  = 0;

    LCD_D4_TRIS = 0;
    LCD_D5_TRIS = 0;
    LCD_D6_TRIS = 0;
    LCD_D7_TRIS = 0;

    LCD_RS_LAT = 0;
    LCD_E_LAT  = 0;

    delay_ms(20);

    // HD44780 initialization sequence

    lcd_write_nibble(0x03);
    delay_ms(5);

    lcd_write_nibble(0x03);
    delay_us(200);

    lcd_write_nibble(0x03);
    delay_us(200);

    lcd_write_nibble(0x02);

    lcd_command(0x28);    // 4 bit, 2 lines, 5x8 font
    lcd_command(0x08);    // display off 
    lcd_command(0x06);    // entry mode 
    lcd_command(0x0C);    // display on, cursor off

    lcd_clear();
}

void lcd_clear(void)
{
    memset(line1, ' ', 16);
    memset(line2, ' ', 16);

    line1[16] = '\0';
    line2[16] = '\0';

    lcd_command(0x01);
}

void lcd_write_line1(const char *text)
{
    uint8_t i = 0;
    memset(line1, ' ', 16);
    while (text[i] != '\0' && i < 16)
    {
        line1[i] = text[i];
        i++;
    }
    line1[16] = '\0';
    lcd_draw();
}

void lcd_write_line2(const char *text)
{
    uint8_t i = 0;
    memset(line2, ' ', 16);
    while (text[i] != '\0' && i < 16)
    {
        line2[i] = text[i];
        i++;
    }
    line2[16] = '\0';
    lcd_draw();
}
