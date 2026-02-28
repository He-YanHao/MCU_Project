#include "Delay.h"

static inline void delay_us_internal(uint32_t us)
{
    volatile uint32_t i;

    while (us--)
    {
        for (i = 0; i < 50; i++)
        {
            __asm volatile ("nop");
        }
    }
}

void Delay_us(uint32_t us)
{
    delay_us_internal(us);
}

void Delay_ms(uint32_t ms)
{
    while (ms--)
    {
        Delay_us(1000);
    }
}