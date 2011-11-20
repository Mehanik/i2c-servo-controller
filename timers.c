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
 * PTIMER, compare match A
 * called every (1 / F_PWM) seconds
 */
ISR(UTILS_TIMERCOMP_VECT(PTIMER, A))
{
    servo_set_all();

    for (int i = 0; i < SERVO_NUM; i++)
        servo[i].pulselength = servo[i].pulselength_buf;

    // Load PTIMER OCRnB
    UTILS_AGGL2(OCR, PTIMER, BH) = servo[0].pulselength >> 8;   // HIGH bit
    UTILS_AGGL2(OCR, PTIMER, BL) = servo[0].pulselength & 0xff; // LOW bit
}

/*
 * PTIMER, compare match B
 */
ISR(UTILS_TIMERCOMP_VECT(PTIMER, B))
{
    servo_clr(0);
}

/*
 * ITIMER, compare match A
 */
ISR(UTILS_TIMERCOMP_VECT(ITIMER, A))
{
    sei();
    for (int i = 0; i < SERVO_NUM; i ++)
    {
        servo_action(i);
    }
}

/*
 * Timer initialization
 * Must be called before sei()
 */
inline void timers_init(void)
{
    /*
     * PTIMER
     */
    UTILS_AGGL2(TCCR, PTIMER, A) = PTIMER_TCCRA;        // load TCCRnA

    // Load OCRnA
    UTILS_AGGL2(OCR, PTIMER, AH) = PTIMER_MOD >> 8;     // HIGH bit
    UTILS_AGGL2(OCR, PTIMER, AL) = PTIMER_MOD & 0xff;   // LOW bit

    // Set interrupts flags
    UTILS_AGGL(TIMSK, PTIMER) |= _BV(UTILS_AGGL2(OCIE, PTIMER, A)) \
                               | _BV(UTILS_AGGL2(OCIE, PTIMER, B));
    /*
     * ITIMER
     */
    UTILS_AGGL2(TCCR, ITIMER, A) = ITIMER_TCCRA;        // load TCCRnA
    UTILS_AGGL2(OCR, ITIMER, A) = ITIMER_OCRA;
    // Set interrupts flags
    UTILS_AGGL(TIMSK, ITIMER) |= _BV(UTILS_AGGL2(OCIE, ITIMER, A));
}

/*
 * Run PWM timer
 */
inline void run_pwm(void)
{
    /*
     * PTIMER
     */
    UTILS_AGGL2(TCCR, PTIMER, B) = PTIMER_TCCRB; // load TCCRnB

    /*
     * ITIMER
     */
    UTILS_AGGL2(TCCR, ITIMER, B) = ITIMER_TCCRB; // load TCCRnB
}
