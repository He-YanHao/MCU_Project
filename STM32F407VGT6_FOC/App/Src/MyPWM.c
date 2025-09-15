#include "MyPWM.h"

void MyPWM_Init(void)
{
    //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);//TIM3_CH1
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);//TIM3_CH2
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);//TIM3_CH3

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //定时器初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 4095 - 1;  //自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 20 - 1; //预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 初始化三个PWM通道
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // 通道1初始化
    TIM_OCInitStructure.TIM_Pulse = 1024;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // 通道2初始化
    TIM_OCInitStructure.TIM_Pulse = 2048; // 初始占空比为0
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    // 通道3初始化
    TIM_OCInitStructure.TIM_Pulse = 3072; // 初始占空比为0
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);//使能ARR预装载
    TIM_Cmd(TIM3, ENABLE);//使能定时器
}

void Change_PWM1(uint16_t PWM)
{
    TIM_SetCompare1(TIM3, PWM);
}

void Change_PWM2(uint16_t PWM)
{
    TIM_SetCompare2(TIM3, PWM);
}

void Change_PWM3(uint16_t PWM)
{
    TIM_SetCompare3(TIM3, PWM);
}
