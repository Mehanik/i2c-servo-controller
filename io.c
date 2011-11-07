/*
 *  Project: servo_controller
 *  File: io.c - 
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 12.02.2011
 *  Copyright (C) 2011 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#include "main.h"

/*
 * I/O initialization
 */
inline void io_init(void)
{
    UTILS_DDR_SET(SERVO0_PORT, SERVO0_PIN);
    UTILS_DDR_SET(SERVO1_PORT, SERVO1_PIN);
    UTILS_DDR_SET(SERVO2_PORT, SERVO2_PIN);
    UTILS_DDR_SET(SERVO3_PORT, SERVO3_PIN);
    UTILS_DDR_SET(SERVO4_PORT, SERVO4_PIN);
    UTILS_DDR_SET(SERVO5_PORT, SERVO5_PIN);
    UTILS_DDR_SET(SERVO6_PORT, SERVO6_PIN);
    UTILS_DDR_SET(SERVO7_PORT, SERVO7_PIN);
    
    UTILS_DDR_SET(LED_PORT, LED_PIN);
}
