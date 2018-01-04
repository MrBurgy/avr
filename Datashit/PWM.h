/*
 * PWM.h
 *
 * Created: 27.3.2014. 18:00:45
 *  Author: FrostGiant
 */ 


#ifndef PWM_H_
#define PWM_H_

void PWMinit(void)
{
	int CompareValue=15001;
	DDRD |= 1<<PIND5;									//pin na kojemu je OCR1A mora bit setan na output
	TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0;		//setings
	TCCR1B |= 1<<WGM13 | 1<<WGM12 | 1<<CS10;		//-//-
	ICR1 = CompareValue;							//sadrži compare vrijednost
	OCR1A = ICR1 - 0; //18000
}
void DutyCicle(int Value)
{
	OCR1A = ICR1 - Value;
}



#endif /* PWM_H_ */