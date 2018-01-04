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
 
	 i2c_stop();
	 i2c_stop();
	 {
		 time.seconds=1;
	 }
	 {
		 time.minutes=1;
	 }
	 {
		 time.hours=1;
	 }
	 {
		 time.day=1;
	 }
	 {
		 time.date=1;
	 }
	 {
		 time.month=1;
	 }
	 {
		 time.year=1;
	 }

#endif /* RTC_H_ */