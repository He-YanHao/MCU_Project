#include "MyDMA.h"


// 定义两个缓冲区
volatile uint8_t adc_buffer0[ADC_BUFFER_SIZE];
volatile uint8_t adc_buffer1[ADC_BUFFER_SIZE];

// 标志变量
volatile uint8_t dma_complete = 0;       // 传输完成标志
volatile uint8_t *current_buffer;       // 指向当前可用的缓冲区

void MyDMA_Init(void)
{
    // 使能DMA2时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);   
    // DMA2 Stream0配置 (ADC1使用DMA2 Stream0/Channel0)
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;//选择DMA通道0
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//设置外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(uint8_t*)adc_buffer0;//设置第一个内存缓冲区的起始地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//设置数据传输方向为从外设到内存
    DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;//设置DMA传输的数据量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//禁止外设地址增量
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//使能内存地址增量
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//设置外设数据宽度为字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;//设置内存数据宽度为字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//设置DMA为循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//设置DMA优先级为高
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//禁用FIFO模式
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//设置FIFO阈值为半满
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//设置内存突发传输为单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//设置外设突发传输为单次传输
    // 初始化DMA
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    printf("DMA_OK\n");

    // 配置双缓冲区模式
    DMA_DoubleBufferModeConfig(DMA2_Stream0, (uint32_t)(uint8_t*)adc_buffer1, DMA_Memory_0);
    DMA_DoubleBufferModeCmd(DMA2_Stream0, ENABLE);
    printf("Double Buffer Mode Enabled\n");

    // 配置DMA中断
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC | DMA_IT_TE);//清除中断标志   

    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC | DMA_IT_TE, ENABLE);
    //使能传输完成(TC)、半传输完成(HT)和传输错误(TE)中断

    // 配置NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 使能DMA流
    DMA_Cmd(DMA2_Stream0, ENABLE);
    printf("DMA Stream Enabled\n");
}

// DMA2 Stream0中断服务程序
void DMA2_Stream0_IRQHandler(void)
{
    // 检查传输完成中断
    if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
    {
        DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
        //DMA_IT_ 是前缀，TC 表示传输，IF 表示中断标志，0 是标志位在寄存器中的位置索引。
        if(DMA_GetCurrentMemoryTarget(DMA2_Stream0) == DMA_Memory_0)
        {
            current_buffer = &adc_buffer0[0];
        }
        else
        {
            current_buffer = &adc_buffer1[0];
        }
        dma_complete = 1;  // 设置传输完成标志
    }

    // 检查传输错误中断
    if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TEIF0))
    {
        DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF0);
        // 处理DMA错误
        // 可以在这里重新初始化DMA
        printf("TE Interrupt - DMA Error!\n");
    }
}


