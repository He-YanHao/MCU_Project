#include "Delay.h"

void delay(void)
{
    for(uint32_t i = 0; i < 70; i++)
    {

    }
}

void Delay_us(uint32_t us)
{
    while(us)
    {
        delay();
        us--;
    }
}

void Delay_ms(uint32_t ms)
{
    while(ms)
    {
        Delay_us(ms);
        ms--;
    }
}
