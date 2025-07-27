#include "Delay.h"

///**
// * @brief 初始化 Systick
// * @param ticks 每毫秒的时钟数（SystemCoreClock / 1000）
// */
//void Systick_Init(uint32_t ticks)
//{
//    if (SysTick_Config(ticks)) {
//        while (1); // Systick 初始化失败
//    }
//    NVIC_SetPriority(SysTick_IRQn, 0x00); // 设置优先级
//}

//static volatile uint32_t TimingDelay; // 全局变量，用于计数

///**
// * @brief Systick 中断处理函数（在 gd32f4xx_it.c 中实现）
// */
////void SysTick_Handler(void)
////{
////    if (TimingDelay != 0x00) {
////        TimingDelay--;
////    }
////}

///**
// * @brief 毫秒级延时
// * @param nTime 延时时间（ms）
// */
//void Delay_ms(uint32_t nTime)
//{
//    TimingDelay = nTime;
//    while (TimingDelay != 0);
//}

///**
// * @brief 微秒级延时（基于循环，不精确）
// * @param nTime 延时时间（us）
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
