#ifndef __LCD_TLI_H__
#define __LCD_TLI_H__

#include <stdint.h>
#include "BSP_SDRAM.h"

#define LCD_FRAME_BUF_ADDR 0XC0000000

/* 屏幕可见分辨率（实际显示像素） */
#define LCD_WIDTH                    800   // 水平方向可见像素数
#define LCD_HEIGHT                   480   // 垂直方向可见像素数

/* 每个像素占用字节数 RGB565 = 2字节 */
#define LCD_FB_BYTE_PER_PIXEL        2     // 当前使用 RGB565

/* HSYNC 脉冲宽度 一行开始时，HSYNC 信号保持有效的像素时钟个数 */
#define HORIZONTAL_SYNCHRONOUS_PULSE 10
/* 行后沿（Back Porch） HSYNC 结束后，到真正显示数据开始前的空白周期 */
#define HORIZONTAL_BACK_PORCH        150

/* 有效显示宽度（真正显示的像素数） */
#define ACTIVE_WIDTH                 800
/* 行前沿（Front Porch） 一行显示结束后，到下一次 HSYNC 之前的空白周期 */
#define HORIZONTAL_FRONT_PORCH       15

/* VSYNC 脉冲宽度 一帧开始时，VSYNC 信号保持有效的“行数” */
#define VERTICAL_SYNCHRONOUS_PULSE   10

/* 帧后沿（Back Porch） VSYNC 结束后，到真正开始显示第一行之前的空白行数 */
#define VERTICAL_BACK_PORCH          140

/* 有效显示高度（真正显示的行数） */
#define ACTIVE_HEIGHT                480

/* 帧前沿（Front Porch） 最后一行显示结束后，到下一次 VSYNC 之前的空白行数 */
#define VERTICAL_FRONT_PORCH         40


#ifdef __cplusplus
extern "C" {
#endif

void lcd_tli_init(void);
void lcd_tli_draw_point(uint16_t x,uint16_t y,uint16_t color);

#if defined(__cplusplus)
}
#endif

#endif /* GD32F450Z_LCD_H */
