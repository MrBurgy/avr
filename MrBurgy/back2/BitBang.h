/*
 * BitBang.h
 *
 * Created: 8.9.2015. 0:10:34
 *  Author: FrostGiant
 */ 


#ifndef BITBANG_H_
#define BITBANG_H_
#include <avr/io.h>


#define		BBDDR				DDRC
#define		BBPort				PORTC
#define		BBclk				PINC1
#define		BBdata				PINC2

void BB_init();
void flash_BBclk();
void BB_send(uint8_t command);

void flash_BBclk()
{
	BBPort |=1<<BBclk;
	BBPort &=~(1<<BBclk);
}
void BB_send(uint8_t command)
{ 
	for (uint8_t i=0; i<8; i++)
	{
		if (command & (1<<(7-i)))
		{
			BBPort |=1<<BBdata;
			
		}
		else
		{
			BBPort &=~(1<<BBdata);
		}
		flash_BBclk();
	}
	//clr data line
	BBPort &=~(1<<BBdata);
}
void BB_init()
{
	BBDDR|=1<<BBclk | 1<<BBdata; 
}



#endif /* BITBANG_H_ */