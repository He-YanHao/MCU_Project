#ifndef __SNAKE_H
#define __SNAKE_H

void snake_head(uint8_t x, uint8_t y);//蛇头，黑色
void snake_bai(uint8_t x, uint8_t y);//清除，白色
void snake_body(uint8_t x, uint8_t y);//蛇身子，红色
void snake_food(uint8_t x, uint8_t y);//食物，绿色
void MyEXTI_Init(void);//中断初始化
void snake_Init(void);//游戏初始化
uint8_t Refang(void);//返回方向

#endif
