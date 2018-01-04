/*
 * BitBangSpi.h
 *
 * Created: 14.9.2015. 23:33:34
 *  Author: FrostGiant
 */ 


#ifndef BITBANGSPI_H_
#define BITBANGSPI_H_
/*
#define BBspiDDR		DDRB //DDRD
#define	BBspiPORT		PORTB//PORTD
#define	BBspiPIN		PINB//PIND

#define BBspiClk		PINB3//PIND6 
#define BBspiData		PINB4//PIND7
*/

#define BBspiDDR		DDRD
#define	BBspiPORT		PORTD
#define	BBspiPIN		PIND

#define BBspiClk		PIND6
#define BBspiData		PINB5
//#define slaveDevice		PIND1		// NOT USE


void BBspiInitMaster();
void BBspiInitSlave();
uint8_t BBspiRecvB();
uint16_t BBspiRecvW();
void flash_BBclk();
void BBspiSendW(uint16_t command);
void BBspiSendB(uint8_t command);

void BBspiInitMaster()
{
	BBspiDDR |=1<<BBspiClk | 1<<BBspiData;
}
void BBspiInitSlave()
{
	BBspiDDR &=~(1<<BBspiClk);
	BBspiDDR &=~(1<<BBspiData);
}
void flash_BBspiClk()
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
		flash_BBspiClk();
	}
	//clr data line
	BBspiPORT &=~(1<<BBspiData);
}
uint8_t BBspiRecvB()
{
	uint8_t clkDown=0;	//0 down
	uint8_t data=0,bit_position=0;

	while(bit_position < 8)
	{

		if (bit_is_set(BBspiPIN,BBspiClk))
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

	}
	return data;
}
void BBspiSendW(uint16_t command)
{
	for (uint8_t i=0; i<16; i++)
	{
		if (command & (1<<(15-i)))
		{
			BBspiPORT |=1<<BBspiData;
			
		}
		else
		{
			BBspiPORT &=~(1<<BBspiData);
		}
		flash_BBspiClk();
	}
	//clr data line
	BBspiPORT &=~(1<<BBspiData);
}
uint16_t BBspiRecvW()
{
	uint8_t clkDown=0;	//0 down
	uint8_t bit_position=0;
	uint16_t data=0;
	
	while(bit_position < 16)
	{

		if (bit_is_set(BBspiPIN,BBspiClk))
		{
			
			//check if clk takt is reseted
			if (clkDown == 0)
			{
				//set var for clk down
				clkDown =1;
				//peocess data on data line
				if (BBspiPIN &(1<<BBspiData))
				{
					data |=1<<(15-bit_position);
					
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

	}
	return data;
}



#endif /* BITBANGSPI_H_ */