/*
 *  Project: hwmpc-bt
 *  File: utils.h - module contains general purpose macros and functions
 *  Author: Eugene Mikhantiev <mikhantiev@gmail.com>
 *  Creation Date: 05.12.2010
 *  Copyright (C) 2010 Eugene Mikhantiev
 *  License: GNU GPL v2 (see License.txt)
 */

#ifndef UTILS_H_
#define UTILS_H_

// Просто сшивает аргументы
#define UTILS_GLUE(arg1, arg2)  					arg1 ## arg2
#define UTILS_GLUE3(arg1, arg2, arg3) 				arg1 ## arg2 ## arg3
#define UTILS_GLUE4(arg1, arg2, arg3, arg4) 		arg1 ## arg2 ## arg3 ## arg4
#define UTILS_GLUE5(arg1, arg2, arg3, arg4, arg5) 	arg1 ## arg2 ## arg3 ## arg4 ## arg5

// Фомирует имя из префикса и номера
#define UTILS_AGGL(name, arg1)				(UTILS_GLUE(name, arg1))
#define UTILS_AGGL2(name, arg1, arg2)		(UTILS_GLUE3(name, arg1, arg2))
#define UTILS_AGGL3(name, arg1, arg2, arg3)	(UTILS_GLUE4(name, arg1, arg2, arg3))

#define UTILS_MASK_SET(adr, mask)	((adr) |=  (mask))
#define UTILS_MASK_CLR(adr, mask)	((adr) &= ~(mask))

#define UTILS_BIT_SET(adr, bit)    ((adr) |=  (1 << (bit)))
#define UTILS_BIT_CLR(adr, bit)    ((adr) &= ~(1 << (bit)))
#define UTILS_BIT_VALUE(adr, bit)  ((adr) &   (1 << (bit)))
#define UTILS_BIT_FLIP(adr, bit)   ((adr) ^=  (1 << (bit)))

// Операции с битами рагистров портов вода-вывода
#define UTILS_PORT_SET(port, pin)    UTILS_BIT_SET(UTILS_AGGL(  PORT, port), pin)
#define UTILS_PORT_CLR(port, pin)    UTILS_BIT_CLR(UTILS_AGGL(  PORT, port), pin)
#define UTILS_PORT_VALUE(port, pin)  UTILS_BIT_VALUE(UTILS_AGGL(PORT, port), pin)
#define UTILS_PORT_FLIP(port, pin)   UTILS_BIT_FLIP(UTILS_AGGL( PORT, port), pin)

#define UTILS_PIN_VALUE(port, pin)   UTILS_BIT_VALUE(UTILS_AGGL(PIN, port), pin)

#define UTILS_DDR_SET(port, pin)     UTILS_BIT_SET(UTILS_AGGL(  DDR, port), pin)
#define UTILS_DDR_CLR(port, pin)     UTILS_BIT_CLR(UTILS_AGGL(  DDR, port), pin)
#define UTILS_DDR_VALUE(port, pin)   UTILS_BIT_VALUE(UTILS_AGGL(DDR, port), pin)

// Сдвиги
#define UTILS_HI(x) ((x) >> 8)
#define UTILS_LO(x) ((x) & 0xFF)

// Операции с битами регистров прерываний по изменению уровня
#define UTILS_PCICR_SET(num) (PCICR |=  (1 << UTILS_GLUE(PCIE, num)))
#define UTILS_PCICR_CLR(num) (PCICR &= ~(1 << UTILS_GLUE(PCIE, num)))

#define UTILS_PCIFR_SET(num) (PCIFR |=  (1 << UTILS_GLUE(PCIE, num)))
#define UTILS_PCIFR_CLR(num) (PCIFR &= ~(1 << UTILS_GLUE(PCIE, num)))

// Для формирования имён прерываний
#define UTILS_TIMERCOMP_VECT(timer_num, c_id) 	UTILS_GLUE5(TIMER, timer_num, _COMP, c_id, _vect)
#define UTILS_TIMEROVF_VECT(timer_num) 			UTILS_GLUE3(TIMER, timer_num, _OVF_vect)
#define UTILS_PCINT_VECT(num)					UTILS_GLUE3(PCINT, num, _vect)

#endif /* UTILS_H_ */

