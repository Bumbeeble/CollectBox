
#include "I2C.h"
#include "stdio.h"
/* 宏定义 --------------------------------------------------------------------*/
#define I2C_SLAVE_ADDRESS7        0xA0                     //作为从设备的地址

#define I2C_SPEED                 100000                   //I2C速度
#define I2C_CLOCK               5
void init_I2C1(void)
{
	P13_OpenDrain_Mode;					// Modify SCL pin to Open drain mode. don't forget the pull high resister in circuit
	P14_OpenDrain_Mode;					// Modify SDA pin to Open drain mode. don't forget the pull high resister in circuit

    /* Set I2C clock rate */
    I2CLK = I2C_CLOCK; 

    /* Enable I2C */
    set_I2CEN;    
}

bit I2C_Reset_Flag;
//========================================================================================================
void I2C_SI_Check(void)
{
		if (I2STAT == 0x00)
		{
				I2C_Reset_Flag = 1;
				set_STO;
				SI = 0;
				if(SI)
				{
						clr_I2CEN;
						set_I2CEN;
						clr_SI;
						clr_I2CEN;		
				} 	
		}	
}
/*******************************************************************/
void I2C_StartTransmission(uint8_t Address, uint8_t Direction)
{
	 /* Step1 */
    set_STA;                                /* Send Start bit to I2C EEPROM */
    clr_SI;
    while (!SI);
	if (I2STAT != 0x08)                     /* 0x08:  A START condition has been transmitted*/
	{
		printf("\nI2C 'start' error");
		return;
	}
	/* Step2 */
    I2DAT = Address +Direction;       /* Send (SLA+W) to EEPROM */
    clr_STA;                                /* Clear STA and Keep SI value in I2CON */    
    clr_SI;
    while (!SI);
	if (I2STAT != 0x18)                     /* 0x18: SLA+W has been transmitted; ACK has been received */              
	{
        printf("\nI2C 'Send SLA+W' error");
		return;
	}
}

/*******************************************************************/
void I2C_WriteData(uint8_t send_data)
{
	
	/* Step3 */
    I2DAT = send_data;             /* Send I2C EEPROM's High Byte Address */
    clr_SI;
    while (!SI);
	if (I2STAT != 0x28)                 /* 0x28:  Data byte in S1DAT has been transmitted; ACK has been received */
    {
		printf("\nI2C 'Write Data' error");
		return;
	}
}


/*******************************************************************/
uint8_t I2C_ReadData(uint8_t ack_status)
{
	if(ack_status)
	{
		clr_AA;
	}
	else
	{
		clr_AA;
	}
	clr_SI;
    while (!SI);
	return I2DAT;
  
}
void I2C_Stop(void)
{
    set_STO;
	clr_SI;
	while (SI)                        /* Check STOP signal */
	{
		I2C_SI_Check();
		if (I2C_Reset_Flag)
		{
			
			return;
		}
	}
}