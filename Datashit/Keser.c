/*
 * Keser.c
 *
 * Created: 27.3.2014. 16:47:14
 *  Author: FrostGiant
 */ 


#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include "Button.h"
#include "PWM.h"
#include "LCD.h"
/************************************************************************
	A program for reading the data from USART and then echo the data back
*************************************************************************/

//#include <avr/io.h>

/************************************************************************
	Initialize the USART with the Baudrate that is passes as an argument
	Enable both the transmitter and receiver of USART
*************************************************************************/
void _init_usart( unsigned int Baudrate )
{
  UBRRH = (unsigned char)(Baudrate>>8);
  //UBRRH = 0x00;
  UBRRL = (unsigned char)Baudrate; // Set the baud rate
  UCSRB = 0x18; // enable transmitter and reciever
  UCSRC = 0x86; // set as 8 bit data, no parity bit and 1 stop bit.
}

/************************************************************************
	Receive the byte that arrives on the receiver
*************************************************************************/
unsigned char usart_receive(void)
{
	  unsigned char data;
	  while ((UCSRA & 0x80) == 0x00); // Wait for till the data arrives data
	  data=UDR;	// Read the data from UDR, as soon as the data arrives
	  return data;
}

/************************************************************************
	Transmit the data that is passed as argument
*************************************************************************/
void usart_transmit_ch( unsigned char data )
{
  while ((UCSRA & 0x20) == 0x00); // Wait until the transmit buffer is empty
  UDR = data; // As soon as the buffer is empty, put the data in UDR
}
/************************************************************************
	Transmit the data that is passed as argument
*************************************************************************/
void UsartTransmitCharByFunction( unsigned char data )
{
	for (uint8_t i=0; i<2; i++)
	{
		while ((UCSRA & 0x20) == 0x00); // Wait until the transmit buffer is empty
		UDR = data; // As soon as the buffer is empty, put the data in UDR
	}
}
/************************************************************************
	Transmit the data that is passed as argument
*************************************************************************/
void usart_transmit_string(char *_string)
{
	while (*_string > 0)
	{
		usart_transmit_ch(*_string++);
	}
}
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR;
}
int main(void)
{
	MCUCSR |=1<<JTD;
	MCUCSR |=1<<JTD;
	DDRD = 0x0F;
	
	DDRA=0xFF;
	
	 char A = 'D',b='K';
	_init_usart(12); // For 4800 bps, value of UBBR = 12
	LCDinit();
	while(1)
	{
		A=USART_Receive();
		Position(1,2);
		Send_Character(A);
		UsartTransmitCharByFunction(A);
	}
}
