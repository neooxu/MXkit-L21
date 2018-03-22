

#ifndef _EMW_EVENT_H_
#define _EMW_EVENT_H_

#include "mx_common.h"
#include "emw_api.h"

#ifdef __cplusplus
extern "C"
{
#endif




MX_WEAK void emw_ev_unknown(void);
MX_WEAK void emw_ev_ali_connection(emw_arg_ali_conn_e conn);
MX_WEAK void emw_ev_ali_get_local_atts(emw_ali_local_attrs_t *attrs);
MX_WEAK void emw_ev_ali_set_local_atts(emw_ali_local_attrs_t *attrs);
MX_WEAK void emw_ev_wlan(emw_arg_wlan_ev_e event);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif