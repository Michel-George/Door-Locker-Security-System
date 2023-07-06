/*
 * dc_motor.h
 *
 *  Created on: May 10, 2023
 *      Author: micho
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"
#include "pwm.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define motor_PORT_ID   PORTB_ID
#define motor_CW_PIN    PIN0_ID
#define motor_A_CW_PIN  PIN1_ID



typedef enum
{
	stop,CW,A_CW
}DcMotor_State;

void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* DC_MOTOR_H_ */
