#include "MyCAN.h"

void MyCAN_Init(void)
{
    //��ʼGPIOAʱ��
    rcu_periph_clock_enable(RCU_GPIOA);
    /* ����CAN_RX����(PA11) */
    gpio_af_set(GPIOA, GPIO_AF_9, GPIO_PIN_11);  // CAN_RX
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_11);
    /* ����CAN_TX����(PA12) */
    gpio_af_set(GPIOA, GPIO_AF_9, GPIO_PIN_12);  // CAN_TX
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_12);

    //��ʼʱ��
    rcu_periph_clock_enable(RCU_CAN0);
    //�����ж����ȼ�
    nvic_irq_enable(CAN0_RX1_IRQn,0,0);
    //ȱʡֵ��ʼ��
    can_deinit(CAN0);
    //CAN��ʼ���ṹ��
    can_parameter_struct can_parameter;
    //��ȡĬ��ֵ
    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);
    //���ò���
    //can_parameter.working_mode = CAN_LOOPBACK_MODE;//����ģʽ
    can_parameter.working_mode = CAN_NORMAL_MODE;//����ģʽ
    can_parameter.time_triggered = DISABLE;//ʱ�䴥��ͨ��ģʽ 
    can_parameter.auto_bus_off_recovery = ENABLE;//�Զ����߻ָ�
    can_parameter.auto_wake_up = DISABLE;//�Զ�����
    can_parameter.auto_retrans = ENABLE;//�Զ��ش� 
    can_parameter.rec_fifo_overwrite = DISABLE;//����FIFO��ʱ���� 
    can_parameter.trans_fifo_order = DISABLE;//����FIFO˳��
    //���ò�����
    can_parameter.time_segment_1 = CAN_BT_BS1_8TQ;//λ��1 
    can_parameter.time_segment_2 = CAN_BT_BS2_7TQ;//λ��2 
    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;//��ͬ��������� 
    can_parameter.prescaler = 5;//�����ʷ�Ƶϵ��
    //��ʼ��CAN
    can_init(CAN0, &can_parameter);

    /* ���ù����� - �������б��� */
    can_filter_parameter_struct can_filter;
    can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);
    can_filter.filter_number = 0;                   // ������0
    can_filter.filter_mode = CAN_FILTERMODE_MASK;   // ����ģʽ
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;  // 32λģʽ
    can_filter.filter_list_high = 0x0000;           // ID��16λ
    can_filter.filter_list_low = 0x0000;            // ID��16λ
    can_filter.filter_mask_high = 0x0000;           // �����16λ
    can_filter.filter_mask_low = 0x0000;            // �����16λ
    can_filter.filter_fifo_number = CAN_FIFO0;      // ʹ��FIFO0
    can_filter.filter_enable = ENABLE;              // ʹ�ܹ�����
    can_filter_init(&can_filter);
}

uint8_t can_send_message(uint32_t id, uint8_t *data, uint8_t length)
{
    can_trasnmit_message_struct tx_message;
    can_receive_message_struct  receive_message;
    uint8_t mailbox;

    can_struct_para_init(CAN_TX_MESSAGE_STRUCT, &tx_message);
    /* initialize transmit message */
    /* ���÷��Ͳ��� */
    tx_message.tx_sfid = id;                // ��׼֡ID
    //tx_message.tx_efid = 0;               // ��չ֡ID(���ʹ��)
    tx_message.tx_ff = CAN_FF_STANDARD;     // ��׼֡��ʽ
    // tx_message.tx_ff = CAN_FF_EXTENDED;  // ����չ֡��ʽ
    tx_message.tx_ft = CAN_FT_DATA;         // ����֡
    tx_message.tx_dlen = length;            // ���ݳ���

    /* ������� */
    for(uint8_t i = 0; i < length; i++)
    {
        tx_message.tx_data[i] = data[i];
    }
    /* ���ͱ��� */
    mailbox = can_message_transmit(CAN0, &tx_message);
    /* �ȴ�������ɻ�ʱ */
    uint32_t timeout = 100000;  // ��ʱ����
    while((CAN_TRANSMIT_OK != can_transmit_states(CAN0, mailbox)) && (timeout > 0))
    {
        timeout--;
    }
    
    return (timeout > 0) ? SUCCESS : ERROR;
}

uint8_t can_receive_message(uint32_t *id, uint8_t *data, uint8_t *length)
{
    can_receive_message_struct rx_message;
    
    /* ����Ƿ��б��� */
    if(can_receive_message_length_get(CAN0, CAN_FIFO0) == 0)
    {
        USART0_DATA(can_transmit_states(CAN0,CAN_MAILBOX0));        
        USART0_DATA(can_transmit_states(CAN0,CAN_MAILBOX1));        
        USART0_DATA(can_transmit_states(CAN0,CAN_MAILBOX2));        
        return ERROR;  // �ޱ���
    }
    
    /* ��ʼ�����սṹ�� */
    can_struct_para_init(CAN_RX_MESSAGE_STRUCT, &rx_message);
    
    /* ���ձ��� */
    can_message_receive(CAN0, CAN_FIFO0, &rx_message);
    
    /* ��ȡ��Ϣ */
    *id = rx_message.rx_sfid;  // ��׼֡ID
    //*id = rx_message.rx_efid;  // ����չ֡ID
    *length = rx_message.rx_dlen;
    
    /* �������� */
    for(uint8_t i = 0; i < *length; i++)
    {
        data[i] = rx_message.rx_data[i];
    }
    
    return SUCCESS;
}
