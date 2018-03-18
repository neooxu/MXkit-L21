

#ifndef _EMW_API_H_
#define _EMW_API_H_

#include "mx_common.h"
#include "mx_debug.h"
#include "emw_type.h"
#include "emw_event.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Module controller */
mx_status	emw_module_init				(void);
mx_status	ewm_module_reset			(void);
const char *emw_module_get_fw_version	(void);
uint32_t    emw_module_get_tick			(void);

/* wlan management */
mx_status			emw_wlan_get_para			(char ssid[33], char pwd[33]);
emw_type_wlan_sta_e emw_wlan_get_sta_status		(void);

/* Ali SDS APIs */
mx_status	emw_ali_config			(const emw_type_ali_config_t *config);
mx_status	emw_ali_start_service	(void);
mx_status	emw_ali_provision		(bool on);
mx_status	emw_ali_send			(emw_type_ali_format_e format, uint8_t *data, int32_t len);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif