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
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/wdt.h>
#include "hardware.h"
#include "utils.h"

typedef struct 
{
    uint8_t position;
    uint8_t target;
    uint8_t target_buf;
    uint8_t speed;
    uint8_t speed_buf;
    uint16_t pulselength;
} servo_t;

uint8_t servo_order[SERVO_NUM]; // 
uint8_t next_servo;

servo_t servo[SERVO_NUM];

uint8_t EEMEM ee_i2c_adders = 0x10;

/*
 * Limits of the of the pulses length.
 */
uint16_t EEMEM ee_min_pulselength[SERVO_NUM] = {W_MIN, W_MIN, W_MIN, W_MIN, \
                                                W_MIN, W_MIN, W_MIN, W_MIN};

uint16_t EEMEM ee_max_pulselength[SERVO_NUM] = {W_MAX, W_MAX, W_MAX, W_MAX, \
                                                W_MAX, W_MAX, W_MAX, W_MAX};

#endif /* HWMPC_H_ */
