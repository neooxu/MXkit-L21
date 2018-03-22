
#ifndef _CLOUD_CHAT_H_
#define _CLOUD_CHAT_H_

#include "emw_api.h"
#include "emw_alicloud_db.h"

#define APP_DEBUG MX_DEBUG_ON
#define cc_log(M, ...) MX_LOG(APP_DEBUG, "CC", M, ##__VA_ARGS__)

typedef enum
{
	eState_M1_initialize           = 1,
	eState_M2_provision            = 2,
	eState_M3_normal               = 3,
	eState_M4_disconnected         = 4,
	eState_M5_fault                = 5,
} cc_device_state_e;

typedef struct {
	cc_device_state_e device_state;
	emw_arg_ali_conn_e cloud_state;
	bool delay_prov;
} cc_context_t;

typedef enum
{
	ALI_HANDLE_CURRENT_HUMIDITY,
	ALI_HANDLE_CURRENT_TEMPERATURE,
	ALI_HANDLE_SATURATION,
	ALI_HANDLE_BRIGHTNESS,
	ALI_HANDLE_HUE,
	ALI_HANDLE_LIGHT_SWITCH,
	ALI_HANDLE_IO_SWITCH_1,
	ALI_HANDLE_IO_SWITCH_2,
	ALI_HANDLE_CONSOLE,
	ALI_HANDLE_MAX,
	ALI_HANDLE_NONE = -1,
}cc_device_handles;

uint8_t cc_init(void);
//void Process_InputData(uint8_t* data_buffer, uint16_t Nb_bytes);
mx_status cc_tick(void);

/* Attribute handlers */
mx_status handle_read_cur_humidity		(ali_att_val *value);
mx_status handle_read_cur_temperature	(ali_att_val *value);


mx_status handle_read_cur_saturation	(ali_att_val *value);
mx_status handle_read_cur_bright		(ali_att_val *value);
mx_status handle_read_cur_hue			(ali_att_val *value);
mx_status handle_read_cur_light_switch	(ali_att_val *value);
mx_status handle_write_cur_saturation	(ali_att_val value);
mx_status handle_write_cur_bright		(ali_att_val value);
mx_status handle_write_cur_hue			(ali_att_val value);
mx_status handle_write_cur_light_switch	(ali_att_val value);

mx_status handle_read_cur_switch_1		(ali_att_val *value);
mx_status handle_read_cur_switch_2		(ali_att_val *value);

mx_status handle_read_console			(ali_att_val *value);
mx_status handle_write_console			(ali_att_val value);


/* Attr write functions */
mx_status handle_write_cur_saturation	(ali_att_val value);
mx_status handle_write_cur_bright		(ali_att_val value);
mx_status handle_write_cur_hue			(ali_att_val value);


void alicloud_indicate_handle			(int handle);
void alicloud_indicate_local_atts		(int attr_handles[], int num);

void alicloud_provision(void);
void alicloud_restore(void);

#endif
