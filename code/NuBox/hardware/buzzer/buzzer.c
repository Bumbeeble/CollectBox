/*
* 
*/

#include  "buzzer.h"

bit	F_BUZ_Status = 0;
static uint8_t Buz0_CNT = 0;			//Buzzer decounter
static uint8_t Buz0_Type = 0;			//蜂鸣器连续响还是间歇响

void  Set_Buz0(uint8_t Buz_CNT, uint8_t Buz_Type)
{
	Buz0_CNT = Buz_CNT;
	Buz0_Type = Buz_Type;
	
}

void Buz0_OFF(void )
{
	
	F_BUZ_Status = 0;
}
void Buz0_ON(void )
{
	F_BUZ_Status = 1;
}

void Buz0_RUN(void )
{
	if(F_BUZ_Status)
		Buz0_OFF();
	else
		Buz0_ON();
}
//Buzzer initial ,using time0 ch0/PA8
void Buzzer_Init(void)
{
	Buz0_OFF();
	IO_BUZ_OFF();
	IO_BUZ_OUT();
}

void Buzzer_Handler()
{
	
	if(Buz0_CNT>0)
	{
		Buz0_CNT --;//F_BUZ_Status;
		if(Buz0_Type)
			Buz0_RUN();
		else
			Buz0_ON();
	}
	else
	{
		Buz0_OFF();
	}
}



