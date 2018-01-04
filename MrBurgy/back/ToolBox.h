/*
 * ToolBox.h
 *
 * Created: 17.1.2015. 23:11:36
 *  Author: MrBurgy
 */ 


#ifndef TOOLBOX_H_
#define TOOLBOX_H_

#include <util/delay.h>
#include "util/Button.h"

#define Bit_Test(Reg,Bit)	(Reg&(1<<bit))
#define Bit_Set(Reg,Bit)	(Reg |=1<<Bit)
#define Bit_Reset(Reg,Bit)	(Reg &=~(1<<Bit))

typedef struct
{
	char DataReg;
	uint8_t	Pin;
}Blinky;

typedef union
{
	int Data;
	uint8_t nibble[2];
}Data_Nibbles;

void DefineBlinky(Blinky *Blinky_id, char *DataDirectionREG, uint8_t PortNumber);
void ToogleBlinky(Blinky *Blinky_id);
void BlinkyState(Blinky *Blinky_id,uint8_t State);
void VolatileDelay_ms(int Delay);
void VolatileDelay_us(int Delay);

void DefineBlinky(Blinky *Blinky_id, char *DataDirectionREG, uint8_t PortNumber)
{
	Blinky_id->DataReg=DataDirectionREG[3];
	Blinky_id->Pin=PortNumber;
	switch (Blinky_id->DataReg)
	{
		case 'A':
		DDRA |=1<<PortNumber;
		break;
		case 'B':
		DDRB |=1<<PortNumber;
		break;
		case 'C':
		DDRC |=1<<PortNumber;
		break;
		case 'D':
		DDRD |=1<<PortNumber;
		break;
	}
	
}
void ToogleBlinky(Blinky *Blinky_id)
{
	switch (Blinky_id->DataReg)
	{
		case 'A':
		PORTA ^=1<<Blinky_id->Pin;
		break;
		case 'B':
		PORTB ^=1<<Blinky_id->Pin;
		break;
		case 'C':
		PORTC ^=1<<Blinky_id->Pin;
		break;
		case 'D':
		PORTD ^=1<<Blinky_id->Pin;
		break;
	}
}
void BlinkyState(Blinky *Blinky_id,uint8_t State)
{
	switch(Blinky_id->DataReg)
	{
		case 'A':
		((State)==(0)?(PORTA &=~(1<<Blinky_id->Pin)):(PORTA |=1<<Blinky_id->Pin));
		break;
		case 'B':
		((State)==(0)?(PORTB &=~(1<<Blinky_id->Pin)):(PORTB |=1<<Blinky_id->Pin));
		break;
		case 'C':
		((State)==(0)?(PORTC &=~(1<<Blinky_id->Pin)):(PORTC |=1<<Blinky_id->Pin));
		break;
		case 'D':
		((State)==(0)?(PORTD &=~(1<<Blinky_id->Pin)):(PORTD |=1<<Blinky_id->Pin));
		break;
	}
} 
void VolatileDelay_ms(int Delay)
{
	for (int i=0;i<Delay;i++)
	{
		_delay_ms(1);
	}
}
void VolatileDelay_us(int Delay)
{
	for (int i=0;i<Delay;i++)
	{
		_delay_us(1);
	}
}
#endif /* TOOLBOX_H_ */