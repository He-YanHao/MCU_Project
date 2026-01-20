#include "stm32f4xx.h"                  // Device header

/**
  * 函    数：AD初始化
  * 参    数：无
  * 返 回 值：无
  */
void MyADC_Init(void)
{
	/*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//开启ADC1的时钟
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE); // 使能时钟
	/*GPIO初始化*/
    GPIO_InitTypeDef  GPIO_InitStructure; // 配置结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;    //模拟输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIO

    ADC_DeInit();//ADC复位
	ADC_CommonInitTypeDef ADC_CommonInitStruct;//ADC公共配置
    ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div4;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);

    ADC_InitTypeDef ADC_InitStruct;//ADC特定配置
    ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
    ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfConversion=1;
    ADC_Init(ADC1, &ADC_InitStruct);

	/*ADC使能*/
	ADC_Cmd(ADC1, ENABLE);									//使能ADC1，ADC开始运行
}

/**********************************************************
 * 函 数 名 称：Get_ADC_Value
 * 函 数 功 能：读取ADC值
 * 传 入 参 数：ADC_CHANNEL_x=要采集的通道
 * 函 数 返 回：测量到的值
 * 作       者：LC
 * 备       注：默认采样周期为15个ADC采样时间
**********************************************************/
uint16_t Get_ADC_Value(uint8_t  ADC_CHANNEL_x)
{
    unsigned int adc_value = 0;
    //设置采集通道ADC_Channel_5
    ADC_RegularChannelConfig( ADC1, ADC_CHANNEL_x, 1, ADC_SampleTime_480Cycles );
    //设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
    //开始软件转换
    ADC_SoftwareStartConv(ADC1);//使能或者失能指定的 ADC 的软件转换启动功能
    //等待转换结束
    while(!(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC))); //检查制定 ADC 标志位置1与否
    //读取采样值
    adc_value = ADC_GetConversionValue(ADC1);//返回最近一次ADCx规则组的转换结果
    //返回采样值
    return adc_value;
}
