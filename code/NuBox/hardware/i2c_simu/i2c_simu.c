#include "i2c_simu.h"

/*
----------------------------------------------------
关于stc的IO口控制寄存器
PxM1: 0：输出；1：输入
PxM0: 0: 普通功能；1：特殊功能
PxM1/PxM0
00：与旧的51兼容，高灌电流，低拉电流
01：普通IO口推挽输出，高灌电流，高拉电流
10：高阻态输入
11：开漏状态，OD，需要禁用内部下拉，开启内部上拉
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
		//Delay(3  );//I2C总线时钟频率400k，操作间隔2.5us以上30
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
		return FALSE;	//SDA线为低电平则总线忙,退出
	}
	I2C_SDA_L;
	I2C_delay();
	if(I2C_SDA_read)
	{	
		#if I2C_DEBUG
		printf("\n\rThe I2C bus is busy!");
		
		# endif	
		return FALSE;	//SDA线为高电平则总线出错,退出

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
void I2C_SendByte(uint8_t SendByte) //数据从高位到低位//
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
unsigned char I2C_ReadByte(void)  //数据从高位到低位//
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
