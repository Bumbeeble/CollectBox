/*
* uart0.c


*/

#include "uart.h"
#include "sys_tim.h"
#include "led.h"
#include  "halfduplex.h"
#include "define.h"
//??

#define Length_TXBuf	100
static xdata uint8_t buf_TX[Length_TXBuf] = {0};
static xdata uint8_t pos_TX_OUT = 0;
static xdata uint8_t pos_TX_IN = 0;	

static xdata uint8_t cnt_TX = 0;
static bit	Flag_TX_Full = 0;
static bit	Flag_TX_Empty = 1;


#define size_recvBuf 50
//xdata uint8_t cnt_recvBuf = 0;

xdata uint8_t cnt_recvBuf = 0;
xdata uint8_t buf_Recv[size_recvBuf] = {0};
#define C_decnt_recvbuf_Set 100
xdata uint8_t decnt_recvbuf = 0;

char putchar (char ch)
{
#if	Bumbee_DEBUG 
	Send_Data_To_UART1(ch);
#endif
	return(ch);
}
/**/
/*
void Send_Data_To_UART1 (uint8_t c)
{
    TI_1 = 0;
    SBUF_1 = c;
    while(TI_1==0);
}
*/
/*
* Redefine of //printf
*/
void Send_Data_To_UART1 (uint8_t c)
{
	while(Flag_TX_Full);
	buf_TX[pos_TX_IN] = c;
	pos_TX_IN++;					//??????
	if(pos_TX_IN >= Length_TXBuf)
		pos_TX_IN = 0; 

UART1_Interrupt_OFF();
	cnt_TX++;	 					//????
UART1_Interrupt_ON();
	if(cnt_TX>=Length_TXBuf)
	 	Flag_TX_Full = 1;			//????	  

	if(Flag_TX_Empty)
	{
		SBUF_1 = (unsigned char)buf_TX[pos_TX_OUT];
		pos_TX_OUT++;
		if(pos_TX_OUT >= Length_TXBuf)
			pos_TX_OUT = 0;

		Flag_TX_Empty = 0;
	}	
}

/*
uart1 
*/
void uart1_interrupt(void ) interrupt 15
{
	if(TI_1)
	{
		TI_1 = 0;
		//if(Flag_TX_Empty == 0)
		//{
			
			if(cnt_TX > 1)
			{
				SBUF_1 = (unsigned char)buf_TX[pos_TX_OUT];
				pos_TX_OUT++;
				if(pos_TX_OUT >= Length_TXBuf)
					pos_TX_OUT = 0;
				Flag_TX_Full = 0;
				cnt_TX--;
			
			}	
			else
			{
				cnt_TX = 0;
		 		Flag_TX_Empty = 1;
			}

		//}
	}
	
	if(RI_1)
	{
		RI_1 = 0;
		
	}
}


///*
//* UART数据接收：

//直接整个存起来，后面处理函数处理
//	收到头码后直接全部存起来，
//	长度超过数据长度位置后读取长度位置并对比整体数据长度
//*/

//code uint8_t UART_RX_Head[] = {0x0a, 0x80, 0x0a, 0x80};
//xdata tUART_RecvStruct RecvBuf_UART0;
////xdata tUART_RecvStruct RecvBuf_UART1;


//void UART0_Rx_Clean(void )
//{
//	RecvBuf_UART0.rx_cnt = 0;
//	RecvBuf_UART0.rx_over = 0;
//	RecvBuf_UART0.rx_TM_Decnt = 0;
//}


/*
UART0 recv handler
Process the received data
and err handler
*/
void UART0_RxHandler(void )
{
	uint8_t i = 0;

//	if(RecvBuf_UART0.rx_over == 1)
//	{
//		RecvBuf_UART0.rx_over = 2;

//		printf("Recv over: UART0: %d bytes\r\n", (uint16_t)RecvBuf_UART0.rx_cnt);
//		i = 0;
//		do
//		{
//			printf("%02X ",(uint16_t)RecvBuf_UART0.rx_data[i]);
//			i++;
//		}
//		while( i < RecvBuf_UART0.rx_cnt);
//		printf("\r\n");
//	}
//	if(RecvBuf_UART0.err > 0 )
//	{
//#if 0
//		printf("%X: Recv Error: UART0: %d bytes\r\n",(uint16_t)RecvBuf_UART0.err, (uint16_t)(RecvBuf_UART0.rx_cnt));
//		i = 0;
//		do
//		{
//			printf("%02X ",(uint16_t)RecvBuf_UART0.rx_data[i]);
//			i++;
//		}
//		while( i < RecvBuf_UART0.rx_cnt);
//		printf("\r\n");
//	#endif
//		if(RecvBuf_UART0.err != 0xA0)		//Header err ,directly reset in interrupt
//		{
//			RecvBuf_UART0.rx_cnt = 0;
//		}
//		RecvBuf_UART0.err = 0;
//					//
//	}
	//set_UART0_RXMode();
}
/*
* UART0 over time check up
*/
void UART0_OverTM_Handler(void )
{
	uint8_t i = 0;
	//
//	if((RecvBuf_UART0.rx_cnt > 0 )&& RecvBuf_UART0.rx_over == 0)
//	{
//		RecvBuf_UART0.rx_TM_Decnt++;
//		if(RecvBuf_UART0.rx_TM_Decnt > C_rx_OverTM_Max)
//		{
//			RecvBuf_UART0.rx_TM_Decnt = 0;
//			RecvBuf_UART0.err = 0xA2;
//			//RecvBuf_UART0.rx_cnt = 0;
//		}
//	}
//	if(decnt_recvbuf == 0)
//	{
//		if(cnt_recvBuf > 0)
//		{
//			printf("Received %d bytes:\n", (uint16_t)cnt_recvBuf);
//			for(i=0; i< cnt_recvBuf; i++)
//			{
//				printf("%02x ", (uint16_t)buf_Recv[i]);
//			}
//			printf("\n");
//			cnt_recvBuf = 0;
//		}
//	}
}

/*
*/
//uart0 interrupt handler
void uart0_interrupt(void ) interrupt 4
{
	uint16_t buf_length = 0;
	
	if(TI)
	{
		TI = 0;
//		if(Struct_TxBuf_UART0.tx_status)		
//		{
//			Struct_TxBuf_UART0.tx_pos++;
//			if(Struct_TxBuf_UART0.tx_pos >= Struct_TxBuf_UART0.tx_len)
//			{
//				Struct_TxBuf_UART0.tx_status = 0;		//Transmit complete
//				set_UART0_RXMode();
//			}
//			else
//			{
//				SBUF = Struct_TxBuf_UART0.tx_buf[Struct_TxBuf_UART0.tx_pos];
//			}
//		}
	}
	
	
	if(RI)
	{
		RI = 0;
//		
//		//uart0 received data
//		decnt_recvbuf = C_decnt_recvbuf_Set;
//		buf_Recv[cnt_recvBuf ] = SBUF;
//		cnt_recvBuf++;
//		
//		
//		//printf("%x\n",(uint16_t)SBUF );
//		//RX handler，that can run in main loop
//		if(RecvBuf_UART0.rx_over == 0)
//		{
//			
//			if(RecvBuf_UART0.rx_cnt < RX_Head_LEN)
//			{
//				//Head
//				RecvBuf_UART0.rx_data[RecvBuf_UART0.rx_cnt] = SBUF;
//				if(RecvBuf_UART0.rx_data[RecvBuf_UART0.rx_cnt] != UART_RX_Head[RecvBuf_UART0.rx_cnt])
//				{
//					RecvBuf_UART0.err = 0xA0;
//					RecvBuf_UART0.rx_cnt = 0;		//not match ,restart
//				}
//				else
//				{
//					RecvBuf_UART0.rx_cnt++;
//				}
//				
//			}
//			else
//			{
//				//body
//				RecvBuf_UART0.rx_data[RecvBuf_UART0.rx_cnt] = SBUF;
//				if(RecvBuf_UART0.rx_cnt > RX_LEN_POS+1)
//				{
//					//get data length ,check last byte
//					buf_length = RecvBuf_UART0.rx_data[RX_LEN_POS];
//					buf_length <<= 8;
//					buf_length += RecvBuf_UART0.rx_data[RX_LEN_POS + 1];
//					if(buf_length > 20)
//						RecvBuf_UART0.err = 0xA1;
//					//RecvBuf_UART0.rx_cnt = 0;
//					if(RecvBuf_UART0.rx_cnt >= (buf_length + RX_Head_LEN + RX_CMD_LEN +2))
//					{
//						//Data all received
//						RecvBuf_UART0.rx_over = 1;
//						//RecvBuf_UART0.rx_cnt = 0;
//					}
//				}
//				
//				RecvBuf_UART0.rx_cnt ++;
//				
//			}
//		}
	}
}

void UART_Deinit(void )
{
//--------------IO-------------------
	/**/
	//UART0
//	P06_Input_Mode;		//Tx
//	P07_Input_Mode;		//Rx
//	//UART1
//	P02_Input_Mode;		//Tx
//	P16_Input_Mode;		//Rx
	//set_UART1_RXMode();
	set_UART1_TXMode();
	UART0_Interrupt_OFF();
	UART1_Interrupt_OFF();
	clr_TR3;
	REN = 0;
	REN_1 = 0;
}
//
void UART_Init(uint32_t baudrate)
{
//--------------IO-------------------
	/**/
	//UART0
	P06_PushPull_Mode;
	P07_Quasi_Mode;		//Rx
	//UART1
	P16_PushPull_Mode;		//Tx
	P02_Quasi_Mode;			//Rx
	
	//P06_Input_Mode;		//Tx
	//P07_Input_Mode;		//Rx
	//UART1
	//P02_Input_Mode;		//Tx
	//P16_Input_Mode;		//Rx
	
	//baudrate setup
    T3CON &= 0xF8;   //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
#if	C_ClockDiv_SET == 0
		RH3    = HIBYTE(65536 - (1000000/(baudrate)));
		RL3    = LOBYTE(65536 - (1000000/(baudrate)));
#else
		RH3    = HIBYTE(65536 - (1000000/(2*C_ClockDiv_SET*baudrate))-1);
		RL3    = LOBYTE(65536 - (1000000/(2*C_ClockDiv_SET*baudrate))-1);
#endif
    set_TR3;         //Trigger Timer3
//----------------UART0------------------
	SCON = 0x50;     //UART0 Mode1,REN=1,TI=1
    set_SMOD;        //UART0 Double Rate Enable
    set_BRCK;        //UART0 baud rate clock source = Timer3
	//set_TI;					 //For //printf function must setting TI = 1
	IP |= 0x10;
	IPH |= 0x10;			//set uart0 interrupt to top level
	
//----------------UART1------------------
	SCON_1 = 0x50;   	//UART1 Mode1,REN_1=1,TI_1=1
	set_SMOD_1;        //UART1 Double Rate Enable
	//UART1 can only chose TIM3 as baud rate
	set_TI_1;
	//set_UART1_RXMode();
	set_UART1_TXMode();
	UART0_Interrupt_ON();
	UART1_Interrupt_ON();
	
	//Struct_TxBuf_UART0.tx_status = 0;
}


void set_UART0_TXMode(void )
{
	//UART0
	//P06_Quasi_Mode;
	P06_PushPull_Mode;		//Tx
	P07_Input_Mode;		//Rx
	//BASE_TX_L = 1;
	//RX off
	REN = 0;
	//UART0_Interrupt_OFF();	
}

void set_UART0_RXMode(void )
{
	P06_Quasi_Mode;
	//P06_Input_Mode;		//Tx
	P07_Input_Mode;		//Rx
	//P06_PushPull_Mode;
	
	//RX on
	REN = 1;
}

void set_UART1_TXMode(void )
{

	//UART1
	//P16_Quasi_Mode;
	P16_PushPull_Mode;		//Tx
	P02_Input_Mode;			//Rx
	
	//RX off
	REN_1 = 0;		
}

void set_UART1_RXMode(void )
{
	//UART1
	P16_Quasi_Mode;		//Tx
	P02_Input_Mode;		//Rx
	//RX on
	REN_1 = 1;		
}

/*char putchar (char ch)
{
#if	Bumbee_DEBUG 
	Send_Data_To_UART0(ch);
#endif
	return(ch);
}
*/

