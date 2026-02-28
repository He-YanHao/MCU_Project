#include "main.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	BSP_LED_Init();		//LED初始化
	Key_Init();			//按键初始化
	Serial_Init();		//串口初始化

    extern volatile uint8_t Serial_RxFlag;
    extern uint8_t RxDataPack[];
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "Length:");
    
	while (1)
	{
        if(Serial_RxFlag == 1)
        {
            Serial_RxFlag = 0;
            OLED_ShowNum(1, 9, RxDataPack[0], 2);
            OLED_ShowHexNum(2, 1, RxDataPack[1], 2);
            OLED_ShowHexNum(2, 4, RxDataPack[2], 2);
            OLED_ShowHexNum(2, 7, RxDataPack[3], 2);
            OLED_ShowHexNum(2, 10, RxDataPack[4], 2);
        }
	}
}
