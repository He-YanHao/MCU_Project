#include "MyDMA.h"

volatile uint8_t SPI_DMA_Sign = 0;

void SPI_DMA(uint32_t txBuffer, uint8_t BUFFER_SIZE)
{
    // 复位DMA Stream
    DMA_DeInit(DMA2_Stream3);  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    //配置SPI TX DMA (内存->外设)
    DMA_InitTypeDef DMA_InitStructure;
    // 配置TX DMA
    DMA_InitStructure.DMA_Channel = DMA_Channel_3;// SPI1_TX通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI1->DR);// SPI数据寄存器地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)txBuffer;// 发送缓冲区地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;// 内存到外设
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;// 传输数据量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 外设地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// 外设数据大小: 字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// 内存数据大小: 字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 正常模式(非循环)
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;// 高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;// 禁用FIFO
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;// FIFO阈值(禁用时无效)
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//单次传输
    DMA_Init(DMA2_Stream3, &DMA_InitStructure);

    DMA_Cmd(DMA2_Stream3, ENABLE);  // 使能TX DMA
    // 使能SPI的DMA请求
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}

/**
  * @brief NVIC中断配置
  */
void SPI_DMA_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // 配置DMA传输完成中断
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream3_IRQn;// SPI TX DMA中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // 使能DMA传输完成中断
    DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE);
}

/**
  * @brief DMA2 Stream3中断处理 (SPI TX)
  */
void DMA2_Stream3_IRQHandler(void)
{
    // 检查传输完成标志
    if(DMA_GetITStatus(DMA2_Stream3, DMA_IT_TCIF3))
    {
        // 清除传输完成标志
        DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_TCIF3);

        SPI_DMA_Sign = 1;
    }
}

volatile uint8_t DMA_135_Sign = 1;
//0 已完成    1 未完成

void DMA_135(void)
{
    while(DMA_135_Sign);
    DMA_135_Sign = 1;
    // 复位DMA Stream
    DMA_DeInit(DMA2_Stream1);
    // 使能DMA2时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    // 配置DMA参数
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;// 内存到内存传输可用任意通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC_Num1;// 源地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_Num2;// 目标地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;// 内存到内存模式
    DMA_InitStructure.DMA_BufferSize = 135;// 传输数据量(135字节)
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;// 源地址递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// 目标地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// 字节传输
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// 字节传输
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 正常模式(非循环)
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;// 高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;// 禁用FIFO
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; 
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);
    // 使能传输完成中断
    DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
}

/**
  * @brief NVIC中断配置
  */
void DMA_135_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // 配置DMA传输完成中断
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief DMA2 Stream1中断处理
  */
void DMA2_Stream1_IRQHandler(void)
{
    // 检查传输完成标志
    if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1)) {
        // 清除传输完成标志
        DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
        // 设置传输完成标志
        DMA_135_Sign = 0;
    }
}



