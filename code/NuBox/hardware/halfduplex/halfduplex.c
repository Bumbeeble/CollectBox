
#include  "halfduplex.h"

code uint8_t uart_Tx_Head[] = {0x0a,0x01,0x0a,0x01};

xdata t_Struct_UART_TxBuf Struct_TxBuf_UART0 = {0};
xdata t_Struct_UART_TxCMD tx_cmdbuf_uart0 = {0};


code uint8_t uart_Tx_CMD[][2] = {
{0x00,0x00},			//CheckState
{0x00,0x01},			//KEY 1 trig
{0x00,0x02},			//KEY 2 trig
{0x00,0x03},			//KEY 3 trig
{0x00,0x04},				//Key long press
{0x00,0x05},				//Key long long press


{0x00,0xff}
};





/*
* 定时处理
常态定时发送心跳包
命令来了就下一次直接发送命令

* This function handler
1ms运行一次
*/
void uart0_CMDHandler(void )
{
	UART0_RxHandler();
	UART0_OverTM_Handler();
	//time conut down
	if(tx_cmdbuf_uart0.decnt_rx > 0)
		tx_cmdbuf_uart0.decnt_rx--;
	//mode handler
	switch(tx_cmdbuf_uart0.mode)
	{
		case UART_TX_MODE_CHECKSTATE:{
		//Normal mode 
			uart0_ModeHandler_Checkstate();
		}
		break;
		
		case UART_TX_MODE_KEY:{
			uart0_ModeHandler_Key();
		}
		break;
		case UART_TX_MODE_Pair:{
			tx_cmdbuf_uart0.mode = UART_TX_MODE_CHECKSTATE;
		}
		break;
		case UART_TX_MODE_Close:{
			tx_cmdbuf_uart0.mode = UART_TX_MODE_CHECKSTATE;
		
		}
		break;
		default:
			tx_cmdbuf_uart0.mode = UART_TX_MODE_CHECKSTATE;
	}
}

int8_t uart0_CMD_Request(UART_TX_MODE cmd_mode, TX_CMD cmd_msg )
{
	if(tx_cmdbuf_uart0.mode_request > 0)
		return -1;
	tx_cmdbuf_uart0.mode_request = cmd_mode;
	tx_cmdbuf_uart0.cmd_request = cmd_msg;
	return 0;
	
}



/*
* 常态模式处理函数
* 1. 定时发送数据
* 2. 遇到切换请求则切换模式
*/
void uart0_ModeHandler_Checkstate(void )
{
	switch(tx_cmdbuf_uart0.tx_status)
	{
		case UART_TX_STATUS_Free:{
			//Free mode 
			if(tx_cmdbuf_uart0.mode_request > UART_TX_MODE_CHECKSTATE)
			{
				//mode switch
				tx_cmdbuf_uart0.mode = tx_cmdbuf_uart0.mode_request;
				tx_cmdbuf_uart0.mode_request = UART_TX_MODE_CHECKSTATE;
				return;
			}
#if 0
			if(tx_cmdbuf_uart0.decnt_rx <= 0)
			{
				//scheduled transmit
				uart_sendCMD(TX_CMD_CHECKSTATE, "\1", 1 );
				tx_cmdbuf_uart0.decnt_rx = 100;
				UART0_Rx_Clean();
				tx_cmdbuf_uart0.tx_status = UART_TX_STATUS_Receiver;
			}
#endif
		}
		break;
		case UART_TX_STATUS_Receiver:{
			//Recv mode 
			if(tx_cmdbuf_uart0.decnt_rx <= 0)
			{
				//Over time
				tx_cmdbuf_uart0.flag_err = 1;
				tx_cmdbuf_uart0.cnt_err++;
//				printf("Link Over time: UART0\r\n");
//				if(tx_cmdbuf_uart0.cnt_err >= 10)
//				{
//					tx_cmdbuf_uart0.cnt_err = 0;
//					printf("Link error: UART0\r\n");
//				}
//				
				tx_cmdbuf_uart0.tx_status = UART_TX_STATUS_Free;
			}
			if(RecvBuf_UART0.rx_over > 0)
			{
				//Recv over
				RecvBuf_UART0.rx_data[RecvBuf_UART0.rx_cnt] = 0;
				//printf("Get cmd: \n 0x%02x 0x%02x\n",(uint16_t)RecvBuf_UART0.rx_data[4], (uint16_t)RecvBuf_UART0.rx_data[5]);
				//printf("Get ack: \n 0x%02x ",(uint16_t)RecvBuf_UART0.rx_data[6]);
				//cnt_LED_UART = 100;
//				if(RecvBuf_UART0.rx_data[4] > 0x0f)
//				{
//					LED_Status ^= 0x0f;
//				}
//				else
//				{
//					LED_Status = RecvBuf_UART0.rx_data[4];
//				}
				//Status switch
				UART0_Rx_Clean();
				tx_cmdbuf_uart0.tx_status = UART_TX_STATUS_Free;
			}
		}
		break;
		
		default:
			tx_cmdbuf_uart0.tx_status = UART_TX_STATUS_Free;
	}
}

/*
* 
* 1. 定时发送数据
* 2. 遇到切换请求则切换模式
*/
void uart0_ModeHandler_Key(void )
{
	switch(tx_cmdbuf_uart0.tx_status)
	{
		case UART_TX_STATUS_Free:{
			//Free mode			
				//scheduled transmit
				if((tx_cmdbuf_uart0.cmd_request > TX_CMD_KEY_LL_Press)|| (tx_cmdbuf_uart0.cmd_request < TX_CMD_KEY_1_Trig))
				{
					printf("cmd error \n");
						
					tx_cmdbuf_uart0.flag_err = 1;				
					tx_cmdbuf_uart0.tx_status = UART_TX_STATUS_Free;
					tx_cmdbuf_uart0.mode = UART_TX_MODE_CHECKSTATE;
				}
				uart_sendCMD(tx_cmdbuf_uart0.cmd_request, "", 0 );
				printf("Send key cmd: %d \n", (uint16_t)tx_cmdbuf_uart0.cmd_request);
				tx_cmdbuf_uart0.decnt_rx = 1000;
				UART0_Rx_Clean();
				tx_cmdbuf_uart0.tx_status = UART_TX_STATUS_Receiver;	
		}
		break;
		case UART_TX_STATUS_Receiver:{
			//Recv mode 
			if(tx_cmdbuf_uart0.decnt_rx == 0)
			{
				//Over time
				tx_cmdbuf_uart0.flag_err = 1;				
				tx_cmdbuf_uart0.tx_status = UART_TX_STATUS_Free;
				tx_cmdbuf_uart0.mode = UART_TX_MODE_CHECKSTATE;
				printf("Key cmd mode, Receive over time\n");
			}
			if(RecvBuf_UART0.rx_over > 0)
			{
				//Recv over
				cnt_LED_UART = 100;
				printf("Key cmd mode, Receive ok\n");
				//Status switch
				UART0_Rx_Clean();
				tx_cmdbuf_uart0.tx_status = UART_TX_STATUS_Free;
				tx_cmdbuf_uart0.mode = UART_TX_MODE_CHECKSTATE;
			}
		}
		break;
		
		default:
			tx_cmdbuf_uart0.tx_status = UART_TX_STATUS_Free;
	}
}


//get all bytes,and send it out
int8_t uart_sendCMD(TX_CMD tx_cmd, uint8_t tx_data[], uint8_t length_tx_data )
 {
	uint8_t pos_buf = 0,i = 0,checksum_tx = 0;
	if(Struct_TxBuf_UART0.tx_status > 0)
		return -1;			//Tx busing
	if(length_tx_data > LENGTH_TX_DATA_MAX)
		return -2;			//too much data
	//head
	while(pos_buf < LENGTH_UART_TX_HEAD)
	{
		tx_cmdbuf_uart0.buf[pos_buf] = uart_Tx_Head[pos_buf];
		pos_buf++;
	}
	//cmd
	i = 0;
	while(i < LENGTH_TX_CMD)
	{
		tx_cmdbuf_uart0.buf[pos_buf] = uart_Tx_CMD[tx_cmd][i];
		pos_buf++;
		i++;
	}
	//length of data
	tx_cmdbuf_uart0.buf[pos_buf++] = HIBYTE(length_tx_data);
	tx_cmdbuf_uart0.buf[pos_buf++] = LOBYTE(length_tx_data);
	//data
	i = 0;
	checksum_tx = 0;
	while(i< length_tx_data)
	{
		tx_cmdbuf_uart0.buf[pos_buf] = tx_data[i];
		checksum_tx += tx_data[i];
		pos_buf++;
		i++;
	}
	checksum_tx = (0xff - checksum_tx) +1;
	tx_cmdbuf_uart0.buf[pos_buf] = checksum_tx;
	uart0_TxBuf(tx_cmdbuf_uart0.buf, pos_buf+1);
	return 0;
}


//#define		UART_Need_Delay	1
#ifdef	UART_Need_Delay
xdata uint8_t delay_uart = 0;
#endif


int8_t uart0_TxBuf(uint8_t *databuf, uint8_t length)
{
	//Busy and return error
	if(Struct_TxBuf_UART0.tx_status > 0)
		return -1;
	Struct_TxBuf_UART0.tx_status = 1;
	//set buf and length
	Struct_TxBuf_UART0.tx_buf = databuf;
	Struct_TxBuf_UART0.tx_len = length;
	//start transmit
	Struct_TxBuf_UART0.tx_pos = 0;
	set_UART0_TXMode();
#ifdef		UART_Need_Delay
	delay_uart = 1;
	while(delay_uart>0);
#endif
	SBUF = Struct_TxBuf_UART0.tx_buf[0];
	return 0;
	
}






