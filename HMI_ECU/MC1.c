

#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "timer1.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MC_READY 0x10

#define set 's'
#define open 'o'
#define change 'c'
#define error 'e'


#define pass_size 5
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8 password[pass_size]={0,0,0,1,0};
uint8 sec=0;
Timer1_ConfigType Timer1_ConfigStruct = {0,31250,F_CPU_256,CTC_OCR};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void LCD_arrayDisplay(uint8 *array,uint8 size);
void UART_sendArray(uint8 *array,uint8 size);
void get_pass(uint8 *password,uint8 size,uint8 LCD_pos);
void set_pass(void);
void open_door(void);
void change_pass(void);
void Timer1_ISR(void);
void Timer1_delay(uint8 delay);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(void)
{
	uint8 key;

	UART_ConfigType UART_configuration = {bit8,Parity_Disabled,one_stopbit,9600};
	UART_init(&UART_configuration);

	LCD_init();

	UART_sendByte(MC_READY);
	while(UART_recieveByte() != MC_READY);


	set_pass();


	while(1)
	{

		LCD_moveCursor(0,0);
		LCD_displayString("+: Open Door");
		LCD_moveCursor(1,0);
		LCD_displayString("-: Change Pass");

		key=KEYPAD_getPressedKey();

		if(key=='+')
		{
			LCD_clearScreen();
			open_door();
		}
		else if (key=='-')
		{
			LCD_clearScreen();
			change_pass();
		}

	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void get_pass(uint8 *password,uint8 size,uint8 LCD_pos)
{
	uint8 key;
	uint8 i=LCD_pos;

	do
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(500);
		if(key>=0 && key<=9)
		{
			if(i!=(size+LCD_pos))
			{
				password[i-LCD_pos]=key;
				LCD_moveCursor(1,i);
				//LCD_displayCharacter('*');
				LCD_intgerToString(password[i-LCD_pos]);
				i++;
			}
		}
		else if(key== '-')
		{

			LCD_moveCursor(1,i);
			LCD_displayCharacter(' ');
			i--;
			LCD_moveCursor(1,i);
			LCD_displayCharacter(' ');
		}

	}
	while(key != '=' );
}

void set_pass(void)
{
	do{
		UART_sendByte(set);
		LCD_clearScreen();
		LCD_moveCursor(0,0);
		LCD_displayString("Pls enter Pass:");
		LCD_moveCursor(1,0);

		get_pass(password,pass_size,0);
		UART_sendArray(password,pass_size);

		LCD_clearScreen();
		LCD_moveCursor(0,0);
		LCD_displayString("Pls re-enter the");
		LCD_moveCursor(1,0);
		LCD_displayString("same Pass:");

		get_pass(password,pass_size,10);
		UART_sendArray(password,pass_size);
		LCD_clearScreen();

	}while(UART_recieveByte()==0);
}
void open_door(void)
{
	uint8 counter=0;

	do{
		if (counter==2){

			UART_sendByte(error);
			LCD_clearScreen();
			LCD_displayString("Wrong Pass");
			while(UART_recieveByte()!=1);

			break;
		}
		else
		{
			UART_sendByte(open);
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("Pls enter Pass:");
			LCD_moveCursor(1,0);

			get_pass(password,pass_size,0);
			//while(KEYPAD_getPressedKey()!='=');
			UART_sendArray(password,pass_size);
			if(UART_recieveByte()==0)
			{
				counter++;
			}else
			{
				LCD_clearScreen();
				LCD_displayString("Door Unlocking");

				while(UART_recieveByte()!=1);

				LCD_clearScreen();
				LCD_displayString("Door Hold");

				while(UART_recieveByte()!=1);

				LCD_clearScreen();
				LCD_displayString("Door Locking");

				while(UART_recieveByte()!=1);

				break;
			}
		}
	}while(1);

}

void change_pass(void)
{
	uint8 counter=0;
	do{
		if (counter==2){
			UART_sendByte(error);
			LCD_clearScreen();
			LCD_displayString("Wrong Pass");
			while(UART_recieveByte()!=1);

			break;

		}
		else
		{
			UART_sendByte(change);
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("Pls enter Pass:");
			LCD_moveCursor(1,0);

			get_pass(password,pass_size,0);
			UART_sendArray(password,pass_size);
			if(UART_recieveByte()==0)
			{
				counter++;
			}else{break;}
		}
	}while(1);

	if(counter!=2)
	{
		set_pass();
	}


}
void LCD_arrayDisplay(uint8 *array,uint8 size)
{
	uint8 i ;
	for(i=0;i<size;i++)
	{
		LCD_intgerToString(array[i]);
	}
}

void UART_sendArray(uint8 *array,uint8 size)
{
	uint8 i ;
	for(i=0;i<size;i++)
	{
		UART_sendByte(array[i]);
	}
}
void Timer1_ISR(void){
	sec++;
}

void Timer1_delay(uint8 delay){
	Timer1_init(&Timer1_ConfigStruct);
	Timer1_setCallBack(Timer1_ISR);
	while(sec != delay);
	Timer1_deInit();
	sec=0;
}


