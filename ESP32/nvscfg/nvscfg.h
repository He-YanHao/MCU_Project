/**
 * @file nvscfg.h
 * @author LongJian (longjian314@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-12-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef _NVSCFG_H_
#define _NVSCFG_H_

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

#define NAMESPACE_SYSTEM_CONFIG    "storage"
#define KEY_CONFIG_DATA            "config_data"

typedef enum {
    NULL_TYPE           = 0,
    NVS_DATA_TYPE_U8    = 1,
    NVS_DATA_TYPE_I8    = 2,
    NVS_DATA_TYPE_U16   = 3, 
    NVS_DATA_TYPE_I16   = 4,
    NVS_DATA_TYPE_U32   = 5,
    NVS_DATA_TYPE_I32   = 6,
    NVS_DATA_TYPE_FLOAT = 7,
    NVS_DATA_TYPE_MAX,
} nvs_data_type;

typedef union {
    uint8_t  u8_value;
    int8_t   i8_value;
    uint16_t u16_value;
    int16_t  i16_value;
    uint32_t u32_value;
    int32_t  i32_value;
    float    f_value;
} nvscfg_value_t;

typedef struct config{
    nvscfg_value_t  data;
    nvscfg_value_t  min;
    nvscfg_value_t  max;
    uint8_t         data_type;
} nvscfg_unit_t;

typedef struct user_data{
    nvscfg_unit_t bluetooth;
    nvscfg_unit_t operate;
    nvscfg_unit_t output;
    nvscfg_unit_t voltage;
    nvscfg_unit_t current;
    nvscfg_unit_t preset_voltage_1;
    nvscfg_unit_t preset_current_1;
    nvscfg_unit_t preset_voltage_2;
    nvscfg_unit_t preset_current_2;
    nvscfg_unit_t preset_voltage_3;
    nvscfg_unit_t preset_current_3;
    nvscfg_unit_t advanced_password;
    nvscfg_unit_t factory_password;
} nvscfg_user_data_t;

#define NUM_CONFIG_ITEMS (sizeof(nvscfg_user_data_t) / sizeof(nvscfg_unit_t))

typedef struct advanced{
    nvscfg_unit_t power_limit;
    nvscfg_unit_t acceleration_temperature;
    nvscfg_unit_t full_speed_temperature;
    nvscfg_unit_t two_stage_charging_A;
    nvscfg_unit_t two_stage_charging_V;
    nvscfg_unit_t three_stage_charging_A;
    nvscfg_unit_t three_stage_charging_V;
    nvscfg_unit_t current_soft_start;
    nvscfg_unit_t full_charge_auto_stop;
    nvscfg_unit_t device_activation_status;
    nvscfg_unit_t password_lock;
    nvscfg_unit_t touch_switch;
    nvscfg_unit_t screen_brightness;
} nvscfg_advanced_t;

#define NUM_ADVANCED_ITEMS (sizeof(nvscfg_advanced_t) / sizeof(nvscfg_unit_t))

typedef struct factory{
    nvscfg_unit_t temporary_password;
    nvscfg_unit_t voltage_calibration;
    nvscfg_unit_t set_current_calibration;
    nvscfg_unit_t input_voltage_calibration;
    nvscfg_unit_t high_temperature_limit;
    nvscfg_unit_t system_power;
    nvscfg_unit_t measured_voltage;
    nvscfg_unit_t voltage_multiplier;
    nvscfg_unit_t setvxz_value;
    nvscfg_unit_t current_calibration;
    nvscfg_unit_t max_current;
    nvscfg_unit_t max_voltage;
    nvscfg_unit_t min_voltage;
} nvscfg_factory_t;

#define NUM_FACTORY_ITEMS (sizeof(nvscfg_factory_t) / sizeof(nvscfg_unit_t))

typedef struct appconfig {
    nvscfg_user_data_t user_data;
    nvscfg_advanced_t  advanced_data;
    nvscfg_factory_t   factory_data;
} nvscfg_t;

int32_t init_config(void);
int32_t deinit_config(void);

int32_t nvscfg_get_config(nvscfg_t* nvscfg_obj);
int32_t nvscfg_set_config(nvscfg_t* nvscfg_obj);
int32_t set_config_flush(void);

void nvs_test(void);

#endif




/**
 * @brief Copy NVS user group data to memory and point the given struct pointer parameter to this memory
 * @param data_manager: Will store the starting address of the read data here
 * @return: 0: Write successful
 *          1: Null pointer passed in
 *          2: Failed to open NVS space and obtain handle
 *          Other values: Failed
 */
int get_config(appnvscfg_unit_t* appconfig_obj)
{
    if (NULL == appconfig_obj)
    {
        return STATUS_ERR_NULL_PTR;
    }
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NAMESPACE_SYSTEM_CONFIG, NVS_READONLY, &handle);
    if (err != ESP_OK)
    {
        return STATUS_ERR_NVS_OPEN;
    }
    size_t struct_size = sizeof(appnvscfg_unit_t);
    err = nvs_get_blob(handle, KEY_CONFIG_DATA, appconfig_obj, &struct_size);
    if (err != ESP_OK)
    {
        return STATUS_ERR_KEY_GET;
    }
    nvs_close(handle);
    return STATUS_OK;
}

/**
 * @brief Write the data pointed to by the parameter structure pointer into NVS space
 * @param : Data from here will be stored in NVS
 * @return: 0: Write successful
 *          Other values: Failed
 */
int set_config(appnvscfg_unit_t* appconfig_obj)
{
    esp_err_t err = validate_config_structure(appconfig_obj);
    if (err != 0)
    {
        return err;
    }
    mutex = xSemaphoreCreateMutex();
    if (NULL == mutex || xSemaphoreTake(mutex, portMAX_DELAY) != pdTRUE)
    {
        return STATUS_ERR_MUTEX_TIMEOUT;
    }
    nvs_handle_t handle;
    do {
        err = nvs_open(NAMESPACE_SYSTEM_CONFIG, NVS_READWRITE, &handle);
        if (err != ESP_OK)
        {
            err = STATUS_ERR_NVS_OPEN;
            break;
        }
        err = nvs_set_blob(handle, KEY_CONFIG_DATA, appconfig_obj, sizeof(appnvscfg_unit_t));
        if (err == ESP_OK)
        {
            err = nvs_commit(handle);
            {
                if (err != ESP_OK)
                {
                    err = STATUS_ERR_NVS_COMMIT;
                }
            }
        } else
        {
            err = STATUS_ERR_KEY_SET;
        }
    } while(0);
    if (handle) {
        nvs_close(handle);
    }
    xSemaphoreGive(mutex);
    return err;
}


void nvs_test(void)
{
    uint64_t start_time = 0, end_time = 0;
    int64_t elapsed_time = 0;
    nvs_stats_t nvs_stats;
    int err = init_config();
    if (err)
    {
        printf("init_config: %d\n",err);
        return;
    }
    appnvscfg_unit_t config = {0};
    while(1)
    {
        start_time = esp_timer_get_time();
        err = get_config(&config);
        if (err)
        {
            printf("get_config: %d\n",err);
            return;
        }
        if(config.advanced_data.acceleration_temperature.data.i16_value >= config.advanced_data.acceleration_temperature.max.i16_value)
        {
            config.advanced_data.acceleration_temperature.data.i16_value = config.advanced_data.acceleration_temperature.min.i16_value;
        }
        end_time = esp_timer_get_time();
        elapsed_time = end_time - start_time;
        printf("get_config run time: %lld us (%lld ms)\n",elapsed_time, elapsed_time/1000);
        
        config.advanced_data.acceleration_temperature.data.i16_value += 5;
        printf("nvs:%d\n",sizeof(appnvscfg_unit_t));
        nvs_get_stats(NULL, &nvs_stats);
        printf("NVS Status:\n");
        printf("  Total Entries: %d\n", nvs_stats.total_entries);
        printf("  Used Entries: %d\n", nvs_stats.used_entries);
        printf("  Free Entries: %d\n", nvs_stats.free_entries);
        printf("data:%d\n", config.advanced_data.acceleration_temperature.data.i16_value);
        printf("max:%d\n", config.advanced_data.acceleration_temperature.max.i16_value);
        printf("min:%d\n", config.advanced_data.acceleration_temperature.min.i16_value);

        start_time = esp_timer_get_time();
        err = set_config(&config);
        if (err)
        {
            printf("set_config: %d\n",err);
            return;
        }
        end_time = esp_timer_get_time();
        elapsed_time = end_time - start_time;
        printf("set_config run time: %lld us (%lld ms)\n",elapsed_time, elapsed_time/1000);

        vTaskDelay(200);
    }
}