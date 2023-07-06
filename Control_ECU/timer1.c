#include "common_macros.h"
#include "timer1.h"
#include <avr/interrupt.h>
#include <avr/io.h>


static volatile void (*g_CallBackPtr)(void) = NULL_PTR;




ISR(TIMER1_OVF_vect){

	(*g_CallBackPtr)();
}

ISR(TIMER1_COMPA_vect){

	(*g_CallBackPtr)();

}

void Timer1_init(const Timer1_ConfigType * Config_Ptr){


	TCCR1A |= (1<<FOC1A);
	TCCR1A |= (1<<FOC1B);

	if(Config_Ptr->mode == NORMAL){

		TIMSK |= (1<<TOIE1);
	}
	else if(Config_Ptr->mode == CTC_OCR){

		TCCR1B|=(1<<WGM12);


		TIMSK |= (1<<OCIE1A);

	}

	TCCR1B |= ( ( (Config_Ptr->prescaler&0x1) >>0) <<CS10)
		     |( ( (Config_Ptr->prescaler&0x2) >>1) <<CS11)
		     |( ( (Config_Ptr->prescaler&0x4) >>2) <<CS12);

	TCNT1 = Config_Ptr->initial_value;

	if(Config_Ptr->mode == CTC_OCR){
		OCR1A = Config_Ptr->compare_value;
	}


	//
	//
	//	if(Config_Ptr->mode == NORMAL){
	//			TCCR1A &= 0x0f;
	//
	//			TCCR1A &= 0xfc;
	//			TCCR1B &= 0xe7;
	//		}
	//		else {
	//			TCCR1A = (TCCR1A & 0x5f) | 0X50;
	//
	//			TCCR1A &= 0xfc;
	//			SET_BIT(TCCR1B,WGM12);
	//			CLEAR_BIT(TCCR1B,WGM13);
	//		}
	//
	//		SET_BIT(TCCR1A,FOC1A);
	//		SET_BIT(TCCR1A,FOC1B);
	//
	//
	//		TCCR1B = (TCCR1B & 0xf8) | (Config_Ptr->prescaler);
	//
	//		TCNT1 = Config_Ptr->initial_value;
	//
	//		if(Config_Ptr->mode == CTC_OCR){
	//			OCR1A = Config_Ptr->compare_value;
	//		}
	//
	//		if(Config_Ptr->mode == NORMAL){
	//			SET_BIT(TIMSK,TOIE1);
	//		}
	//		else if(Config_Ptr->mode == CTC_OCR){
	//			SET_BIT(TIMSK,OCIE1A);
	//		}
	SREG |= (1<<7);
}


void Timer1_deInit(void){
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0;
	TIMSK &= 0x3c;
	SREG &= (0<<7);
	g_CallBackPtr = NULL_PTR;
}

void Timer1_setCallBack(void(*a_ptr)(void)){
	g_CallBackPtr = a_ptr;
}
