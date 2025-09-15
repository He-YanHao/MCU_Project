#include "MyPWM.h"

void TIMER1_GPIO_PWM_Init(void)
{
    //����ʱ��
    rcu_periph_clock_enable(RCU_GPIOA);
    //A5��ʼ�� TIMER1_CH0
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_5);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_5);
    //A1��ʼ�� TIMER1_CH1
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_1);
    //A2��ʼ�� TIMER1_CH2
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);
    //A3��ʼ�� TIMER1_CH3
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);
}

void MyPWM_Init(void)
{
    TIMER1_GPIO_PWM_Init();
    
    rcu_periph_clock_enable(RCU_TIMER1);//ʹ��ʱ��
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//����TIMERʱ��Դ
    timer_deinit(TIMER1);//ȱʡֵ��ʼ��
    timer_parameter_struct timer_initpara;//�ṹ��
    timer_struct_para_init(&timer_initpara);//Ĭ��ֵ

    timer_initpara.prescaler = 200 - 1;//Ԥ��Ƶֵ��0~65535��
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;//����ģʽ
    timer_initpara.counterdirection = TIMER_COUNTER_UP;//��������
    timer_initpara.clockdivision = TIMER_CKDIV_DIV2;//ʱ�ӷ�Ƶ����
    timer_initpara.period = 4095 - 1;//���ڣ�0~65535�� �Զ�����ֵ
    timer_initpara.repetitioncounter = 1;//�ظ�������ֵ��0~255��
    timer_init(TIMER1,&timer_initpara);

    timer_oc_parameter_struct timer_ocintpara;
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;//ͨ���������
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;//ͨ�����״̬
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;//����ͨ���������
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;//����ͨ�����״̬
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;//����״̬��ͨ�����
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;//����״̬�»���ͨ�����
    //��ʼ���ĸ�ͨ��
    timer_channel_output_config(TIMER1,TIMER_CH_0,&timer_ocintpara);
    timer_channel_output_config(TIMER1,TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMER1,TIMER_CH_2,&timer_ocintpara);
    timer_channel_output_config(TIMER1,TIMER_CH_3,&timer_ocintpara);
    //����ͨ��TIMER1_CH0
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,1024-1);              //����ȽϼĴ���
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);         //��������TIMERxͨ������Ƚ�ģʽ
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);  //����TIMERxͨ������Ƚ�Ӱ�ӼĴ�������
    //����ͨ��TIMER1_CH1
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,2048-1);              //����ȽϼĴ���
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);         //��������TIMERxͨ������Ƚ�ģʽ
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);  //����TIMERxͨ������Ƚ�Ӱ�ӼĴ�������
    //����ͨ��TIMER1_CH2
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,3096-1);              //����ȽϼĴ���
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);         //��������TIMERxͨ������Ƚ�ģʽ
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);  //����TIMERxͨ������Ƚ�Ӱ�ӼĴ�������
    //����ͨ��TIMER1_CH3
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,4095-1);              //����ȽϼĴ���
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);         //��������TIMERxͨ������Ƚ�ģʽ
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);  //����TIMERxͨ������Ƚ�Ӱ�ӼĴ�������

    timer_primary_output_config(TIMER1, ENABLE);//���е�ͨ�����ʹ��
    timer_dma_enable(TIMER1,TIMER_DMA_UPD);
    timer_auto_reload_shadow_enable(TIMER1);//TIMERx�Զ�����Ӱ��ʹ��
    timer_enable(TIMER1);//ʹ�ܶ�ʱ��
}

void Change_PWM0(uint32_t PWM)
{
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,PWM);//����ȽϼĴ���
}
void Change_PWM1(uint32_t PWM)
{
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_1,PWM);//����ȽϼĴ���
}
void Change_PWM2(uint32_t PWM)
{
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_2,PWM);//����ȽϼĴ���
}
void Change_PWM3(uint32_t PWM)
{
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_3,PWM);//����ȽϼĴ���
}
