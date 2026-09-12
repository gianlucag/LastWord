/*
*******************************************************************************
 Delay

 Emulator implementation
*******************************************************************************
*/

#include "delay.h"
#include <time.h>

void delay_us(uint16_t us)
{
    struct timespec ts;
    ts.tv_sec  = us / 1000000;
    ts.tv_nsec = (long)(us % 1000000) * 1000L;
    nanosleep(&ts, NULL);
}

void delay_ms(uint16_t ms)
{
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}
