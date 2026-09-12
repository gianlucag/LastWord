/*
*******************************************************************************
 LastWord

 Entry point and main loop
*******************************************************************************
*/

#include "lcd.h"
#include "input.h"
#include "delay.h"
#include "menu.h"
#include "self_test.h"
#include "splash.h"
#include "board.h"

int main(void)
{
	board_init();
	lcd_init();
	input_init();
	splash_show();
	delay_ms(2000);
    self_test_boot();
	delay_ms(2000);
	menu_run();

    return 0;
}
