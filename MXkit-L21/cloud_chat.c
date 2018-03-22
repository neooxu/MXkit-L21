
#include <atmel_start.h>

#include "mx_debug.h"
#include "emw_api.h"
#include "emw_alicloud_db.h"
#include "cloud_chat.h"

#include "json_generator.h"
#include "json_parser.h"

#include "hsb_led.h"

#include "oled.h"

const emw_ali_config_t ali_config =
{
	.product_info = {
		.name			= "microchip-002",
		.modle			= "MICROCHIP_LIVING_AIRBOX_MICROCHIP_002",
		.key			= "Dkqt9OjYC0u0DIWGajKP",
		.secret			= "ciubDzkEOKVi0WS2VZzqAGGIgdmW1dsSatitz6Ie",
		.format			= EMW_ARG_ALI_FORMAT_JSON,
	},
	.dev_info = {
		.type			= "AIRBOX",
		.category		= "LIVING",
		.manufacture	= "MICROCHIP",
	}
};

//BBED
//const char *dev_key = "R41Qd1Rm9CMtWWmUIsTM";
//const char *dev_sec = "CnJ9c7DoRBgrAsVT9IHoYMz0LviLgWjw";

//BBEA
const char *dev_key = "eImkAqUT24v1VYO8fbmf";
const char *dev_sec = "7qSOxEu7HcbDDa0LJSc0sHLVnEjnn3qZ";

//BBE0
//const char *dev_key = "R41Qd1Rm9CMtWWmUIsTM";
//const char *dev_sec = "CnJ9c7DoRBgrAsVT9IHoYMz0LviLgWjw";


//microchip
//const char *dev_key = "7UXu5dclxk6Gja8LzelT";
//const char *dev_sec = "xXKxoAVPtTfeoP2DDwOnUB7ZxnIbuoaJ";







cc_context_t context;

static int changed_handles[ALI_HANDLE_MAX];

const char oled_wifi_connect_line[OLED_DISPLAY_MAX_CHAR_PER_ROW]="Wi-Fi connected";
const char oled_wifi_disconnect_line[OLED_DISPLAY_MAX_CHAR_PER_ROW]="Wi-Fi disconnected";
const char oled_config_line[OLED_DISPLAY_MAX_CHAR_PER_ROW]="Wi-Fi config....";
const char oled_ali_connect_line[OLED_DISPLAY_MAX_CHAR_PER_ROW]="Cloud connected";
const char oled_ali_disconnect_line[OLED_DISPLAY_MAX_CHAR_PER_ROW]="Cloud disconnected";

		
uint8_t cc_init(void)
{
	context.device_state = eState_M1_initialize;
	context.cloud_state = EMW_ARG_ALI_CONN_DISCONNECTED;
	context.delay_prov = false;
	
	for (int i = 0; i<ALI_HANDLE_MAX; i++) {
		changed_handles[i]=ALI_HANDLE_NONE;
	}
}

static mx_status _handle_state_initialize(void)
{
	mx_status err = kNoErr;
	char ssid[33], pwd[33];


	//err = emw_module_restore_settings();
	//require_noerr(err, exit);

	cc_log("FW version: %s", emw_module_get_fw_version());

	err = emw_ali_config(&ali_config);
	require_noerr(err, exit);
	
	/* Set cloud access token */
	//err = emw_ali_set_key(dev_key, dev_sec);
	//require_noerr(err, exit);
	
	/* Start alicloud daemon service*/
	err = emw_ali_start_service();
	require_noerr(err, exit);

	/* Check Wi-Fi configuration */
	err = emw_wlan_get_para(ssid, pwd);
	require_noerr(err, exit);

	if (strlen(ssid)) {
		cc_log("SSID: %s, PWD: %s", ssid, pwd);
		if (EMW_ARG_ALI_STATUS_CONNECTED == emw_ali_get_status()) {
			cc_log("Alicloud connected.");
			mx_delay(200);
			context.device_state = eState_M3_normal;
		}
		else {
			cc_log("Waiting for Alicloud connection");
			context.device_state = eState_M4_disconnected;
		}
	}
	else {
		cc_log("Wlan unconfigured, start provision mode");
		
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, oled_config_line);
		/* Start alicloud Wi-Fi configuration */
		err = emw_ali_provision(true);
		require_noerr(err, exit);
		context.device_state = eState_M2_provision;
	}
	
exit:
	return err;
}

static mx_status _handle_state_provision(void)
{
}



void alicloud_indicate_handle(int handle)
{
	int i;
	for (i = 0; i < ALI_HANDLE_MAX && changed_handles[i] != ALI_HANDLE_NONE; i++) {
		
		if (changed_handles[i] == handle) return;
	}
	changed_handles[i]=handle;
}

mx_status cc_tick(void)
{
	mx_status err = kNoErr;
	alicloud_indicate_local_atts(changed_handles, ALI_HANDLE_MAX);

	switch ( context.device_state ){
		case eState_M1_initialize:
		{
			err = _handle_state_initialize( );
			require_noerr_action(err, exit, context.device_state=eState_M5_fault);
		}
		break;
		
		case eState_M2_provision:
		{
		}
		break;
		
		case eState_M3_normal:
		case eState_M4_disconnected:
		{
		}
		break;
		case eState_M5_fault:
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



void emw_ev_wlan(emw_arg_wlan_ev_e event)
{
	cc_log("Wlan event: %s", emw_arg_for_type(EMW_ARG_WLAN_EV, event));
	if (event == EMW_ARG_WLAN_EV_STA_CONNECTED) {
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, oled_wifi_connect_line);
	}
	else if (event == EMW_ARG_WLAN_EV_STA_DISCONNECTED) {
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, oled_wifi_disconnect_line);
	}
}

void emw_ev_ali_connection(emw_arg_ali_conn_e conn)
{
	cc_log("AliCloud event: %s", emw_arg_for_type(EMW_ARG_ALI_CONN, conn));
	
	context.cloud_state = conn;
	
	if (conn == EMW_ARG_ALI_CONN_CONNECTED) {
		if (context.delay_prov == true) {
			usr_clicked_handler();
			context.delay_prov = false;
		}

		
		mx_delay(1000);
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, oled_ali_connect_line);
		
		context.device_state = eState_M3_normal;
		/* Alicloud get local value event do not trigger, so we trigger automatically */
		emw_ev_ali_get_local_atts(EMW_ARG_ALI_FORMAT_JSON);
	}

	if (conn == EMW_ARG_ALI_CONN_DISCONNECTED) {
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, oled_ali_disconnect_line);
		context.device_state = eState_M4_disconnected;
	}
}

void emw_ev_ali_get_local_atts(emw_ali_local_attrs_t *attr)
{
	int attr_handles[ALI_HANDLE_MAX];
	for (int i = 0; i<ALI_HANDLE_MAX; i++) {
		alicloud_indicate_handle(i);
	}
	
	return;
}

#define NUM_TOKENS	30


void emw_ev_ali_set_local_atts(emw_ali_local_attrs_t *attrs)
{
	jsontok_t json_tokens[NUM_TOKENS];
	jobj_t jobj;
	char val[50];
	char name[50];
	ali_att_val value;
	int attr_handles[50];
	int num;
	int handle;
	
	cc_log("Set local attrs event");
	
	memset(attr_handles, 0, 50);
	mx_status err = json_init(&jobj, json_tokens, NUM_TOKENS, attrs->data, attrs->len);
	if (err != kNoErr) return;
	
	err = json_get_array_object(&jobj, "attrSet", &num);
	require_noerr(err, exit);
	
	for (int i = 0; i < num; i++) {
		err = json_array_get_str(&jobj, i, name, 50);
		require_noerr(err, exit);
		
		for( handle = 0; handle < ALI_HANDLE_MAX; handle++) {
			if (strcmp(name, alicloud_attr_db[handle].name)==0 ) {
				attr_handles[i] = handle;
				break;
			}
		}
	}
	
	err = json_release_array_object(&jobj);
	require_noerr(err, exit);
	
	for (int i = 0; i < num; i++) {
		handle = attr_handles[i];
		
		if (alicloud_attr_db[handle].write_func == NULL) continue;
		if (kNoErr == json_get_composite_object(&jobj, alicloud_attr_db[handle].name)) {
			if (kNoErr == json_get_val_str(&jobj, "value", val, sizeof(val))) {
				if (alicloud_attr_db[handle].att_type == ALI_ATT_TYPE_BOOL
				||  alicloud_attr_db[handle].att_type == ALI_ATT_TYPE_INT) {
					sscanf(val, "%d", &value);
				}
				else if (alicloud_attr_db[handle].att_type == ALI_ATT_TYPE_FLOAT) {
					sscanf(val, "%f", &value);
				}
				else if (alicloud_attr_db[handle].att_type == ALI_ATT_TYPE_STRING) {
					value.stringValue = val;
				}
				alicloud_attr_db[handle].write_func(value);
				json_release_composite_object(&jobj);
				alicloud_indicate_handle(handle);
			}
		}
	}
		
	
	exit:
	return;
}


void alicloud_indicate_local_atts(int attr_handles[], int num)
{
	if (context.device_state != eState_M3_normal ) return;
	
	struct json_str jstr;
	char buff[512] = {0};
	char val_str[20];
	mx_status err = kNoErr;
	ali_att_val value;
	int handle;
	int i;
	
	if(attr_handles[0]==ALI_HANDLE_NONE) return;
	
	cc_log("Indicate local attrs");
	
	json_str_init(&jstr, buff, sizeof(buff));
	
	err = json_start_object(&jstr);
	require_noerr(err, exit);
	
	for (i = 0; i < num && attr_handles[i] != ALI_HANDLE_NONE; i++) {
		handle = attr_handles[i];
		if (alicloud_attr_db[handle].read_func == NULL ) continue;
				
		err = json_push_object(&jstr, alicloud_attr_db[handle].name);
		require_noerr(err, exit);
		
		if (alicloud_attr_db[handle].att_type == ALI_ATT_TYPE_BOOL) {
			err = alicloud_attr_db[handle].read_func(&value);
			require_noerr(err, exit);
			snprintf(val_str, 20, "%d", value.boolValue);
			err = json_set_val_str(&jstr, "value", val_str);
			require_noerr(err, exit);
		}
		else if (alicloud_attr_db[handle].att_type == ALI_ATT_TYPE_INT) {
			err = alicloud_attr_db[handle].read_func(&value);
			require_noerr(err, exit);
			snprintf(val_str, 20, "%d", value.intValue);
			err = json_set_val_str(&jstr, "value", val_str);
			require_noerr(err, exit);
		}
		else if (alicloud_attr_db[handle].att_type == ALI_ATT_TYPE_FLOAT) {
			err = alicloud_attr_db[handle].read_func(&value);
			require_noerr(err, exit);
			snprintf(val_str, 20, "%.2f", value.floatValue);
			err = json_set_val_str(&jstr, "value", val_str);
			require_noerr(err, exit);
		}
		else if (alicloud_attr_db[handle].att_type == ALI_ATT_TYPE_STRING) {
			err = alicloud_attr_db[handle].read_func(&value);
			require_noerr(err, exit);
			err = json_set_val_str(&jstr, "value", value.stringValue);
			require_noerr(err, exit);
		}
		err = json_pop_object(&jstr);
		require_noerr(err, exit);		
	}
	
	/* Create attrset */
	json_push_array_object(&jstr, "attrSet");
	for (int i = 0; i < num && attr_handles[i] != ALI_HANDLE_NONE; i++) {
		handle = attr_handles[i];
		err = json_set_array_str(&jstr, alicloud_attr_db[handle].name);
		require_noerr(err, exit);
	}
	json_pop_array_object(&jstr);
	
	err = json_close_object(&jstr);
	require_noerr(err, exit);
	
	//cc_log("Send to cloud %d bytes > %s", strlen(buff), buff);
	err = emw_ali_set_cloud_atts(EMW_ARG_ALI_FORMAT_JSON, buff, strlen(buff));
	require_noerr(err, exit);
	
	for (i = 0; i < num; i++)
	{
		attr_handles[i] = ALI_HANDLE_NONE;
	}
		
exit:
	return;	

}


void alicloud_provision(void)
{
	mx_status err = kNoErr;
	char provision_ack[50];
	
	if (context.cloud_state == EMW_ARG_ALI_CONN_CONNECTED) {
		snprintf(provision_ack, 50, "{\"prov\":{\"value\":\"%d\"}}", 0);
		err = emw_ali_set_cloud_atts(EMW_ARG_ALI_FORMAT_JSON, provision_ack, strlen(provision_ack));
		require_noerr(err, exit);
		
		snprintf(provision_ack, 50, "{\"prov\":{\"value\":\"%d\"}}", 1);
		err = emw_ali_set_cloud_atts(EMW_ARG_ALI_FORMAT_JSON, provision_ack, strlen(provision_ack));
		require_noerr(err, exit);
		
		cc_log("Send provision acknowledgment to alicloud");
	}
	else {
		context.delay_prov = true;
	}
	
	#if 0
	light_on = (light_on==true)? false:true;
	
	if (light_on == true) {
		hsb2rgb_led_open(hue, saturation, bright);
	}
	else {
		hsb2rgb_led_close();
	}
	
	alicloud_indicate_handle(ALI_HANDLE_LIGHT_SWITCH);
	#endif

	exit:
	return;
}

void alicloud_restore(void)
{
	if ( context.device_state = eState_M2_provision ) {
		emw_ali_unbound();
	}
	
	emw_module_restore_settings();
	context.device_state = eState_M1_initialize;
}