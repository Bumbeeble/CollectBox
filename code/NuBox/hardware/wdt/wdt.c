/*
* wdt.c
*/
#include "wdt.h"
/***********************************************************************
	WDT CONFIG enable 
	warning : this macro is only when ICP not enable CONFIG WDT function
	copy this marco code to you code to enable WDT reset.
	
	This code will change the config code
	
	IAP¸ÄÐ´ÅäÖÃÎÄ¼þ
	
************************************************************************/
void Enable_WDT_Reset_Config(void)
{
	set_IAPEN;		//Enable IAP
    IAPAL = 0x04;
    IAPAH = 0x00;		//address
	
    IAPFD = 0x50;		//Data		
    IAPCN = 0xE1;		//command
    set_CFUEN;			//Enable IAP write or erase CONFIG
    set_IAPGO;          //trigger IAP
	
	while((CHPCON&SET_BIT6)==SET_BIT6);          //check IAPFF (CHPCON.6)
    clr_CFUEN;
    clr_IAPEN;
}
void WDT_config(uint8_t preScalar)
{
	preScalar &= 0x07;
	TA=0xAA;TA=0x55;WDCON = preScalar;						//Setting WDT prescale 
	
	set_WDCLR;														//Clear WDT timer
	while((WDCON|~SET_BIT6)==0xFF);				//confirm WDT clear is ok before into power down mode
	EA = 1;
	set_WDTR;				//WDT run
	
}