/*
*******************************************************************************
 Splash screen

 Shows name and version on screen
*******************************************************************************
*/

#include "splash.h"
#include "lcd.h"
#include "version.h"

void splash_show(void)
{
    lcd_clear();
	lcd_write_line1(FW_NAME);
	lcd_write_line2(FW_VERSION);
}
