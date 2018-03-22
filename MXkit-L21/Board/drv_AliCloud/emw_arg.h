

#ifndef _EMW_ARG_H_
#define _EMW_ARG_H_

#include "mx_toolchain.h"
#include "mx_common.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define EMW_ARG_ERR 0xFF

enum{
	EMW_ARG_ALI_CONN_DISCONNECTED,
	EMW_ARG_ALI_CONN_CONNECTED,
	EMW_ARG_ALI_CONN_MAX,
};
typedef uint8_t emw_arg_ali_conn_e;
extern const char* EMW_ARG_ALI_CONN[];

enum{
	EMW_ARG_ALI_STATUS_UNINITIALIZED,
	EMW_ARG_ALI_STATUS_INITIALIZED,
	EMW_ARG_ALI_STATUS_CONNECTED,
	EMW_ARG_ALI_STATUS_DISCONNECTED,
	EMW_ARG_ALI_STATUS_MAX,
};
typedef uint8_t emw_arg_ali_status_e;
extern const char* EMW_ARG_ALI_STATUS[];

enum{
	EMW_ARG_ALI_FORMAT_JSON,
	EMW_ARG_ALI_FORMAT_RAW,
	EMW_ARG_ALI_FORMAT_MAX,
};
typedef uint8_t emw_arg_ali_format_e;
extern const char* EMW_ARG_ALI_FORMAT[];

enum{
	EMW_ARG_ALI_EV_ALINK,
	EMW_ARG_ALI_EV_SET,
	EMW_ARG_ALI_EV_GET,
	EMW_ARG_ALI_EV_MAX,
};
typedef uint8_t emw_arg_ali_ev_e;
extern const char* EMW_ARG_ALI_EV[];

enum{
	EMW_ARG_WLAN_EV_UAP_ESTABLISHED,
	EMW_ARG_WLAN_EV_UAP_DESTROYED,
	EMW_ARG_WLAN_EV_STA_CONNECTED,
	EMW_ARG_WLAN_EV_STA_DISCONNECTED,
	EMW_ARG_WLAN_EV_MAX,
};
typedef uint8_t emw_arg_wlan_ev_e;
extern const char* EMW_ARG_WLAN_EV[];

enum{
	EMW_ARG_WLAN_STA_CONNECTED,
	EMW_ARG_WLAN_STA_DISCONNECTED,
	EMW_ARG_WLAN_STA_CONNECTING,
	EMW_ARG_WLAN_STA_MAX,
};
typedef uint8_t emw_arg_wlan_sta_e;
extern const char* EMW_ARG_WLAN_STA[];

const char* emw_arg_for_type(char**list, uint8_t type);
uint8_t emw_arg_for_arg( char**list, char *arg);

typedef struct _dfd {
	int32_t len;
	uint8_t* data;
	emw_arg_ali_format_e format;
} emw_ali_local_attrs_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif