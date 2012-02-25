/*
 *  Project: servo_controller 
 *  File: timers.c - 
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 12.02.2011
 *  Copyright (C) 2011 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#include "main.h" 

inline void nextstate(void)
{
    if (state_current < state_max) 
    {
        // Load PTIMER OCRnB
        UTILS_AGGL2(OCR, PTIMER, B) = outstate[state_current].time;
        // Enable interrupt B
        UTILS_AGGL(TIMSK, PTIMER) |= _BV(UTILS_AGGL2(OCIE, PTIMER, B)); 
        // TODO: if outstate.time very closely to current time
        state_current ++;
    }
    else
    {
        if (flags.new_buf_ready)
        {
            for (uint8_t i = 0; i <= SERVO_NUM; i++)
            {
                outstate[i] = outstate_buf[i];
            }
            state_max = state_max_buf;

            flags.new_buf_ready = 0;
        }

        // Disable interrupt B
        UTILS_AGGL(TIMSK, PTIMER) &= ~_BV(UTILS_AGGL2(OCIE, PTIMER, B)); 
    }
}

/*
 * PTIMER, compare match A
 * called every (1 / F_PWM) seconds
 */
ISR(UTILS_TIMERCOMP_VECT(PTIMER, A))
{
    // event: timer reset
    state_current = 0;
    servo_out();
    nextstate();
}

/*
 * PTIMER, compare match B
 */
ISR(UTILS_TIMERCOMP_VECT(PTIMER, B))
{
    // event: change servo out
    servo_out();
    nextstate();
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
    UTILS_AGGL2(TCCR, PTIMER, A) = PTIMER_TCCRA;
    UTILS_AGGL2(OCR, PTIMER, A) = PTIMER_MOD; 

    // Enable interrupt A
    UTILS_AGGL(TIMSK, PTIMER) |= _BV(UTILS_AGGL2(OCIE, PTIMER, A)); 

    /*
     * ITIMER
     */
    UTILS_AGGL2(TCCR, ITIMER, A) = ITIMER_TCCRA;
    UTILS_AGGL2(OCR, ITIMER, A) = ITIMER_OCRA;
}

/*
 * Run timers
 */
inline void run_pwm(void)
{
    /*
     * PTIMER
     */
    UTILS_AGGL2(TCCR, PTIMER, B) = PTIMER_TCCRB;

    /*
     * ITIMER
     */
    UTILS_AGGL2(TCCR, ITIMER, B) = ITIMER_TCCRB;
}
