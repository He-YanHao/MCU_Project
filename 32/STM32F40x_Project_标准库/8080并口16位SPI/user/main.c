#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "40Pin_LCD.h"
void LED_CS()
{
    GPIO_InitTypeDef GPIO_InitStructure;

 	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//无所谓
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最快速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//无上拉无下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

#define LCD_LED_ON	GPIO_SetBits(GPIOB,GPIO_Pin_2)//置为高电平
#define LCD_LED_OFF	GPIO_ResetBits(GPIOB,GPIO_Pin_2)//置为低电平

int main()
{
    board_init();
	LCD_Init();
	LED_CS();//LED测试
	LCD_Display_Dir(USE_LCM_DIR);//屏幕方向
	LCD_Clear(0x1F);//0000 0000 0001 1111
	delay_ms(1000);
	LCD_Clear(0xFFE0);//1111 1111 1110 0000
	LCD_LED_ON;

    while(1)
    {
    LCD_LED_OFF;
	delay_ms(1000);
	LCD_LED_ON;
    }
}
