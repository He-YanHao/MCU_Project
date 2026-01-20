#include "stm32f10x.h"                  // Device header
#include "LCD.h"
#include "Delay.h"

int main()
{
	LCD_Init();
    Gui_DrawFont_GBK16(0, 0, BLACK, WHITE, "aaaaaa");
	while(1)
	{
	}
}
