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

uint8_t servo_currentpos[SERVO_NUM];
uint8_t servo_target[SERVO_NUM];
uint8_t servo_speed[SERVO_NUM];
uint8_t servo_currentpos_buf[SERVO_NUM];
uint8_t servo_target_buf[SERVO_NUM];
uint8_t servo_speed_buf[SERVO_NUM];

uint8_t pwm_counter = 0;
uint8_t speed_counter[SERVO_NUM];
uint8_t tmpcounter = 0;

uint8_t EEMEM i2c_adders = 0x01;

#endif /* HWMPC_H_ */
