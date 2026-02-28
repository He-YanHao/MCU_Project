#include "test.h"

extern PID_NUM MyPID;

void TIMER_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    //定时器初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;  //自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1; //预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM7,TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7,ENABLE);
}

void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7,TIM_IT_Update) == SET)
    {
        printf("Speed:%.2f\n",Speed(&MyPID));
    }
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
}
