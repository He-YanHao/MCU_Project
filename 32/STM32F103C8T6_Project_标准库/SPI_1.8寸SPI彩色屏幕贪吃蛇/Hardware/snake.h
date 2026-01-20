#ifndef __SNAKE_H
#define __SNAKE_H

#define MAX_PARTS 12

void snake_Init(void);//游戏初始化
void snake_move(void);//蛇移动
uint8_t snake_judement(void);//蛇状态判断
void food(void);//出现食物
void food_judement(void);//判断食物是否被吃

#endif

