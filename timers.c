/*
 *  Project: servo_controller 
 *  File: timers.c - 
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 12.02.2011
 *  Copyright (C) 2011 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#include "main.h" 

/*
 * TIMER_INT, compare match A
 * triggered with SERVO_PWMFREQ frequency
 */
ISR(UTILS_TIMERCOMP_VECT(TIMER_INT, A))
{
    // Turn off compare match interruption A
    UTILS_BIT_CLR(UTILS_AGGL(TIMSK, TIMER_INT), UTILS_AGGL2(OCIE, TIMER_INT, A));

    /* Servo PWM counter overflow */
    if (pwm_counter == 0)
    {
        /* Copy servo targets and speeds from buffer */
        for (int i = 0; i < SERVO_NUM; i++)
        {
            servo_currentpos[i] = servo_currentpos_buf[i];
            servo_target[i] = servo_target_buf[i];
            servo_speed[i] = servo_speed_buf[i];
        }

        /* Set all servo-outputs to 1, except PWM duty cycle == 0 */
        if (servo_currentpos[0] != 0)
            UTILS_PORT_SET(SERVO0_PORT, SERVO0_PIN);
        if (servo_currentpos[1] != 0)
            UTILS_PORT_SET(SERVO1_PORT, SERVO1_PIN);
        if (servo_currentpos[2] != 0)
            UTILS_PORT_SET(SERVO2_PORT, SERVO2_PIN);
        if (servo_currentpos[3] != 0)
            UTILS_PORT_SET(SERVO3_PORT, SERVO3_PIN);
        if (servo_currentpos[4] != 0)
            UTILS_PORT_SET(SERVO4_PORT, SERVO4_PIN);
        if (servo_currentpos[5] != 0)
            UTILS_PORT_SET(SERVO5_PORT, SERVO5_PIN);
        if (servo_currentpos[6] != 0)
            UTILS_PORT_SET(SERVO6_PORT, SERVO6_PIN);
        if (servo_currentpos[7] != 0)
            UTILS_PORT_SET(SERVO7_PORT, SERVO7_PIN);
    }

    if (servo_currentpos[0] == pwm_counter)
        UTILS_PORT_CLR(SERVO0_PORT, SERVO0_PIN);
    if (servo_currentpos[1] == pwm_counter)
        UTILS_PORT_CLR(SERVO1_PORT, SERVO1_PIN);
    if (servo_currentpos[2] == pwm_counter)
        UTILS_PORT_CLR(SERVO2_PORT, SERVO2_PIN);
    if (servo_currentpos[3] == pwm_counter)
        UTILS_PORT_CLR(SERVO3_PORT, SERVO3_PIN);
    if (servo_currentpos[4] == pwm_counter)
        UTILS_PORT_CLR(SERVO4_PORT, SERVO4_PIN);
    if (servo_currentpos[5] == pwm_counter)
        UTILS_PORT_CLR(SERVO5_PORT, SERVO5_PIN);
    if (servo_currentpos[6] == pwm_counter)
        UTILS_PORT_CLR(SERVO6_PORT, SERVO6_PIN);
    if (servo_currentpos[7] == pwm_counter)
        UTILS_PORT_CLR(SERVO7_PORT, SERVO7_PIN);

    /* Smooth servo rotation */
    if (pwm_counter % 64 == 0)
        for (int i = 0; i < SERVO_NUM; i++)
        {
            if (servo_currentpos_buf[i] != servo_target[i])
            {
                if (servo_speed[i] == 0)
                {
                    /* immediate action */
                    servo_currentpos_buf[i] = servo_target[i];
                    speed_counter[i] = 1;
                }
                else
                    if (speed_counter[i] == servo_speed[i])
                    {
                        if (servo_target[i] > servo_currentpos[i])
                            servo_currentpos[i]++;
                        else 
                            servo_currentpos[i]--;
                        speed_counter[i] = 1;
                    }
                    else
                        speed_counter[i]++;
            }
        }

    pwm_counter++;
}

/*
 * TIMER_INT overflow
 */
ISR(UTILS_TIMEROVF_VECT(TIMER_INT))
{
    if (tmpcounter == TIMER_NCYCLES - 1) 
    {
        tmpcounter = 0;
        // Turn on compare match interruption A
        UTILS_BIT_SET(UTILS_AGGL(TIMSK, TIMER_INT), \
                                              UTILS_AGGL2(OCIE, TIMER_INT, A));
        // if TIMER_MOD == 0, set interruption on counter match flag
        if (TIMER_MOD == 0)
            UTILS_BIT_SET(UTILS_AGGL(TIFR, TIMER_INT), \
                                               UTILS_AGGL2(OCF, TIMER_INT, A));
    }
    else 
        tmpcounter++;
}

/*
 * Timer initialization
 */
inline void timers_init(void)
{
    UTILS_AGGL2(OCR, TIMER_INT, A) = TIMER_MOD;
    /* Clear timer on compare match */
    UTILS_AGGL2(TCCR, TIMER_INT, A) |= UTILS_AGGL2(WGM, TIMER_INT, 1);
    /* Turn on interruption on counter overflow*/
    UTILS_BIT_SET(UTILS_AGGL(TIMSK, TIMER_INT), UTILS_AGGL(TOIE, TIMER_INT));
}
