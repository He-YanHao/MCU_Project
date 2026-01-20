idf_component_register(
    SRCS "nvsvfg_mng.c" "test_nvscfg.c" "nvscfg.c"
    INCLUDE_DIRS "."
    REQUIRES nvs_flash esp_timer
)