#include "encoder.h"


uint8_t State_Encoder = 0;
uint8_t State_Encoder_Before = 0;			//上一次读取的结果
uint8_t Real_Encoder = 0;					//消抖后的结果

#define C_Encoder_Debounce_Max 10				//消抖
uint8_t cnt_Encoder = 0;

uint8_t Trg_Encoder = 0;					//下降沿触发
uint8_t Cont_Encoder = 0;					//上一次的状态

int16_t output_Encoder = 0;

static void Get_EncoderState(void);
static void Check_Encoder_MSG(void);

/**
  * @brief Read encoder data
  * @param  None
  * @retval the data  of encoder
  */
int16_t Get_EncoderData(void )
{
	return output_Encoder;
}
/**
  * @brief Clr encoder data
  * @param  None
  * @retval the data  of encoder
  */
void Clr_EncoderData(void )
{
	output_Encoder = 0;
}
/**
  * @brief Set encoder data
  * @param  int16_t data_Set; //data 
  * @retval None
  */
void Set_EncoderData(int16_t data_Set )
{
	output_Encoder = data_Set;
}

Status Get_Encoder_Status(uint8_t pos)
{
	return (Status)(Real_Encoder&(0x01<<pos));
}

Status Get_Encoder_Continue(uint8_t pos)
{
	return (Status)(Cont_Encoder&(0x01<<pos));
}

/**
  * @brief Set encoder data
  * @param  
  * @retval None
  */
Status Get_Encoder_Trg(uint8_t pos)
{
	return (Status)(Trg_Encoder&(0x01<<pos));
}

/**
  * @brief Clear trg status
  * @param  int16_t data_Set; //data 
  * @retval None
  */
void Clear_Encoder_Trg(uint8_t pos)
{
	Trg_Encoder &= ~(0x01<<pos);
}


/**
  * @brief Encoder init
  * @param  None
  * @retval None
  */
void Encoder_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//IO init
	_Encoder_Config();
	//防止上电时为按下状态触发按下动作
	Get_EncoderState();
	State_Encoder_Before = State_Encoder;
	Real_Encoder = State_Encoder;
	Check_Encoder_MSG();
	Trg_Encoder = 0;
}


static void Get_EncoderState(void )
{
	if(_Read_EncodeA())
		State_Encoder &= ~(0x01<<EncodeA);
	else
		State_Encoder |= 0x01<<EncodeA;
	
	if(_Read_EncodeB())
		State_Encoder &= ~(0x01<<EncodeB);
	else
		State_Encoder |= 0x01<<EncodeB;
	
}

/**
  * @brief Encoder debounce
  * @param  None
  * @retval None
  */
static void Check_Encoder_Debounce(void)
{
	if(State_Encoder == State_Encoder_Before)
	{
		//状态未改变
		if(cnt_Encoder >= C_Encoder_Debounce_Max)
		{
			//时间到
			//cnt_Encoder = 0;
			Real_Encoder = State_Encoder;		//得到稳定值
		}
		else
		{
			cnt_Encoder++;		//计数
		}
	}
	else
	{
		cnt_Encoder = 0;
		State_Encoder_Before = State_Encoder;			//按键状态改变，保存按键状态
	}
}


/**
  * @brief Get trige signal 
  * @param  None
  * @retval None
  */
static void Check_Encoder_MSG(void)
{
	Trg_Encoder = Real_Encoder&(Real_Encoder^Cont_Encoder);			//发生改变且为按下状态的键为按下触发
	Cont_Encoder = Real_Encoder;												//
}




void Read_Encoder(void)
{
	Get_EncoderState();
	Check_Encoder_Debounce();
	Check_Encoder_MSG();
	if(Get_Encoder_Trg(EncodeA))
	{
		
		Clear_Encoder_Trg(EncodeA);
		if(Get_Encoder_Status(EncodeB))
			output_Encoder++;
		else
			output_Encoder--;
	}
	
}












