#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "LCD.h"

int main()
{
    board_init();
	HC595_LED_init();
	uint16_t i = 0, j = 0;
	LED_Display(1234);
    while(1)
    {
//		for(i = 1; i < 5; i++)
//		{
//			for(j = 0; j < 10; j++)
//			{
//				LED_XIAN(i,j);
//				delay_ms(100);
//			}
//		}
    }
}
