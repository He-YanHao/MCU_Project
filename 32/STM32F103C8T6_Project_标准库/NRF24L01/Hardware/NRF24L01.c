#include "stm32f10x.h"                  // Device header
#include "NRF24L01.h"
#include "MySPI.h"

#define NRF24L01_CE(x)		GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(x))
#define NRF24L01_IRQ(x)		GPIO_WriteBit(GPIOA, GPIO_Pin_3, (BitAction)(x))

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址

void NRF24L01_Init(void)//初始化
{
	MySPI_Init();
	NRF24L01_CE(0); 	//使能24L01
}

//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)//写数据区
{
	uint8_t status,u8_ctr;	    
	MySPI_Start();//使能SPI传输
  	status = MySPI_SwapByte(reg);//发送寄存器值(位置),并读取状态值
  	for(u8_ctr = 0; u8_ctr < len; u8_ctr++)
  	{
		MySPI_SwapByte(*pBuf++); //写入数据
	}
  	MySPI_Stop();//关闭SPI传输
  	return status;          //返回读到的状态值
}

//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)//读数据区	
{
	u8 status,u8_ctr;	       
	MySPI_Start();
  	status = MySPI_SwapByte(reg);//发送寄存器值(位置),并读取状态值   	   
 	for(u8_ctr=0; u8_ctr < len; u8_ctr++)
	{
		pBuf[u8_ctr] = MySPI_SwapByte(0XFF);//读出数据
	}
  	MySPI_Stop();//关闭SPI传输
  	return status;//返回读到的状态值
}

//检测24L01是否存在
//返回值:0，成功;  1，失败	
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	NRF24L01_Write_Buf(WRITE_REG + TX_ADDR, buf, 5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR, buf, 5); //读出写入的地址  
	for(i=0;i<5;i++)
	{
		if(buf[i]!=0XA5)
		{
			break;
		}
	}				   
	if(i!=5)
	{
		return 1;//检测24L01错误
	}
	return 0;//检测到24L01
}

//void RX_Mode(void);//配置为接收模式
//void TX_Mode(void);//配置为发送模式
//uint8_t NRF24L01_Read_Reg(uint8_t reg);			//读寄存器
//uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value);//写寄存器
//uint8_t NRF24L01_Check(void);//检查24L01是否存在
//uint8_t NRF24L01_TxPacket(u8 *txbuf);//发送一个包的数据
//uint8_t NRF24L01_RxPacket(u8 *rxbuf);//接收一个包的数据
