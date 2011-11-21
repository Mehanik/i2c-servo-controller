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
    next_servo = 0;
    UTILS_AGGL2(OCR, PTIMER, BH) = servo[0].pulselength >> 8;   // HIGH bit
    UTILS_AGGL2(OCR, PTIMER, BL) = servo[0].pulselength & 0xff; // LOW bit
}

/*
 * PTIMER, compare match B
 */
ISR(UTILS_TIMERCOMP_VECT(PTIMER, B))
{
    uint8_t pr = 0;
    while (!pr)
    {
        servo_clr(servo_order[next_servo]);

        pr = 1;
        uint16_t currentOCR;
        currentOCR = UTILS_AGGL2(OCR, PTIMER, BL);       // Low byte
        currentOCR = UTILS_AGGL2(OCR, PTIMER, BH) << 8;  // High byte
        uint16_t next_servo_pl = servo[servo_order[next_servo]].pulselength;
        if (next_servo < SERVO_NUM)
            if (next_servo_pl <= currentOCR)
            {
                next_servo ++;
                pr = 0;
            }
    }

    if (next_servo < SERVO_NUM)
    {
        next_servo ++;
        UTILS_AGGL2(OCR, PTIMER, BH) = servo[next_servo].pulselength >> 8;   // High byte
        UTILS_AGGL2(OCR, PTIMER, BL) = servo[next_servo].pulselength & 0xff; // Low byte
    }
}

/*
 * ITIMER, compare match A
 */
ISR(UTILS_TIMERCOMP_VECT(ITIMER, A))
{
    sei();
    for (int i = 0; i < SERVO_NUM; i ++)
    {
        if (servo[i].position != servo[i].target)
        {
            if (servo[i].speed != 0)
            {
                if (servo[i].speed_counder == servo[i].speed)
                {
                    // Change position
                    if (servo[i].position < servo[i].target)
                        servo[i].position ++;
                    else
                        servo[i].position --;

                    servo_adjust(i);
                    servo[i].speed_counder = 0;
                }
                else
                    servo[i].speed_counder ++;
            }
            else
            {
                // Immediate position change
                servo[i].position = servo[i].target;
                servo_adjust(i);
            }
        }
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
