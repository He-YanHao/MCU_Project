#include "MyPWM.h"

void TIMER1_GPIO_PWM_Init(void)
{
    //开启时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    //A5初始化 TIMER1_CH0
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_5);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_5);
    //A1初始化 TIMER1_CH1
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_1);
    //A2初始化 TIMER1_CH2
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);
    //A3初始化 TIMER1_CH3
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);
}

void MyPWM_Init(void)
{
    TIMER1_GPIO_PWM_Init();
    
    rcu_periph_clock_enable(RCU_TIMER1);//使能时钟
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//配置TIMER时钟源
    timer_deinit(TIMER1);//缺省值初始化
    timer_parameter_struct timer_initpara;//结构体
    timer_struct_para_init(&timer_initpara);//默认值

    timer_initpara.prescaler = 200 - 1;//预分频值（0~65535）
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;//对齐模式
    timer_initpara.counterdirection = TIMER_COUNTER_UP;//计数方向
    timer_initpara.clockdivision = TIMER_CKDIV_DIV2;//时钟分频因子
    timer_initpara.period = 4095 - 1;//周期（0~65535） 自动重载值
    timer_initpara.repetitioncounter = 1;//重复计数器值（0~255）
    timer_init(TIMER1,&timer_initpara);

    timer_oc_parameter_struct timer_ocintpara;
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;//通道输出极性
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;//通道输出状态
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;//互补通道输出极性
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;//互补通道输出状态
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;//空闲状态下通道输出
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;//空闲状态下互补通道输出
    //初始化四个通道
    timer_channel_output_config(TIMER1,TIMER_CH_0,&timer_ocintpara);
    timer_channel_output_config(TIMER1,TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMER1,TIMER_CH_2,&timer_ocintpara);
    timer_channel_output_config(TIMER1,TIMER_CH_3,&timer_ocintpara);
    //配置通道TIMER1_CH0
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,1024-1);              //输出比较寄存器
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);         //配置外设TIMERx通道输出比较模式
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);  //配置TIMERx通道输出比较影子寄存器功能
    //配置通道TIMER1_CH1
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,2048-1);              //输出比较寄存器
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);         //配置外设TIMERx通道输出比较模式
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);  //配置TIMERx通道输出比较影子寄存器功能
    //配置通道TIMER1_CH2
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,3096-1);              //输出比较寄存器
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);         //配置外设TIMERx通道输出比较模式
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);  //配置TIMERx通道输出比较影子寄存器功能
    //配置通道TIMER1_CH3
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,4095-1);              //输出比较寄存器
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);         //配置外设TIMERx通道输出比较模式
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);  //配置TIMERx通道输出比较影子寄存器功能

    timer_primary_output_config(TIMER1, ENABLE);//所有的通道输出使能
    timer_dma_enable(TIMER1,TIMER_DMA_UPD);
    timer_auto_reload_shadow_enable(TIMER1);//TIMERx自动重载影子使能
    timer_enable(TIMER1);//使能定时器
}

void Change_PWM0(uint32_t PWM)
{
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,PWM);//输出比较寄存器
}
void Change_PWM1(uint32_t PWM)
{
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_1,PWM);//输出比较寄存器
}
void Change_PWM2(uint32_t PWM)
{
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_2,PWM);//输出比较寄存器
}
void Change_PWM3(uint32_t PWM)
{
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_3,PWM);//输出比较寄存器
}
