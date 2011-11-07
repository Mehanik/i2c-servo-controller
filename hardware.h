/*
 *  Project: servo_controller
 *  File: hardware.h - defines for pins, timers, etc
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 19.11.2010.
 *  Copyright (C) 2011 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "utils.h"

/*
 * Servo Ports
 */
#define SERVO_NUM   8

#define SERVO0_PORT D
#define SERVO0_PIN  6

#define SERVO1_PORT D
#define SERVO1_PIN  7

#define SERVO2_PORT B
#define SERVO2_PIN  0

#define SERVO3_PORT B
#define SERVO3_PIN  1

#define SERVO4_PORT B
#define SERVO4_PIN  2

#define SERVO5_PORT B
#define SERVO5_PIN  3

#define SERVO6_PORT B
#define SERVO6_PIN  4

#define SERVO7_PORT B
#define SERVO7_PIN  5

/*
 * I2C Ports
 */
#define I2CSCL_PORT C
#define I2CSCL_PIN  5
#define I2CSDA_PORT C
#define I2CSDA_PIN  4

#define LED_PORT    C
#define LED_PIN     2
/*
 * Timers
 */
#define SERVO_PWMFREQ    12800   // 50 * 256 Hz

#define TIMER_INT		2 	// 8bit timer
#define TIMER_INT_DIV   1   // timer divider
#define TIMER_INT_TCCR  ((0 << UTILS_AGGL2(CS, TIMER_INT, 2)) \
                       | (0 << UTILS_AGGL2(CS, TIMER_INT, 1)) \
                       | (1 << UTILS_AGGL2(CS, TIMER_INT, 0))) 

/* Number of full TIMER_INT cycles between two increments of the PWM couner */
#define TIMER_NCYCLES  (F_CPU / TIMER_INT_DIV / SERVO_PWMFREQ / 256) 

/* Length of last cycle */
#define TIMER_MOD      ((F_CPU / TIMER_INT_DIV / SERVO_PWMFREQ) % 256)
/* Period of counter PWM increments = 1 / (F_CPU / TIMER_INT_DIV) * 
 *                                      * (TIMER_NCYCLES * 256 + SERVOPWMFREQ)
 */

#endif // HARDWARE_H_
