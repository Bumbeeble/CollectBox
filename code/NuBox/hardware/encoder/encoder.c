#include "encoder.h"


uint8_t State_Encoder = 0;
uint8_t State_Encoder_Before = 0;			//��һ�ζ�ȡ�Ľ��
uint8_t Real_Encoder = 0;					//������Ľ��

#define C_Encoder_Debounce_Max 10				//����
uint8_t cnt_Encoder = 0;

uint8_t Trg_Encoder = 0;					//�½��ش���
uint8_t Cont_Encoder = 0;					//��һ�ε�״̬

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
	//��ֹ�ϵ�ʱΪ����״̬�������¶���
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
		//״̬δ�ı�
		if(cnt_Encoder >= C_Encoder_Debounce_Max)
		{
			//ʱ�䵽
			//cnt_Encoder = 0;
			Real_Encoder = State_Encoder;		//�õ��ȶ�ֵ
		}
		else
		{
			cnt_Encoder++;		//����
		}
	}
	else
	{
		cnt_Encoder = 0;
		State_Encoder_Before = State_Encoder;			//����״̬�ı䣬���水��״̬
	}
}


/**
  * @brief Get trige signal 
  * @param  None
  * @retval None
  */
static void Check_Encoder_MSG(void)
{
	Trg_Encoder = Real_Encoder&(Real_Encoder^Cont_Encoder);			//�����ı���Ϊ����״̬�ļ�Ϊ���´���
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












