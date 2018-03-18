

#ifndef _EMW_EVENT_H_
#define _EMW_EVENT_H_

#include "mx_toolchain.h"
#include "emw_type.h"

#ifdef __cplusplus
extern "C"
{
#endif


MX_WEAK void emw_ev_unknown(void);
MX_WEAK void emw_ev_ali_connection(emw_type_ali_conn_e conn);
MX_WEAK void emw_ev_ali_get_local_atts(emw_type_ali_format_e format);
MX_WEAK void emw_ev_ali_set_local_atts(emw_type_ali_format_e format, uint8_t data, int32_t len);
MX_WEAK void emw_ev_wlan(emw_type_wlan_ev_e event);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif