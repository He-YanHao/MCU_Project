#include "Delay.h"

///**
// * @brief ��ʼ�� Systick
// * @param ticks ÿ�����ʱ������SystemCoreClock / 1000��
// */
//void Systick_Init(uint32_t ticks)
//{
//    if (SysTick_Config(ticks)) {
//        while (1); // Systick ��ʼ��ʧ��
//    }
//    NVIC_SetPriority(SysTick_IRQn, 0x00); // �������ȼ�
//}

//static volatile uint32_t TimingDelay; // ȫ�ֱ��������ڼ���

///**
// * @brief Systick �жϴ��������� gd32f4xx_it.c ��ʵ�֣�
// */
////void SysTick_Handler(void)
////{
////    if (TimingDelay != 0x00) {
////        TimingDelay--;
////    }
////}

///**
// * @brief ���뼶��ʱ
// * @param nTime ��ʱʱ�䣨ms��
// */
//void Delay_ms(uint32_t nTime)
//{
//    TimingDelay = nTime;
//    while (TimingDelay != 0);
//}

///**
// * @brief ΢�뼶��ʱ������ѭ��������ȷ��
// * @param nTime ��ʱʱ�䣨us��
// */
//void Delay_us(uint32_t nTime)
//{
//    uint32_t i;
//    for (i = 0; i < nTime * (SystemCoreClock / 1000000); i++);
//}

void Delay_ms(uint32_t ms)
{
    uint32_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < (SystemCoreClock / 10000); j++);
    }
}
