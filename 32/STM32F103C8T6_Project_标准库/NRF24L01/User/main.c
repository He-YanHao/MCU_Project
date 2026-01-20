#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "NRF24L01.h"

int main()
{
	OLED_Init();
	OLED_ShowNum(2,1,4,3);
	while(1)
	{
		OLED_ShowNum(1,1,NRF24L01_Check(),1);
	}
}
