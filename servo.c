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
 * Adjust pulselength for servo[num]
 */
void adjust_servo(uint8_t num)
{
    uint16_t min_pl =  eeprom_read_word(& ee_min_pulselength[num]);
    uint16_t max_pl =  eeprom_read_word(& ee_max_pulselength[num]);

    servo[num].pulselength = min_pl \
                     + (uint32_t)servo[num].position * (max_pl - min_pl) / 256;
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
}
