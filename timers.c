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
}

/*
 * PTIMER, compare match B
 */
ISR(UTILS_TIMERCOMP_VECT(PTIMER, B))
{
    servo_clr(0);
}

/*
 * Timer initialization
 * Must be called before sei()
 */
inline void timers_init(void)
{
    UTILS_AGGL2(TCCR, PTIMER, A) = PTIMER_TCCRA; // load TCCRnA

    // Load OCRnA
    // Timer work in CTC mode, so TCNT will be set to 0x0000 when compare match
    UTILS_AGGL2(OCR, PTIMER, AH) = PTIMER_MOD >> 8;   // HIGH bit
    UTILS_AGGL2(OCR, PTIMER, AL) = PTIMER_MOD & 0xff; // LOW bit

    UTILS_AGGL(TIMSK, PTIMER) |= _BV(UTILS_AGGL2(OCIE, PTIMER, A)) \
                               | _BV(UTILS_AGGL2(OCIE, PTIMER, B));
}

/*
 * Run PWM timer
 */
inline void run_pwm(void)
{
    UTILS_AGGL2(TCCR, PTIMER, B) = PTIMER_TCCRB; // load TCCRnB
}
