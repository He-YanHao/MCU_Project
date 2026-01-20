#include "nvscfg.h"

static const char* TAG = "NVS_MANAGER";

static SemaphoreHandle_t mutex = NULL;


/**
 * @brief :
 * @return:
 *         
 */
int32_t init_config(void)
{
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "NVS Flash init failed");
    }
    
    return err;
}

/**
 * @brief : 
 * @return: 
 */
int32_t deinit_config(void)
{
    return ESP_OK;
}

/**
 * @brief : Validate the range legality of a single config parameter.
 * @param config_t：Pointer to the config type data to be checked
 * @return: 
 */
static int32_t validate_unit_config(nvscfg_unit_t* data_unit)
{
    if (NULL == data_unit || data_unit->data_type >= NVS_DATA_TYPE_MAX)
    {
        ESP_LOGE(TAG, "Invalid config type or null pointer or passed in parameter is invalid");
        return ESP_FAIL;
    }

    switch (config->data_type)
    {
        case NVS_DATA_TYPE_U8:
            if (config->data.u8_value < config->min.u8_value) return STATUS_ERR_DATA_TOO_SMALL;
            if (config->data.u8_value > config->max.u8_value) return STATUS_ERR_DATA_TOO_LARGE;
            break;
        case NVS_DATA_TYPE_I8:
            if (config->data.i8_value < config->min.i8_value) return STATUS_ERR_DATA_TOO_SMALL;
            if (config->data.i8_value > config->max.i8_value) return STATUS_ERR_DATA_TOO_LARGE;
            break;
        case NVS_DATA_TYPE_U16:
            if (config->data.u16_value < config->min.u16_value) return STATUS_ERR_DATA_TOO_SMALL;
            if (config->data.u16_value > config->max.u16_value) return STATUS_ERR_DATA_TOO_LARGE;
            break;
        case NVS_DATA_TYPE_I16:
            if (config->data.i16_value < config->min.i16_value) return STATUS_ERR_DATA_TOO_SMALL;
            if (config->data.i16_value > config->max.i16_value) return STATUS_ERR_DATA_TOO_LARGE;
            break;
        case NVS_DATA_TYPE_U32:
            if (config->data.u32_value < config->min.u32_value) return STATUS_ERR_DATA_TOO_SMALL;
            if (config->data.u32_value > config->max.u32_value) return STATUS_ERR_DATA_TOO_LARGE;
            break;
        case NVS_DATA_TYPE_I32:
            if (config->data.i32_value < config->min.i32_value) return STATUS_ERR_DATA_TOO_SMALL;
            if (config->data.i32_value > config->max.i32_value) return STATUS_ERR_DATA_TOO_LARGE;
            break;
        case NVS_DATA_TYPE_FLOAT: /* TODO: The float type cannot be checked with "<" ">" */
            if (config->data.f_value < config->min.f_value) return STATUS_ERR_DATA_TOO_SMALL;
            if (config->data.f_value > config->max.f_value) return STATUS_ERR_DATA_TOO_LARGE;
            break;
        default:
            return STATUS_ERR_INVALID_TYPE;
    }
    return ESP_OK;
}

/**
 * @brief Validate parameter range legality
 * @param data_ptr: Pointer to the parameter
 * @return:
 */
int32_t validate_config_structure(nvscfg_unit_t* data_unit, size_t num_items)
{
    if (NULL == data_ptr)
    {
        ESP_LOGE(TAG, "Null pointer passed in");
        return ESP_FAIL;
    }

    for (size_t i = 0; i < num_items; i++)
    {
        int32_t result = validate_unit_config(uint_cfg);
        if (result != 0)
        {
            ESP_LOGE(TAG, "Invalid nvscfg_unit_t at index %" PRIu32, (uint32_t)i);
            return ESP_FAIL;
        }
    }
    return ESP_OK;
}

/**
 * @brief Copy NVS user group data to memory and point the given struct pointer parameter to this memory
 * @param data_manager: Will store the starting address of the read data here
 * @return: 0: Write successful
 *          1: Null pointer passed in
 *          2: Failed to open NVS space and obtain handle
 *          Other values: Failed
 */
int32_t nvscfg_get_config(appconfig_t* appconfig_obj)
{

}

/**
 * @brief Write the data pointed to by the parameter structure pointer into NVS space
 * @param : Data from here will be stored in NVS
 * @return: 0: Write successful
 *          Other values: Failed
 */
int32_t nvscfg_set_config(appconfig_t* appconfig_obj)
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
        err = nvs_set_blob(handle, KEY_CONFIG_DATA, appconfig_obj, sizeof(appconfig_t));
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

int32_t set_config_flush(void)
{
    return 0;
}