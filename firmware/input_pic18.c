/*
*******************************************************************************
 Input

 Implementation for the PIC18F47K42
*******************************************************************************
*/

#include "input.h"
#include "delay.h"
#include "board.h"
#include "board_pic18.h"

static uint8_t last_button;

void input_init(void)
{
    BUTTON_A_TRIS = 1;
    BUTTON_B_TRIS = 1;
}

void input_wait(void)
{
    while (1)
    {
        if (BUTTON_A_PORT == 0)
        {
            delay_ms(20);

            if (BUTTON_A_PORT == 0)
            {
                while (BUTTON_A_PORT == 0);
				delay_ms(20);
                last_button = INPUT_BUTTON_A;
                return;
            }
        }

        if (BUTTON_B_PORT == 0)
        {
            delay_ms(20);

            if (BUTTON_B_PORT == 0)
            {
                while (BUTTON_B_PORT == 0);
				delay_ms(20);
                last_button = INPUT_BUTTON_B;
                return;
            }
        }
    }
}

int input_get(void)
{
    return last_button;
}
