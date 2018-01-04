/*
* bb.c
*
* Created: 18-Mar-17 21:10:42
* Author : MrBurgy
*/


//***************************************************************************
//  File Name    : i2cslave.c
//  Version      : 1.0
//  Description  : I2C Slave AVR Microcontroller
//  Author       : MrBurgy
//  Target       : AVR ATmega32
//  IDE          : Atmel AVR Studio 7
//  Programmer   : USBasp
//  Last Updated : 18-Mar-17 21:10:42
//***************************************************************************
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

// MCP23008 8 Bit I/O Extension Simulation Address and Register Address
#define MCP23008_ADDR 0x4E

void i2c_slave_action(void)
{
	
}
ISR(TWI_vect)
{
	unsigned char twi_status;
	// Disable Global Interrupt
	cli();
	// Get TWI Status Register, mask the prescaler bits (TWPS1,TWPS0)
	twi_status=TWSR & 0xF8;

	switch(twi_status) 
	{
		case TW_SR_SLA_ACK:      // 0x60: SLA+W received, ACK returned
			//i2c_state=0;           // Start I2C State for Register Address required
			TWCR |= (1<<TWINT);    // Clear TWINT Flag
			break;
		case TW_SR_DATA_ACK:     // 0x80: data received, ACK returned
			PORTA = TWDR;
			TWCR |= (1<<TWINT);    // Clear TWINT Flag
			break;
		case TW_SR_STOP:         // 0xA0: stop or repeated start condition received while selected
			TWCR |= (1<<TWINT);    // Clear TWINT Flag
			break;
		case TW_ST_SLA_ACK:      // 0xA8: SLA+R received, ACK returned
		case TW_ST_DATA_ACK:     // 0xB8: data transmitted, ACK received
			TWDR = 0x66;
			TWCR |= (1<<TWINT);    // Clear TWINT Flag
			break;
		case TW_ST_DATA_NACK:    // 0xC0: data transmitted, NACK received
		case TW_ST_LAST_DATA:    // 0xC8: last data byte transmitted, ACK received
		case TW_BUS_ERROR:       // 0x00: illegal start or stop condition
		default:
			TWCR |= (1<<TWINT);    // Clear TWINT Flag
	}
	// Enable Global Interrupt
	sei();
}
void main(void)
{
	DDRA = 0xFF;
	PORTD = 0x0;
	DDRC = 0xFF;
	DDRB = 0x0;
	PORTB |= 1<<PINB3;
	//PORTC |= 1<<PINC0 | 1<<PINC1;
	// Initial I2C Slave
	TWAR = MCP23008_ADDR & 0xFE; // Set I2C Address, Ignore I2C General Address 0x00
	TWDR = 0x00;                 // Default Initial Value
	// Start Slave Listening: Clear TWINT Flag, Enable ACK, Enable TWI, TWI Interrupt Enable
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
	// Enable Global Interrupt
	sei();

	while (1)
	{
		_delay_us(100);                       // Wait for debouching
		if (bit_is_clear(PINB, PB3))
		{
			PORTC |= 1<<PINC6 | 1<<PINC7;
		}
		else
		{
			PORTC = 0x0;
		}
	}
}