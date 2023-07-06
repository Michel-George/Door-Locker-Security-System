
#include "uart.h"
#include "external_eeprom.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "timer1.h"

#include "lcd.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MC_READY 0x10

#define set 's'
#define open 'o'
#define change 'c'
#define error 'e'

#define EEPROM_address  0x0311

#define pass_size 5
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 mainPassword[pass_size]={0,0,0,0,0};
uint8 password1[pass_size]={0,0,0,0,0};
uint8 password2[pass_size]={0,0,0,0,0};
uint32 pass;
uint8 sec=0;
Timer1_ConfigType Timer1_ConfigStruct = {0,31250,F_CPU_256,CTC_OCR};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LCD_arrayDisplay(uint8 *array,uint8 size);

void UART_recieveArray(uint8 *array,uint8 size);
uint8 password_checker(uint8 *pass1,uint8 *pass2,uint8 size);
uint32 convertArrayToInt(uint8 *array,uint8 size);
uint32 calc(uint8 key,uint8 position, uint32 currentVal);
void assignPass(uint8 *array1,uint8 *array2,uint8 size);
void savePasstoEEPROM(uint16 address , uint8 *array,uint8 size);
void readPassfromEEPROM(uint16 address , uint8 *array,uint8 size);
void Timer1_ISR(void);
void Timer1_delay(uint8 delay);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{

	uint8 option=0;
	uint8 pass_check;

	UART_ConfigType UART_configuration = {bit8,Parity_Disabled,one_stopbit,9600};
	TWI_ConfigType TWI_configuration = {bit_3,0b00000010};

	LCD_init();

	TWI_init(&TWI_configuration);
	//TWI_init();
	Buzzer_init();
	UART_init(&UART_configuration);
	DcMotor_Init();


	while(UART_recieveByte() != MC_READY);
	UART_sendByte(MC_READY);


	while(1)
	{
		LCD_clearScreen();
		readPassfromEEPROM(EEPROM_address,mainPassword,pass_size);
		LCD_arrayDisplay(mainPassword,pass_size);

		option=UART_recieveByte();

		switch(option)
		{
		case set :
			UART_recieveArray(password1,pass_size);
			UART_recieveArray(password2,pass_size);
			pass_check=password_checker(password1,password2,pass_size);

			if(pass_check==1)
			{
				UART_sendByte(1);
				assignPass(mainPassword,password1,pass_size);
				savePasstoEEPROM(EEPROM_address,mainPassword,pass_size);
			}
			else
			{
				UART_sendByte(0);

			}
			break;

		case open :
			UART_recieveArray(password1,pass_size);
			readPassfromEEPROM(EEPROM_address,mainPassword,pass_size);
			pass_check=password_checker(mainPassword,password1,pass_size);

			if(pass_check==1)
			{
				UART_sendByte(1);
				DcMotor_Rotate(CW,100);
				Timer1_delay(15);


				UART_sendByte(1);
				DcMotor_Rotate(stop,0);
				Timer1_delay(3);


				UART_sendByte(1);
				DcMotor_Rotate(A_CW,100);
				Timer1_delay(15);

				UART_sendByte(1);
				DcMotor_Rotate(stop,0);


			}
			else
			{
				UART_sendByte(0);
			}
			break;

		case change :
			UART_recieveArray(password1,pass_size);
			readPassfromEEPROM(EEPROM_address,mainPassword,pass_size);
			pass_check=password_checker(mainPassword,password1,pass_size);

			if(pass_check==1)
			{
				UART_sendByte(1);
			}
			else
			{
				UART_sendByte(0);

			}
			break;

		case error :

			Buzzer_on();
			Timer1_delay(60);
			Buzzer_off();
			UART_sendByte(1);

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LCD_arrayDisplay(uint8 *array,uint8 size)
{
	uint8 i ;
	for(i=0;i<size;i++)
	{
		LCD_intgerToString(array[i]);
		LCD_displayCharacter(' ');
	}
}
void UART_recieveArray(uint8 *array,uint8 size)
{
	uint8 i ;
	for(i=0;i<size;i++)
	{
		array[i] = UART_recieveByte();
	}
}

uint8 password_checker(uint8 *pass1,uint8 *pass2,uint8 size)
{
	uint8 i;
	for(i=0;i<size;i++)
	{
		if(pass1[i]!= pass2[i])
		{return 0;}
	}
	return 1;
}

uint32 calc(uint8 key,uint8 position, uint32 currentVal){
	uint32 multi =0;

	if(position ==0)
		multi=10000;
	else if(position ==1)
		multi=1000;
	else if(position ==2)
		multi=100;
	else if(position ==3)
		multi=10;
	else if(position ==4)
		multi=1;

	return currentVal+key*multi;

}


uint32 convertArrayToInt(uint8 *array,uint8 size)
{
	uint8 i ;
	uint32	number=0;
	for(i=0;i<size;i++)
	{
		number=calc(array[i],i,number);
	}
	return number;
}


void assignPass(uint8 *array1,uint8 *array2,uint8 size)
{
	uint8 i ;

	for(i=0;i<size;i++)
	{
		array1[i]=array2[i];
	}

}

void savePasstoEEPROM(uint16 address , uint8 *array,uint8 size)
{
	uint8 i ;
	for(i=0;i<size;i++)
	{
		EEPROM_writeByte(address+i,array[i]);
		_delay_ms(10);
	}

}
void readPassfromEEPROM(uint16 address , uint8 *array,uint8 size)
{
	uint8 i ;
	for(i=0;i<size;i++)
	{
		EEPROM_readByte(address+i,&array[i]);
		_delay_ms(10);
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

