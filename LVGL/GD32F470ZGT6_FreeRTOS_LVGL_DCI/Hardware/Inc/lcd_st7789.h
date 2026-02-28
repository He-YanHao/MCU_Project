#ifndef __LCD_ST7789_H__
#define __LCD_ST7789_H__

#include <stdint.h>

#define LCD_WIDTH   320
#define LCD_HEIGHT  240

// PD6  --> LCD_BL
// PG10 --> LCD_CS
// PE3  --> LCD_RS
// PG12 --> LCD_WR
// PG13 --> LCD_RD
// PG14 --> LCD_RST

//LCD重要参数集
typedef struct  
{										    
	uint16_t width;   //LCD 宽度
	uint16_t height;  //LCD 高度
	uint16_t id;      //LCD ID
	uint8_t  dir;     //横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t wramcmd; //开始写gram指令
	uint16_t setxcmd; //设置x坐标指令
	uint16_t setycmd; //设置y坐标指令	 
}_lcd_dev;

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   

extern uint16_t BACK_COLOR; //背景颜色.默认为白色
extern uint16_t POINT_COLOR;

// 初始化
void lcd_init(void);
// 打点
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
//快速画点
void LCD_Fast_DrawPoint(uint16_t x, uint16_t y, uint16_t color);

//LCD 开关显示
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
// 设置屏幕方向
void LCD_Display_Dir(uint8_t dir);
// 清屏
void LCD_Clear(uint16_t color);
// 填色
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);
void LCD_Fill_Windows(uint16_t *color);

#endif
