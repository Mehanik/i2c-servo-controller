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
    for (uint8_t i = 0; i < SERVO_NUM; i++)
    {
        servo[i].target = 0;
        servo[i].target_buf = 0;
        servo[i].position = 0;
        servo[i].speed = 0; //
        servo[i].speed_buf = 0;
        adjust_servo(i);
    }
    
    io_init();
    timers_init();
    wdt_enable(WDTO_1S);
    sei();
    run_pwm();

    for(;;)
    {
        wdt_reset();
    }
}
