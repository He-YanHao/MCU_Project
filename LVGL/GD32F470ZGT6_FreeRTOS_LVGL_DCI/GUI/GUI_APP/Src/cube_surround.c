#include "cube_surround.h"
#include "lcd_st7789.h"
#include "lvgl.h"
#include "stdio.h"

#define COLOR_R 0xFF0000
#define COLOR_G 0x00FF00
#define COLOR_B 0x0000FF
#define COLOR_Y 0xFFFF00
#define COLOR_W 0xFFFFFF

// 变色回调函数
static void color_toggle_event_cb(lv_event_t * e)
{
    // 获取被点击的对象
    lv_obj_t * obj = lv_event_get_target(e);
    // 获取当前背景颜色
    lv_color_t cur_color = lv_obj_get_style_bg_color(obj, LV_PART_MAIN);
    // 判断颜色
    if(cur_color.full == lv_color_hex(COLOR_R).full) {
        lv_obj_set_style_bg_color(obj, lv_color_hex(COLOR_G), LV_PART_MAIN);
    } else if(cur_color.full == lv_color_hex(COLOR_G).full) {
        lv_obj_set_style_bg_color(obj, lv_color_hex(COLOR_B), LV_PART_MAIN);
    } else if(cur_color.full == lv_color_hex(COLOR_B).full) {
        lv_obj_set_style_bg_color(obj, lv_color_hex(COLOR_R), LV_PART_MAIN);
    }
}

static void anim_set(void * var, int32_t v)
{
    lv_obj_t * obj = (lv_obj_t *)var;
    lv_obj_set_x(obj, v);
}

static void start_perimeter_anim(lv_obj_t * obj)
{
    // 获得默认动画效果
    lv_anim_t d;
    lv_anim_init(&d);
    // 设置动画对象
    lv_anim_set_var(&d, obj);
    // 设置动画执行函数
    lv_anim_set_exec_cb(&d, anim_set);
//    lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);

    // 动画持续时间
    lv_anim_set_time(&d, 50);
    // 设置动画曲线函数
    lv_anim_set_path_cb(&d, lv_anim_path_linear);
    // 动画结束时调用的函数
//    lv_anim_set_ready_cb(&d, anim_left_done);
    // 启动动画
    lv_anim_start(&d);
}

void cube_surround(void)
{
    /* 绘制中心变色方块 */
    // 获取当前活动屏幕
    lv_obj_t * scr = lv_scr_act();
    // 创建一个新对象 父类是当前活动的屏幕
    lv_obj_t * color_toggle_block = lv_obj_create(scr);
    // 设置该对象的宽度和高度为
    lv_obj_set_size(color_toggle_block, LCD_WIDTH, LCD_HEIGHT);
    // 设置位置
    lv_obj_set_pos(color_toggle_block, 0, 0);
    // 设置对象颜色
    lv_obj_set_style_bg_color(color_toggle_block, lv_color_hex(COLOR_R), 0);
    // 设置边框  设置 80 像素的边框
    lv_obj_set_style_border_width(color_toggle_block, 20, LV_STATE_DEFAULT);
    /* 设置边框颜色为红色（为了看到边框效果） */
    lv_obj_set_style_border_color(color_toggle_block, lv_color_hex(COLOR_W), 0);
    // 设置圆角
    lv_obj_set_style_radius(color_toggle_block, 50, 0);
}

//    /* 注册点击事件 */
//    lv_obj_add_event_cb(color_toggle_block,
//                        color_toggle_event_cb,
//                        LV_EVENT_PRESSED,
//                        NULL);

//    // 创建一个新对象，父类是当前活动的屏幕
//    lv_obj_t * perimeter_move_block = lv_obj_create(scr);
//    // 设置该对象的宽度和高度为 800x480
//    lv_obj_set_size(perimeter_move_block, 80, 80);
//    // 设置位置
//    lv_obj_set_pos(perimeter_move_block, 0, 0);
//    // 设置对象颜色
//    lv_obj_set_style_bg_color(perimeter_move_block, lv_color_hex(COLOR_Y), 0);
//    // 设置边框
//    lv_obj_set_style_border_width(perimeter_move_block, 0, LV_STATE_DEFAULT);
//    // 设置圆角
//    lv_obj_set_style_radius(perimeter_move_block, 0, 0);
//    // 开启动画
//    // start_perimeter_anim(perimeter_move_block);