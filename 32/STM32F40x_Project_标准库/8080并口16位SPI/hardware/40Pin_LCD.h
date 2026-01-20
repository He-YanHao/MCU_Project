#ifndef __40PIN_LCD_H
#define __40PIN_LCD_H

#include <stm32f4xx.h>
#include "delay.h"

void LCD_GPIO_Init(void);//GPIO初始化
void DATAOUT(uint16_t data);//GPIO输出数据
void LCD_WR_REG(u16 data);//写寄存器函
void LCD_WR_DATA(u16 data);//写数据函数
void LCD_WriteRAM(uint16_t RGB_Code);//LCD写入RAM颜色值
void LCD_WriteRAM_Prepare(void);//LCD写入GRAM值
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue);//写入寄存器数值
void opt_delay(u8 i);//延时i
void LCD_Init(void);//初始化lcd
void LCD_DisplayOn(void);//LCD开启显示
void LCD_DisplayOff(void);//LCD关闭显示
void LCD_Display_Dir(u8 dir);//方向选择
void LCD_SetCursor(u16 Xpos, u16 Ypos);//设置光标位置
void LCD_DrawPoint(u16 x,u16 y);//画点
void LCD_Clear(u16 color);//清屏函数

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16 setxcmd;		//设置x坐标指令
	u16 setycmd;		//设置y坐标指令	 
}_lcd_dev;

//LCD参数
extern _lcd_dev lcddev;//管理LCD重要参数

#define USE_LCM_DIR	0	//定义液晶屏顺时针旋转方向 	0-0度旋转，1-180度旋转，2-270度旋转，3-90度旋转
#define USE_TP_TYPE	RTP	//定义触摸类型	CTP = 电容模式, RTP = 电阻模式

#define CTP 0X80		//电容模式
#define RTP 0X00		//电阻模式

//画笔颜色
#define WHITE	0xFFFF
#define BLACK	0x0000	  
#define BLUE	0x001F  
#define BRED	0XF81F
#define GRED	0XFFE0
#define GBLUE	0X07FF
#define RED		0xF800
#define MAGENTA	0xF81F
#define GREEN	0x07E0
#define CYAN	0x7FFF
#define YELLOW	0xFFE0
#define BROWN	0XBC40 //棕色
#define BRRED	0XFC07 //棕红色
#define GRAY	0X8430 //灰色

//LCD的画笔颜色和背景色	   
extern u16	POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色 
#define LGRAY			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//屏幕像素240x320
//CS		片选		低电平使能
//RS		地址|数据	0为地址 1为数据
//WR		写信号		低电平使能
//RD		读信号		低电平使能
//RST		复位		低电平复位
//DB0~15	数据		传输数据，读或写
//BL		背光		PWM调节亮度
//电阻触屏	标准SPI通讯
//TSDO	电阻屏输出信号
//TSDI	电阻屏输入信号
//TCS	片选信号
//TCLK	时钟信号
//TPEN	中断

#define LCD_CS_SET	GPIO_SetBits(GPIOB,GPIO_Pin_10)//置为高电平
#define LCD_CS_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_10)//置为低电平
#define LCD_RS_SET	GPIO_SetBits(GPIOB,GPIO_Pin_11)//置为高电平
#define LCD_RS_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_11)//置为低电平
#define LCD_WR_SET	GPIO_SetBits(GPIOB,GPIO_Pin_12)//置为高电平
#define LCD_WR_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_12)//置为低电平
#define LCD_RD_SET	GPIO_SetBits(GPIOB,GPIO_Pin_13)//置为高电平
#define LCD_RD_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_13)//置为低电平
#define LCD_RST_SET	GPIO_SetBits(GPIOB,GPIO_Pin_14)//置为高电平
#define LCD_RST_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_14)//置为低电平
#define LCD_BL_SET	GPIO_SetBits(GPIOB,GPIO_Pin_15)//置为高电平
#define LCD_BL_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_15)//置为低电平

#endif
