#include "MyCAN.h"

void MyCAN_Init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // GPIOB时钟（PB8/PB9）

    GPIO_InitTypeDef GPIO_InitStruct;
    // TX引脚
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;  // 推挽输出
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    // RX引脚
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;    // 上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    CAN_DeInit(CAN1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); // CAN1时钟

    CAN_InitTypeDef CAN_InitStruct;
    //CAN控制器初始化
    CAN_InitStruct.CAN_TTCM = DISABLE;       // 禁止时间触发模式
    CAN_InitStruct.CAN_ABOM = ENABLE;        // 自动离线恢复
    CAN_InitStruct.CAN_AWUM = ENABLE;        // 自动唤醒模式
    CAN_InitStruct.CAN_NART = DISABLE;       // 禁止自动重传
    CAN_InitStruct.CAN_RFLM = DISABLE;       // 接收FIFO不锁定（覆盖旧消息）
    CAN_InitStruct.CAN_TXFP = DISABLE;       // 发送优先级按ID排序
    //CAN_InitStruct.CAN_Mode = CAN_Mode_LoopBack; // 工作模式：环回模式
    CAN_InitStruct.CAN_Mode = CAN_Mode_Normal; // 工作模式：正常模式
    CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;    // 同步跳转宽度 = 1时间单位
    CAN_InitStruct.CAN_BS1 = CAN_BS1_6tq;    // 时间段1 = 6时间单位
    CAN_InitStruct.CAN_BS2 = CAN_BS2_5tq;    // 时间段2 = 5时间单位
    CAN_InitStruct.CAN_Prescaler = 3;        // 分频系数 = 3
    CAN_Init(CAN1, &CAN_InitStruct);

    CAN_FilterInitTypeDef CAN_FilterInitStruct;
    CAN_FilterInitStruct.CAN_FilterNumber = 0;          // 使用过滤器0
    CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask; // 掩码模式
    CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit; // 32位宽
    CAN_FilterInitStruct.CAN_FilterIdHigh = 0x0000;     // 期望ID高16位（标准ID）
    CAN_FilterInitStruct.CAN_FilterIdLow = 0x0000;      // 期望ID低16位
    CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x0000; // 掩码高16位（全匹配）
    CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;  // 掩码低16位
    CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; // 关联到FIFO0
    CAN_FilterInitStruct.CAN_FilterActivation = ENABLE; // 激活过滤器
    CAN_FilterInit(&CAN_FilterInitStruct);

    //映射GPIO到CAN功能
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1); // PB8 -> CAN1_RX
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1); // PB9 -> CAN1_TX

//    NVIC_EnableIRQ(CAN1_RX0_IRQn);           // 在NVIC中启用中断
    //配置CAN接收中断
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); // 使能FIFO0消息中断
    
    // 8. 配置NVIC
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

// CAN发送函数
uint8_t CAN_SendMessage(uint32_t id, uint8_t* data, uint8_t length)
{
    CanTxMsg TxMessage;
    uint8_t mailbox = 0;
    // 配置发送消息
    TxMessage.StdId = id;              // 标准ID
    TxMessage.ExtId = 0;               // 扩展ID（0表示使用标准ID）
    TxMessage.IDE = CAN_Id_Standard;   // 标准帧
    TxMessage.RTR = CAN_RTR_DATA;      // 数据帧
    TxMessage.DLC = length;            // 数据长度

    // 复制数据
    for(uint8_t i = 0; i < length; i++) {
        TxMessage.Data[i] = data[i];
    }
    
    // 发送消息
    mailbox = CAN_Transmit(CAN1, &TxMessage);
    
    // 等待发送完成
    uint32_t timeout = 1000000;
    while((CAN_TransmitStatus(CAN1, mailbox) != CANTXOK) && (timeout > 0)) {
        timeout--;
    }
    
    return (timeout > 0) ? 1 : 0;
}

// CAN接收中断处理函数
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;

    if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        // 读取接收到的消息
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        
        // 处理接收到的消息
        uint32_t id = RxMessage.StdId;
        uint8_t length = RxMessage.DLC;
        uint8_t data[8];
        
        for(uint8_t i = 0; i < length; i++)
        {
            data[i] = RxMessage.Data[i];
        }
        // 用户自定义处理函数
        
        CAN_SendMessage(3, data, length);

        
        // 清除中断标志
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    }
}
