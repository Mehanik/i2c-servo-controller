/*
 *  Project: servo_controller
 *  File: servo.c
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 5.11.2011
 *  Copyright (C) 2011 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#include "main.h"

/*
 * Generate outstate_buf
 */
inline void outstate_gen(void)
{
    outstate_t outstate_tmp[SERVO_NUM + 1];
    uint8_t state_max_tmp;

    for (uint8_t i = 0; i < SERVO_NUM + 1; i++)
    {
        outstate_tmp[i].pin = -1; // 0xff
    }

    uint8_t i = 0;
    outstate_tmp[0].time = 0;   // outstate[0].pin it is output condition on
                                // timer overflow

    for(int j = 0; j < SERVO_NUM; j++)
    {
        uint8_t num = sorted_servo[j];
        if (servo[num].pd != outstate_tmp[i].time)
        {
            i++;
            outstate_tmp[i].time = servo[num].pd;
        }
        outstate_tmp[i].pin &= ~_BV(num);
    }
    state_max_tmp = i;

    for (uint8_t i = 0; i <= state_max_tmp - 1; i++)
    {
        outstate_tmp[i + 1].pin &= outstate_tmp[i].pin;
    }

    flags.new_buf_ready = 0;
    for (uint8_t i = 0; i <= state_max_tmp; i++)
    {
        outstate_buf[i] = outstate_tmp[i];
    }
    state_max_buf = state_max_tmp;
    flags.new_buf_ready = 1;
    //TODO: 
}

/*
 * Find servo.pd
 */
inline void servo_find_pd(int num)
{
    uint8_t position = servo[num].position;

    if (servo[num].position == 0)
        servo[num].pd = 0;
    else
        servo[num].pd = servo[num].min_pd + (uint32_t) position \
                        * (servo[num].max_pd - servo[num].min_pd) / 255;
}

/*
 * Sort servos by servo.pd
 */
inline void servo_sort(void)
{
    for (int i = 0; i < SERVO_NUM; i++)
    {
        uint8_t n = 0;
        uint16_t curpd = servo[i].pd;
        for (int num = 0; num < SERVO_NUM; num++)
            if ((servo[num].pd < curpd) \
                    || ((servo[num].pd == curpd) && (num < i))) 
                n++;
        sorted_servo[n] = i;
    }
}

/*
 * Initialize servo
 */
inline void servo_init(void)
{
    for (uint8_t i = 0; i < SERVO_NUM; i++)
    {
        // Load Default/Saved values.
        uint8_t default_position = eeprom_read_byte(& ee_default_position[i]);
        servo[i].target = default_position;
        servo[i].position = default_position;
        servo[i].speed = 0;
        servo[i].min_pd = eeprom_read_word(& ee_min_pd[i]);
        servo[i].max_pd = eeprom_read_word(& ee_max_pd[i]);
        servo_find_pd(i);
    }

    servo_sort();
    outstate_gen();
}

/*
 * Set outputs accordance with outstate
 */
#if SERVO_NUM != 8
# error "This function should be modified"
#endif
inline void servo_out(void)
{
    uint8_t servo_state = outstate[state_current].pin;

    // 0
    if (servo_state & _BV(0))
        UTILS_PORT_SET(SERVO0_PORT, SERVO0_PIN);
    else
        UTILS_PORT_CLR(SERVO0_PORT, SERVO0_PIN);
    // 1
    if (servo_state & _BV(1))
        UTILS_PORT_SET(SERVO1_PORT, SERVO1_PIN);
    else
        UTILS_PORT_CLR(SERVO1_PORT, SERVO1_PIN);
    // 2
    if (servo_state & _BV(2))
        UTILS_PORT_SET(SERVO2_PORT, SERVO2_PIN);
    else
        UTILS_PORT_CLR(SERVO2_PORT, SERVO2_PIN);
    // 3
    if (servo_state & _BV(3))
        UTILS_PORT_SET(SERVO3_PORT, SERVO3_PIN);
    else
        UTILS_PORT_CLR(SERVO3_PORT, SERVO3_PIN);
    // 4
    if (servo_state & _BV(4))
        UTILS_PORT_SET(SERVO4_PORT, SERVO4_PIN);
    else
        UTILS_PORT_CLR(SERVO4_PORT, SERVO4_PIN);
    // 5
    if (servo_state & _BV(5))
        UTILS_PORT_SET(SERVO5_PORT, SERVO5_PIN);
    else
        UTILS_PORT_CLR(SERVO5_PORT, SERVO5_PIN);
    // 6
    if (servo_state & _BV(6))
        UTILS_PORT_SET(SERVO6_PORT, SERVO6_PIN);
    else
        UTILS_PORT_CLR(SERVO6_PORT, SERVO6_PIN);
    // 7
    if (servo_state & _BV(7))
        UTILS_PORT_SET(SERVO7_PORT, SERVO7_PIN);
    else
        UTILS_PORT_CLR(SERVO7_PORT, SERVO7_PIN);
}
