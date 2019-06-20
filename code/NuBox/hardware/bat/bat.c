#include "bat.h"
#include "output.h"
#include "adc.h"

//电池电量控制电压 3.0V/0x999; 3.2V/0xA3D; 3.3V/0xA8F; 3.4V/0xAE1; 3.6V/0xB85; 
#define 	C_vol_Bat_Low	0xAE1		//低于3.4V切换低电量
#define 	C_vol_Bat_High	0xB85		//高于3.6V切换到高电量

#define 	C_vol_Bat_outputLow		0x999		//放电保护；低于3.0V放电保护
#define 	C_vol_Bat_inputLow		0xA3D		//充电解除保护；低于3.2V不解除保护

xdata uint8_t offset_vol_Bat_State = 0;
xdata uint8_t offset_vol_Bat_outputLow = 0;
xdata uint8_t offset_vol_Bat_inputLow = 0;

#define C_offset_vol_Bat_State_Max 100
#define C_offset_vol_Bat_State_Min 10

#define C_offset_vol_Bat_outputLow_Max 100
#define C_offset_vol_Bat_outputLow_Min 10

#define C_offset_vol_Bat_inputLow_Max 100
#define C_offset_vol_Bat_inputLow_Min 10

bit F_vol_Bat_State = 0;
bit F_Bat_Protect = 0;
bit F_vol_Bat_outputLow = 0;
bit F_vol_Bat_inputLow = 0;

bit F_Bat_Full = 0;
xdata uint8_t Decnt_BatFull = 0;
#define C_Decnt_BatFull_Set	8

xdata uint8_t bat_level = 0, bat_level_Before = 0;
#define C_cnt_debounce_bat_level_SET	2*4
xdata uint8_t cnt_debounce_bat_level = C_cnt_debounce_bat_level_SET;//(C_cnt_debounce_bat_level_SET+C_SET_cnt_debounce_bat_level_MIN)/2;

const uint16_t C_Batlevel_Threshold[] = 
{
	0x1088,//3.1V------1
	0x1222,//3.4V------2
	0x13BB,//3.70V-----3
	0x1444,//3.80V-----4
	0x14A3,//3.87V-----5
	0x1570,//4.02V-----6
	0xFFFF//max
};


void Check_BatLevel(void )
{
	
	uint8_t i = 0;
	//比较得到电量等级
	for(i=0; i<=C_BatLevel_SetMax; i++)
	{
		if(vol_Bat <= C_Batlevel_Threshold[i])
			break;
	}
	//累计滤波
	if(i ==  bat_level)
	{
		cnt_debounce_bat_level = C_cnt_debounce_bat_level_SET;
	}
	else if(cnt_debounce_bat_level == 0)
	{
		cnt_debounce_bat_level = C_cnt_debounce_bat_level_SET;
		bat_level  = i;
	}
//	if(i > bat_level)
//	{
//		cnt_debounce_bat_level++;
//	}
//	else if(i < bat_level)
//	{
//		cnt_debounce_bat_level--;
//	}
//	
//	if(cnt_debounce_bat_level < C_SET_cnt_debounce_bat_level_MIN || cnt_debounce_bat_level > C_cnt_debounce_bat_level_SET)
//	{
//		//数据有效
//		bat_level  = i;
//		cnt_debounce_bat_level = (C_cnt_debounce_bat_level_SET+C_cnt_debounce_bat_level_SET)/2;
//	}
	
}
void bat_Handler(void )
{
	uint8_t i = 0;

//	if(F_Input_Status && F_CHG_END)
//	{
//		if(Decnt_BatFull == 0)
//		{
//			
//			F_Bat_Full = 1;
//		}
//	}
//	else
//	{
//		F_Bat_Full = 0;
//		Decnt_BatFull = C_Decnt_BatFull_Set;
//	}
//	
	if(F_Input_Status)
	{
		if(F_CHG_END)
		{
			if(Decnt_BatFull == 0)
			{
				
				F_Bat_Full = 1;
			}
		}
		else
		{
			Decnt_BatFull = C_Decnt_BatFull_Set;
		}
	}
	else
	{
		F_Bat_Full = 0;
		Decnt_BatFull = C_Decnt_BatFull_Set;
	}
	
	if(F_Bat_Protect)
	{
		//Protected
		if((!F_vol_Bat_inputLow) && F_Input_Status)
		{
			F_Bat_Protect = 0;
		}
	}
	else
	{
		//Normal
		if(F_vol_Bat_outputLow)
			F_Bat_Protect = 1;
	}
}


void check_BatVol(void )
{
	//State
	if(vol_Bat >= C_vol_Bat_High)
	{
		if((offset_vol_Bat_State++) >= C_offset_vol_Bat_State_Max)
		{
			offset_vol_Bat_State = (C_offset_vol_Bat_State_Max+C_offset_vol_Bat_State_Min)/2;
			F_vol_Bat_State = 1;
		}
	}
	else if(vol_Bat <= C_vol_Bat_Low)
	{
		if((offset_vol_Bat_State--) <= C_offset_vol_Bat_State_Min)
		{
			offset_vol_Bat_State = (C_offset_vol_Bat_State_Max+C_offset_vol_Bat_State_Min)/2;
			F_vol_Bat_State = 0;
		}
	}
	
	//output low
	if(vol_Bat <= C_vol_Bat_outputLow)
	{
		if((offset_vol_Bat_outputLow++) >= C_offset_vol_Bat_outputLow_Max)
		{
			offset_vol_Bat_outputLow = (C_offset_vol_Bat_outputLow_Max+C_offset_vol_Bat_outputLow_Min)/2;
			F_vol_Bat_outputLow = 1;
		}
	}
	else
	{
		if((offset_vol_Bat_outputLow--) <= C_offset_vol_Bat_outputLow_Min)
		{
			offset_vol_Bat_outputLow = (C_offset_vol_Bat_outputLow_Max+C_offset_vol_Bat_outputLow_Min)/2;
			F_vol_Bat_outputLow = 0;
		}
	}
	
	
	//input low
	if(vol_Bat <= C_vol_Bat_inputLow)
	{
		if((offset_vol_Bat_inputLow++) >= C_offset_vol_Bat_inputLow_Max)
		{
			offset_vol_Bat_inputLow = (C_offset_vol_Bat_inputLow_Max+C_offset_vol_Bat_inputLow_Min)/2;
			F_vol_Bat_inputLow = 1;
		}
	}
	else
	{
		if((offset_vol_Bat_inputLow--) <= C_offset_vol_Bat_inputLow_Min)
		{
			offset_vol_Bat_inputLow = (C_offset_vol_Bat_inputLow_Max+C_offset_vol_Bat_inputLow_Min)/2;
			F_vol_Bat_inputLow = 0;
		}
	}
		
}
