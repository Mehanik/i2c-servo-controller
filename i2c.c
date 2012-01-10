/*
 *  Project: servo_controller
 *  File: i2c.c - i2c Slave interface
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 5.11.2011
 *  Copyright (C) 2011 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#include "main.h"



void i2c_init(void)
{
    /* Set i2c address from EEPROM */
    uint8_t addr = eeprom_read_byte(& ee_i2c_adders);
    TWAR = addr << 1;
}

