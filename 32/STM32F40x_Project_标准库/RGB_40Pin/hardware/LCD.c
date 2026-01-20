#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "LCD.h"
#include "LCD_font.h"

//写入数据低位GPIOA
void LCD_GPIOACmd(uint16_t data)//R
{
    GPIO_Write(GPIOA, GPIO_ReadOutputData(GPIOA)|data);
}

//写入数据高位GPIOD
void LCD_GPIODCmd(uint16_t data)//G
{
    GPIO_Write(GPIOD, GPIO_ReadOutputData(GPIOD)|(data<<8));
}

//写入数据高位GPIOE
void LCD_GPIOECmd(uint16_t data)//B
{
    GPIO_Write(GPIOE, GPIO_ReadOutputData(GPIOE)|(data<<8));
}

void LCD_RGBCmd(uint32_t data)
{
    LCD_GPIOECmd(data&0X000000FF);
    LCD_GPIODCmd((data&0X0000FF00)>>8);
    LCD_GPIOACmd((data&0X00FF0000)>>16);
}

void LCD_GPIO_Init(void)//GPIO初始化
{
    //开启 GPIOA GPIOD GPIOE 时钟
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);
    //结构体
	GPIO_InitTypeDef GPIO_InitStructure;
    //GPIOA初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//无所谓
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//最快速    //GPIO_Speed_2MHz   GPIO_Speed_25MHz    GPIO_Speed_50MHz    GPIO_Speed_100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIOD初始化
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//无所谓
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//最快速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//推挽
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    //GPIOE初始化
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//无所谓
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//最快速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//推挽
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    LCD_VSYNC_1;//帧同步 默认
    LCD_HSYNC_1;//行同步 默认
    LCD_DE_0;//数据失能 默认
    LCD_CLK_0;//时钟低 默认
    LCD_DISP_1;//显示使能
    LCD_RGBCmd(0XFF0000);
}

void ceshi()
{
    uint16_t zhen = 0, hang = 0, lie = 0;
    for(zhen = 0; zhen < 999; zhen++)//
    {
        LCD_VSYNC_1;//帧同步
        delay_us(100);
        LCD_VSYNC_0;//帧同步
        delay_us(100);
            for(lie = 0; lie < 100; lie++)//
            {
                LCD_HSYNC_1;//行同步
                delay_us(100);
                LCD_HSYNC_0;//行同步
                delay_us(100);
                LCD_DE_1;//数据使能
                delay_us(100);
                    for(hang = 0; hang < 200; hang++)//
                    {
                        LCD_CLK_1;//时钟高
                        delay_us(100);
                        LCD_CLK_0;//时钟低
                        delay_us(100);
                    }
                LCD_DE_0;//数据失能
                delay_us(100);
            }
     }
}

#define YS0 delay_us(10)
#define YS1 delay_us(20)
#define YS2 delay_us(40)

void ceshi2()
{
    uint16_t hang = 0, lie = 0;
    uint32_t RGB = 0, zhen = 0;
    
    LCD_VSYNC_1;//帧同步 默认
    LCD_HSYNC_1;//行同步 默认
    LCD_DE_0;//数据失能 默认
    LCD_CLK_0;//时钟低 默认
    LCD_DISP_1;//显示使能
    
    for(zhen = 0; zhen < 0xFFFFFFFE; zhen++)//
    {
        LCD_VSYNC_0;//帧同步
        YS2;
        LCD_VSYNC_1;//帧同步
        YS2;
            for(lie = 0; lie < 100; lie++)//
            {
                LCD_HSYNC_0;//行同步
                YS1;
                LCD_HSYNC_1;//行同步
                YS1;
                LCD_DE_1;//数据使能
                YS1;
                    for(hang = 0; hang < 200; hang++)//
                    {
                        LCD_RGBCmd(RGB);
                        RGB++;
                        LCD_CLK_1;//时钟高
                        YS0;
                        LCD_CLK_0;//时钟低
                        YS0;
                    }
                LCD_DE_0;//数据失能
                YS1;
            }
    if(RGB > 0XFFFFFF)
    {
        RGB = 0;
    }
    }
}
