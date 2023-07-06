/*
 * pwm.h
 *
 *  Created on: May 10, 2023
 *      Author: micho
 */

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PWM_PORT_ID   PORTB_ID
#define PWM_PIN_ID    PIN3_ID



void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
