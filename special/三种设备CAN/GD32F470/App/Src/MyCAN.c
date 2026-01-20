#include "MyCAN.h"

void MyCAN_Init(void)
{
    //开始GPIOA时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    /* 配置CAN_RX引脚(PA11) */
    gpio_af_set(GPIOA, GPIO_AF_9, GPIO_PIN_11);  // CAN_RX
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_11);
    /* 配置CAN_TX引脚(PA12) */
    gpio_af_set(GPIOA, GPIO_AF_9, GPIO_PIN_12);  // CAN_TX
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_12);

    //开始时钟
    rcu_periph_clock_enable(RCU_CAN0);
    //定义中断优先级
    nvic_irq_enable(CAN0_RX1_IRQn,0,0);
    //缺省值初始化
    can_deinit(CAN0);
    //CAN初始化结构体
    can_parameter_struct can_parameter;
    //获取默认值
    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);
    //配置参数
    //can_parameter.working_mode = CAN_LOOPBACK_MODE;//环回模式
    can_parameter.working_mode = CAN_NORMAL_MODE;//正常模式
    can_parameter.time_triggered = DISABLE;//时间触发通信模式 
    can_parameter.auto_bus_off_recovery = ENABLE;//自动离线恢复
    can_parameter.auto_wake_up = DISABLE;//自动唤醒
    can_parameter.auto_retrans = ENABLE;//自动重传 
    can_parameter.rec_fifo_overwrite = DISABLE;//接收FIFO满时覆盖 
    can_parameter.trans_fifo_order = DISABLE;//发送FIFO顺序
    //配置波特率
    can_parameter.time_segment_1 = CAN_BT_BS1_8TQ;//位段1 
    can_parameter.time_segment_2 = CAN_BT_BS2_7TQ;//位段2 
    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;//再同步补偿宽度 
    can_parameter.prescaler = 5;//波特率分频系数
    //初始化CAN
    can_init(CAN0, &can_parameter);

    /* 配置过滤器 - 接收所有报文 */
    can_filter_parameter_struct can_filter;
    can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);
    can_filter.filter_number = 0;                   // 过滤器0
    can_filter.filter_mode = CAN_FILTERMODE_MASK;   // 掩码模式
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;  // 32位模式
    can_filter.filter_list_high = 0x0000;           // ID高16位
    can_filter.filter_list_low = 0x0000;            // ID低16位
    can_filter.filter_mask_high = 0x0000;           // 掩码高16位
    can_filter.filter_mask_low = 0x0000;            // 掩码低16位
    can_filter.filter_fifo_number = CAN_FIFO0;      // 使用FIFO0
    can_filter.filter_enable = ENABLE;              // 使能过滤器
    can_filter_init(&can_filter);
}

uint8_t can_send_message(uint32_t id, uint8_t *data, uint8_t length)
{
    can_trasnmit_message_struct tx_message;
    can_receive_message_struct  receive_message;
    uint8_t mailbox;

    can_struct_para_init(CAN_TX_MESSAGE_STRUCT, &tx_message);
    /* initialize transmit message */
    /* 配置发送参数 */
    tx_message.tx_sfid = id;                // 标准帧ID
    //tx_message.tx_efid = 0;               // 扩展帧ID(如果使用)
    tx_message.tx_ff = CAN_FF_STANDARD;     // 标准帧格式
    // tx_message.tx_ff = CAN_FF_EXTENDED;  // 或扩展帧格式
    tx_message.tx_ft = CAN_FT_DATA;         // 数据帧
    tx_message.tx_dlen = length;            // 数据长度

    /* 填充数据 */
    for(uint8_t i = 0; i < length; i++)
    {
        tx_message.tx_data[i] = data[i];
    }
    /* 发送报文 */
    mailbox = can_message_transmit(CAN0, &tx_message);
    /* 等待发送完成或超时 */
    uint32_t timeout = 100000;  // 超时计数
    while((CAN_TRANSMIT_OK != can_transmit_states(CAN0, mailbox)) && (timeout > 0))
    {
        timeout--;
    }
    
    return (timeout > 0) ? SUCCESS : ERROR;
}

uint8_t can_receive_message(uint32_t *id, uint8_t *data, uint8_t *length)
{
    can_receive_message_struct rx_message;
    
    /* 检查是否有报文 */
    if(can_receive_message_length_get(CAN0, CAN_FIFO0) == 0)
    {
        USART0_DATA(can_transmit_states(CAN0,CAN_MAILBOX0));        
        USART0_DATA(can_transmit_states(CAN0,CAN_MAILBOX1));        
        USART0_DATA(can_transmit_states(CAN0,CAN_MAILBOX2));        
        return ERROR;  // 无报文
    }
    
    /* 初始化接收结构体 */
    can_struct_para_init(CAN_RX_MESSAGE_STRUCT, &rx_message);
    
    /* 接收报文 */
    can_message_receive(CAN0, CAN_FIFO0, &rx_message);
    
    /* 提取信息 */
    *id = rx_message.rx_sfid;  // 标准帧ID
    //*id = rx_message.rx_efid;  // 或扩展帧ID
    *length = rx_message.rx_dlen;
    
    /* 复制数据 */
    for(uint8_t i = 0; i < *length; i++)
    {
        data[i] = rx_message.rx_data[i];
    }
    
    return SUCCESS;
}
