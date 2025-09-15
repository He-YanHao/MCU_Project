#include "MyPWM.h"

void MyPWM_Init(void)
{
    //����ʱ��
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
    
    //��ʱ����ʼ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 4095 - 1;  //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 20 - 1; //Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷ�Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // ��ʼ������PWMͨ��
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // ͨ��1��ʼ��
    TIM_OCInitStructure.TIM_Pulse = 1024;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // ͨ��2��ʼ��
    TIM_OCInitStructure.TIM_Pulse = 2048; // ��ʼռ�ձ�Ϊ0
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    // ͨ��3��ʼ��
    TIM_OCInitStructure.TIM_Pulse = 3072; // ��ʼռ�ձ�Ϊ0
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);//ʹ��ARRԤװ��
    TIM_Cmd(TIM3, ENABLE);//ʹ�ܶ�ʱ��
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
