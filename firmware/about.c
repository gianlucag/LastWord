/*
*******************************************************************************
 About

 Show firmware information
*******************************************************************************
*/

#include "about.h"
#include "lcd.h"
#include "input.h"
#include "version.h"

void about_run(void)
{
    lcd_clear();

    lcd_write_line1(FW_NAME);
    lcd_write_line2(FW_VERSION);

    input_wait();
}
