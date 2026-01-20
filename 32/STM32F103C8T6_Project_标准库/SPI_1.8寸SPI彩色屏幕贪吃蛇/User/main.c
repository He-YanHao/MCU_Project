#include "stm32f10x.h"                  // Device header
#include "LCD.h"
#include "Delay.h"
#include "sanke.h"

int main()
{
	LCD_Init();
    snake_Init();
    Gui_DrawLine(0, 128, 127, 128, RED);
	while(1)
	{
        //出现食物
        food();
        //延时
        Delay_ms(500);
        //移动
        snake_move();
        //判断 死亡？
        if(snake_judement() == 1)//=1 是撞墙死亡
        {
            Gui_DrawFont_GBK16(0, 136, BLACK, WHITE, "zhuang qiang");
            break;
        }
        //判断 吃掉食物？
        food_judement();
	}
}
