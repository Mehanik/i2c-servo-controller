/*
 *  Project: servo_controller
 *  File: i2c.c - i2c Slave interface
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 5.11.2011
 *  Copyright (C) 2011 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#include "main.h"

#define SERVO_NUM_MAX 32
#define PD_AERA_BEGINING ((SERVO_NUM_MAX - 1) * 2 + 2)
#define CURPOS_AERA_BEGINIGNG (PD_AERA_BEGINING + (SERVO_NUM_MAX - 1) * 4 + 4)
#define STATE_AERA_BEGINING (CURPOS_AERA_BEGINIGNG + (SERVO_NUM_MAX - 1) * 2 + 2)

void i2c_init(void)
{
    /* Set i2c address from EEPROM */
    uint8_t addr = eeprom_read_byte(& ee_i2c_adders);
    TWAR = addr << 1;

    TWCR = _BV(TWEA) | _BV(TWEN);
}

void i2c_read (void)
{
    if (i2cMemAddr < PD_AERA_BEGINING)
    {
        if (i2cMemAddr % 2 == 0)
        {
            // speed
            servo[i2cMemAddr / 2].speed = TWDR;
        } else
        {
            // target
            servo[i2cMemAddr / 2].target= TWDR;
        }
    } else
        if (i2cMemAddr < CURPOS_AERA_BEGINIGNG)
        {
            switch (i2cMemAddr % 4)
            {
                uint8_t servNum = i2cMemAddr / 4;
                case 0: // ee_min_pd HIGH byte
                    eeprom_write_byte (((uint8_t*) &ee_min_pd[servNum]) + 1, TWDR);
                    break;
                case 1: // ee_min_pd LOW byte
                    eeprom_write_byte ((uint8_t*) &ee_min_pd[servNum], TWDR);
                    // servo.min_pd update after receiving low byte
                    *(((uint8_t *) &servo[servNum].min_pd) + 1) = \
                        eeprom_read_byte ((uint8_t*) &ee_min_pd[servNum] + 1);
                    *((uint8_t *) &servo[servNum].min_pd) = TWDR;
                    flags.servo_minmaxpd_cnahged = 1;
                    break;
                case 2: // ee_max_pd HIGH byte
                    eeprom_write_byte (((uint8_t*) &ee_max_pd[servNum]) + 1, TWDR);
                    break;
                case 3: // ee_max_pd LOW byte
                    eeprom_write_byte ((uint8_t*) &ee_max_pd[servNum], TWDR);
                    // servo.max_pd update after receiving low byte
                    *(((uint8_t *) &servo[servNum].max_pd) + 1) = \
                        eeprom_read_byte ((uint8_t*) &ee_max_pd[servNum] + 1);
                    *((uint8_t *) &servo[servNum].max_pd) = TWDR;
                    flags.servo_minmaxpd_cnahged = 1;
                    break;
            }
        }
        else
            if (i2cMemAddr >= STATE_AERA_BEGINING + 4)
            {
                eeprom_write_byte(&misc_info[i2cMemAddr - STATE_AERA_BEGINING - 4], TWDR);
            }
}

void i2c_write (void)
{
    if (i2cMemAddr < PD_AERA_BEGINING)
    {
        if (i2cMemAddr % 2 == 0)
        {
            // speed
            TWDR = servo[i2cMemAddr / 2].speed;
        } else
        {
            // target
            TWDR = servo[i2cMemAddr / 2].target;
        }
    } else
        if (i2cMemAddr < CURPOS_AERA_BEGINIGNG)
        {
            switch (i2cMemAddr % 4)
            {
                uint8_t servNum = (i2cMemAddr - PD_AERA_BEGINING) / 4;
                case 0: // ee_min_pd HIGH byte
                    TWDR = eeprom_read_byte (((uint8_t*) &ee_min_pd[servNum]) + 1);
                    break;
                case 1: // ee_min_pd LOW byte
                    TWDR = eeprom_read_byte ((uint8_t*) &ee_min_pd[servNum]);
                    break;
                case 2: // ee_max_pd HIGH byte
                    TWDR = eeprom_read_byte (((uint8_t*) &ee_max_pd[servNum]) + 1);
                    break;
                case 3: // ee_max_pd LOW byte
                    TWDR = eeprom_read_byte ((uint8_t*) &ee_max_pd[servNum]);
                    break;

            }
        } else
            if (i2cMemAddr < STATE_AERA_BEGINING)
            {
                uint8_t servNum = (i2cMemAddr - CURPOS_AERA_BEGINIGNG);
                TWDR = servo[servNum].position;
            }
            else 
            {
                uint8_t stateRegNum = i2cMemAddr - STATE_AERA_BEGINING;
                if (stateRegNum < 4)
                {
                    TWDR = 0;
                    for (int i = stateRegNum * 8; i < SERVO_NUM; i++)
                    {
                        if (servo[i].target == servo[i].position)
                            TWDR |= 1 << (i % 8);
                    }

                } else
                {
                    // ???
                    TWDR = eeprom_read_byte(&misc_info[i2cMemAddr - STATE_AERA_BEGINING - 4]);
                }
            }
}
