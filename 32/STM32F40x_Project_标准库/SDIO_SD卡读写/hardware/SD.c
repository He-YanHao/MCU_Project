#include "stm32f4xx.h"                  // Device header
#include "SD.h"

/*
* 函数名：NVIC_Configuration
* 描述  ：SDIO 优先级配置为最高优先级。
* 输入  ：无
* 输出  ：无
*/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void GPIO_Configuration(void)
{
    /*开启时钟*/
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
    /*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//最快速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//最快速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    
}

/**
* 函数名：SD_Init
* 描述  ：初始化SD卡，使卡处于就绪状态(准备传输数据)
* 输入  ：无
* 输出  ：-SD_Error SD卡错误代码
*         成功时则为 SD_OK
* 调用  ：外部调用
*/
SD_Error SD_Init(void)
{
    /*重置SD_Error状态*/
    SD_Error errorstatus = SD_OK;

    NVIC_Configuration();

    /* SDIO 外设底层引脚初始化 */
    GPIO_Configuration();

    /*对SDIO的所有寄存器进行复位*/
    SDIO_DeInit();

    /*上电并进行卡识别流程，确认卡的操作电压  */
    errorstatus = SD_PowerON();

    /*如果上电，识别不成功，返回“响应超时”错误 */
    if (errorstatus != SD_OK) {
        /*!< CMD Response TimeOut (wait for CMDSENT flag) */
        return (errorstatus);
    }

    /*卡识别成功，进行卡初始化    */
    errorstatus = SD_InitializeCards();

    if (errorstatus != SD_OK) {  //失败返回
        /*!< CMD Response TimeOut (wait for CMDSENT flag) */
        return (errorstatus);
    }

    /* 配置SDIO外设
    * 上电识别，卡初始化都完成后，进入数据传输模式，提高读写速度
    */

    /* SDIOCLK = HCLK, SDIO_CK = HCLK/(2 + SDIO_TRANSFER_CLK_DIV) */
    SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV;

    /*上升沿采集数据 */
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;

    /* Bypass模式使能的话，SDIO_CK不经过SDIO_ClockDiv分频 */
    SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;

    /* 若开启此功能，在总线空闲时关闭sd_clk时钟 */
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;

    /* 暂时配置成1bit模式 */
    SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;

    /* 硬件流，若开启，在FIFO不能进行发送和接收数据时，数据传输暂停 */
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;

    SDIO_Init(&SDIO_InitStructure);

    if (errorstatus == SD_OK) {
        /* 用来读取csd/cid寄存器 */
        errorstatus = SD_GetCardInfo(&SDCardInfo);
    }

    if (errorstatus == SD_OK) {
        /* 通过cmd7  ,rca选择要操作的卡 */
    errorstatus = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
    }

    if (errorstatus == SD_OK) {
        /* 最后为了提高读写，开启4bits模式 */
        errorstatus = SD_EnableWideBusOperation(SDIO_BusWide_4b);
    }

    return (errorstatus);
}

/************************************************
函数名称 ： SD_DMAEndOfTransferStatus
功    能 ： SD DMA结束传输状态
参    数 ： 无
返 回 值 ： 无
*************************************************/
uint32_t SD_DMAEndOfTransferStatus(void)
{
    return (uint32_t)DMA_GetFlagStatus(DMA2_FLAG_TC4);//
}

//SDIO_FIOF地址=SDIO地址+0x80至 sdio地址+0xfc
#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40018080)

/*
* 函数名：SD_DMA_RxConfig
* 描述  ：为SDIO接收数据配置DMA2的通道4的请求
* 输入  ：BufferDST：用于装载数据的变量指针
*       : BufferSize： 缓冲区大小
* 输出  ：无
*/
void SD_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize)
{
    DMA_InitTypeDef DMA_InitStructure;

    DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 |
                DMA2_FLAG_HT4 | DMA2_FLAG_GL4);//清除DMA标志位

    /*!< 配置前先禁止DMA */
    DMA_Cmd(DMA2_Channel4, DISABLE);

    /*!< DMA2 传输配置 */
    //外设地址，fifo
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
    //目标地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferDST;
    //外设为源地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //除以4，把字转成字节单位
    DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
    //外设地址不自增
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //存储目标地址自增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //外设数据大小为字，32位
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    //外设数据大小为字，32位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    //不循环
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //通道优先级高
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //非 存储器至存储器模式
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA2_Channel4, &DMA_InitStructure);

    /*!< 使能DMA通道 */
    DMA_Cmd(DMA2_Channel4, ENABLE);
}

/*
* 函数名：SD_DMA_RxConfig
* 描述  ：为SDIO发送数据配置DMA2的通道4的请求
* 输入  ：BufferDST：装载了数据的变量指针
    BufferSize：  缓冲区大小
* 输出  ：无
*/
void SD_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
{
    DMA_InitTypeDef DMA_InitStructure;

    DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 |
                DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

    /*!< 配置前先禁止DMA */
    DMA_Cmd(DMA2_Channel4, DISABLE);

    /*!< DMA2 传输配置 */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferSRC;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//外设为写入目标
    DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel4, &DMA_InitStructure);

    /*!< 使能DMA通道 */
    DMA_Cmd(DMA2_Channel4, ENABLE);
}