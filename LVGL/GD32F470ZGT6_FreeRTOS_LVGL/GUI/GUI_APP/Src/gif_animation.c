#include "gif_animation.h"
#include "lvgl.h"
// 文件系统移植层 
#include "ff.h"
#include "diskio.h"
#include "stdio.h"
#include <stdlib.h>

void display_png_from_sd_card(const char *filename) {
    // 打开文件
    FIL file;
    FRESULT res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // 获取文件大小
    uint32_t file_size = f_size(&file);
    uint8_t *buffer = malloc(file_size);
    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        f_close(&file);
        return;
    }

    // 读取文件到缓冲区
    UINT br;
    res = f_read(&file, buffer, file_size, &br);
    if (res != FR_OK || br != file_size) {
        printf("Error reading file\n");
        free(buffer);
        f_close(&file);
        return;
    }
    f_close(&file);

    // 创建图像对象并设置源
    lv_obj_t * img_obj = lv_img_create(lv_scr_act());
    lv_img_set_src(img_obj, buffer);

    // 释放内存
    free(buffer);

    // 刷新显示
    lv_task_handler();
}

void png_animation(void) {
    // 设置文件路径，文件路径区分大小写
    const char *file_path = "/LVGL/GIF/cover_1.PNG";

    // 调用函数显示 PNG 文件
    display_png_from_sd_card(file_path);
}
