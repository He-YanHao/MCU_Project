#include "lv_port_indev.h"
#include "lvgl.h"
#include "touch.h"

// 保存触摸驱动获得的触摸情况
static const touch_t *touch_status = NULL;
// 保存触摸驱动获得的触摸情况转化为LVGL需要的格式的情况
lv_indev_t * indev_touchpad;

/** 
 * @brief  初始化触摸屏
 * @param  无 
 * @retval 无 
 */
static void touchpad_init(void)
{
    FT5206_Init();
    touch_status = FT5206_GetState();
}

/**
* @brief 获取触摸屏设备的状态
* @param 无
* @retval 返回触摸屏设备是否被按下
*/
static bool touchpad_is_pressed(void)
{
    return (bool)FT5206_Scan();
}

/**
* @brief 在触摸屏被按下的时候读取 x、y 坐标
* @param x : x 坐标的指针
* @arg y : y 坐标的指针
* @retval 无
*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    (*x) = touch_status->x[0];
    (*y) = touch_status->y[0];
}

/** 
 * @brief  该函数由 LVGL 内部周期性调用，用于读取触摸屏状态。
 * @param  无 
 * @retval 无 
 */
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    if(touchpad_is_pressed()) {
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    data->point.x = last_x;
    data->point.y = last_y;
}

void lv_port_indev_init(void)
{

   /* 在这里你可以找到 LittlevGL 支持的出入设备的实现示例:
    * - 触摸屏
    * - 鼠标 (支持光标)
    * - 键盘 (仅支持按键的 GUI 用法)
    * - 编码器 (支持的 GUI 用法仅包括: 左, 右, 按下)
    * - 按钮 (按下屏幕上指定点的外部按钮)
    *
    * 你需要根据具体的硬件来完成这些函数 */

    // 初始化触摸硬件驱动
    touchpad_init();

    // 输入设备驱动描述结构体 并设置默认值
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);

    // 选择输入设备类型为触摸屏
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    // 设置触摸坐标读取回调函数
    indev_drv.read_cb = touchpad_read;
    // 注册输入设备
    indev_touchpad = lv_indev_drv_register(&indev_drv);
}
