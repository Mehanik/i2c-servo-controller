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

    for (uint8_t i = 0; i < SERVO_NUM; i++)
    {
        servo_s[i].pd = servo_s_buf[i].pd;
        servo_s[i].num = servo_s_buf[i].num;
        servo_s[i].position = servo_s_buf[i].position;
    }

    // Load PTIMER OCRnB
    current_servo = 0;
    UTILS_AGGL2(OCR, PTIMER, B) = servo_s[current_servo].pd; 
}

/*
 * PTIMER, compare match B
 */
ISR(UTILS_TIMERCOMP_VECT(PTIMER, B))
{
    _LED_FLIP;
    // Find next servo with another time
    uint8_t upto = current_servo;
    uint8_t position = servo_s[current_servo].position;
    uint16_t pd = servo_s[current_servo].pd;
    if (position > 0)
        while (servo_s[upto].position == position)
            upto ++;

    _LED_FLIP;
    // 
    for (uint8_t i = current_servo; i <= upto; i++)
    {
        servo_clr(servo_s[i].num);      
    }

    _LED_FLIP;
    upto ++;
    uint16_t currentOCR = UTILS_AGGL(TCNT, PTIMER); 
    if (servo_s[upto].pd >= currentOCR + 1)
    {
        // Set interrupt flag
        UTILS_AGGL(TIFR, PTIMER) |= UTILS_AGGL2(OCF, PTIMER, B);
    }
    else
    {
        UTILS_AGGL2(OCR, PTIMER, B) = servo_s[upto].pd; 
    }
    current_servo = upto;
    _LED_FLIP;
}

/*
 * ITIMER, compare match A
 */
ISR(UTILS_TIMERCOMP_VECT(ITIMER, A))
{
    sei();
    for (uint8_t i = 0; i < SERVO_NUM; i ++)
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
    UTILS_AGGL2(OCR, PTIMER, A) = PTIMER_MOD; 

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
