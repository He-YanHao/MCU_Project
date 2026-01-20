#include "stm32f4xx.h"                  // Device header

/**
  * 函    数：DMA初始化
  * 参    数：AddrA 原数组的首地址
  * 参    数：AddrB 目的数组的首地址
  * 参    数：Size 转运的数据大小（转运次数）
  * 返 回 值：无
  */
void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)//
{
	/*开启时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//开启DMA的时钟
	/*定义结构体变量*/
	DMA_InitTypeDef DMA_InitStructure;
	/*DMA初始化*/
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;//通道0
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)AddrA;//存储器基地址，给定形参AddrA
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)AddrB;//存储器基地址，给定形参AddrB
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;//数据传输方向，选择由存储器到存储器
    DMA_InitStructure.DMA_BufferSize = (uint16_t)Size;//设置DMA在传输时缓冲区的长度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//外设地址自增，选择使能;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址自增，选择使能;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//存储器数据宽度，选择字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//存储器数据宽度，选择字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//模式，选择正常模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//优先级，选择中等;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//禁用FIFO模式
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//禁用FIFO模式
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发传输配置 单次模式
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发传输配置 单次模式
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    /* 清除DMA数据流传输完成标志位 */
    DMA_ClearFlag(DMA2_Stream0,DMA_FLAG_TCIF0);
    /* 使能DMA数据流，开始DMA数据传输 */
    DMA_Cmd(DMA2_Stream0, ENABLE);
    while (DMA_GetCmdStatus(DMA2_Stream0) != ENABLE)
	{
    }
}
