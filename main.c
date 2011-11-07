/*
 *  Project: servo_controller
 *  File: main.c
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 19.11.2010.
 *  Copyright (C) 2011 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#include "main.h"
#include "io.c"
#include "timers.c"
#include "i2c.c"

void main (void)
{
    for (uint8_t i = 0; i < SERVO_NUM; i++)
    {
        servo_target[i] = 0;
        servo_currentpos[i] = 0;
        servo_speed[i] = 0;
        speed_counter[i] = 0;
    }
    
    servo_target[0] = 128;

    io_init();
    timers_init();
    wdt_enable(WDTO_1S);
    sei();

    for(;;)
    {
        wdt_reset();
    }
}
