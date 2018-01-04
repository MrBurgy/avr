/*
 * IncFile1.h
 *
 * Created: 6.10.2013. 3:23:46
 *  Author: FrostGiant
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
// #define  F_CPU	1000000UL
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#define		DDRLCDdataPort		DDRA
#define		LCDdataPort			PORTA
#define		DDRLCDcontrol		DDRC
#define		LCDcontrol			PORTC
#define		DDRLCDBackLight		DDRC
#define		LCDBackLightPort	PORTC

#define		BackLightPin		4
#define		Enable				7	//2
#define		ReadWrite			6	//1
#define		RScontrol			5	//0

void Check_if_busy();
void Flash_Enable();
void Send_Command(int command);
void Send_String(char *_string );
void Send_Character(char figure );
void LCDinit();
void Send_Number(int Num,uint8_t x, uint8_t y);
void Position(uint8_t X,uint8_t Y);

uint8_t rows[4]= {0, 64,20,84};

void LCD_clr_line(uint8_t LineNum);

void LCD_clr_line(uint8_t LineNum)
{
	Position(0,LineNum);
	Send_String("                ");	//16 spaceova
	Position(0,LineNum);
}
void Flash_Enable()
{
	_delay_us(5);
	LCDcontrol |= 1<<Enable;
	_delay_us(10);
	LCDcontrol &=~(1<<Enable);
	_delay_us(5);
}
void Check_if_busy()
{
	DDRLCDdataPort=0;					//set data port for reading(for flag)
	LCDcontrol&=~(1<<RScontrol);
	LCDcontrol|=1<<ReadWrite;_delay_us(5);
	while(LCDdataPort>=0x80)		//refresh data from display
	{
		Flash_Enable();
	}
	DDRLCDdataPort = 0xFF;				//data port for sending
	_delay_us(5);
}
void Send_Character(char figure )
{ 
	Check_if_busy();
	LCDdataPort=figure;
	LCDcontrol |=1<<RScontrol;
	LCDcontrol &=~(1<<ReadWrite);
	Flash_Enable();
	LCDdataPort=0;
	_delay_us(5);
}
void Send_String(char *_string)
{
	while (*_string > 0)
	{
		Send_Character(*_string++);
		_delay_us(10);
	}
}
void Send_Command(int command)
{
	Check_if_busy();
	LCDdataPort=command;
	LCDcontrol &=~((1<<RScontrol) | (1<<ReadWrite)); 
	Flash_Enable();
	LCDdataPort=0;
}
void LCDinit()
{
	DDRLCDcontrol|=1<<Enable | 1<<RScontrol | 1<<ReadWrite;
	_delay_ms(1);
	
 	Send_Command(0x01);
	_delay_ms(2);
	Send_Command(0x38);
	_delay_us(60);
	Send_Command(0b00001110);
	_delay_us(60);
	DDRLCDBackLight|=1<<BackLightPin;
	LCDBackLightPort|=1<<BackLightPin;
	Send_String("LCD ready!");
	_delay_ms(2000);
	Send_Command(0x01);
	_delay_ms(2);
}
void Send_NumberI(int Num)
{
	char _string[5];
	itoa(Num,_string,10);
	Send_String(_string);
}
void Send_NumberF(float Num)
{
	char _string[5];
	dtostrf(Num,6,2,_string);
	Send_String(_string);
}
void Position(uint8_t X,uint8_t Y)
{
	Send_Command(0x80 + rows[Y-1]+(X-1));
	_delay_ms(2);
}
#endif /* LCD_H_ */