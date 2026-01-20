#include "stm32f10x.h"                  // Device header
#include "LCD.h"
#include "Delay.h"
#include "luoji.h"
#include "snake.h"

int main()
{
	LCD_Init();
    snake_Init();
	while(1)
	{
        //出现食物
        //food_chuxian();
                
        //移动过程
        snake_move();//蛇的移动

        //延时
        Delay_ms(600);
        
        //判断 死亡？
        
        //判断 吃掉食物？
        
	}
}
