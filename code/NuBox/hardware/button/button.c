/*
* button.c
	button0: PC14
*/

#include "button.h"
uint8_t cnt_Button = 0;
uint8_t State_Button = 0;
uint8_t State_Button_Before = 0;

uint8_t Real_Button = 0;
uint8_t Trg_Button = 0;					//����������Ϣ
uint8_t Cont_Button = 0;					//����������Ϣ


static void Check_ButtonState(void);
static void Check_Button_MSG(void);


Status Get_Button_Trg(uint8_t pos)
{
	return (Status)(Trg_Button&(0x01<<pos));
}

void Clear_Button_Trg(uint8_t pos)
{
	Trg_Button &= ~(0x01<<pos);
}

Status Get_Button_Status(uint8_t pos)
{
	return (Status)(Real_Button&(0x01<<pos));
}

Status Get_Button_Continue(uint8_t pos)
{
	return (Status)(Cont_Button&(0x01<<pos));
}



/**
  * @brief ������ʼ��
  * @param  None
  * @retval None
  */
void Button_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
	P2S |= 0x81;		//P20 as input, and pull up 
	//��ֹ�ϵ�ʱΪ����״̬�������¶���
	Check_ButtonState();
	State_Button_Before = State_Button;
	Real_Button = State_Button;
	Check_Button_MSG();
	Trg_Button = 0;
}
/**
  * @brief ��ȡ����״̬
  * @param  None
  * @retval None
  */
static void Check_ButtonState(void)
{
//��ȡ����״̬�����µ͵�ƽΪ1
	if(_Read_Button0())
		State_Button &= ~(0x01);
	else
		State_Button |= 0x01;
	if(_Read_Button1())
		State_Button &= ~(0x01<<1);
	else
		State_Button |= 0x01<<1;
	

}

/**
  * @brief ����״̬����
  * @param  None
  * @retval None
  */
static void Check_Button_Debounce(void)
{
	//msg_ButtonClick = _Read_Button0();
	if(State_Button == State_Button_Before)
	{
		//״̬δ�ı�
		if(cnt_Button >= C_Button_Debounce_Max)
		{
			//ʱ�䵽
			//cnt_Button = 0;
			Real_Button = State_Button;		//�õ��ȶ�ֵ
		}
		else
		{
			cnt_Button++;		//����
		}
	}
	else
	{
		cnt_Button = 0;
		State_Button_Before = State_Button;			//����״̬�ı䣬���水��״̬
	}
}

/**
  * @brief �õ�������Ϣ
  * @param  None
  * @retval None
  */
static void Check_Button_MSG(void)
{
	Trg_Button = Real_Button&(Real_Button^Cont_Button);			//�����ı���Ϊ����״̬�ļ�Ϊ���´���
	Cont_Button = Real_Button;												//
}

void Read_Button(void)
{
	Check_ButtonState();
	Check_Button_Debounce();
	Check_Button_MSG();
}


