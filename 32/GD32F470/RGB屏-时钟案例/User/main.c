  /********************************************************************************
   * 测试硬件：立创·梁山派开发板GD32F470ZGT6    使用主频200Mhz    晶振25Mhz
   * 版 本 号: V1.0
   * 修改作者: LCKFB
   * 修改日期: 2023年08月28日
   * 功能介绍: 简易桌面时钟，可通过串口调时；可调整时钟界面背景色     
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
#include "bsp_rtc.h"
#include "bsp_w25q64.h"
#include "sdcard.h"
#include "pic.h"

//最大颜色数  目前只设置了14个颜色
#define COLOR_MAX  	14

//像素最大放大倍数
#define PIXEL_SIZE_MAX 	20

//边框流水灯速度 越大越慢
#define FLOW_LAMP_SPEED			40

//默认背景色
uint16_t BACKGROUND_COLOR=BLACK;

//控件对象数据
typedef  struct button_struct{
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	uint16_t color;
	uint16_t value;
	uint8_t time[20];
}_widget_object_struct; 
//内存设备状态
typedef struct device_struct{
	uint8_t spi_flash_flag;
	uint8_t sdram_flag;
	uint8_t sdcard_flag;
}device_state_struct;

device_state_struct  device_state;

//背景颜色数组
uint16_t color_buf[ COLOR_MAX ] = {BLACK,GRAYBLUE,BLUE,BRED,GRED,GBLUE,RED,MAGENTA,GREEN,YELLOW,CYAN,BROWN,BRRED,GRAY};

//界面控件类结构体
_widget_object_struct widget_clock, button_color;

//RTC时钟类结构体
rtc_parameter_struct   rtc_initpara_time;

char color_num = 0;
	


void widget_object_init(_widget_object_struct *obj,uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color,uint16_t value,uint8_t* time);
void ui_init(void);
void init_test(void);


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
	int outtime = 0;
	char temp_buf[20]={0};
	int again_time_sec = 0;
	int again_time_min = 0;
	
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组
	
	systick_config();          					// 滴答定时器初始化
	led_gpio_config();   								// led初始化
	key_gpio_config(); 				 					// key初始化
	
	// 串口0初始化
	usart_gpio_config(9600U);  					

	//SDRAM初始化
	exmc_synchronous_dynamic_ram_init(EXMC_SDRAM_DEVICE0);

	//RTC初始化
	rtc_config();
	
	//w25q64初始化
	w25q64_init_config();
	
	// 屏幕初始化
	lcd_disp_config();	
	
	// 触摸初始化
    FT5206_Init();
	//全屏清屏
	tli_draw_Rectangle(0,0,800,480,BLACK,1);
	
	//初始化[时钟]参数
	widget_object_init(&widget_clock,18,10,800-36,NULL,WHITE,20,widget_clock.time);
	//初始化[颜色]按钮的xywh
	widget_object_init(&button_color,20, 480-90, 120, 70, BLUE,NULL,NULL);

	//板载资源测试
	init_test();

	//UI显示
	ui_init();	
	
	while(1) 
	{
		//获取RTC时间
		rtc_current_time_get(&rtc_initpara_time);  

		//如果分有更新
		if( again_time_min  != rtc_initpara_time.minute )
		{
			again_time_min = rtc_initpara_time.minute;
			//显示时分
			sprintf((char*)widget_clock.time,"%02x:%02x",rtc_initpara_time.hour,rtc_initpara_time.minute);
			tli_show_string(widget_clock.x,widget_clock.y,widget_clock.color,BACKGROUND_COLOR,widget_clock.value,widget_clock.time,0);			
			
			//显示年月日周
			sprintf((char*)widget_clock.time,"20%02x-%02x-%02x-%02x",rtc_initpara_time.year,rtc_initpara_time.month,rtc_initpara_time.date,rtc_initpara_time.day_of_week);
			tli_show_string(widget_clock.x,widget_clock.y+300,widget_clock.color,BACKGROUND_COLOR,4,widget_clock.time,0);	

		}
		
		//如果秒有更新
		if( again_time_sec != rtc_initpara_time.second )
		{
			again_time_sec = rtc_initpara_time.second;  
			//实现秒
			sprintf(temp_buf, "%02x",rtc_initpara_time.second);
			tli_show_string(545,widget_clock.y+270,widget_clock.color,BACKGROUND_COLOR,widget_clock.value-6,(uint8_t*)temp_buf,0);
		}

		//边框流水灯
		outtime++;
		if( outtime >= FLOW_LAMP_SPEED )
		{
			outtime = 0;
			frame_flow_light(BACKGROUND_COLOR);
		}
		
		//板载LED流水灯
		led_waterfall_light();
		
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
					//重新设置[颜色]按钮的背景色为下一个颜色
					widget_object_init(&button_color,button_color.x,button_color.y,button_color.w,button_color.h,color_buf[color_num+1%COLOR_MAX],NULL,NULL);
					//修改全屏背景色
					BACKGROUND_COLOR = color_buf[color_num];
					//UI重新显示
					ui_init();
				}
			}
		}		
		
		//串口调时,要求调时格式为  TIME=YYYY-MM-DD-K hh:mm:ss
		if( Serial_set_time( &rtc_initpara_time.year, &rtc_initpara_time.month, &rtc_initpara_time.date, \
			&rtc_initpara_time.day_of_week, &rtc_initpara_time.hour, &rtc_initpara_time.minute, &rtc_initpara_time.second )  
		  )
		{
			//等到时钟同步后，再进行寄存器更新
			while( ERROR == rtc_register_sync_wait() );
			RtcTimeConfig(rtc_initpara_time.year, rtc_initpara_time.month, rtc_initpara_time.date, \
			rtc_initpara_time.day_of_week, rtc_initpara_time.hour, rtc_initpara_time.minute, rtc_initpara_time.second );
		}
    }
}


/* 初始化测试 */
void init_test(void)
{
    uint16_t flash_id; // flash 的 id
    sd_error_enum sd_error;  // sd 状态
    uint16_t timeout = 5;    // sd 超时

    printf("Comprehensive test example!\r\n\r\n"); // 测试串口0
    tli_show_string(0, 0,RED, BLACK, 2, "Comprehensive test!", 0);
	
    /* W25Q64 */
    tli_show_string(0, 32, RED, BLACK,2, "===================",0);
	
    printf("W25Q64 init start!\n"); // 测试flash
    tli_show_string(0, 32*2, RED, BLACK,2, "W25Q64 init start!",0);
    flash_id = W25Q64_readID(); // 读取flash id
    if ( flash_id == W25Q64 )
    {
        printf("W25Q64 init ok!\r\n"); // 测试ok
		tli_show_string(0, 32*3, RED, BLACK,2, "W25Q64 init ok!",0);
		device_state.spi_flash_flag = 1;
    }
    else
    {
        printf("W25Q64 init error!\r\n"); // 测试error
		tli_show_string(0, 32*3, RED, BLACK,2, "W25Q64 init error!",0);
		device_state.spi_flash_flag = 0;
    }
    printf("\r\n"); // 换行

	tli_show_string(0,  32 * 4, RED, BLACK,2, "===================",0);
	
	
    /* config the EXMC access mode */
    printf("SDRAM init start!\n"); // 测试SDRAM
    tli_show_string(0,  32 * 5, RED, BLACK,2, "SDRAM init start!",0);
	
    if (sdram_test_date1())
    {
        printf("SDRAM init ok!\r\n"); // 测试ok
		tli_show_string(0,  32 * 6, RED, BLACK,2, "SDRAM init ok!",0);
		tli_draw_line(0,0,800,0,BLACK);
		device_state.sdram_flag = 1;

    }
    else
    {
        printf("SDRAM init error!\r\n"); // 测试error
		tli_show_string(0,  32 * 6, RED, BLACK,2, "SDRAM init error!",0);
		tli_draw_line(0,0,800,0,BLACK);
		device_state.sdram_flag = 0;
    }
    printf("\r\n"); // 换行

	tli_show_string(0, 32 * 7, RED, BLACK,2, "===================",0);
	
    /* SD CARD */
    printf("SD CARD init start!\n"); // 测试SDIO
	tli_show_string(0,  32 * 8, RED, BLACK,2, "SD CARD init start!",0);
    do
    {
        /* initialize the card, get the card information and configurate the bus mode and transfer mode */
        sd_error = sd_config();       
    } while ((SD_OK != sd_error) && (--timeout));

    if (timeout)
    {
        printf("SD CARD init ok!\r\n"); // 测试ok
		tli_show_string(0,  32 * 9, RED, BLACK,2, "SD CARD init ok!",0);
		device_state.sdcard_flag = 1;
    }
    else
    {
        printf("SD CARD init error!\r\n"); // 测试error
		tli_show_string(0,  32 * 9, RED, BLACK,2, "SD CARD init error!",0);
		device_state.sdcard_flag = 0;
    }

    /* get the information of the card and print it out by USART */
    // card_info_get();
    printf("\r\n"); // 换行

    printf("\r\nComprehensive test complete!\n");
	tli_show_string(0, 32 * 10, RED, BLACK,2, "Test complete!",0);

	tli_show_string(0, 32 * 11, RED, BLACK,2, "===================",0);

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
void widget_object_init(_widget_object_struct *obj,uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color,uint16_t value,uint8_t* time)
{
	obj->x = x;
	obj->y = y;
	obj->h = h;
	obj->w = w;
	obj->color = color;
	obj->value = value;
	sprintf((char*)obj->time, "%s", time );
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

	//全屏清屏
	tli_draw_Rectangle(0,0,800,480,color_buf[color_num],1);
	
	//获取RTC时间
	rtc_current_time_get(&rtc_initpara_time);  

	//显示时分
	sprintf((char*)widget_clock.time,"%02x:%02x",rtc_initpara_time.hour,rtc_initpara_time.minute);
	tli_show_string(widget_clock.x,widget_clock.y,widget_clock.color,BACKGROUND_COLOR,widget_clock.value,widget_clock.time,0);	
	
	//显示秒
	sprintf(temp_buf, "%02x",rtc_initpara_time.second);
	tli_show_string(545,widget_clock.y+270, widget_clock.color,BACKGROUND_COLOR,widget_clock.value-6,(uint8_t*)temp_buf,0);

	//显示年月日周
	sprintf((char*)widget_clock.time,"20%02x-%02x-%02x-%02x",rtc_initpara_time.year,rtc_initpara_time.month,rtc_initpara_time.date,rtc_initpara_time.day_of_week);
	tli_show_string(widget_clock.x,widget_clock.y+300,widget_clock.color,BACKGROUND_COLOR,4,widget_clock.time,0);	

	
	//绘制[背景色]按钮
	//绘制一个圆角按钮
	tli_show_button(button_color.x, button_color.y, button_color.w, button_color.h, 12,  button_color.color);
	//绘制圆角按钮的文本
	tli_show_string(button_color.x+30, button_color.y+20, WHITE, button_color.color,2,(uint8_t*)"back",0);

	//绘制串口调时提示文本 
	tli_show_string(button_color.x+button_color.w+20, button_color.y+10, WHITE, BACKGROUND_COLOR,1,(uint8_t*)"UART set time format(Baud rate 9600):",0);	
	tli_show_string(button_color.x+button_color.w+20, button_color.y+10+32, WHITE, BACKGROUND_COLOR,1,(uint8_t*)"TIME=YYYY-MM-DD-K hh:mm:ss",0);	

	//显示flash状态
	if( device_state.spi_flash_flag == 0 )
	{ 
		tli_show_string(button_color.x+button_color.w+20, button_color.y-15, WHITE, RED,1,(uint8_t*)" FLASH X ",0);	
	}
	else
	{
		tli_show_string(button_color.x+button_color.w+20, button_color.y-15, BLACK, GREEN,1,(uint8_t*)" FLASH ",0);	
	}
	//显示sdram状态
	if( device_state.sdram_flag == 0 )
	{
		tli_show_string(button_color.x+button_color.w+100, button_color.y-15, WHITE, RED,1,(uint8_t*)" SDRAM X ",0);	
	}
	else
	{
		tli_show_string(button_color.x+button_color.w+100, button_color.y-15, BLACK, GREEN,1,(uint8_t*)" SDRAM ",0);	
	}	
	
	//显示sdcard状态
	if( device_state.sdcard_flag == 0 )
	{
		tli_show_string(button_color.x+button_color.w+180, button_color.y-15, WHITE, RED,1,(uint8_t*)" SDCARD X ",0);	
	}
	else
	{
		tli_show_string(button_color.x+button_color.w+180, button_color.y-15, BLACK, GREEN,1,(uint8_t*)" SDCARD ",0);	
	}		
}
