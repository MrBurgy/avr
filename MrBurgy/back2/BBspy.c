/*
 * BBspy.c
 *
 * Created: 12.9.2015. 22:48:35
 *  Author: FrostGiant
 */ 

#define	F_CPU	8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BBspiDDR		DDRD
#define	BBspiPORT		PORTD
#define	BBspiPIN		PORTD

#define BBspiClk		PIND7
#define BBspiData		PIND6
#define slaveDevice		PIND5


void BBspiInitMaster();
void BBspiInitSlave();

int main(void)
{
	DDRD |=1<<slaveDevice;
	PORTD |=1<<slaveDevice;
    while(1)
    {
        //TODO:: Please write your application code 
    }
}
void BBspiInitMaster()
{
	BBspiDDR |=1<<BBspiClk | 1<<BBspiData;
}
void BBspiInitSlave()
{
	BBspiDDR &=~(1<<BBspiClk | 1<<BBspiData);
}
void flash_BBclk()
{
	BBspiPORT |=1<<BBspiClk;
	_delay_ms(1);
	BBspiPORT &=~(1<<BBspiClk);
}
void BBspiSendB(uint8_t command)
{
	for (uint8_t i=0; i<8; i++)
	{
		if (command & (1<<(7-i)))
		{
			BBspiPORT |=1<<BBspiData;
			
		}
		else
		{
			BBspiPORT &=~(1<<BBspiData);
		}
		flash_BBclk();
	}
	//clr data line
	BBspiPORT &=~(1<<BBspiData);
}
uint8_t BBspiRecvB(uint8_t EnableFlag)
{
	uint8_t clkDown=0;	//0 down
	uint8_t data=0,bit_position=0;

	do
	{
		if (!(BBspiPIN &(1<<BBspiClk)))
		{
			//check if clk takt is reseted
			if (clkDown == 0)
			{
				//set var for clk down
				clkDown =1;
				//peocess data on data line
				if (BBspiPIN &(1<<BBspiData))
				{
					data |=1<<(7-bit_position);
				}
				//increment position
				bit_position++;
			} 	
		}
		else
		{
			//reset var for clk down
			clkDown=0;

		}

	}while(bit_position < 8);
	return data;
}