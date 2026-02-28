#include "main.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	BSP_LED_Init();		//LED初始化
	Key_Init();			//按键初始化
	Serial_Init();		//串口初始化
    OLED_ShowString(1,1,"AAAAAAA");
	while (1)
	{

	}
}
