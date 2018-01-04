/*
 * RTC.h
 *
 * Created: 20.1.2015. 18:50:09
 *  Author: FrostGiant
 */ 


#ifndef RTC_H_
#define RTC_H_

#ifndef F_CPU
#define F_CPU	1000000UL
#endif

#include <util/delay.h>  
  
#define RTC_adress	0xD0 //0b1101000

typedef union
{
	uint16_t Int;
	uint8_t Byte[2];
}Byte_Concat;


typedef struct
{
	uint8_t seconds,minutes,hours,day;
	uint8_t date,month,year;
}TimeStamp;

TimeStamp RTC_GetTime();
TimeStamp RTC_ParseTime(TimeStamp Time);
void RTC_SetTime(TimeStamp Time);
TimeStamp RTC_ParseToTime(TimeStamp time);
int Get_RTC_Temp();
  TimeStamp RTC_GetTime() {	 TimeStamp Time;	 //set reg pointer increments automatically	 i2c_start_wait(RTC_adress | I2C_WRITE);	 i2c_write(0x00);
	 i2c_stop();	 _delay_us(10);	 	 i2c_start_wait(RTC_adress | I2C_READ);	 Time.seconds = i2c_readAck();	 Time.minutes = i2c_readAck();	 Time.hours	 = i2c_readAck();	 Time.day	 = i2c_readAck();	 Time.date	 = i2c_readAck();	 Time.month	 = i2c_readAck();	 Time.year	 = i2c_readNak();	 i2c_stop();	 Time = RTC_ParseTime(Time);	 return Time; }int Get_RTC_Temp(){	uint8_t a,b=0;	 i2c_start_wait(RTC_adress | I2C_WRITE);	 i2c_write(0x11);
	 i2c_stop();	 _delay_us(10);	 	 i2c_start_wait(RTC_adress | I2C_READ);	 a=i2c_readAck();	 b=i2c_readNak();	 i2c_stop();	 Byte_Concat Temp;	 Temp.Byte[0]=a; 	/* Temp.Byte[1]=a;*/	 return Temp.Int;} TimeStamp RTC_ParseTime(TimeStamp Time) {	 uint8_t Hnibble,Lnibble;	 //parsing Seconds	 Hnibble=Time.seconds>>4;	 Lnibble=Time.seconds & 0x0F;	 Time.seconds=Hnibble*10 | Lnibble;	 //parsing Minutes	 Hnibble=Time.minutes>>4;	 Lnibble=Time.minutes & 0x0F;	 Time.minutes=Hnibble*10 + Lnibble;	 //parsing hours 24 h mode	 Hnibble =Time.hours<<2;	 Hnibble =Hnibble>>6;	 Lnibble= Time.hours & 0x0F;	 Time.hours = Hnibble*10 + Lnibble;	 //parsing date	 Hnibble = Time.date>>4;	 Lnibble = Time.date & 0x0F;	 Time.date=Hnibble*10+Lnibble;	 //parsing month	 Hnibble = Time.month>>4;	 Lnibble = Time.month & 0x0F;	 Time.month = Hnibble*10 + Lnibble;	 //parsing year	 Hnibble = Time.year>>4;	 Lnibble = Time.year & 0x0F;	 Time.year = Hnibble*10 + Lnibble;	 return Time; } void RTC_SetTime(TimeStamp Time) {	 Time=RTC_ParseToTime(Time);	 //to do : set RTC regs	 i2c_start_wait(RTC_adress | I2C_WRITE);	 i2c_write(0x00);	 i2c_write(Time.seconds);	 i2c_write(Time.minutes);	 i2c_write(Time.hours);	 i2c_write(Time.day);	 i2c_write(Time.date);	 i2c_write(Time.month);	 i2c_write(Time.year);	 i2c_stop(); } TimeStamp RTC_ParseToTime(TimeStamp time) {	 uint8_t Hnibble,Lnibble;	 if (time.seconds<0 || time.seconds>59)
	 {
		 time.seconds=1;
	 }	 if (time.minutes<0 || time.minutes >59)
	 {
		 time.minutes=1;
	 }	 if (time.hours<0 || time.hours>23)
	 {
		 time.hours=1;
	 }	 if (time.day<1 || time.day>7)
	 {
		 time.day=1;
	 }	 if (time.date<1 || time.date>31)
	 {
		 time.date=1;
	 }	 if (time.month<1 || time.month>12)
	 {
		 time.month=1;
	 }	 if (time.year<0 || time.year>99)
	 {
		 time.year=1;
	 }	 //parse seconds	 Hnibble=time.seconds/10;	 Lnibble=time.seconds%10;	 time.seconds=(Hnibble<<4)|Lnibble;	 //parse minutes	 Hnibble=time.minutes/10;	 Lnibble=time.minutes%10;	 time.minutes=(Hnibble<<4)+Lnibble;	 //parse hours 24 h mode	 Hnibble=time.hours/10;	 Lnibble=time.hours%10;	 time.hours=(Hnibble<<4)+Lnibble;	 //parse date	 Hnibble=time.date/10;	 Lnibble=time.date%10;	 time.date=(Hnibble<<4)+Lnibble;	 //month	 Hnibble=time.month/10;	 Lnibble=time.month%10;	 time.month=(Hnibble<<4)+Lnibble;	 //year	 Hnibble=time.year/10;	 Lnibble=time.year%10;	 time.year=(Hnibble<<4)+Lnibble;	 return time; }

#endif /* RTC_H_ */