

#ifndef _EMW_TYPE_H_
#define _EMW_TYPE_H_

#include "mx_toolchain.h"
#include "mx_common.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define EMW_TYPE_ERR 0xFF

enum{
	EMW_TYPE_ALI_CONN_DISCONNECTED,
	EMW_TYPE_ALI_CONN_CONNECTED,
	EMW_TYPE_ALI_CONN_MAX,
};
typedef uint8_t emw_type_ali_conn_e;
extern const char* EMW_TYPE_ALI_CONN[];

enum{
	EMW_TYPE_ALI_FORMAT_JSON,
	EMW_TYPE_ALI_FORMAT_RAW,
	EMW_TYPE_ALI_FORMAT_MAX,
};
typedef uint8_t emw_type_ali_format_e;
extern const char* EMW_TYPE_ALI_FORMAT[];

enum{
	EMW_TYPE_ALI_EV_ALINK,
	EMW_TYPE_ALI_EV_SET,
	EMW_TYPE_ALI_EV_GET,
	EMW_TYPE_ALI_EV_MAX,
};
typedef uint8_t emw_type_ali_ev_e;
extern const char* EMW_TYPE_ALI_EV[];

enum{
	EMW_TYPE_WLAN_EV_UAP_ESTABLISHED,
	EMW_TYPE_WLAN_EV_UAP_DESTROYED,
	EMW_TYPE_WLAN_EV_STA_CONNECTED,
	EMW_TYPE_WLAN_EV_STA_DISCONNECTED,
	EMW_TYPE_WLAN_EV_MAX,
};
typedef uint8_t emw_type_wlan_ev_e;
extern const char* EMW_TYPE_WLAN_EV[];

enum{
	EMW_TYPE_WLAN_STA_CONNECTED,
	EMW_TYPE_WLAN_STA_DISCONNECTED,
	EMW_TYPE_WLAN_STA_CONNECTING,
	EMW_TYPE_WLAN_STA_MAX,
};
typedef uint8_t emw_type_wlan_sta_e;
extern const char* EMW_TYPE_WLAN_STA[];

const char* emw_arg_for_type(char**list, uint8_t type);
uint8_t emw_type_for_arg( char**list, char *arg);


typedef struct{
	struct{
		const char*				name;			/**< ALINKTEST */
		const char*				modle;			/**< ALINKTEST_LUA */
		const char*				key;			/**< LIVING */
		const char*				secret;			/**< LIGHT */
		emw_type_ali_format_e	format;			/**< ALINKTEST_LIVING_LIGHT_ALINK_TEST_LUA */
	} product_info;
	struct{
		const char*				type;
		const char*				category;
		const char*				manufacture;
	} dev_info ;
} emw_type_ali_config_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif