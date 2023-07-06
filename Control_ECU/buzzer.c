/*
 * buzzer.c
 *
 *  Created on: Jun 23, 2023
 *      Author: micho
 */

#include "buzzer.h"
#include"gpio.h"



void Buzzer_init(void)
{
	GPIO_setupPinDirection(buzzer_Port,buzzer_pin,buzzerDir);
	GPIO_writePin(buzzer_Port,buzzer_pin,buzzerOff);
}

void Buzzer_on(void)
{
	GPIO_writePin(buzzer_Port,buzzer_pin,buzzerOn);
}

void Buzzer_off(void)
{
	GPIO_writePin(buzzer_Port,buzzer_pin,buzzerOff);
}
