#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q16_Ins.h"

void W25Q16_Init(void)
{
	MySPI_Init();					//先初始化底层的SPI
}

void W25Q16_ReadID(uint8_t *MID, uint16_t *DID)//读取ID
{
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q16_JEDEC_ID);			//交换发送读取ID的指令
	*MID = MySPI_SwapByte(W25Q16_DUMMY_BYTE);	//交换接收MID工厂ID，通过输出参数返回
	*DID = MySPI_SwapByte(W25Q16_DUMMY_BYTE);	//交换接收DID设备ID高8位
	*DID <<= 8;									//高8位移到高位
	*DID |= MySPI_SwapByte(W25Q16_DUMMY_BYTE);	//或上交换接收DID的低8位，通过输出参数返回
	MySPI_Stop();								//SPI终止
}

void W25Q16_WriteEnable(void)//W25Q16写使能
{
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q16_WRITE_ENABLE);		//交换发送写使能的指令
	MySPI_Stop();								//SPI终止
}

void W25Q16_WaitBusy(void)	//W25Q16等待忙
{
	uint32_t Timeout;
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q16_READ_STATUS_REGISTER_1);				//交换发送读状态寄存器1的指令
	Timeout = 100000;							//给定超时计数时间
	while ((MySPI_SwapByte(W25Q16_DUMMY_BYTE) & 0x01) == 0x01)	//循环等待忙标志位
	{
		Timeout --;								//等待时，计数值自减
		if (Timeout == 0)						//自减到0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此处*/
			break;								//跳出等待，不等了
		}
	}
	MySPI_Stop();								//SPI终止
}

void W25Q16_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)	//W25Q16页编程 写入的地址范围不能跨页
{
	uint16_t i;
	
	W25Q16_WriteEnable();						//写使能
	
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q16_PAGE_PROGRAM);		//交换发送页编程的指令
	MySPI_SwapByte(Address >> 16);				//交换发送地址23~16位 页编程的起始地址，范围：0x000000~0x7FFFFF
	MySPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	MySPI_SwapByte(Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次 要写入数据的数量，范围：0~256
	{
		MySPI_SwapByte(DataArray[i]);			//依次在起始地址后写入数据 写入数据的数组
	}
	MySPI_Stop();								//SPI终止
	
	W25Q16_WaitBusy();							//等待忙
}

void W25Q16_SectorErase(uint32_t Address)	//W25Q16扇区擦除（4KB）
{
	W25Q16_WriteEnable();						//写使能
	
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q16_SECTOR_ERASE_4KB);	//交换发送扇区擦除的指令 
	MySPI_SwapByte(Address >> 16);				//交换发送地址23~16位 Address 指定扇区的地址，范围：0x000000~0x7FFFFF
	MySPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	MySPI_SwapByte(Address);					//交换发送地址7~0位
	MySPI_Stop();								//SPI终止
	
	W25Q16_WaitBusy();							//等待忙
}

void W25Q16_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)		//W25Q16读取数据
{
	uint32_t i;
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q16_READ_DATA);			//交换发送读取数据的指令
	MySPI_SwapByte(Address >> 16);				//交换发送地址23~16位 Address 读取数据的起始地址，范围：0x000000~0x7FFFFF
	MySPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	MySPI_SwapByte(Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次 Count 要读取数据的数量，范围：0~0x800000
	{
		DataArray[i] = MySPI_SwapByte(W25Q16_DUMMY_BYTE);	//依次在起始地址后读取数据 DataArray 用于接收读取数据的数组，通过输出参数返回
	}
	MySPI_Stop();								//SPI终止
}
