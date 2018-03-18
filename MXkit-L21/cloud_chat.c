
#include "mx_debug.h"
#include "emw_api.h"
#include "cloud_chat.h"

const emw_type_ali_config_t ali_config =
{
	.product_info = {
		.name			= "microchip-002",
		.modle			= "MICROCHIP_LIVING_AIRBOX_MICROCHIP_002",
		.key			= "Dkqt9OjYC0u0DIWGajKP",
		.secret			= "ciubDzkEOKVi0WS2VZzqAGGIgdmW1dsSatitz6Ie",
		.format			= EMW_TYPE_ALI_FORMAT_JSON,
	},
	.dev_info = {
		.type			= "AIRBOX",
		.category		= "LIVING",
		.manufacture	= "MICROCHIP",
	}
};

static cc_context_t context;
			
uint8_t cc_init(void)
{
	context.device_state = eState_M1_initialize;
}

static mx_status _handle_state_initialize(void)
{
	mx_status err = kNoErr;
	char ssid[33], pwd[33];
	
	err = emw_module_init();
	require_noerr(err, exit);

	//err = emw_module_restore_settings();
	//require_noerr(err, exit);

	cc_log("FW version: %s", emw_module_get_fw_version());

	err = emw_ali_config(&ali_config);
	require_noerr(err, exit);

	err = emw_wlan_get_para(ssid, pwd);
	require_noerr(err, exit);

	if (strlen(ssid)) {
		cc_log("SSID: %s, PWD: %s", ssid, pwd);
		if (EMW_TYPE_WLAN_STA_CONNECTED == emw_wlan_get_sta_status()) {
			cc_log("Wlan connected");
			context.device_state = eState_M3_normal;
		}
		else {
			cc_log("Waiting for wlan connection");
			context.device_state = eState_M4_disconnected;
		}
	}
	else {
		cc_log("Wlan unconfigured, start provision mode");
		err = emw_ali_provision(true);
		require_noerr(err, exit);
		context.device_state = eState_M2_provision;
	}
	
exit:
	return err;
}

static mx_status _handle_state_provision(void)
{
	mx_status err = kNoErr;
	char ssid[33], pwd[33];
	
	err = emw_module_init();
	require_noerr(err, exit);
	
	cc_log("FW version: %s", emw_module_get_fw_version());
	
	/* Set product catalog information registed on alicloud */
	err = emw_ali_config(&ali_config);
	require_noerr(err, exit);
	
	/* wlan is configured? */
	err = emw_wlan_get_para(ssid, pwd);
	require_noerr(err, exit);
	
	if (strlen(ssid)) {
		cc_log("SSID: %s, PWD: %s", ssid, pwd);
		context.device_state = eState_M4_disconnected;
	}
	else {
		cc_log("Wlan not configured, start provision...", ssid, pwd);
		context.device_state = eState_M2_provision;
	}
	
exit:
	while(1);
}

mx_status cc_tick(void)
{
	mx_status err = kNoErr;
	switch ( context.device_state ){
		case eState_M1_initialize:
		{
			err = _handle_state_initialize( );
			require_noerr_action( err, exit, context.device_state = eState_M1_initialize);
		}
		break;
		
		case eState_M3_normal:
		case eState_M4_disconnected:
		{
		}
		break;

		default:
			cc_log("STATE ERROR");
			err = kNoErr;
	}
exit:
	return err;
}

void emw_ev_wlan(emw_type_wlan_ev_e event)
{
	cc_log("Wlan event: %s", emw_arg_for_type(EMW_TYPE_WLAN_EV, event));
	
	if (event == EMW_TYPE_WLAN_EV_STA_CONNECTED) {
		context.device_state = eState_M3_normal;
	}
	
	if (event == EMW_TYPE_WLAN_EV_STA_DISCONNECTED) {
		context.device_state = eState_M4_disconnected;
	}
}