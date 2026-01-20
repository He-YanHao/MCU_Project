#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "OLED_Data.h"
#include "Key.h"



int main(void)
{	
	uint8_t key = 0;
	OLED_Init();
	Key_Init();
	while (1)
	{
	key =	Key_GetNum();
	OLED_Clear();
	OLED_ShowImage(27, 56, 16, 16, wan);
	OLED_ShowImage(64, 56, 4, 8, di);
	OLED_Update();
	}
}

//定时器中断函数，可以复制到使用它的地方
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
