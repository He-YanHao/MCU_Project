#include "main.h"
// GD32外设实现头文件
#include "Delay.h"
#include "BSP_USART.h"
#include "BSP_RTC.h"
// 开发板外设实现头文件
#include "BSP_LED.h"
#include "BSP_KEY.h"
#include "BSP_SDRAM.h"
#include "BSP_W25Q64.h"
#include "BSP_SD_card.h"
// 开发板外接硬件实现头文件
#include "lcd_st7789.h"
#include "touch.h"
#include "ov7670.h"
// OS移植层
#include "FreeRTOS_Task.h"
// 文件系统移植层 
#include "ff.h"
#include "diskio.h"
// GUI移植层
#include "lvgl.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"
#include "lv_port_fs.h"
// GUI
#include "gif_animation.h"
#include "cube_surround.h"

void my_log_cb(const char * buf)
{
    printf("%s", buf);
}

/**
 * @brief 
 * 
 * @return int 
 */
int main(void)
{
    USART_Init();
    BSP_LED_ALL_Init();
//    W25Q64_init();
//    rtc_time_init();
//    BSP_SD_card_init();
//    // FatFs 初始化
//    static FATFS SDFatFS;
//    FRESULT fres = f_mount(&SDFatFS, "0:", 1);
//    if(fres != FR_OK) {
//        printf("FatFs mount failed: %d\n", fres);
//    } else {
//        printf("FatFs mount OK\n");
//    }

    lcd_init();
//    // LVGL 初始化
//    lv_init();            // lvgl系统初始化
//    lv_log_register_print_cb(my_log_cb); // 日志
//    lv_port_disp_init();  // 显示接口初始化
//    // lv_port_indev_init(); // 输入接口初始化
//    lv_port_fs_init();    // 文件系统接口初始化
//    printf("lv_init OK\n");
//    cube_surround();

    OV7670_Init();
    OV7670_read_pid_ver();

    // 开启 FreeRTOS 调度
    printf("\n\n\n\n\nFreeRTOS_Start\n");
    FreeRTOS_Start();

    while(1)
    {
        BSP_LED_ALL_TOGGLE();
        Delay_ms(200);
    }
}
