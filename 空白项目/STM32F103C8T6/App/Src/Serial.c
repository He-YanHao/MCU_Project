#include "Serial.h"


/**
  * 函    数：串口初始化
  * 参    数：无
  * 返 回 值：无
  */
void Serial_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA9引脚初始化为复用推挽输出
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA10引脚初始化为上拉输入
	
	/*USART初始化*/
	USART_InitTypeDef USART_InitStructure;					//定义结构体变量
	USART_InitStructure.USART_BaudRate = 9600;				//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长，选择8位
	USART_Init(USART1, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART1
	
	/*中断输出配置*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//配置NVIC为分组2
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//选择配置NVIC的USART1线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
	
	/*USART使能*/
	USART_Cmd(USART1, ENABLE);								//使能USART1，串口开始运行
}

/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(uint16_t Length, uint8_t *Array)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Serial_SendByte(Array[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Serial_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

//帧头为0xAA
//校验为数据位求和
void Serial_SendPack(uint16_t Length, uint8_t *Array)
{
    uint8_t Checksum = 0;
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Checksum += Array[i];		//依次调用Serial_SendByte发送每个字节数据
	}
    Serial_SendByte(0xAA);
    Serial_SendByte(Length);
    Serial_SendArray(Length, Array);
    Serial_SendByte(Checksum);
}

/**
  * 函    数：USART1中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
uint8_t RxDataPack[100] = {};
volatile uint8_t Serial_RxFlag = 0;//标志位
uint8_t  RxState  = 0; //接收状态
uint8_t  BitState = 0; //接收位数
uint8_t  Checksum = 0; //接收位数
uint16_t Length   = 0; //接收数据包长度
uint8_t  RxData   = 0; //临时存放数据
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
//        if(USART_ReceiveData(USART1) != 0)
//        {
//            OLED_ShowHexNum(2, 1, USART_ReceiveData(USART1), 4);
//        }
        RxData = USART_ReceiveData(USART1);//读取数据寄存器，存放在接收的数据变量
        /*当前状态为0，接收数据包包头*/
		if (RxState == 0)      //RxState=0意思是没开始接收数据头
		{
			if (RxData == 0xAA)//而且数据确实是包头
			{
				RxState = 1;   //开始接收下一位
			}
		}
		/*当前状态为1，接收数据包长度*/
		else if (RxState == 1)//RxState=1意思是接收完数据头，开始接受数据长度。
		{
            RxDataPack[BitState] = RxData;   //RxDataPack[0]存数据长度
            BitState++;
            RxState = 2;
        }
		else if (RxState == 2)//RxState=2意思是接收完数据长度，开始接受数据位。
        {
            RxDataPack[BitState] = RxData; //RxDataPack[1]开始存数据本体
            Checksum += RxData;
            BitState++;
            if(BitState-1 == RxDataPack[0])//接受数据位=接收完数据长度 则开始接收校验位
            {
                RxState = 3;
            }
        }
        else if (RxState == 3)//开始接受校验位
        {
            if(Checksum == RxData)
            {
                //OK
                //Serial_SendPack(Length, RxDataPack);
            }
            Serial_RxFlag = 1;
            RxState = 0;//接收状态
            BitState = 0;//接收位数
            Checksum = 0;//接收位数
            Length = 0;//接收数据包长度
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除标志位
    }
}
