#include "lv_port_fs.h"
#include "lvgl.h"
#include <stdlib.h>
#include <stdio.h>

// 文件系统移植层 
#include "ff.h"
#include "diskio.h"


static void fs_init(void);

static void * fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close(lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence);
static lv_fs_res_t fs_size(lv_fs_drv_t * drv, void * file_p, uint32_t * size_p);
static lv_fs_res_t fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);

static void * fs_dir_open(lv_fs_drv_t * drv, const char * path);
static lv_fs_res_t fs_dir_read(lv_fs_drv_t * drv, void * rddir_p, char * fn);
static lv_fs_res_t fs_dir_close(lv_fs_drv_t * drv, void * rddir_p);

void lv_port_fs_init(void)
{
    fs_init();

    static lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    fs_drv.letter = 'S';
    fs_drv.open_cb = fs_open;
    fs_drv.close_cb = fs_close;
    fs_drv.read_cb = fs_read;
    fs_drv.write_cb = fs_write;
    fs_drv.seek_cb = fs_seek;
    fs_drv.tell_cb = fs_tell;

    fs_drv.dir_close_cb = fs_dir_close;
    fs_drv.dir_open_cb = fs_dir_open;
    fs_drv.dir_read_cb = fs_dir_read;

    lv_fs_drv_register(&fs_drv);
}

/*Initialize your Storage device and File system.*/
static void fs_init(void)
{
    // 只是占位置
    // 因为文件系统不止是LVGL一个组件使用 因此要在BSP级别初始化
}

/**
 * 打开一个文件
 * @param drv   指向所属文件系统驱动的指针
 * @param path  文件路径，以驱动字母开头（例如：S:/folder/file.txt）
 * @param mode  打开模式：只读 FS_MODE_RD，只写 FS_MODE_WR，读写 FS_MODE_RD | FS_MODE_WR
 * @return      文件描述符，出错时返回 NULL
 */
static void * fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode)
{

printf("fs_open path = [%s]\n", path);

    LV_UNUSED(drv);

    FIL * file = lv_mem_alloc(sizeof(FIL));
    if(file == NULL) {
        return NULL;
    }

    BYTE fatfs_mode = 0;

    if(mode == LV_FS_MODE_WR) {
        fatfs_mode = FA_WRITE | FA_CREATE_ALWAYS;
    } else if(mode == LV_FS_MODE_RD) {
        fatfs_mode = FA_READ;
    } else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) {
        fatfs_mode = FA_READ | FA_WRITE;
    } else {
        lv_mem_free(file);
        return NULL;
    }

    // 关键修复
    char real_path[256];
    if(path[0] == '/') {
        snprintf(real_path, sizeof(real_path), "0:%s", path);
    } else {
        /* 理论上不会走到这里，但写上更稳 */
        snprintf(real_path, sizeof(real_path), "0:/%s", path);
    }

    FRESULT fres = f_open(file, real_path, fatfs_mode);

printf("real_path = [%s]\n", real_path);
printf("f_open result = %d\n", fres);

    if(fres != FR_OK) {
        lv_mem_free(file);
        return NULL;
    }

    return file;
}

/**
 * 关闭一个已打开的文件
 * @param drv     指向所属文件系统驱动的指针
 * @param file_p  文件句柄指针（由 fs_open() 打开并返回的对象）
 * @return        返回 LV_FS_RES_OK 表示无错误，
 *                若发生错误则返回 lv_fs_res_t 枚举中的对应错误码
 */
static lv_fs_res_t fs_close(lv_fs_drv_t * drv, void * file_p)
{
    LV_UNUSED(drv);
    // 把 LVGL 传下来的 void * 转换回 FatFs FIL *
    FIL * file = (FIL *)file_p;
    if(file == NULL) {
        return LV_FS_RES_INV_PARAM;
    }
    // 关闭文件系统句柄
    f_close(file);
    // 释放指针
    free(file);
    return LV_FS_RES_OK;
}

/**
 * 从已打开的文件中读取数据
 * @param drv     指向所属文件系统驱动的指针
 * @param file_p  文件句柄指针（由 fs_open() 打开并返回的对象）
 * @param buf     用于存放读取数据的内存缓冲区指针
 * @param btr     请求读取的字节数（Bytes To Read）
 * @param br      实际读取到的字节数（Byte Read）
 * @return        返回 LV_FS_RES_OK 表示读取成功；
 *                若发生错误则返回 lv_fs_res_t 枚举中的对应错误码
 */
static lv_fs_res_t fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    LV_UNUSED(drv);

    FIL * file = (FIL *)file_p;
    UINT read_bytes = 0;
    if(file == NULL) {
        return LV_FS_RES_INV_PARAM;
    }
    // 
    if(f_read(file, buf, btr, &read_bytes) != FR_OK) {
        return LV_FS_RES_FS_ERR;
    }
    // 
    if(br) {
        *br = read_bytes;
    }
    return LV_FS_RES_OK;
}

/**
 * 向已打开的文件中写入数据
 * @param drv     指向所属文件系统驱动的指针
 * @param file_p  文件句柄指针（由 fs_open() 打开并返回的对象）
 * @param buf     指向待写入数据缓冲区的指针
 * @param btw     请求写入的字节数（Bytes To Write）
 * @param bw      实际成功写入的字节数（Bytes Written），
 *                如果不关心该值可以传入 NULL
 * @return        返回 LV_FS_RES_OK 表示写入成功；
 *                若发生错误则返回 lv_fs_res_t 枚举中的对应错误码
 */
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
    LV_UNUSED(drv);
    FIL * file = (FIL *)file_p;
    UINT written_bytes = 0;
    if(file == NULL) {
        return LV_FS_RES_INV_PARAM;
    }
    // 
    if(f_write(file, buf, btw, &written_bytes) != FR_OK) {
        return LV_FS_RES_FS_ERR;
    }
    // 
    if(bw) {
        *bw = written_bytes;
    }
    return LV_FS_RES_OK;
}

/**
 * 设置文件的读写指针位置。
 * 如果设置的位置超出当前文件大小，可能会扩展文件。
 * 
 * @param drv     指向所属文件系统驱动的指针
 * @param file_p  文件句柄指针（由 fs_open() 打开获得）
 * @param pos     新的读写指针位置
 * @param whence  指示 pos 的基准位置，参见 lv_fs_whence_t 枚举
 * @return        LV_FS_RES_OK 表示成功；
 *                发生错误时返回 lv_fs_res_t 中的错误码
 */
static lv_fs_res_t fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence)
{
    LV_UNUSED(drv);

    FIL * file = (FIL *)file_p;
    FSIZE_t new_pos = 0;

    if(file == NULL)
        return LV_FS_RES_INV_PARAM;

    switch(whence) {

        case LV_FS_SEEK_SET:
            new_pos = pos;
            break;

        case LV_FS_SEEK_CUR:
            new_pos = f_tell(file) + pos;
            break;

        case LV_FS_SEEK_END:
            new_pos = f_size(file) + pos;
            break;

        default:
            return LV_FS_RES_INV_PARAM;
    }

    if(f_lseek(file, new_pos) != FR_OK)
        return LV_FS_RES_FS_ERR;

    return LV_FS_RES_OK;
}

/**
 * 获取当前文件读写指针的位置
 * @param drv     指向所属文件系统驱动的指针
 * @param file_p  文件句柄指针（由 fs_open() 打开并返回的对象）
 * @param pos_p   用于存放当前读写指针位置的指针
 * @return        返回 LV_FS_RES_OK 表示获取成功；
 *                若发生错误则返回 lv_fs_res_t 枚举中的对应错误码
 */
static lv_fs_res_t fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    LV_UNUSED(drv);

    FIL * file = (FIL *)file_p;

    if(file == NULL || pos_p == NULL)
        return LV_FS_RES_INV_PARAM;

    *pos_p = (uint32_t)f_tell(file);

    return LV_FS_RES_OK;
}

/**
 * 初始化一个目录读取对象（用于遍历目录）
 * @param drv   指向所属文件系统驱动的指针
 * @param path  目录路径（以驱动字母开头，例如 "S:/folder"）
 * @return      成功时返回目录描述符指针；
 *              出错时返回 NULL
 */
static void * fs_dir_open(lv_fs_drv_t * drv, const char * path)
{
    LV_UNUSED(drv);

    DIR * dir = lv_mem_alloc(sizeof(DIR));
    if(dir == NULL)
        return NULL;
    FRESULT res = f_opendir(dir, path);
    if(res != FR_OK) {
        lv_mem_free(dir);
        return NULL;
    }
    return dir;
}

/**
 * 读取目录中的下一个文件名
 * 目录名需要以 '/' 开头返回
 * @param drv      指向所属文件系统驱动的指针
 * @param rddir_p  指向已初始化的目录描述符（由 fs_dir_open 创建）
 * @param fn       用于存储读取到的文件名的缓冲区
 * @return         返回 LV_FS_RES_OK 表示成功，
 *                 若发生错误则返回 lv_fs_res_t 枚举中的对应错误码
 */
static lv_fs_res_t fs_dir_read(lv_fs_drv_t * drv, void * rddir_p, char * fn)
{
    LV_UNUSED(drv);

    DIR * dir = (DIR *)rddir_p;
    FILINFO fno;

    FRESULT fres = f_readdir(dir, &fno);

    if(fres != FR_OK)
        return LV_FS_RES_UNKNOWN;

    /* Directory end */
    if(fno.fname[0] == '\0')
    {
        fn[0] = '\0';
    }
    else
    {
        /* If directory add '/' prefix */
        if(fno.fattrib & AM_DIR)
        {
            fn[0] = '/';
            strcpy(&fn[1], fno.fname);
        }
        else
        {
            strcpy(fn, fno.fname);
        }
    }

    return LV_FS_RES_OK;
}

/**
 * 关闭目录读取并释放相关资源
 * @param drv     指向所属文件系统驱动的指针
 * @param rddir_p 目录句柄指针（由 fs_dir_open() 打开并返回的对象）
 * @return        返回 LV_FS_RES_OK 表示关闭成功；
 *                若发生错误则返回 lv_fs_res_t 枚举中的对应错误码
 */
static lv_fs_res_t fs_dir_close(lv_fs_drv_t * drv, void * rddir_p)
{
    LV_UNUSED(drv);

    DIR * dir = (DIR *)rddir_p;

    if(dir == NULL)
        return LV_FS_RES_INV_PARAM;

    f_closedir(dir);
    lv_mem_free(dir);

    return LV_FS_RES_OK;
}
