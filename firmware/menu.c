/*
*******************************************************************************
 Menu

 Main menu
*******************************************************************************
*/

#include "menu.h"

#include "lcd.h"
#include "input.h"
#include "entropy_input.h"
#include "self_test.h"
#include "about.h"

typedef enum
{
    MENU_ENTROPY_INPUT = 0,
    MENU_SELF_TEST,
    MENU_ABOUT
} menu_item_t;

#define MENU_COUNT 3

static const char *menu_items[MENU_COUNT] =
{
    "ENTROPY INPUT",
    "SELF TEST",
    "ABOUT"
};

void menu_run(void)
{
    menu_item_t current = MENU_ENTROPY_INPUT;

    while (1)
    {
        lcd_write_line1(menu_items[current]);
        lcd_write_line2("A=OK  B=NEXT");

        input_wait();

        if (input_get() == INPUT_BUTTON_B)
        {
            current++;

            if (current >= MENU_COUNT)
            {
                current = MENU_ENTROPY_INPUT;
            }
        }
        else
        {
            switch (current)
            {
                case MENU_ENTROPY_INPUT:
                    entropy_input_run();
                    break;

                case MENU_SELF_TEST:
                    self_test_run();
                    break;

                case MENU_ABOUT:
                    about_run();
                    break;
            }
        }
    }
}
