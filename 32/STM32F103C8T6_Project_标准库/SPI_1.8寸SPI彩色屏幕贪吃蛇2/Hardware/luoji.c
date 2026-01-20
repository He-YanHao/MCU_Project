#include "stm32f10x.h"                  // Device header
#include <stdlib.h>
#include "snake.h"

uint8_t food_x, food_y;//食物的位置
uint8_t fangxiang;//蛇的移动方向
uint8_t length = 1;//蛇的长度
uint8_t shetou = 0;//蛇头的位置

typedef struct
{
    uint8_t snake_x;
    uint8_t snake_y;
}snake_xy;

snake_xy snake[12] = {8, 8};

void food_chuxian()//出现食物
{
    food_x = rand()%16;
    food_y = rand()%16;
    snake_food(food_x, food_y);
}

void snake_move()//蛇的移动
{
    fangxiang = Refang();//获取移动方向
    shetou++;
    if(shetou == 12)//长度到最大了
    {
        shetou = 0;//归零
        switch(fangxiang)
        {
            case 0: snake[0].snake_x = snake[11].snake_x + 1; break;//右
            case 1: snake[0].snake_x = snake[11].snake_x - 1; break;//左
            case 2: snake[0].snake_y = snake[11].snake_y + 1; break;//下
            case 3: snake[0].snake_y = snake[11].snake_y + 1; break;//上
        }
        //蛇头移位
        snake_head(snake[0].snake_x, snake[0].snake_y);
        //蛇尾清除
        snake_bai(snake[11].snake_x, snake[11].snake_y);
    }
    else
    {
        switch(fangxiang)
        {
            case 0: snake[shetou].snake_x = snake[shetou-1].snake_x + 1; break;//右
            case 1: snake[shetou].snake_x = snake[shetou-1].snake_x - 1; break;//左
            case 2: snake[shetou].snake_y = snake[shetou-1].snake_y + 1; break;//下
            case 3: snake[shetou].snake_y = snake[shetou-1].snake_y + 1; break;//上
        }
    //蛇头移位
    snake_head(snake[shetou].snake_x, snake[shetou].snake_y);
    //蛇尾清除
    snake_bai(snake[shetou-1].snake_x, snake[shetou-1].snake_y);
    }
}
