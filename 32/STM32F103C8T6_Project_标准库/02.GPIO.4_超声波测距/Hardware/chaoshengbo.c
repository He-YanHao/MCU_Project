#include "stm32f10x.h"                  // Device header

#define HCSR04_TRIG PC_ODR_ODR0

void chaoshengbo_Init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIO口的时钟

GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//配置工作模式
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//配置GPIO口的位置
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//配置速度。
GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化GPIOA，并将其放在GPIO_InitStructure里
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//配置工作模式
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//配置GPIO口的位置
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//配置速度。
GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化GPIOA，并将其放在GPIO_InitStructure里



}

RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//开启AFIO的时钟，外部中断必须开启AFIO的时钟。
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);	//将外部中断的X号线映射到GPIO口，即选择X为外部中断引脚。
															//EXTI初始化
EXTI_InitTypeDef EXTI_InitStructure;						//定义结构体变量

EXTI_InitStructure.EXTI_Line = EXTI_Line1;					//选择配置外部中断的X号线
EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//指定外部中断线使能
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//指定外部中断线为中断模式
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;		//指定外部中断线为下降沿触发
EXTI_Init(&EXTI_InitStructure);								//将结构体变量交给EXTI_Init

NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC分组
															//NVIC初始化
NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量

NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		  //选择中断名字
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//指定NVIC线路的抢占优先级为1
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//指定NVIC线路的响应优先级为1
NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设


RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			//开启TIM2的时钟
TIM_InternalClockConfig(TIM2);									//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟。

TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量

TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器模式，选择向上计数
TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;				//计数周期，即ARR的值
TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//预分频器，即PSC的值
TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			//重复计数器，高级定时器才会用到
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);				//将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元	

TIM_ClearFlag(TIM2, TIM_FLAG_Update);						//清除定时器更新标志位																	//TIM_TimeBaseInit函数末尾，手动产生了更新事件，若不清除此标志位，则开启中断后，会立刻进入一次中断，如果不介意此问题，则不清除此标志位也可。
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);					//开启TIM2的更新中断
TIM_Cmd(TIM2, ENABLE);			//使能TIM2，定时器开始运行

NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;		  //选择中断函数名字
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为1
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
