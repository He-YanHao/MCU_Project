/**
 * @file nvsvfg_mng.h
 * @author LongJian (longjian314@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-12-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef __NVSVFG_MNG_H__
 #define __NVSVFG_MNG_H__ 

 #include "nvscfg.h"

nvscfg_t* nvscfg_get_instance(void);
/**
 * @brief 
 * 
 * @return nvscfg_t* 
 */
nvscfg_t* nvscfg_get_instance(void);

int32_t nvscfg_set_userdate(nvscfg_user_data_t *nvscfg_obj);
int32_t nvscfg_set_factory(const nvscfg_factory_t* nvscfg_obj);
int32_t nvscfg_set_advanced(const nvscfg_advanced_t* nvscfg_obj);

int32_t nvscfg_get_userdate(nvscfg_user_data_t *nvscfg_obj);
int32_t nvscfg_get_factory(nvscfg_factory_t *nvscfg_obj);
int32_t nvscfg_get_advanced(nvscfg_advanced_t *nvscfg_obj);

int32_t nvscfg_init(void);

int32_t nvscfg_get_power_limit(uint32_t* power_limit);
int32_t nvscfg_set_power_limit(uint32_t power_limit);

#endif /* __NVSVFG_MNG_H__ */