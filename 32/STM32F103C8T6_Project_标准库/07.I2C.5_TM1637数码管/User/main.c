#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "TM1637.h"

int main()
{
	uint8_t keydate = 0;
	OLED_Init();
	MyI2C_Init();
	TM1637();
	while(1)
	{
	}
}
