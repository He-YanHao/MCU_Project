#include "stm32f4xx.h"                  // Device header
#include "HC595_LED.h"
#include "delay.h"

uint8_t HC595_LED_Num[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

void HC595_LED_init(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void HC595_LED_I2C(uint8_t X)
{
	uint8_t i = 0;
	LED_DIO_SET;//DIO高
	LED_SCLK_SET;//SCLK高
	for(i = 0; i < 8; i++)
	{
		if(X & 0x80)
		{
			LED_DIO_SET;//DIO高
		}
		else
		{
			LED_DIO_CLR;//DIO低
		}
		X<<=1;
		LED_SCLK_CLR;//SCLK低
		LED_SCLK_SET;//SCLK高
	}
}

void LED_XIAN (uint8_t X, uint8_t Y)
{
	HC595_LED_I2C(HC595_LED_Num[Y]);
	HC595_LED_I2C(0x01<<(X-1));
	LED_RCLK_CLR;//RCLK低
	LED_RCLK_SET;//RCLK高
}

void LED_Display (uint16_t Num)
{
//	1234
	
	uint16_t Num4 = Num/1000;
	uint16_t Num3 = (Num-Num%100)/100;
	uint16_t Num2 = (Num-Num%10)/10;
	uint16_t Num1 = Num%10;
	
	LED_XIAN(Num4,4);
	LED_XIAN(Num3,3);
	LED_XIAN(Num2,2);
	LED_XIAN(Num1,1);
}
