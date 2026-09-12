/*
*******************************************************************************
 Input

 Detects A and B button presses
*******************************************************************************
*/

#ifndef INPUT_H
#define INPUT_H

#define INPUT_BUTTON_A 0
#define INPUT_BUTTON_B 1

void input_init(void);
void input_wait(void); // waits until any button is pressed (blocking function)
int input_get(void); // get the last pressed button: INPUT_BUTTON_A or INPUT_BUTTON_B

#endif
