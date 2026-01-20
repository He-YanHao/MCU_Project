#include "delay.h"

void Delay1us(void)	//@11.0592MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 1;
	while (--i);
}


void delay_us(unsigned long us)
{
    for(us; us!=0; us--)
    {
    Delay1us();
    }
}

void delay_ms(unsigned long ms)
{
    delay_us(ms*1000);
}

void delay_s(unsigned long s)
{
    delay_ms(s*1000);
}
