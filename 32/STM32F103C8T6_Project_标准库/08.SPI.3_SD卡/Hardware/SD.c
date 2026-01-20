#include "stm32f10x.h"                  // Device header

void SD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);	//开启SPI1的时钟

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA4 CS引脚初始化为推挽输出
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA5 CLK 和PA7 DI 引脚初始化为复用推挽输出
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA6 DO 引脚初始化为上拉输入
    
	/*SPI初始化*/
	SPI_InitTypeDef SPI_InitStructure;						//定义结构体变量
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			//模式，选择为SPI主模式
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//方向，选择2线全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//数据宽度，选择为8位
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		//先行位，选择高位先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	//波特率分频，选择128分频
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;				//SPI极性，选择低极性
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;			//SPI相位，选择第一个时钟边沿采样，极性和相位决定选择SPI模式0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;				//NSS，选择由软件控制
	SPI_InitStructure.SPI_CRCPolynomial = 7;				//CRC多项式，暂时用不到，给默认值7
    //SPI_InitStructure.SPI_BaudRatePrescaler=tmp;
	SPI_Init(SPI1, &SPI_InitStructure);						//将结构体变量交给SPI_Init，配置SPI1
	
	/*SPI使能*/
	SPI_Cmd(SPI1, ENABLE);									//使能SPI1，开始运行
	
	/*设置默认电平*/
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)1);//SS默认高电平
}

//读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
  u8 tmp=0;

  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE))
  {	//等待发送结束
    tmp++;
	if(tmp>200)
	  return 0;
  }
  SPI_I2S_SendData(SPI1,TxData);

  tmp=0;
  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE))
  {	//等待接收完成
    tmp++;
	if(tmp>200)
	  return 0;
  }
  return SPI_I2S_ReceiveData(SPI1);
}

//---------------------------------------------------------------------------------------------------------
//读SD卡应答并判断
//response:正确回应值
//成功返回0,失败返回1
u8 SD_GetResponse(u16 response)
{
  u16 ii;

  ii=5000; //读应答最多5000次
  while ((SPI1_ReadWriteByte(0xff)!=response) && (ii!=0))
    ii--;
  
  if (ii==0)
    return 1;//返回失败
  else 
    return 0;//返回成功
}
//---------------------------------------------------------------------------------------------------------
//写命令到SD卡
//cmd:命令
//arg:参数
//crc:校验码
//reset:(0/1)退出时(保持/失能)SD卡片选
//返回SD卡响应															  
u8 SD_SendCommand(u8 cmd,u32 arg,u8 crc,u8 reset)
{
  u8 tmp;
  u8 i;	         
	
  SD_CS=1;
  
  SPI1_ReadWriteByte(0xff); //增加8个时钟确保上次操作完成

  SD_CS=0; 
  
  //依次写入命令,参数,校验码
  SPI1_ReadWriteByte(cmd | 0x40);
  SPI1_ReadWriteByte(arg >> 24);
  SPI1_ReadWriteByte(arg >> 16);
  SPI1_ReadWriteByte(arg >> 8);
  SPI1_ReadWriteByte(arg);
  SPI1_ReadWriteByte(crc); 
  
    
  i=0; //成功或超时退出
  do
  {
	tmp=SPI1_ReadWriteByte(0xff);
    i++;	    
    if(i>200)
	  break;
  }while(tmp==0xff);   
  
  if(reset)	//根据要求保持或失能SD卡
  {
    SD_CS=1;
    SPI1_ReadWriteByte(0xff); //增加8个时钟确保本次操作完成
  }

  return tmp;
}		  	