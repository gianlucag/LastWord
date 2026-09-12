/*
*******************************************************************************
 Self test

 Run the tests and show results
*******************************************************************************
*/

#include "self_test.h"
#include "lcd.h"
#include "input.h"
#include "delay.h"
#include "tests.h"

static void self_test(void)
{
    lcd_clear();
    lcd_write_line1("SELF TEST: ...");
    delay_ms(300);

    if (!test_run())
    {
        lcd_clear();

        lcd_write_line1("SELF TEST: FAIL");
        while (1)
        {
            lcd_write_line2("DO NOT USE");
            delay_ms(1000);

            lcd_write_line2("CHECK FIRMWARE");
            delay_ms(1000);
        }
    }

    lcd_clear();
    lcd_write_line1("SELF TEST: OK");
}

void self_test_boot(void)
{
    self_test();
}

void self_test_run(void)
{
    self_test();
    input_wait();
}
