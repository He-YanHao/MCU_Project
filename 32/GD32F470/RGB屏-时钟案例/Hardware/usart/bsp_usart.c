/********************************************************************************
  * 文 件 名: bsp_usart.c
  * 版 本 号: 初版
  * 修改作者: LC
  * 修改日期: 2022年04月14日
  * 功能介绍:          
  ******************************************************************************
  * 注意事项:
*********************************************************************************/
#include "bsp_usart.h"
#include "stdio.h"
#include "string.h"


uint8_t 	g_recv_buff[USART_RECEIVE_LENGTH];   // 接收缓冲区
uint16_t 	g_recv_length = 0;									 // 接收数据长度
uint8_t		g_recv_complete_flag = 0; 					 // 接收数据完成标志位

/************************************************
函数名称 ： usart_gpio_config
功    能 ： 串口配置GPIO
参    数 ： band_rate:波特率
返 回 值 ： 无
作    者 ： LC
*************************************************/
void usart_gpio_config(uint32_t band_rate)
{
	/* 开启时钟 */
	rcu_periph_clock_enable(BSP_USART_TX_RCU);   // 开启串口时钟
	rcu_periph_clock_enable(BSP_USART_RX_RCU);   // 开启端口时钟
	rcu_periph_clock_enable(BSP_USART_RCU);      // 开启端口时钟
	
	/* 配置GPIO复用功能 */
  gpio_af_set(BSP_USART_TX_PORT,BSP_USART_AF,BSP_USART_TX_PIN);	
	gpio_af_set(BSP_USART_RX_PORT,BSP_USART_AF,BSP_USART_RX_PIN);	
	
	/* 配置GPIO的模式 */
	/* 配置TX为复用模式 上拉模式 */
	gpio_mode_set(BSP_USART_TX_PORT,GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_USART_TX_PIN);
	/* 配置RX为复用模式 上拉模式 */
	gpio_mode_set(BSP_USART_RX_PORT, GPIO_MODE_AF,GPIO_PUPD_PULLUP,BSP_USART_RX_PIN);
	
	/* 配置TX为推挽输出 50MHZ */
	gpio_output_options_set(BSP_USART_TX_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_USART_TX_PIN);
	/* 配置RX为推挽输出 50MHZ */
	gpio_output_options_set(BSP_USART_RX_PORT,GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_USART_RX_PIN);

	/* 配置串口的参数 */
	usart_deinit(BSP_USART);                                 // 复位串口
	usart_baudrate_set(BSP_USART,band_rate);                 // 设置波特率
	usart_parity_config(BSP_USART,USART_PM_NONE);            // 没有校验位
	usart_word_length_set(BSP_USART,USART_WL_8BIT);          // 8位数据位
	usart_stop_bit_set(BSP_USART,USART_STB_1BIT);     			 // 1位停止位

  /* 使能串口 */
	usart_enable(BSP_USART);                                 // 使能串口
	usart_transmit_config(BSP_USART,USART_TRANSMIT_ENABLE);  // 使能串口发送
	usart_receive_config(BSP_USART,USART_RECEIVE_ENABLE);    // 使能串口接收
	
	/* 中断配置 */
	nvic_irq_enable(BSP_USART_IRQ, 2, 2); 									 // 配置中断优先级
	usart_interrupt_enable(BSP_USART,USART_INT_RBNE);				 // 读数据缓冲区非空中断和溢出错误中断
	usart_interrupt_enable(BSP_USART,USART_INT_IDLE);				 // 空闲检测中断
}

/************************************************
函数名称 ： usart_send_data
功    能 ： 串口重发送一个字节
参    数 ： ucch：要发送的字节
返 回 值 ： 
作    者 ： LC
*************************************************/
void usart_send_data(uint8_t ucch)
{
	usart_data_transmit(BSP_USART,(uint8_t)ucch);							 // 发送数据
	while(RESET == usart_flag_get(BSP_USART,USART_FLAG_TBE));  // 等待发送数据缓冲区标志置位
}


/************************************************
函数名称 ： usart_send_String
功    能 ： 串口发送字符串
参    数 ： ucstr:要发送的字符串
返 回 值 ： 
作    者 ： LC
*************************************************/
void usart_send_string(uint8_t *ucstr)
{
	while(ucstr && *ucstr)        // 地址为空或者值为空跳出
	{
	  usart_send_data(*ucstr++);  // 发送单个字符
	}
}

void g_recv_buff_clear(void)
{
	//清除接收缓存
	g_recv_length=0;
	memset(g_recv_buff,0,USART_RECEIVE_LENGTH-1);
}
/************************************************
函数名称 ： fputc
功    能 ： 串口重定向函数
参    数 ： 
返 回 值 ： 
作    者 ： LC
*************************************************/
int fputc(int ch, FILE *f)
{
     usart_send_data(ch);
     // 等待发送数据缓冲区标志置位
     return ch;
}



uint8_t decimalToBCD(int decimal) {
    int bcd = 0;
    int weight = 1;
    
    while (decimal > 0) {
        int digit = decimal % 10;
        bcd += digit * weight;
        weight *= 16;
        decimal /= 10;
    }
	return bcd;
}

/**********************************************************
 * 函 数 名 称：Serial_set_time
 * 函 数 功 能：串口调时
 * 传 入 参 数：year：年的保存地址
 * 				month：月的保存地址
 * 				date：日的保存地址
 * 				week：周的保存地址
 * 				hour：小时的保存地址
 * 				minute：分钟的保存地址
 * 				second：秒的保存地址
 * 函 数 返 回：
 * 作       者：LCKFB
 * 备       注：要求调时格式为  TIME=YYYY-MM-DD-K hh:mm:ss
 *  			YYYY(年)-MM(月)-DD(日)-K(周) hh(时):mm(分):ss(秒)
**********************************************************/
uint8_t Serial_set_time(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *week, uint8_t *hour, uint8_t *minute, uint8_t *second)
{
	char buff[200]={0};
	char temp[10]={0};
	char *p = buff;
	
	int y = 0, mth = 0, day = 0, wk = 0, h = 0, m = 0, s = 0;
	
	if( g_recv_complete_flag == 1 )
	{
		g_recv_complete_flag = 0;
		//如果接收到调时命令
		if( strstr((const char*)g_recv_buff,"TIME=") != NULL  )
		{
			strcpy(buff,(const char*)g_recv_buff);
			p = strstr((const char*)buff,"TIME=");
/**************************************	解析年份 **********************************************/
			
			//过滤“TIME=”
			p += strlen("TIME=");
			
			//拷贝p的4个字节数据到temp
			strncpy(temp,p,4);

			//将字符串改为整型
			y = atoi(temp);
			memset(temp,0,10-1);
			
			//如果不符合年限
			if( y < 2000 || y > 2100 )	goto YYYY_end;
			
			//保存年份 
			*year = decimalToBCD(y%1000);
			
/**************************************	解析月份 **********************************************/
			//过滤“YYYY-”
			p += strlen("YYYY-");
			
			//拷贝p的2个字节数据到temp
			strncpy(temp,p,2);

			//将字符串改为整型
			mth = atoi(temp);
			memset(temp,0,10-1);

			//如果不符合月限
			if( mth < 1 || mth > 12 )	goto MM_end;
			
			//保存月份
			*month = decimalToBCD(mth);			
			
/**************************************	解析日 **********************************************/
			//过滤“MM-”
			p += strlen("MM-");
			//拷贝p的2个字节数据到temp
			strncpy(temp,p,2);

			//将字符串改为整型
			day = atoi(temp);
			memset(temp,0,10-1);		
			
			//如果不符合日限
			if( day < 1 || day > 31 )	goto DD_end;
			
			//保存日期
			*date = decimalToBCD(day);			

/**************************************	解析周 **********************************************/
			//过滤“DD-”
			p += strlen("DD-");
			//拷贝p的2个字节数据到temp
			strncpy(temp,p,1);
			//将字符串改为整型
			wk = atoi(temp);
			memset(temp,0,10-1);
			//如果不符合周限
			if( wk < 1 || wk > 7 )	goto K_end;
			
			//保存周
			*week = decimalToBCD(wk);			

/**************************************	解析时 **********************************************/
			//过滤“K ”
			p += strlen("K ");
			//拷贝p的2个字节数据到temp
			strncpy(temp,p,2);

			//将字符串改为整型
			h = atoi(temp);
			memset(temp,0,10-1);
			//如果不符合周限
			if( h < 0 || h > 23 )	goto hh_end;
			
			//保存时
			*hour = decimalToBCD(h);			

/**************************************	解析分 **********************************************/
			//过滤“hh:”
			p += strlen("hh:");
			//拷贝p的2个字节数据到temp
			strncpy(temp,p,2);

			//将字符串改为整型
			m = atoi(temp);
			memset(temp,0,10-1);
			//如果不符合分限
			if( m < 0 || m > 59 )	goto mm_end;
			
			//保存分
			*minute = decimalToBCD(m);			

/**************************************	解析秒 **********************************************/
			//过滤“mm:”
			p += strlen("mm:");
			//拷贝p的2个字节数据到temp
			strncpy(temp,p,2);

			//将字符串改为整型
			s = atoi(temp);
			memset(temp,0,10-1);
			//如果不符合分限
			if( s < 0 || s > 59 )	goto ss_end;
			
			//保存秒
			*second = decimalToBCD(s);			
			printf("\r\nSET OK\r\n");
			g_recv_buff_clear();
			return 1;
		}   
		else{
			printf("Not recognized: TIME=YYYY-MM-DD-K hh:mm:ss\r\n");
			return 0;
		}
		 
		YYYY_end:
		printf("YYYY format error\r\n");
		g_recv_buff_clear();
		return 0;
		
		MM_end:
		printf("MM format error\r\n");
		g_recv_buff_clear();
		return 0;
		
		DD_end:
		printf("DD format error\r\n");
		g_recv_buff_clear();
		return 0;
		
		K_end:
		printf("K format error\r\n");
		g_recv_buff_clear();
		return 0;		
		
		hh_end:
		printf("hh format error\r\n");
		g_recv_buff_clear();
		return 0;		
		
		mm_end:
		printf("mm format error\r\n");
		g_recv_buff_clear();
		return 0;		

		ss_end:
		printf("ss format error\r\n");
		g_recv_buff_clear();
		return 0;		

	}
	return 0;
}


/************************************************
函数名称 ： BSP_USART_IRQHandler
功    能 ： 串口接收中断服务函数
参    数 ： 无
返 回 值 ： 无
作    者 ： LC
*************************************************/
void BSP_USART_IRQHandler(void)
{
	if(usart_interrupt_flag_get(BSP_USART,USART_INT_FLAG_RBNE) == SET)   // 接收缓冲区不为空
	{
		g_recv_buff[g_recv_length++] = usart_data_receive(BSP_USART);      // 把接收到的数据放到缓冲区中
	}
	
	if(usart_interrupt_flag_get(BSP_USART,USART_INT_FLAG_IDLE) == SET)   // 检测到帧中断
	{
		usart_data_receive(BSP_USART);                                     // 必须要读，读出来的值不能要
		g_recv_buff[g_recv_length] = '\0';																 // 数据接收完毕，数组结束标志
		g_recv_complete_flag = 1;                                          // 接收完成 
	}
	
}
