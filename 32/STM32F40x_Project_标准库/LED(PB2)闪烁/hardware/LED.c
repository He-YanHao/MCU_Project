#include "stm32f4xx.h"                  // Device header

void LED_GPIO_Init(void)
{
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//无所谓
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//最快速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//推挽
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LED_GPIO_Liang(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_2);
}

void LED_GPIO_Mie(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_2);
}
