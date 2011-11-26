/*
 *  Project: servo_controller
 *  File: main.h
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 09.01.2011
 *  Copyright (C) 2010 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#ifndef HWMPC_H_
#define HWMPC_H_

#include <stdlib.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "hardware.h"
#include "utils.h"

typedef struct 
{
    uint8_t position;           // Current position
    uint8_t target;             // Servo moves from position to target with speed
    uint8_t speed;              // Number of increases or decreases of position
                                // in time interval 1/200s
    uint8_t speed_counder;
} servo_t;

typedef struct
{
    uint8_t num;
    uint16_t pd;       // The pulse duration in PTIMER ticks
    uint8_t position;
} sorted_servo_t;

volatile servo_t servo[SERVO_NUM];
volatile sorted_servo_t servo_s[SERVO_NUM + 1];    // Servos, sorted by position
volatile sorted_servo_t servo_s_buf[SERVO_NUM];
volatile sorted_servo_t servo_s_tmp[SERVO_NUM];
volatile uint8_t current_servo;

volatile uint8_t servo_state[SERVO_NUM];    // If servo_state[n] is 0,
                                            // output is set to 1

uint8_t EEMEM ee_i2c_adders = 0x10;

/*
 * Limits of the of the pulses length.
 */
uint16_t EEMEM ee_min_pd[SERVO_NUM] = {W_MIN, W_MIN, W_MIN, W_MIN, \
                                                W_MIN, W_MIN, W_MIN, W_MIN};

uint16_t EEMEM ee_max_pd[SERVO_NUM] = {W_MAX, W_MAX, W_MAX, W_MAX, \
                                                W_MAX, W_MAX, W_MAX, W_MAX};

/*
 * Servo position after power on
 */
uint8_t EEMEM ee_default_position[SERVO_NUM] = {0, 0, 0, 0, \
                                                0, 0, 0, 0};

#define _LED_BLINK \
{\
    UTILS_PORT_SET(LED_PORT, LED_PIN);\
    _delay_us(2);\
    UTILS_PORT_CLR(LED_PORT, LED_PIN);\
}

#define _LED_FLIP UTILS_PORT_FLIP(LED_PORT, LED_PIN);

#endif /* HWMPC_H_ */
