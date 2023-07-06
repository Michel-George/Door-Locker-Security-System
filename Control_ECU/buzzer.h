/*
 * buzzer.h
 *
 *  Created on: Jun 23, 2023
 *      Author: micho
 */

#ifndef BUZZER_H_
#define BUZZER_H_


#include "std_types.h"

#define buzzer_Port PORTB_ID
#define buzzer_pin 	PIN7_ID
#define buzzerDir PIN_OUTPUT
#define buzzerOn LOGIC_HIGH
#define buzzerOff LOGIC_LOW

void Buzzer_init(void);

void Buzzer_on(void);

void Buzzer_off(void);

#endif /* BUZZER_H_ */
