#include "key.h"

/**
  * @brief  ��ȷ��ʱ1ms��72MHz��Ƶ��
  * @param  ��
  * @retval ��
  * @note ����72��ʱ������/΢����㣬��ʵ��У׼
  */
void Delay_1ms(void)
{
    volatile uint32_t delay = 72 * 1000 / 4;  // ���������Ż����������ֵ
    while(delay--);
}

void Delay_ms(uint16_t ms)
{
    while(ms--)
    {
        Delay_1ms();
    }
}

void Key_Init(void)
{
    // 1. ʹ��GPIOEʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    // 2. ����GPIO
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // 3. ��ʼ��
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

uint8_t Key_return(void)
{
    // ����ɨ�躯��
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
    {  // �������£��͵�ƽ��
        Delay_ms(15);// ������ʱ
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET); // �ȴ��ɿ�
        }
        return KEY0;
    }
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
    {  // �������£��͵�ƽ��
        Delay_ms(15);// ������ʱ
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET); // �ȴ��ɿ�
        }
        return KEY1;
    }
    return 0;
}
