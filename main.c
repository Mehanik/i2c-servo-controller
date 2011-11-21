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
        /*wdt_reset();*/

        _delay_ms(40);
        UTILS_PORT_FLIP(LED_PORT, LED_PIN);
        _delay_us(1);
        UTILS_PORT_FLIP(LED_PORT, LED_PIN);
        for (int i = 0; i < SERVO_NUM; i++)
        {
            servo[i].speed = 0;
            servo[i].target = 128;
        }

        _delay_ms(40);
        for (int i = 0; i < SERVO_NUM; i++)
        {
            servo[i].speed = 0;
            servo[i].target = 1;
        }

        _delay_ms(40);
        for (int i = 0; i < SERVO_NUM; i++)
        {
            servo[i].speed = 0;
            servo[i].target = 255;
        }
    }
}
