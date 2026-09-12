/*
*******************************************************************************
 Input

 Emulator implementation
*******************************************************************************
*/

#include "input.h"

#include <stdio.h>
#include <ctype.h>

static int last_button;

void input_init(void)
{
}

void input_wait(void)
{
    char c;

    while (1)
    {
        printf("[A/B] > ");
        scanf(" %c", &c);

        c = (char)toupper((unsigned char)c);

        if (c == 'A')
        {
            last_button = INPUT_BUTTON_A;
            return;
        }

        if (c == 'B')
        {
            last_button = INPUT_BUTTON_B;
            return;
        }
    }
}

int input_get(void)
{
    return last_button;
}
