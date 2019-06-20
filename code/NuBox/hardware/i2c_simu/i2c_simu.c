#include "i2c_simu.h"

/*
----------------------------------------------------
����stc��IO�ڿ��ƼĴ���
PxM1: 0�������1������
PxM0: 0: ��ͨ���ܣ�1�����⹦��
PxM1/PxM0
00����ɵ�51���ݣ��߹��������������
01����ͨIO������������߹��������������
10������̬����
11����©״̬��OD����Ҫ�����ڲ������������ڲ�����
----------------------------------------------------
*/
void I2C_GPIOInit()
{
	I2C_SDA_H;	
	I2C_SCK_H;
	P13_OpenDrain_Mode;					// Modify SCL pin to Open drain mode. don't forget the pull high resister in circuit
	P14_OpenDrain_Mode;					// Modify SDA pin to Open drain mode. don't forget the pull high resister in circuit

}


/*******************************************************************************
* Function Name  : I2C_delay
* Description    : Simulation IIC Timing series delay
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_delay(void)
{
		//Delay(3  );//I2C����ʱ��Ƶ��400k���������2.5us����30
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
bool I2C_Start(void)
{
	I2C_SDA_H;
	I2C_delay();
	I2C_SCK_H;
	I2C_delay();
	if(!I2C_SDA_read)
	{
		#if I2C_DEBUG
		printf("\n\rThe I2C bus is busy!");
		
		# endif	
		return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	}
	I2C_SDA_L;
	I2C_delay();
	if(I2C_SDA_read)
	{	
		#if I2C_DEBUG
		printf("\n\rThe I2C bus is busy!");
		
		# endif	
		return FALSE;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�

	}	
	I2C_SDA_L;
	I2C_delay();
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Stop(void)
{
	I2C_SCK_L;
	I2C_delay();
	I2C_SDA_L;
	I2C_delay();
	I2C_SCK_H;
	I2C_delay();
	I2C_SDA_H;
	I2C_delay();
	I2C_delay();
}
#if 0
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack(void)
{	
	I2C_SCK_L;
	I2C_delay();
	I2C_SDA_L;
	I2C_delay();
	I2C_SCK_H;	
	I2C_delay();
	I2C_delay();
	
	I2C_SCK_L;
	I2C_delay();
}   
#endif
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck(void)
{	
	I2C_SCK_L;
	I2C_delay();
	I2C_SDA_H;
	I2C_delay();
	I2C_SCK_H;
	I2C_delay();
	I2C_SCK_L;
	I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
									 The statue of the ACK will be output by the USART
* Input          : None
* Output         : None
* Return         : none
****************************************************************************** */
void I2C_WaitAck(void) 	
{

	I2C_SCK_L;
	I2C_delay();
	I2C_SDA_H;			
	I2C_delay();
	
	I2C_SCK_H;
	I2C_delay();
	I2C_delay();

	
	if(I2C_SDA_read)
	{
		#ifdef I2C_DEBUG
		printf("No ACK from the I2Cbus");
		#endif
	}
	else
	{
		#ifdef I2C_DEBUG
//		printf("Get ACK from the I2Cbus\n");
		#endif
	}
	I2C_SCK_L;
	I2C_delay();
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte(uint8_t SendByte) //���ݴӸ�λ����λ//
{
    uint8_t i=8;
    while(i--)
    {
		I2C_delay();
        I2C_SCK_L;
        I2C_delay();
		
		if(SendByte&0x80)
			I2C_SDA_H;  
		else 
			I2C_SDA_L;
		
        SendByte<<=1;
        I2C_delay();
	  
		I2C_SCK_H;
        I2C_delay();
    }
    I2C_SCK_L;
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
unsigned char I2C_ReadByte(void)  //���ݴӸ�λ����λ//
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    I2C_SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      I2C_SCK_L;
      I2C_delay();
	  I2C_SCK_H;
      I2C_delay();	
      if(I2C_SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    I2C_SCK_L;
    return ReceiveByte;
} 
