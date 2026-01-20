#include "stm32f10x.h"
#include "LCD.h"
#include "snake.h"
#include <stdlib.h>

uint8_t fang = 4;//移动方向，默认4，不移动。0右，1左，2下，3上。
uint16_t grades = 0;//分数
uint8_t food_x, food_y, food_ju = 0;//食物位置以及状态 0表示被吃 1表示没被吃

struct snake_part{
uint8_t snake_x;//坐标
uint8_t snake_y;//坐标
};//蛇结构体
struct snake_part snake[12] = {8, 8};//蛇结构体数组，最多储存12的长度

struct snake_part sss;
int ssssnake_y = 98;

uint8_t snake_head_xy = 0;//蛇头位于结构体第几位
int length = 1;//长度

void snake_head(uint8_t x, uint8_t y)//蛇头，黑色
{
    uint8_t i, j;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            Gui_DrawPoint(8*x+i, 8*y+j,0x0000);//画一个点
        }
    }
}

void snake_bai(uint8_t x, uint8_t y)//蛇头，黑色
{
    uint8_t i, j;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            Gui_DrawPoint(8*x+i, 8*y+j,0xffff);//画一个点
        }
    }
}

void snake_body(uint8_t x, uint8_t y)//蛇身子，红色
{
    uint8_t i, j;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            Gui_DrawPoint(8*x+i, 8*y+j,0xf800);//画一个点
        }
    }
}

void snake_food(uint8_t x, uint8_t y)//食物，绿色
{
    uint8_t i, j;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            Gui_DrawPoint(8*x+i, 8*y+j,0x07e0);//画一个点
        }
    }
}

void gezi(uint8_t x, uint8_t y)//格子
{
    Gui_DrawLine(8*x+8, 8*y, 8*x+8, 8*y+8, RED);
    Gui_DrawLine(8*x, 8*y+8, 8*x+8, 8*y+8, RED);
}

void MyEXTI_Init(void)//中断初始化
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//开启AFIO的时钟，外部中断必须开启AFIO的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2| GPIO_Pin_4| GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*AFIO选择中断引脚*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);

	/*EXTI初始化*/
	EXTI_InitTypeDef EXTI_InitStructure;						//定义结构体变量
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line2| EXTI_Line4 | EXTI_Line6;//选择配置外部中断的0号线和1号线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//指定外部中断线使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//指定外部中断线为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//指定外部中断线为上升沿触发
	EXTI_Init(&EXTI_InitStructure);								//将结构体变量交给EXTI_Init，配置EXTI外设
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//选择配置NVIC的EXTI0线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//选择配置NVIC的EXTI0线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//选择配置NVIC的EXTI0线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	//选择配置NVIC的EXTI1线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为2
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
}

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)		//判断是否是外部中断0号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
		{
            fang = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);			//清除外部中断0号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}
void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2) == SET)		//判断是否是外部中断0号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 1)
		{
            fang = 1;
		}
		EXTI_ClearITPendingBit(EXTI_Line2);			//清除外部中断0号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}
void EXTI4_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line4) == SET)		//判断是否是外部中断0号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1)
		{
            fang = 2;
		}
		EXTI_ClearITPendingBit(EXTI_Line4);			//清除外部中断0号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line6) == SET)		//判断是否是外部中断0号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 1)
		{
            fang = 3;
		}
		EXTI_ClearITPendingBit(EXTI_Line6);			//清除外部中断0号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}


void snake_Init(void)//游戏初始化
{
    MyEXTI_Init();
}

void food(void)//出现食物
{
    if(food_ju == 0)
    {
        food_ju = 1;
        food_x = rand()%16, food_y = rand()%16;
        snake_food(food_x, food_y);
    }
}

void snake_move(void)//蛇移动
{
    //蛇头移动
    snake_head_xy++;
    if(snake_head_xy == 12)
    {
    snake_head_xy = 0;
    }
    switch (fang)//蛇头移动
    {
    case 0: snake.snake_x[snake_head_xy] = snake.snake_x[snake_head_xy-1] + 1; break;
    case 1: snake.snake_x[snake_head_xy] = snake.snake_x[snake_head_xy-1] - 1; break;
    case 2: snake.snake_y[snake_head_xy] = snake.snake_y[snake_head_xy-1] + 1; break;
    case 3: snake.snake_y[snake_head_xy] = snake.snake_y[snake_head_xy-1] - 1; break;
    }
    snake_bai(snake.snake_x[snake_head_xy - length], snake.snake_y[snake_head_xy - length]);//蛇尾消除
}

uint8_t snake_judement(void)//蛇判断是否死亡
{
    uint8_t ret = 0;
    if(snake_x < 0 | snake_x > 15 )
    {
        ret = 1;
    }
    if(snake_y < 0 | snake_y > 15 )
    {
        ret = 1;
    }
    return ret;
}

void food_judement()//判断食物是否被吃
{
    if(food_x == snake_x && food_y == snake_y)
    {
        food_ju = 0;
        grades = grades + 1;
        Gui_DrawFont_GBK16(96, 136, BLACK, WHITE, "    ");
        Gui_DrawFont_Number(96, 136, BLACK, WHITE, grades);
    }
}
