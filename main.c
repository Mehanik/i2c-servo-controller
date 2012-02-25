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
    i2c_init();
    wdt_enable(WDTO_1S);
    sei();
    run_pwm();

    flags.new_buf_ready = 0;
    flags.i2c_first_byte = 0;
    flags.servo_pd_changed = 0;
    flags.servo_minmaxpd_cnahged = 0;

    /*UTILS_DDR_CLR(D, 2);*/
    /*UTILS_PORT_SET(D, 2);*/

    for (;;)
    {

        /*if (UTILS_PIN_VALUE(D, 2) == 0)*/
        /*{*/
            /*servo[0].target += 1; */
        /*}*/

        // i2c
        if (TWCR & _BV(TWINT))
        {
            // i2c 
            switch (TWSR)
            {
                case TW_SR_SLA_ACK: // i2c start
                    flags.i2c_first_byte = 1;
                    break;
                case TW_SR_DATA_ACK: // i2c byte received
                    if (flags.i2c_first_byte)
                    {
                        i2cMemAddr = TWDR;
                        flags.i2c_first_byte = 0;
                    }
                    else
                    {
                        i2c_read();
                        i2cMemAddr ++;
                    }
                    break;
                case TW_ST_DATA_ACK:
                    i2c_write();
                    i2cMemAddr ++;
                    break;
            }

            TWCR |= _BV(TWINT);
        }

        // servo.position 
        if (UTILS_AGGL(TIFR, ITIMER) & _BV(UTILS_AGGL2(OCF, ITIMER, A)))
        {
            // Timer ITIMER compare match A
            UTILS_AGGL(TIFR, ITIMER) |= _BV(UTILS_AGGL2(OCF, ITIMER, A)); 

            for (uint8_t i = 0; i < SERVO_NUM; i ++)
            {
                if (servo[i].position != servo[i].target)
                {
                    UTILS_PORT_SET(LED_PORT, LED_PIN);
                    if (servo[i].speed == 0)
                    {
                        // Change position immediately
                        servo[i].position = servo[i].target;
                        servo_find_pd(i);
                        flags.servo_pd_changed = 1;
                    }
                    else
                    {
                        if (servo[i].speed_counter == servo[i].speed)
                        {
                            // Change position
                            if (servo[i].position < servo[i].target)
                                servo[i].position ++;
                            else
                                servo[i].position --;
                            servo_find_pd(i);
                            flags.servo_pd_changed = 1;
                            servo[i].speed_counter = 0;
                        }
                        else
                            servo[i].speed_counter ++;
                    }
                }
                else
                {
                    UTILS_PORT_CLR(LED_PORT, LED_PIN);
                }
            }

            if (flags.servo_minmaxpd_cnahged)
            {
                for (uint8_t i = 0; i < SERVO_NUM; i ++)
                {
                    servo_find_pd(i);
                }
                flags.servo_pd_changed = 1;
                flags.servo_minmaxpd_cnahged = 0;
            }

            if (flags.servo_pd_changed)
            {
                servo_sort();
                outstate_gen();
                flags.servo_pd_changed = 0;
            }
        }
        wdt_reset();
    }
}
