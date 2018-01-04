/*
 * Button.h
 *
 * Created: 13.12.2013. 1:12:47
 *  Author: FrostGiant
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>


char Button_Test(int ButtonNumber, unsigned char PinOfButton, unsigned char PortBit, int Confidence_Level);

char pressed[5];
int pressed_confidence_level[5];
int released_confidence_level[5];

char Button_Test(int ButtonNumber, unsigned char PinOfButton, unsigned char PortBit, int Confidence_Level)
{
	if (bit_is_clear(PinOfButton,PortBit))
	{
		pressed_confidence_level[ButtonNumber]++;
		released_confidence_level[ButtonNumber]=0;
		if (pressed_confidence_level[ButtonNumber] > Confidence_Level)
		{
			if (pressed[ButtonNumber] == 0)
			{
				pressed[ButtonNumber]=1;
				return 1;
			}
			pressed_confidence_level[ButtonNumber]=0;
		}
	}
	else
	{
		released_confidence_level[ButtonNumber]++;
		pressed_confidence_level[ButtonNumber]=0;
		if (released_confidence_level[ButtonNumber] > Confidence_Level)
		{
			pressed[ButtonNumber]=0;
			released_confidence_level[ButtonNumber]=0;
		}
	}
	return 0;
}



#endif /* BUTTON_H_ */