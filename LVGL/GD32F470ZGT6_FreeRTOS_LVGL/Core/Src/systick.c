#include "systick.h"

void delay_1ms(void)
{
    volatile uint32_t delay;
    // 根据主频 240 MHz 计算需要的时钟周期
    uint32_t cycles_per_ms = 240000;

    for (delay = 0; delay < cycles_per_ms; delay++)
    {
        __NOP();  // 空操作，确保每个循环消耗一个时钟周期
    }
}

void delay_ms(uint32_t delay)
{
    for(uint32_t i = 0; i < delay; i++)
    {
        delay_1ms();
    }
}
