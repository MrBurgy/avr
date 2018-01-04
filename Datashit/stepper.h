/*
 * IncFile1.h
 *
 * Created: 7.10.2013. 1:34:50
 *  Author: FrostGiant
 */ 


#ifndef stepper_H_
#define stepper_H_

#include <avr/io.h>
#include <util/delay.h>

#define		DDRStepper_Port			DDRC
#define		Stepper_Port			PORTC
#define		Stepper_Speed			1

#define		Step1			0
#define		Step2			1
#define		Step3			2
#define		Step4			3

#define		N_of_steps		64
#define		gear_reduction		8

void Stepper_CWrevolution(float Num_of_rot)
{
	for(float i=0;i<N_of_steps*gear_reduction*Num_of_rot;i++)
	{
		Stepper_Port |= 1<<Step1;//1
		_delay_ms(Stepper_Speed);
		Stepper_Port |= 1<<Step2;//12
		_delay_ms(Stepper_Speed);
		Stepper_Port &=~(1<<Step1);//2
		_delay_ms(Stepper_Speed);
		Stepper_Port |= 1<<Step3;//23
		_delay_ms(Stepper_Speed);
		Stepper_Port &=~(1<<Step2);//3
		_delay_ms(Stepper_Speed);
		Stepper_Port |= 1<<Step4;//34
		_delay_ms(Stepper_Speed);
		Stepper_Port &=~ (1<<Step3);//4
		_delay_ms(Stepper_Speed);
		Stepper_Port |= 1<<Step1;//41
		_delay_ms(Stepper_Speed);
		Stepper_Port &=~(1<<Step4);//1
	}
	clear_stepper();
	_delay_ms(5);
}

void Stepper_init()
{
	DDRStepper_Port |= 1<<Step1 | 1<<Step2 | 1<<Step3 | 1<<Step4;
	Stepper_Port &=~(1<<Step1 | 1<<Step2 | 1<<Step3 | 1<<Step4);
}

void clear_stepper()
{
	Stepper_Port &=~(1<<Step1 | 1<<Step2 | 1<<Step3 | 1<<Step4);
}

void Stepper_CCWrevolution(float Num_of_rot)
{
	_delay_ms(10);
	for(float i=0;i<N_of_steps*gear_reduction*Num_of_rot;i++)
	{
		Stepper_Port |=1<<Step4;//4
		_delay_ms(Stepper_Speed);
		Stepper_Port |=1<<Step3;//43
		_delay_ms(Stepper_Speed);
		Stepper_Port &=~ (1<<Step4);//3
		_delay_ms(Stepper_Speed);
		Stepper_Port |=1<<Step2;//32
		_delay_ms(Stepper_Speed);
		Stepper_Port &=~(1<<Step3);//2
		_delay_ms(Stepper_Speed);
		Stepper_Port |=1<<Step1;//21
		_delay_ms(Stepper_Speed);
		Stepper_Port &=~ (1<<Step2);//1
		_delay_ms(Stepper_Speed);
		Stepper_Port |=1<<Step4;//14
		_delay_ms(Stepper_Speed);
		Stepper_Port &=~ (1<<Step1);//4
	}
	clear_stepper();
	_delay_ms(5);
}


#endif /* INCFILE1_H_ */