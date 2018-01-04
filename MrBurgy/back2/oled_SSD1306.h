/*
* oled_SSD1306.h
*
* Created: 7.12.2014. 14:20:09
*  Author: FrostGiant
*/


#ifndef OLED_SSD1306_H_
#define OLED_SSD1306_H_
#include "i2cmaster.c"
#include "font.c"
#include <stdlib.h>

#define OLED_ADDR_W	0x78
#define SSD1306_LCDWIDTH      128
#define SSD1306_LCDHEIGHT      64
#define SSD1306_SETCONTRAST   0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP	0xA0
#define SSD1306_CHARGEPUMP	0x8D
#define SSD1306_EXTERNALVCC	0x1
#define SSD1306_SWITCHCAPVCC	0x2

void oled_init();
void oled_setXY(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void oled_command(uint8_t cmd);
void oled_data(uint8_t cmd);
void oled_clr_char(uint8_t X,uint8_t Y);
void oled_clr();
void oled_send_CHAR_table(uint8_t rows, uint8_t X,uint8_t Y);
void oled_send_char(char Ch, uint8_t X,uint8_t Y);
void oled_send_string(char *_string,uint8_t posX,uint8_t posY);
void oled_Fill_area(uint8_t startX,uint8_t endX,uint8_t startY,uint8_t endY);
void oled_send_int(unsigned int num,uint8_t posX,uint8_t posY);
char a[] = {0x00,0x00,0x00,0x14,0x24,0x23,0x36,0x00};

void oled_command(uint8_t cmd)
{
	uint8_t control = 0x00; // some use 0X00 other examples use 0X80. I tried both
	i2c_start(OLED_ADDR_W | I2C_WRITE);
	//i2c_write(OLED_ADDR_W);
	i2c_write(control); // This is Command
	i2c_write(cmd);
	i2c_stop();
}
void oled_data(uint8_t cmd)
{
	i2c_start(OLED_ADDR_W | I2C_WRITE);
	//i2c_write(OLED_ADDR_W);
	i2c_write(0X40); // This byte is DATA
	i2c_write(cmd);
	i2c_stop();
}
void oled_init()
{
	//printf("oled_init() pocetak\n");

	// Init sequence for 128x64 OLED module
	i2c_init();
	_delay_ms(100);
	oled_command(SSD1306_DISPLAYOFF);                    // 0xAE
	oled_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
	oled_command(0x80);                			// the suggested ratio 0x80
	oled_command(SSD1306_SETMULTIPLEX);                  // 0xA8
	oled_command(0x3F);
	oled_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
	oled_command(0x0);                                   // no offset
	oled_command(SSD1306_SETSTARTLINE);// | 0x0);        // line #0
	oled_command(SSD1306_CHARGEPUMP);                    // 0x8D
	oled_command(0x14); 								// using internal VCC
	oled_command(SSD1306_MEMORYMODE);                    // 0x20
	oled_command(0x00);									// 0x00 horizontal addressing
	oled_command(SSD1306_SEGREMAP | 0x1);				// rotate screen 180
	oled_command(SSD1306_COMSCANDEC);					// rotate screen 180
	oled_command(SSD1306_SETCOMPINS);                    // 0xDA
	oled_command(0x12);
	oled_command(SSD1306_SETCONTRAST);                   // 0x81
	oled_command(0xCF);
	oled_command(SSD1306_SETPRECHARGE);                  // 0xd9
	oled_command(0xF1);
	oled_command(SSD1306_SETVCOMDETECT);                 // 0xDB
	oled_command(0x40);
	oled_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
	oled_command(SSD1306_NORMALDISPLAY);                 // 0xA6
	oled_command(SSD1306_DISPLAYON);                     //switch on OLED
	_delay_ms(1);
	oled_clr();
}
void oled_clr_char(uint8_t X,uint8_t Y)
{
	oled_setXY(X,Y,X+8,Y+8);
	i2c_start_wait(OLED_ADDR_W | I2C_WRITE);
	i2c_write(0x40);
	for (int i=0;i<7;i++)
	{
		i2c_write(0x00);
	}
	i2c_stop();
}
void oled_Fill_area(uint8_t startX,uint8_t endX,uint8_t startY,uint8_t endY)
{
	if(endX > 128)endX=128;
	if(endY > 64)endY = 64;
	uint8_t Xsize,Ysize;
	Xsize=endX-startX;
	Ysize=endY-startY;
	oled_setXY(startX,startY,endX,endY);
	i2c_start_wait(OLED_ADDR_W | I2C_WRITE);
	i2c_write(0x40);
	for (uint16_t i=0; i<=Xsize*Ysize/8;i++)
	{
		i2c_write(0xFF);
	}
	i2c_stop();
}
void oled_clr()
{
	oled_setXY(0,0,127,63);
	i2c_start_wait(OLED_ADDR_W | I2C_WRITE);
	i2c_write(0x40);
	for (uint16_t i=0; i<=128*64/8;i++)
	{
		i2c_write(0x00); 
	}
	i2c_stop(); 
}

void oled_send_CHAR_table(uint8_t rows, uint8_t X,uint8_t Y)
{
	oled_setXY(X,Y,X+8,Y+8);
	oled_clr_char(X,Y);
	i2c_start_wait(OLED_ADDR_W | I2C_WRITE);
	i2c_write(0x40);
	i2c_write(0x00);
	for (int i=0;i<8;i++)
	{
		i2c_write(pgm_read_byte(&simpleFont[rows][i]));
	}
	i2c_stop();
}
void oled_send_char(char Ch, uint8_t X,uint8_t Y)
{
	oled_setXY(X,Y,X+6,Y+8);
	oled_clr_char(X,Y);
	i2c_start_wait(OLED_ADDR_W | I2C_WRITE);
	i2c_write(0x40);
	i2c_write(0x00);
	uint8_t rows =  Ch-32;
	for (int i=0;i<7;i++)
	{
		//simpleFont [Ch][i];
		i2c_write(pgm_read_byte(&simpleFont[rows][i]));
	}
	i2c_stop();
}
void oled_send_string(char *_string,uint8_t posX,uint8_t posY)
{
	while (*_string > 0)
	{
		oled_send_char(*_string,posX,posY);
		posX+=6;
		_string++;
	}
	
}
void oled_setXY(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	if(x1<128)
	{
		oled_command(0x21);	// column
		oled_command(x0);
		oled_command(x1);
	}
	if(y1<64)
	{
		oled_command(0x22);	// row
		oled_command(y0);
		oled_command(y1);
	}
}
void oled_send_int(unsigned int num,uint8_t posX,uint8_t posY)
{
	char buffer[15];
	//itoa(num,buffer,10);
	sprintf(buffer,"%d  ",num);
	oled_send_string(buffer,posX,posY);
}
#endif /* OLED_SSD1306_H_ */