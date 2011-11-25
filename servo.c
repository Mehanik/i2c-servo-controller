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
 * Adjust pd for servo[num]
 */
void servo_adjust(uint8_t num)
{
    uint8_t  position = servo[num].position;

    // Find servo's position in servo_order_buf.
    uint8_t order = 0;
    while (servo_s_tmp[order].num != num) // && (order < SERVO_NUM - 1)
        order++;

    // Change servo_order_buf
    uint8_t sort_done = 0;
    while (! sort_done)
    {
        sort_done = 1;
        if (order > 0)
        {
            uint8_t prevnum = servo_s_tmp[order - 1].num;
            if (servo[prevnum].position > position)
            {
                servo_s_tmp[order].num = servo_s_tmp[order - 1].num;
                servo_s_tmp[order].pd = servo_s_tmp[order - 1].pd;
                servo_s_tmp[order].position = servo_s_tmp[order - 1].position;
                order --;
                sort_done = 0;
            }
        }
        if (order < SERVO_NUM - 1)
        {
            uint8_t nextnum = servo_s_tmp[order + 1].num;
            if (servo[nextnum].position < position)
            {
                servo_s_tmp[order].num = servo_s_tmp[order + 1].num;
                servo_s_tmp[order].pd = servo_s_tmp[order + 1].pd;
                servo_s_tmp[order].position = servo_s_tmp[order + 1].position;
                order ++;
                sort_done = 0;
            }
        }
    }

    // Find length of pulses, in PTIMER ticks.
    uint16_t min_pl =  eeprom_read_word(& ee_min_pd[num]);
    uint16_t max_pl =  eeprom_read_word(& ee_max_pd[num]);

    servo_s_tmp[order].num = num;
    servo_s_tmp[order].pd = min_pl \
                     + (uint32_t) position * (max_pl - min_pl) / 255;
    servo_s_tmp[order].position = position;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        for (int i = 0; i < SERVO_NUM; i++)
        {
            servo_s_buf[i].num = servo_s_tmp[i].num;
            servo_s_buf[i].pd = servo_s_tmp[i].pd;
            servo_s_buf[i].position = servo_s_tmp[i].position;
        }
    }
}

/*
 * Set all servo pins to HIGH
 */
#if SERVO_NUM != 8
# error "This function should be modified"
#endif
void servo_set_all(void)
{
    if (servo[0].position > 0)
        UTILS_PORT_SET(SERVO0_PORT, SERVO0_PIN);
    if (servo[1].position > 0)
        UTILS_PORT_SET(SERVO1_PORT, SERVO1_PIN);
    if (servo[2].position > 0)
        UTILS_PORT_SET(SERVO2_PORT, SERVO2_PIN);
    if (servo[3].position > 0)
        UTILS_PORT_SET(SERVO3_PORT, SERVO3_PIN);
    if (servo[4].position > 0)
        UTILS_PORT_SET(SERVO4_PORT, SERVO4_PIN);
    if (servo[5].position > 0)
        UTILS_PORT_SET(SERVO5_PORT, SERVO5_PIN);
    if (servo[6].position > 0)
        UTILS_PORT_SET(SERVO6_PORT, SERVO6_PIN);
    if (servo[7].position > 0)
        UTILS_PORT_SET(SERVO7_PORT, SERVO7_PIN);
}

/*
 * Set servo pin to LOW
 */
#if SERVO_NUM != 8
# error "This function should be modified"
#endif
void servo_clr(uint8_t servo_num)
{
#if 1
    switch(servo_num)
    {
        case 0: UTILS_PORT_CLR(SERVO0_PORT, SERVO0_PIN);
            break;
        case 1: UTILS_PORT_CLR(SERVO1_PORT, SERVO1_PIN);
            break;
        case 2: UTILS_PORT_CLR(SERVO2_PORT, SERVO2_PIN);
            break;
        case 3: UTILS_PORT_CLR(SERVO3_PORT, SERVO3_PIN);
            break;
        case 4: UTILS_PORT_CLR(SERVO4_PORT, SERVO4_PIN);
            break;
        case 5: UTILS_PORT_CLR(SERVO5_PORT, SERVO5_PIN);
            break;
        case 6: UTILS_PORT_CLR(SERVO6_PORT, SERVO6_PIN);
            break;
        case 7: UTILS_PORT_CLR(SERVO7_PORT, SERVO7_PIN);
            break;
    }
#endif
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
    }

    // Sort servos by positions
    for (int i = 0; i < SERVO_NUM; i++)
    {
        uint8_t ltc = 0;
        uint8_t ipos = servo[i].position;
        for (int num = 0; num < SERVO_NUM; num++)
            if ((servo[num].position < ipos) \
                    || ((servo[num].position == ipos) && (num < i))) 
                ltc++;
        servo_s_tmp[ltc].num = i;
    }

    for (uint8_t i = 0; i < SERVO_NUM; i++)
    {
        servo_adjust(i);
    }
}
