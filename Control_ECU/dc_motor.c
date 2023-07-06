/*
 * dc_motor.c
 *
 *  Created on: May 10, 2023
 *      Author: micho
 */
#include "gpio.h"
#include "dc_motor.h"


void DcMotor_Init(void)
{
	GPIO_setupPinDirection(motor_PORT_ID, motor_CW_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(motor_PORT_ID, motor_A_CW_PIN, PIN_OUTPUT);

	GPIO_writePin(motor_PORT_ID,motor_CW_PIN,LOGIC_LOW);
	GPIO_writePin(motor_PORT_ID,motor_A_CW_PIN,LOGIC_LOW);
}


void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{

	switch(state){

	case stop:
		GPIO_writePin(motor_PORT_ID,motor_CW_PIN,LOGIC_LOW);
		GPIO_writePin(motor_PORT_ID,motor_A_CW_PIN,LOGIC_LOW);
		PWM_Timer0_Start(speed);
		break;

	case A_CW:
		GPIO_writePin(motor_PORT_ID,motor_CW_PIN,LOGIC_LOW);
		GPIO_writePin(motor_PORT_ID,motor_A_CW_PIN,LOGIC_HIGH);
		PWM_Timer0_Start(speed);
		break;
	case CW:
		GPIO_writePin(motor_PORT_ID,motor_CW_PIN,LOGIC_HIGH);
		GPIO_writePin(motor_PORT_ID,motor_A_CW_PIN,LOGIC_LOW);
		PWM_Timer0_Start(speed);
	}

}

