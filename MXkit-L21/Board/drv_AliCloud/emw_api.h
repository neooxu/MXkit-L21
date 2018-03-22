

#ifndef _EMW_API_H_
#define _EMW_API_H_

#include "mx_common.h"
#include "mx_debug.h"
#include "emw_arg.h"
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
mx_status	emw_module_restore_settings	(void);

/* wlan management */
mx_status			emw_wlan_get_para			(char ssid[33], char pwd[33]);
emw_arg_wlan_sta_e emw_wlan_get_sta_status		(void);

/* Ali SDS APIs */

typedef struct{
	struct{
		const char*				name;			/**< ALINKTEST */
		const char*				modle;			/**< ALINKTEST_LUA */
		const char*				key;			/**< LIVING */
		const char*				secret;			/**< LIGHT */
		emw_arg_ali_format_e	format;			/**< ALINKTEST_LIVING_LIGHT_ALINK_TEST_LUA */
	} product_info;
	struct{
		const char*				type;
		const char*				category;
		const char*				manufacture;
	} dev_info ;
} emw_ali_config_t;

mx_status				emw_ali_config			(const emw_ali_config_t *config);
mx_status				emw_ali_start_service	(void);
mx_status				emw_ali_provision		(bool on);
emw_arg_ali_status_e	emw_ali_get_status		(void);
mx_status				emw_ali_set_cloud_atts	(emw_arg_ali_format_e format, uint8_t *data, int32_t len);
mx_status				emw_ali_set_key         (const char *dev_key, const char *dev_sec);

mx_status				emw_ali_unbound			(void);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif