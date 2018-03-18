#include <stdlib.h>
#include <string.h>
#include "mx_debug.h"
#include "mx_common.h"
#include "emw_api.h"
#include "ATCmdParser.h"

mx_status emw_ali_config(const emw_type_ali_config_t* config)
{
	const char* format_arg = emw_arg_for_type(EMW_TYPE_ALI_FORMAT, config->product_info.format);

	if (ATCmdParser_send("AT+ALINKPRODUCT=%s,%s,%s,%s,%s", 
	                      config->product_info.name, config->product_info.modle, 
	                      config->product_info.key,  config->product_info.secret, format_arg )
	 && ATCmdParser_recv("OK\n")
	 && ATCmdParser_send("AT+ALINKDEV=%s,%s,%s",
	                      config->dev_info.type, config->dev_info.category,
	                      config->dev_info.manufacture)
	 && ATCmdParser_recv("OK\n")) {
		return kNoErr;
	}
	
	return kGeneralErr;
}

mx_status emw_ali_start_service(void)
{
	if (ATCmdParser_send("AT+ALINKSTART")
     && ATCmdParser_recv("OK\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

mx_status emw_ali_provision(bool on)
{
	if ((on? ATCmdParser_send("AT+ALINKAWSSTART"):
		     ATCmdParser_send("AT+ALINKAWSSTOP"))
		  && ATCmdParser_recv("OK\n")) {
		return kNoErr;
	}
	return kGeneralErr;
}

mx_status emw_ali_start_provision(void)
{
	if (ATCmdParser_send("AT+ALINKAWSSTART")
     && ATCmdParser_recv("OK\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

mx_status emw_ali_stop_provision(void)
{
	if (ATCmdParser_send("AT+ALINKAWSSTOP")
     && ATCmdParser_recv("OK\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

mx_status emw_ali_set_cloud_atts(emw_type_ali_format_e format, uint8_t *data, int32_t len)
{
	if (ATCmdParser_send("AT+ALINKSEND")
	 && ATCmdParser_recv(">")
	 && ATCmdParser_write(data, len) == len
	 && ATCmdParser_recv("OK")) {
		return kNoErr;
	}
	return kGeneralErr;
}

void emw_ali_event_handler(void)
{
	mx_status err = kNoErr;
	char arg1[10], arg2[10];
	emw_type_ali_format_e format;
	emw_type_ali_conn_e conn;

	// parse out the packet
	require_action(ATCmdParser_recv("%s,", arg1), exit, err = kMalformedErr);
		
	emw_type_ali_ev_e event = emw_type_for_arg(EMW_TYPE_ALI_EV, arg1);
	require_action(event != EMW_TYPE_ERR, exit,  err = kMalformedErr);

	/* ALINK Server connection event */
	if (event == EMW_TYPE_ALI_EV_ALINK) {
		require_action(ATCmdParser_recv("%s\n", arg2), exit, err = kMalformedErr);
		conn = emw_type_for_arg(EMW_TYPE_ALI_CONN, arg2);
		require_action(conn != EMW_TYPE_ERR, exit, err = kMalformedErr);
		emw_ev_ali_connection(conn);
	}
	/* ALINK server <=== attribute value=== device */
	else if (event == EMW_TYPE_ALI_EV_GET) {
		require_action(ATCmdParser_recv("%s\n", arg2), exit, err = kMalformedErr);
		format =  emw_type_for_arg(EMW_TYPE_ALI_FORMAT, arg2);
		require_action(format != EMW_TYPE_ERR, exit, err = kMalformedErr);
		emw_ev_ali_get_local_atts(format);
	}
	/* ALINK server === attribute value===> device */
	else if (event == EMW_TYPE_ALI_EV_SET) {
		require_action(ATCmdParser_recv("%s,", arg2), exit, err = kMalformedErr);
		format = emw_type_for_arg(EMW_TYPE_ALI_FORMAT, arg2);
		require_action(format != EMW_TYPE_ERR, exit, err = kMalformedErr);
		
		/* Read package data */
		int32_t count;
		require_action(ATCmdParser_recv("%d,", &count), exit, err = kMalformedErr);

		uint8_t *data = malloc(count);
		require_action(data, exit, err = kNoMemoryErr);
		require_action(ATCmdParser_read(data, count) == true, exit, err = kTimeoutErr);

		emw_ev_ali_set_local_atts(format, data, count);
		free(data);
	}
	
exit:
	if (err == kMalformedErr) emw_ev_unknown();
	return;
}
