#ifndef __LCD_H__
#define __LCD_H__

#include "BSP_SDRAM.h"
#include <stdint.h>

// #define LCD_SPI

#define LCD_FRAME_BUF_ADDR 0XC0000000

/* 屏幕可见分辨率（实际显示像素） */
#define LCD_WIDTH                    800   // 水平方向可见像素数
#define LCD_HEIGHT                   480   // 垂直方向可见像素数

/* 每个像素占用字节数 RGB565 = 2字节 */
#define LCD_FB_BYTE_PER_PIXEL        2     // 当前使用 RGB565

#ifdef __cplusplus
extern "C" {
#endif

void lcd_init(void);
void lcd_draw_point(uint16_t x,uint16_t y,uint16_t color);
void lcd_disp_en_config(void);
void lcd_disp_off(void);
void lcd_disp_on(void);

#if defined(__cplusplus)
}
#endif

#endif
