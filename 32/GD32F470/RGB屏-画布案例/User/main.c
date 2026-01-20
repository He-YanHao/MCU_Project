  /********************************************************************************
   * 测试硬件：立创·梁山派开发板GD32F470ZGT6    使用主频200Mhz    晶振25Mhz
   * 版 本 号: V1.0
   * 修改作者: LCKFB
   * 修改日期: 2023年08月28日
   * 功能介绍:      
   ******************************************************************************
   * 梁山派软硬件资料与相关扩展板软硬件资料官网全部开源  
   * 开发板官网：www.lckfb.com   
   * 技术支持常驻论坛，任何技术问题欢迎随时交流学习  
   * 立创论坛：club.szlcsc.com   
   * 其余模块移植手册：https://dri8c0qdfb.feishu.cn/docx/EGRVdxunnohkrNxItYTcrwAnnHe
   * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
   * 不靠卖板赚钱，以培养中国工程师为己任
 *********************************************************************************/

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "bsp_led.h"
#include "sys.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "bsp_basic_timer.h"
#include "string.h"
#include "exmc_sdram.h"
#include "touch.h"
#include "lcd.h"
#include "lcd_ui.h"
#include "stdio.h"

//最大颜色数  目前只设置了14个颜色
#define COLOR_MAX  	14

//像素最大放大倍数
#define PIXEL_SIZE_MAX 	20


typedef  struct button_struct{
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	uint16_t color;
	uint16_t value;
}_widget_object_struct; 

uint16_t color_buf[ COLOR_MAX ] = {GRAYBLUE,BLACK,BLUE,BRED,GRED,GBLUE,RED,MAGENTA,GREEN,YELLOW,CYAN,BROWN,BRRED,GRAY};
_widget_object_struct button_clear, button_color, button_pixel, button_eraser;


void widget_object_init(_widget_object_struct *obj,uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color,uint16_t value);
void ui_init(void);

/************************************************
函数名称 ： main
功    能 ： 主函数
参    数 ： 无
返 回 值 ： 无
作    者 ： LC
*************************************************/
int main(void)
{
	int touch_state = 0;
	char color_num = 0;
	char pixel_size = 0;
	
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组
	
	
	
	systick_config();          					// 滴答定时器初始化
	led_gpio_config();   								// led初始化
	key_gpio_config(); 				 					// key初始化
	usart_gpio_config(9600U);  					// 串口0初始化

	
	exmc_synchronous_dynamic_ram_init(EXMC_SDRAM_DEVICE0);

	// 屏幕初始化
	lcd_disp_config();	
	// 触摸初始化
    FT5206_Init();
	//全屏清屏
	tli_draw_Rectangle(0,0,800,480,WHITE,1);
	
	
	//初始化[清除]按钮的xywh
	widget_object_init(&button_clear,800-130, 480-80, 120, 70, BLUE,NULL);
	//初始化[颜色]按钮的xywh
	widget_object_init(&button_color,10, 480-80, 120, 70, BLUE,NULL);
	//初始化[像素]按钮的xywh
	widget_object_init(&button_pixel,400-60, 480-80, 120, 70, BLUE,pixel_size);
	//初始化[橡皮擦]按钮的xywh
	widget_object_init(&button_eraser,800-130-130, 480-80, 120, 70, BLUE,0);


	//UI显示
	ui_init();	
	
	while(1) 
	{

		//触摸扫描
		touch_state = FT5206_Scan(0);
		//如果屏幕被触摸
		if( touch_state == 1 )
		{
			//[颜色]按钮被按下
			if( tp_dev.x[0] >= button_color.x && tp_dev.x[0] <= (button_color.x+button_color.w) )
			{
				if( tp_dev.y[0] >= button_color.y && tp_dev.y[0] <= (button_color.y+button_color.h) )
				{
					//判断是否松手
					letgo_scan(0,button_color.x,button_color.y,(button_color.x+button_color.w),(button_color.y+button_color.h));
					//改变颜色
					color_num = ( color_num + 1 ) % COLOR_MAX;
					//重新设置[颜色]按钮的背景色
					widget_object_init(&button_color,button_color.x,button_color.y,button_color.w,button_color.h,color_buf[color_num],NULL);
					//UI重新显示
					ui_init();
				}
			}
			//[清除]按钮被按下
			if( tp_dev.x[0] >= button_clear.x && tp_dev.x[0] <= (button_clear.x+button_clear.w) )
			{
				if( tp_dev.y[0] >= button_clear.y && tp_dev.y[0] <= (button_clear.y+button_clear.h) )
				{
					//判断是否松手
					letgo_scan(0,button_clear.x,button_clear.y,(button_clear.x+button_clear.w),(button_clear.y+button_clear.h));
					//清屏为背景色
					tli_draw_Rectangle(0,0,800,480,WHITE,1);
					//UI重新显示
					ui_init();
				}
			}
			//[像素]按钮被按下
			if( tp_dev.x[0] >= button_pixel.x && tp_dev.x[0] <= (button_pixel.x+button_pixel.w) )
			{
				if( tp_dev.y[0] >= button_pixel.y && tp_dev.y[0] <= (button_pixel.y+button_pixel.h) )
				{
					//判断是否松手
					letgo_scan(0,button_pixel.x,button_pixel.y,(button_pixel.x+button_pixel.w),(button_pixel.y+button_pixel.h));
					//像素放大倍数自增
					pixel_size++;
					//如果像素放大倍数大于最大倍数
					if( pixel_size > PIXEL_SIZE_MAX ) pixel_size = 1;
					//重新设置[颜色]按钮的背景色
					widget_object_init(&button_pixel,button_pixel.x,button_pixel.y,button_pixel.w,button_pixel.h,button_pixel.color,pixel_size);
					//UI重新显示
					ui_init();
					
				}
			}			
			
			//[擦除]按钮被按下
			if( tp_dev.x[0] >= button_eraser.x && tp_dev.x[0] <= (button_eraser.x+button_eraser.w) )
			{
				if( tp_dev.y[0] >= button_eraser.y && tp_dev.y[0] <= (button_eraser.y+button_eraser.h) )
				{
					//判断是否松手
					letgo_scan(0,button_eraser.x,button_eraser.y,(button_eraser.x+button_eraser.w),(button_eraser.y+button_eraser.h));
					//修改擦除按钮的状态
					button_eraser.value = !button_eraser.value;
					//如果是擦除状态
					if( button_eraser.value )
					{
						//初始化[橡皮擦]按钮的xywh颜色修改为黑色
						widget_object_init(&button_eraser,800-130-130, 480-80, 120, 70, BLACK,button_eraser.value);
					}
					else
					{
						//初始化[橡皮擦]按钮的xywh颜色修改为蓝色
						widget_object_init(&button_eraser,800-130-130, 480-80, 120, 70, BLUE,button_eraser.value);
					}
					//UI重新显示
					ui_init();					
				}
			}				
			
			//如果不是擦除状态
			if( !button_eraser.value )
			{
				//绘制触摸点
				//根据[颜色]按钮的背景色设置触摸点的颜色
				point_enlargement(tp_dev.x[0],tp_dev.y[0],color_buf[color_num],button_pixel.value);
			}
			else
			{
				//擦除
				point_enlargement(tp_dev.x[0],tp_dev.y[0],WHITE,button_pixel.value);				
			}
		}		
    }
}


/**********************************************************
 * 函 数 名 称：widget_object_init
 * 函 数 功 能：控件对象初始化
 * 传 入 参 数：obj：要初始化的控件
 * 				xywh：控件的(x,y)起点坐标 w宽h高
 * 				color：控件颜色
 * 				value：控件值
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
**********************************************************/
void widget_object_init(_widget_object_struct *obj,uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color,uint16_t value)
{
	obj->x = x;
	obj->y = y;
	obj->h = h;
	obj->w = w;
	obj->color = color;
	obj->value = value;
}


/**********************************************************
 * 函 数 名 称：ui_init
 * 函 数 功 能：UI界面初始化
 * 传 入 参 数：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
**********************************************************/
void ui_init(void)
{
	char temp_buf[20]={0};

	//绘制[清除]按钮
	//绘制一个圆角按钮
	tli_show_button(button_clear.x, button_clear.y, button_clear.w, button_clear.h, 12, button_clear.color);
	//绘制圆角按钮的文本
	tli_show_string(button_clear.x+20, button_clear.y+20, WHITE, button_clear.color,2,"clear",0);

	//绘制[颜色]按钮
	//绘制一个圆角按钮
	tli_show_button(button_color.x, button_color.y, button_color.w, button_color.h, 12,  button_color.color);
	//绘制圆角按钮的文本
	tli_show_string(button_color.x+20, button_color.y+20, WHITE, button_color.color,2,"color",0);

	//绘制[像素]按钮
	//绘制一个圆角按钮
	tli_show_button(button_pixel.x, button_pixel.y, button_pixel.w, button_pixel.h, 12,  button_pixel.color);
	if( button_pixel.value == 0 )
	{
		//绘制圆角按钮的文本
		tli_show_string(button_pixel.x+20, button_pixel.y+20, WHITE, button_pixel.color,2,"pixel",0);
		button_pixel.value = 1;
	}
	else
	{
		//绘制居中显示的字符串
		sprintf(temp_buf, "%d", button_pixel.value );
		tli_show_string(button_pixel.x+(button_pixel.w/2)-(strlen(temp_buf)/2*16), button_pixel.y+20, WHITE, button_pixel.color,2,(uint8_t*)temp_buf,0);
	}
	
	//绘制[擦除]按钮
	//绘制一个圆角按钮
	tli_show_button(button_eraser.x, button_eraser.y, button_eraser.w, button_eraser.h, 12,  button_eraser.color);
	//绘制圆角按钮的文本
	tli_show_string(button_eraser.x+(button_eraser.w/2)-(strlen("eraser")/2*16), button_eraser.y+20, WHITE, button_eraser.color,2,"eraser",0);
}
