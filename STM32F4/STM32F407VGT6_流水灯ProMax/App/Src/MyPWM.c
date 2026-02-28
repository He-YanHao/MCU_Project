#include "MyPWM.h"

void MyPWM_Init(void)
{
    //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);//TIM2_CH1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);//TIM2_CH2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);//TIM2_CH3
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);//TIM2_CH4

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);//TIM3_CH1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);//TIM3_CH2
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);//TIM3_CH3
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);//TIM3_CH4

    GPIO_InitTypeDef GPIO_InitStructureA;
    GPIO_InitStructureA.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructureA.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructureA.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructureA.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructureA.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructureA);

    GPIO_InitTypeDef GPIO_InitStructureB;
    GPIO_InitStructureB.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructureB.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructureB.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructureB.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructureB);

    //定时器初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = PWM_MAX - 1;  //自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 200 - 1; //预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 初始化PWM通道
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0; //初始占空比
    // 通道1初始化
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    // 通道2初始化
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    // 通道3初始化
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    // 通道4初始化
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // 通道1初始化
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    // 通道2初始化
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    // 通道3初始化
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    // 通道4初始化
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);


    TIM_ARRPreloadConfig(TIM2, ENABLE);//使能ARR预装载
    TIM_ARRPreloadConfig(TIM3, ENABLE);//使能ARR预装载
    TIM_Cmd(TIM2, ENABLE);//使能定时器
    TIM_Cmd(TIM3, ENABLE);//使能定时器
}

void Change_TIM2_PWM1(uint16_t PWM)
{
    TIM_SetCompare1(TIM2, PWM);
}

void Change_TIM2_PWM2(uint16_t PWM)
{
    TIM_SetCompare2(TIM2, PWM);
}

void Change_TIM2_PWM3(uint16_t PWM)
{
    TIM_SetCompare3(TIM2, PWM);
}

void Change_TIM2_PWM4(uint16_t PWM)
{
    TIM_SetCompare4(TIM2, PWM);
}

void Change_TIM3_PWM1(uint16_t PWM)
{
    TIM_SetCompare1(TIM3, PWM);
}

void Change_TIM3_PWM2(uint16_t PWM)
{
    TIM_SetCompare2(TIM3, PWM);
}

void Change_TIM3_PWM3(uint16_t PWM)
{
    TIM_SetCompare3(TIM3, PWM);
}

void Change_TIM3_PWM4(uint16_t PWM)
{
    TIM_SetCompare4(TIM3, PWM);
}

void PWM_ALL_Change(uint8_t PWM)
{
    switch(PWM)
    {
        case 1:
        for(int16_t i = PWM_MAX; i >= 0; i--)
        {
            Change_TIM2_PWM1(i);
            DELAY;
        }break;
    case 2:
        for(int16_t i = PWM_MAX; i >= 0; i--)
        {
            Change_TIM2_PWM2(i);
            DELAY;
        }break;
    case 3:
        for(int16_t i = PWM_MAX; i >= 0; i--)
        {
            Change_TIM2_PWM3(i);
            DELAY;
        }break;
    case 4:
        for(int16_t i = PWM_MAX; i >= 0; i--)
        {
            Change_TIM2_PWM4(i);
            DELAY;
        }break;
    case 5:
        for(int16_t i = PWM_MAX; i >= 0; i--)
        {
            Change_TIM3_PWM1(i);
            DELAY;
        }break;
    case 6:
        for(int16_t i = PWM_MAX; i >= 0; i--)
        {
            Change_TIM3_PWM2(i);
            DELAY;
        }break;
    case 7:
        for(int16_t i = PWM_MAX; i >= 0; i--)
        {
            Change_TIM3_PWM3(i);
            DELAY;
        }break;
    case 8:
        for(int16_t i = PWM_MAX; i >= 0; i--)
        {
            Change_TIM3_PWM4(i);
            DELAY;
        }break;
    }
}

void PWM_ALL_MAX(void)
{
    Change_TIM2_PWM1(PWM_MAX);
    Change_TIM2_PWM2(PWM_MAX);
    Change_TIM2_PWM3(PWM_MAX);
    Change_TIM2_PWM4(PWM_MAX);

    Change_TIM3_PWM1(PWM_MAX);
    Change_TIM3_PWM2(PWM_MAX);
    Change_TIM3_PWM3(PWM_MAX);
    Change_TIM3_PWM4(PWM_MAX);
}

void PWM_ALL_MIN(void)
{
    Change_TIM2_PWM1(0);
    Change_TIM2_PWM2(0);
    Change_TIM2_PWM3(0);
    Change_TIM2_PWM4(0);

    Change_TIM3_PWM1(0);
    Change_TIM3_PWM2(0);
    Change_TIM3_PWM3(0);
    Change_TIM3_PWM4(0);
}
