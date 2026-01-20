/**
 * @file nvsvfg_mng.c
 * @author LongJian (longjian314@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-12-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "nvscfg.h"

static nvscfg_t nvscfg_instance = {0};

/**
 * @brief 
 * 
 * @return nvscfg_t* 
 */
nvscfg_t* nvscfg_get_instance(void)
{
    return &nvscfg_instance;
}

// NVSCFG_SET_USERDATA_MACRO(
//     obj,        // 外部传入的新配置
//     TYPE,       // 完整配置结构体类型
//     INSTANCE,   // 当前全局/静态配置实例
//     UNIT_TYPE,  // 最小配置单元（如 uint8_t / uint16_t）
//     TAG_STR     // 日志 TAG
// )
#define NVSCFG_SET_USERDATA_MACRO(obj, TYPE, INSTANCE, UNIT_TYPE, TAG_STR)       \
    do {                                                                         \
        /* 空指针预警 */                                                          \
        if ((obj) == NULL) {                                                     \
            ESP_LOGE(TAG_STR, "Null pointer passed in");                         \
            return ESP_FAIL;                                                     \
        }                                                                        \
        /* 比较两处内存的数据是否相同 */                                           \
        if (memcmp(&(INSTANCE), (obj), sizeof(TYPE)) != 0) {                     \
            if (validate_config_structure(                                       \
                    (UNIT_TYPE *)(obj),                                          \
                    sizeof(TYPE) / sizeof(UNIT_TYPE)) != ESP_OK) {               \
                ESP_LOGE(TAG_STR, "Invalid configuration structure");            \
                return ESP_FAIL;                                                 \
            }                                                                    \
                                                                                 \
            memcpy(&(INSTANCE), (obj), sizeof(TYPE));                            \
            return nvscfg_set_config(&(INSTANCE));                               \
        }                                                                        \
                                                                                 \
        return ESP_OK;                                                           \
    } while (0)

int32_t nvscfg_set_userdate(nvscfg_user_data_t *nvscfg_obj)
{
    NVSCFG_SET_USERDATA_MACRO(
        nvscfg_obj,
        nvscfg_user_data_t,
        nvscfg_instance,
        nvscfg_unit_t,
        TAG
    );
}

int32_t nvscfg_set_factory(const nvscfg_factory_t* nvscfg_obj)
{
    NVSCFG_SET_USERDATA_MACRO(
        nvscfg_obj,
        nvscfg_factory_t,
        nvscfg_instance,
        nvscfg_unit_t,
        TAG
    );
}

int32_t nvscfg_set_advanced(const nvscfg_advanced_t* nvscfg_obj)
{
    NVSCFG_SET_USERDATA_MACRO(
        nvscfg_obj,
        nvscfg_advanced_t,
        nvscfg_instance,
        nvscfg_unit_t,
        TAG
    );
}
/**
 * @brief 
 * 
 * @param nvscfg_obj 
 * @return int32_t 
 */
int32_t nvscfg_get_userdate(nvscfg_user_data_t* nvscfg_obj)
{
    if (NULL == nvscfg_obj)
    {
        ESP_LOGE(TAG, "Null pointer passed in");
        return ESP_FAIL;
    }

    memcpy(nvscfg_obj, &nvscfg_instance, sizeof(vnscfg_t));

    return ESP_OK;
}

/**
 * @brief 
 * 
 * @param nvscfg_obj 
 * @return int32_t 
 */
int32_t nvscfg_get_factory(nvscfg_factory_t* nvscfg_obj)
{
    if (NULL == nvscfg_obj)
    {
        ESP_LOGE(TAG, "Null pointer passed in");
        return ESP_FAIL;
    }

    memcpy(nvscfg_obj, &nvscfg_instance, sizeof(vnscfg_t));

    return ESP_OK;
}

/**
 * @brief 
 * 
 * @param nvscfg_obj 
 * @return int32_t 
 */
int32_t nvscfg_get_advanced(nvscfg_advanced_t* nvscfg_obj)
{
    if (NULL == nvscfg_obj)
    {
        ESP_LOGE(TAG, "Null pointer passed in");
        return ESP_FAIL;
    }

    memcpy(nvscfg_obj, &nvscfg_instance, sizeof(vnscfg_t));

    return ESP_OK;
}

int32_t nvscfg_get_power_limit(uint32_t* power_limit)
{
    nvscfg_unit_t *pValue = &nvscfg_instance.advanced_data.power_limit;
    if (NULL == power_limit)
    {
        ESP_LOGE(TAG, "Null pointer passed in");
        return ESP_FAIL;
    }

    *power_limit = pValue->u32_value;

    return ESP_OK;
}

int32_t nvscfg_set_power_limit(uint32_t power_limit)
{
    nvscfg_unit_t *pValue = &nvscfg_instance.advanced_data.power_limit;

    pValue->u32_value = power_limit;
    return nvscfg_set_config(&nvscfg_instance);
}