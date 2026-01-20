#ifndef __LCD_H
#define __LCD_H

//颜色的宏定义
#define RED  	0xf800  //红色
#define GREEN	0x07e0  //绿色
#define BLUE 	0x001f  //蓝色
#define WHITE	0xffff  //白色
#define BLACK	0x0000  //黑色
#define YELLOW  0xFFE0  //黄色

void LCD_GPIOACmd(u16 data);//写入数据低位GPIOA
void LCD_GPIODCmd(u16 data);//写入数据高位GPIOD
void LCD_GPIOECmd(u16 data);//写入数据高位GPIOE
void LCD_RGBCmd(uint32_t data);//写入RGB颜色值
void LCD_GPIO_Init(void);//GPIO初始化
void ceshi(void);
void ceshi2(void);

//各种线的宏定义
#define LCD_VSYNC_1  GPIO_SetBits(GPIOD,GPIO_Pin_0)//帧同步使能
#define LCD_VSYNC_0  GPIO_ResetBits(GPIOD,GPIO_Pin_0)//帧同步失能
#define LCD_HSYNC_1  GPIO_SetBits(GPIOD,GPIO_Pin_1)//行同步使能
#define LCD_HSYNC_0  GPIO_ResetBits(GPIOD,GPIO_Pin_1)//行同步失能
#define LCD_DE_1  GPIO_SetBits(GPIOD,GPIO_Pin_4)//数据有效使能
#define LCD_DE_0  GPIO_ResetBits(GPIOD,GPIO_Pin_4)//数据有效失能
#define LCD_CLK_1  GPIO_SetBits(GPIOD,GPIO_Pin_5)//时钟使能
#define LCD_CLK_0  GPIO_ResetBits(GPIOD,GPIO_Pin_5)//时钟失能
//#define LCD_RST_1 GPIO_SetBits(GPIOD,GPIO_Pin_6)
//#define LCD_RST_0 GPIO_ResetBits(GPIOD,GPIO_Pin_6)
#define LCD_DISP_1  GPIO_SetBits(GPIOD,GPIO_Pin_7)//显示使能
#define LCD_DISP_0  GPIO_ResetBits(GPIOD,GPIO_Pin_7)//显示失能

#endif
