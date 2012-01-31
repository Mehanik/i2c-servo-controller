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
    uint8_t speed_counter;
    uint16_t pl;                // length of pulses, in PTIMER ticks
} servo_t;

typedef struct
{
    uint16_t time;
#if SERVO_NUM <= 8
    uint8_t pin;    // if bit n of "out" is set to 1,
                    // out of servo[n] is HIGH, othrwise -- LOW.
#else
    uint16_t pin;
#endif
} outstate_t;

volatile servo_t servo[SERVO_NUM];
volatile outstate_t outstate[SERVO_NUM + 1];
volatile outstate_t outstate_buf[SERVO_NUM + 1];
volatile uint8_t state_current;
volatile uint8_t state_max;
volatile uint8_t state_max_buf;

volatile uint8_t i2cMemAddr;

struct
{
    uint8_t new_buf_ready : 1;  
    uint8_t i2c_first_byte : 1;
} flags;

volatile uint8_t sorted_servo[SERVO_NUM]; // numbers of servos sorted by position

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
    _delay_us(1);\
    UTILS_PORT_CLR(LED_PORT, LED_PIN);\
}

#define _LED_FLIP UTILS_PORT_FLIP(LED_PORT, LED_PIN);

#endif /* HWMPC_H_ */
