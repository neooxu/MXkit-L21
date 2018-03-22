#include <stdlib.h>
#include <string.h>
#include "emw_alicloud_db.h"
#include "cloud_chat.h"

ali_att_val alicloud_attr_val_db[ALI_HANDLE_MAX];

const ali_dev_attr_t alicloud_attr_db[ALI_HANDLE_MAX] = 
{
	[ALI_HANDLE_CURRENT_HUMIDITY] = {
		.name		= "CurrentHumidity",
		.att_type	= ALI_ATT_TYPE_FLOAT,
		.read_func  = handle_read_cur_humidity,
		.write_func = NULL,
	},
	[ALI_HANDLE_CURRENT_TEMPERATURE] = {
		.name		= "CurrentTemperature_1",
		.att_type	= ALI_ATT_TYPE_FLOAT,
		.read_func  = handle_read_cur_temperature,
		.write_func = NULL,
	},
	[ALI_HANDLE_SATURATION] = {
		.name		= "Saturation",
		.att_type	= ALI_ATT_TYPE_INT,
		.read_func  = handle_read_cur_saturation,
		.write_func = handle_write_cur_saturation,
	},
	[ALI_HANDLE_BRIGHTNESS] = {
		.name		= "Luminance",
		.att_type	= ALI_ATT_TYPE_INT,
		.read_func  = handle_read_cur_bright,
		.write_func = handle_write_cur_bright,
	},
	[ALI_HANDLE_HUE] = {
		.name		= "Hue",
		.att_type	=  ALI_ATT_TYPE_INT,
		.read_func  = handle_read_cur_hue,
		.write_func = handle_write_cur_hue,
	},
	[ALI_HANDLE_LIGHT_SWITCH] = {
		.name		= "lampswitch",
		.att_type	=  ALI_ATT_TYPE_BOOL,
		.read_func  = handle_read_cur_light_switch,
		.write_func = handle_write_cur_light_switch,
	},
	[ALI_HANDLE_IO_SWITCH_1] = {
		.name		= "switch1",
		.att_type	=  ALI_ATT_TYPE_BOOL,
		.read_func  = handle_read_cur_switch_1,
		.write_func = NULL,
	},
	[ALI_HANDLE_IO_SWITCH_2] = {
		.name		= "switch2",
		.att_type	=  ALI_ATT_TYPE_BOOL,
		.read_func  = handle_read_cur_switch_2,
		.write_func = NULL,
	},
	[ALI_HANDLE_CONSOLE] = {
		.name		= "DebVal",
		.att_type	= ALI_ATT_TYPE_STRING,
		.read_func  = handle_read_console,
		.write_func = handle_write_console,
	}
};


