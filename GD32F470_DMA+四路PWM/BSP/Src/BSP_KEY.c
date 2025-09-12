#include "BSP_KEY.h"


void BSP_KEY_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_0);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
}

uint8_t key_scan(void)
{
    /* �ȶ�ȡ�������ŵĵ�ƽ ����ߵ�ƽ���������� */
    if(gpio_input_bit_get(GPIOA,GPIO_PIN_0) == SET) // ��������
    {
        /* �ӳ����� */
        Delay_ms(20);
        if(gpio_input_bit_get(GPIOA,GPIO_PIN_0) == SET) // �ٴμ�ⰴ���Ƿ���
        {
            while(gpio_input_bit_get(GPIOA,GPIO_PIN_0) == SET); // �ȴ������ɿ�
            return 1;
        }
    }
    return 0;
}
