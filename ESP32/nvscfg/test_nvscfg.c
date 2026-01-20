/**
 * @file test_nvscfg.c
 * @author LongJian (longjian314@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-12-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 void nvs_test(void)
{
    uint64_t start_time = 0, end_time = 0;
    int64_t elapsed_time = 0;
    nvs_stats_t nvs_stats;
    int32_t err = init_config();
    if (err)
    {
        printf("init_config: %d\n",err);
        return;
    }

    appconfig_t config = {0};
    
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
        printf("nvs:%d\n",sizeof(appconfig_t));
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