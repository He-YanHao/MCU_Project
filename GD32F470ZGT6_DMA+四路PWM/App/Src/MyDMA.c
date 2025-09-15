#include "MyDMA.h"

uint32_t sin_table_12bit[] = {0, 50, 100, 151, 201, 251, 301, 351, 401, 451, 501, 551, 601, 651, 700, 750, 799, 848, 897, 946, 995, 1044, 1092, 1141, 1189, 1237, 1285, 1332, 1380, 1427, 1474, 1521, 1567, 1613, 1659, 1705, 1751, 1796, 1841, 1886, 1930, 1975, 2018, 2062, 2105, 2148, 2191, 2233, 2275, 2317, 2358, 2399, 2439, 2480, 2519, 2559, 2598, 2636, 2675, 2713, 2750, 2787, 2824, 2860, 2896, 2931, 2966, 3000, 3034, 3068, 3101, 3133, 3165, 3197, 3228, 3259, 3289, 3319, 3348, 3377, 3405, 3433, 3460, 3486, 3512, 3538, 3563, 3588, 3611, 3635, 3658, 3680, 3702, 3723, 3744, 3764, 3783, 3802, 3821, 3838, 3856, 3872, 3888, 3904, 3919, 3933, 3947, 3960, 3972, 3984, 3996, 4006, 4016, 4026, 4035, 4043, 4051, 4058, 4064, 4070, 4075, 4080, 4084, 4087, 4090, 4092, 4094, 4095, 4095, 4094, 4092, 4090, 4087, 4084, 4080, 4075, 4070, 4064, 4058, 4051, 4043, 4035, 4026, 4016, 4006, 3996, 3984, 3972, 3960, 3947, 3933, 3919, 3904, 3888, 3872, 3856, 3838, 3821, 3802, 3783, 3764, 3744, 3723, 3702, 3680, 3658, 3635, 3611, 3588, 3563, 3538, 3512, 3486, 3460, 3433, 3405, 3377, 3348, 3319, 3289, 3259, 3228, 3197, 3165, 3133, 3101, 3068, 3034, 3000, 2966, 2931, 2896, 2860, 2824, 2787, 2750, 2713, 2675, 2636, 2598, 2559, 2519, 2480, 2439, 2399, 2358, 2317, 2275, 2233, 2191, 2148, 2105, 2062, 2018, 1975, 1930, 1886, 1841, 1796, 1751, 1705, 1659, 1613, 1567, 1521, 1474, 1427, 1380, 1332, 1285, 1237, 1189, 1141, 1092, 1044, 995, 946, 897, 848, 799, 750, 700, 651, 601, 551, 501, 451, 401, 351, 301, 251, 201, 151, 100, 50, 0};

void MyDMA_Init(void)
{    
    rcu_periph_clock_enable(RCU_DMA0);                              //开启DMA0时钟
    dma_deinit(DMA0, DMA_CH1);                                      //DMA0复位

    dma_single_data_parameter_struct dma_init_parameter;            //申请结构体
    dma_single_data_para_struct_init(&dma_init_parameter);          //赋予默认值
    
    dma_init_parameter.periph_addr = (uint32_t)0x40000034;          //外设数据寄存器地址
    dma_init_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;    //外设地址自增

    dma_init_parameter.memory0_addr = (uint32_t)sin_table_12bit;    //内存缓冲区首地址
    dma_init_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;     //内存地址自增模式

    dma_init_parameter.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;//单次传输的数据宽度
    dma_init_parameter.circular_mode = DMA_CIRCULAR_MODE_ENABLE;    //循环模式
    dma_init_parameter.direction = DMA_MEMORY_TO_PERIPH;            //数据传输方向 内存→外设
    dma_init_parameter.number = 256;                                //总传输数据项数量
    dma_init_parameter.priority = DMA_PRIORITY_HIGH;                //通道优先级

    dma_single_data_mode_init(DMA0, DMA_CH1, &dma_init_parameter);  //DMA单条通道初始化
    dma_channel_subperipheral_select(DMA0, DMA_CH1, DMA_SUBPERI3);
    dma_channel_enable(DMA0, DMA_CH1);                              //启动DMA
}
