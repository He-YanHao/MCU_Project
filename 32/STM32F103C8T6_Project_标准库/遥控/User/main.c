#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NEC.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();//OLED初始化
	NEC_Init();//定时中断初始化
    uint8_t i = 0;
	while (1)
	{
        i = reNEC();
        if(i != 0)
        {
            OLED_ShowHexNum(1, 1, i, 2);
            OLED_ShowBinNum(2, 1, i, 8);
        }
	}
}
