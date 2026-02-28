#include "lv_port_disp.h"
#include "lvgl.h"
#include "lcd.h"

/* 屏幕可见分辨率（实际显示像素） */
#define MY_DISP_HOR_RES LCD_WIDTH    /* 屏幕宽度 800 */
#define MY_DISP_VER_RES LCD_HEIGHT   /* 屏幕高度 480 */

__attribute__((section(".sdram.lvgl_heap")))
uint8_t lvgl_heap;

// 
volatile bool disp_flush_enabled = true;

/** 
 * @brief  初始化显示设备和必要的外围设备 
 * @param  无 
 * @retval 无 
 */
static void disp_init(void)
{
    lcd_init();
    lvgl_heap = 1;
}

/** 
 * @brief  将内部缓冲区的内容刷新到显示屏上的特定区域 
 *   @note 可以使用 DMA 或者任何硬件在后台加速执行这个操作 但是，需要在刷新完成后调用函数 `lv_disp_flush_ready()` 
 * @param  disp_drv    : 显示设备 
 *   @arg  area        : 要刷新的区域，包含了填充矩形的对角坐标 
 *   @arg  color_p     : 颜色数组
 * @retval 无 
 */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(disp_flush_enabled) {
        int32_t x, y;
        for(y = area->y1; y <= area->y2; y++) {
            for(x = area->x1; x <= area->x2; x++) {
                lcd_draw_point(x, y, color_p->full);
                color_p++;
            }
        }
    }
    lv_disp_flush_ready(disp_drv);
}

/** 
 * @brief  初始化LVGL显示
 * @param  无 
 * @retval 无 
 */
void lv_port_disp_init(void)
{
    // 初始化显示设备
    disp_init();
    // 确定缓冲区      单缓冲区 双缓冲区 全尺寸双缓冲区
    // 使用双缓冲 缓冲区保存 10 行屏幕的大小
    static lv_disp_draw_buf_t disp_draw_buf_dsc;
    __attribute__((section(".sdram.draw")))
    static lv_color_t disp_buf1[MY_DISP_HOR_RES * 10];
    __attribute__((section(".sdram.draw")))
    static lv_color_t disp_buf2[MY_DISP_HOR_RES * 10];
    lv_disp_draw_buf_init(&disp_draw_buf_dsc, disp_buf1, disp_buf2, MY_DISP_HOR_RES * 10);

////    全缓冲 暂时保留
//    static lv_disp_draw_buf_t disp_full_draw_buf;
//    static lv_color_t disp_full_buf_front[MY_DISP_HOR_RES * MY_DISP_VER_RES];   /*A screen sized buffer*/
//    static lv_color_t disp_full_buf_back[MY_DISP_HOR_RES * MY_DISP_VER_RES];    /*Another screen sized buffer*/
//    lv_disp_draw_buf_init(&disp_full_draw_buf,
//                          disp_full_buf_front,
//                          disp_full_buf_back,
//                          MY_DISP_VER_RES * LV_VER_RES_MAX);

    // 在 LVGL 中注册显示设备
    // 显示设备的描述符
    static lv_disp_drv_t disp_drv;
    // 初始化为默认值
    lv_disp_drv_init(&disp_drv);
    // 设置显示设备的分辨率
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;
    // 将缓冲区的内容复制到显示设备
    disp_drv.flush_cb = disp_flush;
    // 设置显示缓冲区
    disp_drv.draw_buf = &disp_draw_buf_dsc;
    // 是否每一帧都刷新整个屏幕，而不是只刷新变化区域。
    //disp_drv.full_refresh = 1; // 0：刷新变化区域 1：全刷新
    // 是否使用GPU加速
    //disp_drv.gpu_fill_cb = gpu_fill;
    // 注册显示设备
    lv_disp_drv_register(&disp_drv);
}

/** 
 * @brief  启用屏幕更新（即当 LVGL 调用 disp_flush() 时执行真正的刷新过程）
 * @param  无 
 * @retval 无 
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/** 
 * @brief  禁用屏幕更新（即当 LVGL 调用 disp_flush() 时不执行真正的刷新过程）
 * @param  无 
 * @retval 无 
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

#if 0
/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
                    const lv_area_t * fill_area, lv_color_t color)
{
    /*It's an example code which should be done by your GPU*/
    int32_t x, y;
    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/

    for(y = fill_area->y1; y <= fill_area->y2; y++) {
        for(x = fill_area->x1; x <= fill_area->x2; x++) {
            dest_buf[x] = color;
        }
        dest_buf+=dest_width;    /*Go to the next line*/
    }
}
#endif
