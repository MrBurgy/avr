/*
 * nokia.h
 *
 * Created: 8.3.2015. 15:33:03
 *  Author: FrostGiant
 */ 


#ifndef NOKIA_H_
#define NOKIA_H_

#include "MrBurgy/spi.c"
#include <stdlib.h>
#include "MrBurgy/font.c"

#define Data	1
#define	Command	0
#define Enable	0
#define Disable	1

#define		Right_arrow			130
#define		Left_arrow			131
#define		Down_arrow			132
#define		Up_arrow			133
#define		Rotate_Cw			134
#define		Rotate_CCw			135


#define		nokia_DDR			DDRB
#define		nokia_PORT			PORTB

//All active low

#define		nokia_RST_pin		PINB0
#define		nokia_CE_pin		PINB1//when low -> command
#define		nokia_DC_pin		PINB2

#define		nokia_CE(x)			((x)==(0) ? (nokia_PORT &=~(1<<nokia_CE_pin)) : (nokia_PORT |=1<<nokia_CE_pin))
#define		nokia_RST(x)		((x)==(0) ? (nokia_PORT &=~(1<<nokia_RST_pin)) : (nokia_PORT |=1<<nokia_RST_pin))
#define		nokia_DC(x)			((x)==(0) ? (nokia_PORT &=~(1<<nokia_DC_pin)) : (nokia_PORT |=1<<nokia_DC_pin))

void nokia_send_comand(uint8_t command);
void nokia_send_comandS(uint8_t command);
void nokia_send_Byte(uint8_t data,uint8_t X,uint8_t Y);
void nokia_init();
void nokia_setX_Y(uint8_t X,uint8_t Y);
void nokia_clr(void);
void nokia_Enable_transmision(uint8_t DC_mode,uint8_t X,uint8_t Y);
void nokia_Disable_transmision();
void nokia_send_data(uint8_t data);
void nokia_reset();
void nokia_send_char(char Ch, uint8_t X, uint8_t Y);
void nokia_send_string(char *Buffer, uint8_t X, uint8_t Y);
void nokia_send_int(int data,uint8_t X,uint8_t Y);
void nokia_send_float(float data,uint8_t X,uint8_t Y);
void nokia_set_pixel(uint8_t on_off,uint8_t X,uint8_t Y);
void nokia_DrawLine(uint8_t on_off, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void nokia_dynamic_init();
void nokia_clr_line(uint8_t Y);


void nokia_clr_line(uint8_t Y)
{
	nokia_Enable_transmision(1,0,Y);
	for (int i=0;i<7;i++)
	{
		nokia_send_data(0x00);
	}
	nokia_Disable_transmision();
}
void nokia_dynamic_init()
{
	spi_init(0,1,1,2,0);
	_delay_ms(1);
	nokia_DDR |=1<<nokia_RST_pin | 1<<nokia_CE_pin | 1<<nokia_DC_pin;
	nokia_PORT|=1<<nokia_RST_pin | 1<<nokia_CE_pin | 1<<nokia_DC_pin;
	_delay_ms(1);
	nokia_RST(0);
	_delay_ms(60);
	nokia_RST(1);
	
	nokia_DC(Command);
	nokia_CE(Enable);
	spi_send(0b00100001);	//extended func set
	spi_send(0b10010000);	//set Vop
	spi_send(0b00100000);	//func set default
	spi_send(0b00001100);	//sets display configuration D=1 E=0;
	nokia_CE(Disable);
	nokia_clr();
	_delay_ms(200);
}

void nokia_DrawLine(uint8_t on_off, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	float slope;
	slope=((float)(y2-y1)/(float)(x2-x1));
	//nokia_send_float(slope,0,0);
	for (uint8_t i=x1;i<x2;i++)
	{
		y1=(y2-(slope*(x2-i)));
		nokia_set_pixel(on_off,i,y1);
	}
	
}

void nokia_send_comand(uint8_t command)
{
	nokia_DC(Command);
	nokia_CE(Enable);
	spi_send(command);
	nokia_CE(Disable);
}
void nokia_send_comandS(uint8_t command)
{
	nokia_DC(Command);
	spi_send(command);
}
void nokia_send_Byte(uint8_t data,uint8_t X,uint8_t Y)
{
	if (Y<0) Y=0;
	if (Y>5) Y=5;
	if (X<0) X=0;
	if (X>83)X=83;
	nokia_DC(Command);
	nokia_CE(Enable);
	spi_send(0x40 | Y);
	spi_send(0x80 | X);
	_delay_ms(1);
	nokia_DC(Data);
	spi_send(data);
	nokia_CE(Disable);
}
void nokia_send_data(uint8_t data)
{
	nokia_DC(Data);
	spi_send(data);
}
void nokia_init()
{
	spi_init(0,1,1,2,0);
	_delay_ms(1);
	nokia_DDR |=1<<nokia_RST_pin | 1<<nokia_CE_pin | 1<<nokia_DC_pin;
	nokia_PORT|=1<<nokia_RST_pin | 1<<nokia_CE_pin | 1<<nokia_DC_pin;
	_delay_ms(1);
	nokia_RST(0);
	_delay_ms(50);
	nokia_RST(1);
	
	nokia_DC(Command);
	nokia_CE(Enable);
// 	spi_send(0b00100001);	//extended func set
// 	spi_send(0b10010000);	//set Vop
// 	spi_send(0b00100000);	//func set default
// 	spi_send(0b00001100);	//sets display configuration D=1 E=0;
	
	spi_send(0x21);	// set LCD mode
	spi_send(0xc8);	// set bias voltage
	spi_send(0x06);	// temperature correction
	spi_send(0x13);	// 1:48
	spi_send(0x20);	// use bias command 
	spi_send(0x0c);	// set LCD mode,display normally
	nokia_CE(Disable);
	nokia_clr();
	nokia_send_string("Nokia 5110:",0,0);
	nokia_send_string("Init: !",0,1);
	_delay_ms(900);
	nokia_clr();
}
void nokia_setX_Y(uint8_t X,uint8_t Y)
{
	if (Y<0) Y=0;
	if (Y>5) Y=5;
	if (X<0) X=0;
	if (X>83)X=83;
	nokia_DC(Command);
	nokia_CE(Enable);
	spi_send(0x40 | Y);
	spi_send(0x80 | X);
	nokia_CE(Disable);
}
void nokia_clr()
{
	nokia_setX_Y(0,0);
	nokia_DC(Data);
	nokia_CE(Enable);
	for (uint8_t i=0;i<6;i++)
	{
		for (uint8_t j=0;j<84;j++)
		{
			spi_send(0x00);
		}
	}
	nokia_CE(Disable);
}
void nokia_Enable_transmision(uint8_t DC_mode, uint8_t X,uint8_t Y)
{
	nokia_setX_Y(X,Y);
	if (DC_mode == 0) nokia_DC(0);
	else  nokia_DC(1);
	nokia_CE(Enable);
}
void nokia_Disable_transmision()
{
	nokia_CE(Disable);
}
void nokia_reset()
{
	nokia_RST(0);
	_delay_ms(50);
	nokia_RST(1);
	nokia_init();
}
void nokia_send_char(char Ch, uint8_t X, uint8_t Y)
{
	nokia_Enable_transmision(1,X,Y);
	uint8_t table_row=Ch-32;
	for (int i=0;i<7;i++)
	{
		nokia_send_data(pgm_read_byte(&simpleFont[table_row][i]));
	}
	nokia_Disable_transmision();
}
void nokia_send_string(char *Buffer, uint8_t X, uint8_t Y)
{
	while (*Buffer > 0)
	{
		nokia_send_char(*Buffer,X,Y);
		X+=6;
		Buffer++;
	}
}
void nokia_send_int(int data,uint8_t X,uint8_t Y)
{
	char buffer[10];
	//itoa(data,buffer,10);
	sprintf(buffer," %d  ",data);
	nokia_send_string(buffer,X,Y);
}
void nokia_send_float(float data,uint8_t X,uint8_t Y)
{
	char buffer[10];
	dtostrf(data,3,1,buffer);
	nokia_send_string(buffer,X,Y);
}
void nokia_set_pixel(uint8_t on_off,uint8_t X,uint8_t Y)
{
	//84x6ratio
	uint8_t v=0, Byte=0;
	v=Y/8;
	if (on_off == 0) Byte &=~(1<<(Y%8));
	else Byte|=1<<(Y%8);
	nokia_send_Byte(Byte,X,v);
}


#endif /* NOKIA_H_ */