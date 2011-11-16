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

/*
 * Indication
 */
#define LED_PORT    C
#define LED_PIN     2

/*
 * Timers
 */
#define PTIMER      1 // 16-bit timer
#define PTIMER_DIV  8

#if ! ((PTIMER == 1) && (MCU == atmega48))
# warning "Check TCCRxA and TCCRxB registers"
#endif

#define PTIMER_TCCRA    (0x00 \
                        | (0 << UTILS_AGGL2(COM, PTIMER, A1)) \
                        | (0 << UTILS_AGGL2(COM, PTIMER, A0)) \
                        | (0 << UTILS_AGGL2(COM, PTIMER, B1)) \
                        | (0 << UTILS_AGGL2(COM, PTIMER, B0)) \
                        | (0 << UTILS_AGGL2(WGM, PTIMER, 1))  \
                        | (0 << UTILS_AGGL2(WGM, PTIMER, 0)))

#define PTIMER_TCCRB    (0x00 \
                        | (0 << UTILS_AGGL(ICNC, PTIMER))     \
                        | (0 << UTILS_AGGL(ICES, PTIMER))     \
                        | (0 << UTILS_AGGL2(WGM, PTIMER, 3))  \
                        | (1 << UTILS_AGGL2(WGM, PTIMER, 2))  \
                        | (0 << UTILS_AGGL2(CS,  PTIMER, 2))  \
                        | (1 << UTILS_AGGL2(CS,  PTIMER, 1))  \
                        | (0 << UTILS_AGGL2(CS,  PTIMER, 0)))

#define F_PWM       50 // Hz

#if F_CPU / F_PWM / PTIMER_DIV > 0xffff
# error "Unable to adjust the timer. Try to increase PTIMER_DIV or decrease F_PWM."
#endif 

#define PTIMER_MOD  (F_CPU / F_PWM / PTIMER_DIV) 
#define W_MIN       (F_CPU / PTIMER_DIV * 8 / 10000)  // 0.8us in counter ticks
#define W_MAX       (F_CPU / PTIMER_DIV * 23 / 10000) // 2.3us in counter ticks

/* PWM signal diagram
 *
 *    ________________________                                 _______________
 * __/        |               \______|______ . . . ___________/    
 *   ^        ^                      ^                        ^
 *   0        |                      |                        20 us PTIMER_MOD
 *   (0x00)   |                      |
 *            |                      maximum pulse length, default value: W_MAX
 *            |                      (0xff)
 *            |
 *            minimum pulse length, default value: W_MIN
 *            (0x01)
 *  The value in brackets indicates compliance with the servo[n].position
 */

#endif // HARDWARE_H_
