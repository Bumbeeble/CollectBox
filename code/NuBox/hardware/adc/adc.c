/*
* adc.c
	N76E003一共8路ADC
	
*/
#include "adc.h"

xdata uint16_t MAX_ADCValue = 0;          //最大值
xdata uint16_t MIN_ADCValue = 0;          //最小值
xdata uint16_t SUM_ADCValue = 0;          //累加和
xdata uint8_t cnt_ADCCycle = 0;           //采样次数计数
xdata uint8_t Flag_ADCCycle = 0;           //10次完成标志位
xdata uint16_t Data_ADCValue = 0;          //累加和
xdata uint8_t cnt_adc_channel = 0;

xdata  uint8_t F_ADC_Over = 0;
xdata uint16_t current_Left = 0;
xdata uint16_t current_Right = 0;
xdata uint16_t vol_Bat = 0;
bit		F_TIM_ADCValue = 0;
idata struct ADC_Type ADC_Data[NUM_ADChannel];
bit	F_Ready_ADCData = 0;


void ADC_Deconfig(void)
{
	
	
	ADCCON0 = 0x00;			//	
	ADCCON1 = 0x00;			//ADC使能
	ADCCON2 = 0x00;			//
	//关闭引脚数字输入
	AINDIDS = 0x00;//(0x01<<channel_LeftCur)| (0x01<<channel_BGVol);//| (0x01<<channel_RightCur);//	channel_LeftCur | channel_RightCur | channel_RightCur;
	ADCDLY = 0x00;			//ADC延时启动
	//READ_BANDGAP();
}

void ADC_Config(void)
{
	uint8_t i = 0;
	ADC_Data[0].channel = ADC_Channel_0;
	ADC_Data[1].channel = ADC_Channel_1;
	ADC_Data[2].channel = ADC_Channel_4;
	ADC_Data[3].channel = ADC_Channel_5;
	ADC_Data[4].channel = ADC_Channel_6;
	ADC_Data[5].channel = ADC_Channel_7;
	
	ADCCON0 = 0x00;			//	
	ADCCON1 = 0x01;			//ADC使能
	ADCCON2 = 0x00;			//
	//关闭引脚数字输入
	AINDIDS = (0x01<<ADC_Channel_0)| (0x01<<ADC_Channel_1)| (0x01<<ADC_Channel_6)| (0x01<<ADC_Channel_7)| (0x01<<ADC_Channel_4)| (0x01<<ADC_Channel_5);//| (0x01<<channel_RightCur);//	channel_LeftCur | channel_RightCur | channel_RightCur;
//	for(i=0; i<NUM_ADChannel; i++)
//	{
//		AINDIDS |= (0x01<<(ADC_Data[i].channel));
//	}
	ADCDLY = 0x00;			//ADC延时启动
	READ_BANDGAP();
}

void ADC_SelectChannel(uint8_t adc_ch)
{
	if(adc_ch >8 )
		return;
	ADCCON0 &= 0xF0;					//低4bit清零
	ADCCON0 += adc_ch;					//加上通道数
}
//单通道多次采样
void Read_ADCChannel(void )
{
	uint16_t Result_ADCValue = 0;
    //AD转换完成，同时10次未完成
	if(ADCF && Flag_ADCCycle)           
	{
		ADCF = 0;
		//Get ADCValue result
        Result_ADCValue = ADCRH;
		Result_ADCValue <<= 4;
		Result_ADCValue += ADCRL;
		
		if(Result_ADCValue > MAX_ADCValue)
			MAX_ADCValue = Result_ADCValue;
		
		if(Result_ADCValue < MIN_ADCValue)
			MIN_ADCValue = Result_ADCValue;
			
		SUM_ADCValue += Result_ADCValue;
		
        //判断10次采样是否结束
        cnt_ADCCycle++;
		if(cnt_ADCCycle >= MAX_ADCCycle)
		{
            //10次转化完毕
			cnt_ADCCycle = 0;
			SUM_ADCValue -= MIN_ADCValue;
			SUM_ADCValue -= MAX_ADCValue;
			
			SUM_ADCValue /= (MAX_ADCCycle-2);	
			
			Data_ADCValue = SUM_ADCValue;
			
			MIN_ADCValue = 0xFFFF;
			MAX_ADCValue = 0x0000;
			SUM_ADCValue = 0;
			Flag_ADCCycle = 0;				//10 times of ADCValue convert complete
			
//			tim_cal = cnt_cal;
			
		}
		else
		{
            ADCF = 0;
			ADCS = 1;			//Start ADC conv
		}
		
	}
}


//读取单通道采样值，设定下一通道采样值
void Cycle_ADCChannel(void)
{
    if (Flag_ADCCycle == 0 && F_ADC_Over == 0)
    {
		Flag_ADCCycle = 1;
		ADC_Data[cnt_adc_channel].Data = Data_ADCValue;//((uint32_t)Data_ADCValue) *3300/4096;
		
		cnt_adc_channel++;
		if(cnt_adc_channel >= NUM_ADChannel)
		{
			cnt_adc_channel = 0;
			F_ADC_Over = 1;
			F_Ready_ADCData = 1;
			//3.072V as voltage reference
			if(ADC_Data[0].Data > 2048)
			{
				ADC_Data[0].sign = 1;
				ADC_Data[0].value = ADC_Data[0].Data - 2048;
			}
			else
			{
				ADC_Data[0].sign = 0;
				ADC_Data[0].value = 2048 - ADC_Data[0].Data;
			}
			ADC_Data[0].value = ((uint32_t)ADC_Data[0].value)*3300/4096;
			
			
		}
		//else
		{
			ADC_SelectChannel(ADC_Data[cnt_adc_channel].channel);
			ADCF = 0;
			ADCS = 1;
		}
    }
}


//多通道采样函数
void Read_ADC(void )
{
	Read_ADCChannel();			//单通道多次采样
	Cycle_ADCChannel();				//读取采样值，切换到下一通道
		
}
void ADC_Cycle_Start(void )
{
	//start ADC
	Flag_ADCCycle = 1;
	cnt_adc_channel = 0;
	ADC_SelectChannel(ADC_Data[cnt_adc_channel].channel);
	ADCF = 0;
	ADCS = 1;
}
void Get_ADCValue(void )
{
	if(F_ADC_Over)//F_TIM_ADCValue && 
	{
		//F_TIM_ADCValue = 0;
		F_ADC_Over = 0;
		
		//start ADC
		
		ADC_Cycle_Start();
	}
	
	
}

xdata uint16_t Bandgap_Value, Bandgap_Voltage;

xdata uint8_t BandgapHigh,BandgapLow,BandgapMark;

void READ_BANDGAP()
{
		uint16_t Bandgap_Voltage_Temp;
	
set_IAPEN;
	IAPCN = READ_UID;
	IAPAL = 0x0d;
    IAPAH = 0x00;
    set_IAPGO;
	BandgapLow = IAPFD;
	BandgapMark = BandgapLow&0xF0;
			
		if (BandgapMark==0x80)
		{
				BandgapLow = BandgapLow&0x0F;
				IAPAL = 0x0C;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapHigh = IAPFD;
				Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
				Bandgap_Voltage_Temp = Bandgap_Value*3/4;
				Bandgap_Voltage = Bandgap_Voltage_Temp - 33;			//the actually banggap voltage value is similar this value.
		}
		if (BandgapMark==0x00)
		{
				BandgapLow = BandgapLow&0x0F;
				IAPAL = 0x0C;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapHigh = IAPFD;
				Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
				Bandgap_Voltage= Bandgap_Value*3/4;
		}
		if (BandgapMark==0x90)
		{
				IAPAL = 0x0E;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapHigh = IAPFD;
				IAPAL = 0x0F;
				IAPAH = 0x00;
				set_IAPGO;
				BandgapLow = IAPFD;
				BandgapLow = BandgapLow&0x0F;
				Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
				Bandgap_Voltage= Bandgap_Value*3/4;
		}
		clr_IAPEN;
//			printf ("\n BG High = %bX",BandgapHigh); 
//			printf ("\n BG Low = %bX",BandgapLow); 
//			printf ("\n BG ROMMAP = %e",Bandgap_Voltage); 
}
	


