#ifndef __BSP_SDRAM_ADDR_H
#define __BSP_SDRAM_ADDR_H

/* SDRAM 基地址 和 总大小 */
#define SDRAM_BASE_ADDR        0xC0000000UL
#define SDRAM_TOTAL_SIZE       (32UL * 1024UL * 1024UL)

/* FreeRTOS Heap */
#define SDRAM_FREERTOS_HEAP_SIZE     (512UL * 1024UL)

/* LVGL Heap */
#define SDRAM_LVGL_HEAP_SIZE         (512UL * 1024UL)

/* LVGL Draw Buffer */
#define SDRAM_LVGL_DRAW_BUF_SIZE     (512UL * 1024UL)

/* LCD Framebuffer 0 */
#define SDRAM_LCD_FB0_SIZE           (1UL * 1024UL * 1024UL)

/* LCD Framebuffer 1 */
#define SDRAM_LCD_FB1_SIZE           (1UL * 1024UL * 1024UL)

/* ========================================================= */
/* ==================== 地址自动计算 ======================= */
/* ========================================================= */

#define SDRAM_FREERTOS_HEAP_ADDR     (SDRAM_BASE_ADDR)

#define SDRAM_LVGL_HEAP_ADDR         (SDRAM_FREERTOS_HEAP_ADDR + SDRAM_FREERTOS_HEAP_SIZE)

#define SDRAM_LVGL_DRAW_BUF_ADDR     (SDRAM_LVGL_HEAP_ADDR + SDRAM_LVGL_HEAP_SIZE)

#define SDRAM_LCD_FB0_ADDR           (SDRAM_LVGL_DRAW_BUF_ADDR + SDRAM_LVGL_DRAW_BUF_SIZE)

#define SDRAM_LCD_FB1_ADDR           (SDRAM_LCD_FB0_ADDR + SDRAM_LCD_FB0_SIZE)

#define SDRAM_RESERVED_ADDR          (SDRAM_LCD_FB1_ADDR + SDRAM_LCD_FB1_SIZE)

/* 剩余空间 */
#define SDRAM_RESERVED_SIZE          (SDRAM_TOTAL_SIZE - \
                                     (SDRAM_RESERVED_ADDR - SDRAM_BASE_ADDR))

#endif
