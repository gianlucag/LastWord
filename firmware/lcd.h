/*
*******************************************************************************
 LCD 2x16 interface

 High level api for the 2x16 display
*******************************************************************************
*/

#ifndef LCD_H
#define LCD_H

void lcd_init(void);
void lcd_clear(void);
void lcd_write_line1(const char *text);
void lcd_write_line2(const char *text);

#endif
