/*
 *  Project: servo_controller
 *  File: main.c
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 19.11.2010.
 *  Copyright (C) 2011 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#include "main.h"
#include "servo.c"
#include "io.c"
#include "timers.c"
#include "i2c.c"

void main (void)
{
    io_init();
    servo_init();
    timers_init();
    /*wdt_enable(WDTO_1S);*/
    sei();
    run_pwm();

    for(;;)
    {

        for (uint8_t i = 0; i < 7; i++)
        {
            servo[i].target = 1;
        }

        /*for (uint8_t i = 4; i < 7; i++)*/
        /*{*/
            /*servo[i].target = 128;*/
        /*}*/
        
        /*wdt_reset();*/
    }
}
