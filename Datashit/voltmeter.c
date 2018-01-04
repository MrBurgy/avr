#include <avr/io.h>
#define  F_CPU	1000000UL
#include "LCD.h"
#include "Button.h"
#include <util/delay.h>

int main(void)
{
	DDRB=0;
	PORTB=0b00001111;
	ADMUX |=1<<REFS0;
	ADCSRA |=1<<ADEN | 1<<ADPS2;
	LCDinit();
	int var=0;
	float prc=0,temp=19.52;
	uint8_t highB=0, lowB=0;
	while(1)
	{
		if (Button_Test(0,PINB,0,100))
		{
			/*var+=15;
			Position(10,2);
			Send_NumberI(var);
			Send_String("    ");*/
			temp+=0.1;
		
		}
		if (Button_Test(1,PINB,1,100))
		{
			//Send_Command(0x01);
			temp-=0.1;
		}
		ADCSRA |=1<<ADSC;
		while (ADSC==1)
		{		
		}
		lowB=ADCL;
		highB=ADCH;
		Position(1,1);
		Send_String("volts: ");
		var = (highB<<8)|lowB;
		prc= (float) var/1023*temp;
		Position(8,1);
		Send_NumberF(prc);
		Send_Character('V');
		Send_String("   ");
		Position(1,2);
		Send_NumberF(temp);

	}
}