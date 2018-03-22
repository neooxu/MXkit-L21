#include <atmel_start.h>
#include "mx_debug.h"
#include "mx_common.h"
#include "cloud_chat.h"

#include "oled.h"
#include "color_led.h"
#include "hsb_led.h"
#include "sht2x.h"
#include "io_button.h"

#define APP_DEBUG MX_DEBUG_ON
#define app_log(M, ...) MX_LOG(APP_DEBUG, "APP", M, ##__VA_ARGS__)

#define SENSOR_READ_INTERVAL    5000

char sensor_display[2][OLED_DISPLAY_MAX_CHAR_PER_ROW + 1];

float temperature = 24.00;
float humidity = 50.00;
uint32_t latest_sensor_read_tick = 0;

uint32_t latest_console_send_tick = 0;

float bright = 20.0;
float saturation = 100.0;
float hue = 50.0;
bool  light_on = true;
bool  switch_1 = false;
bool  switch_2 = false;

void switch_task(void);
void SHT20_task(void);


void usr_btn_isr(void);
void usr_clicked_handler(void);
void usr_long_pressed_handler(void);


btn_instance_t usr_btn =
{
	.port                   = GPIO_PORTA,
	.pin					= PIN_PA02,
	.io_irq                 = usr_btn_isr,
	.idle					= IOBUTTON_IDLE_STATE_HIGH,
	.long_pressed_timeout	= 5000,
	.pressed_func           = usr_clicked_handler,
	.long_pressed_func		= usr_long_pressed_handler,
};


int main(void)
{
	mx_status err = kNoErr;
	
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	//board_test();
	//ATCmdParser_test();
	board_init();
	cc_init();
	
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_1, "MicroChip");

	hsb2rgb_led_open(hue, saturation, bright);
	temperature = SHT2x_GetTempPoll();
	humidity = SHT2x_GetHumiPoll();
	
	sprintf(sensor_display[0], "Temp: %.2f C", temperature);
	sprintf(sensor_display[1], "humi: %.2f %%", humidity);
		
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2, sensor_display[0]);
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, sensor_display[1]);
	
	switch_1 = gpio_get_pin_level(SW_1);
	switch_2 = gpio_get_pin_level(SW_2);
	
	err = emw_module_init();
	require_noerr(err, exit);
		
	button_init(&usr_btn);
	
	while(1)
	{
		/* Application tick */
		cc_tick();
		SHT20_task();
		switch_task();
		button_srv(&usr_btn);
		while (ATCmdParser_process_oob());
	}
	
exit:
	app_log("App exit reason %d", err);
	while(1);
}

void SHT20_task(void)
{
	if ( ms_ticker_read()-latest_sensor_read_tick > SENSOR_READ_INTERVAL) {
		temperature = SHT2x_GetTempPoll();
		humidity = SHT2x_GetHumiPoll();
		sprintf(sensor_display[0], "Temp: %.2f C", temperature);
		sprintf(sensor_display[1], "Humi: %.2f %%", humidity);
		
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2, sensor_display[0]);
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, sensor_display[1]);
		
		alicloud_indicate_handle(ALI_HANDLE_CURRENT_TEMPERATURE);
		alicloud_indicate_handle(ALI_HANDLE_CURRENT_HUMIDITY);
		
		latest_sensor_read_tick = ms_ticker_read();
	}
}

void switch_task(void)
{
	if (switch_1 != gpio_get_pin_level(SW_1)) {
		switch_1 = gpio_get_pin_level(SW_1);
		alicloud_indicate_handle(ALI_HANDLE_IO_SWITCH_1);
	}

	if (switch_2 != gpio_get_pin_level(SW_2)) {
		switch_2 = gpio_get_pin_level(SW_2);
		alicloud_indicate_handle(ALI_HANDLE_IO_SWITCH_2);
	}
}

#if 0
#define CONSOLE_PACKET_SIZE   512
int con_seq = 0;

char console_buff[CONSOLE_PACKET_SIZE+20];

void console_task(void)
{
	mx_status err = kNoErr;
	struct json_str jstr;
	
	char console_string[CONSOLE_PACKET_SIZE];

	json_str_init(&jstr, console_buff, sizeof(console_buff));
			
	if ( ms_ticker_read()-latest_console_send_tick > SENSOR_READ_INTERVAL) {
		
		snprintf(console_string, CONSOLE_PACKET_SIZE, "This is Packet %d\r\n", con_seq);

		err = json_start_object(&jstr);
		require_noerr(err, exit);
		
		json_set_val_str(&jstr, "value", console_string);
		json_set_val_int(&jstr, "seq", con_seq);
		
		err = json_close_object(&jstr);
		require_noerr(err, exit);
		
		alicloud_indicate_handle(ALI_HANDLE_CONSOLE);
		con_seq++;
	}
exit:
	return;
}
#endif

/* SHT20 cloud attribute handlers */
mx_status handle_read_cur_humidity(ali_att_val *value)
{
	(*value).floatValue = humidity;
	return kNoErr;
}


mx_status handle_read_cur_temperature(ali_att_val *value)
{
	(*value).floatValue = temperature;
	return kNoErr;
}

/* RGB led attribute handlers */
mx_status handle_write_cur_saturation	(ali_att_val value)
{
	saturation = value.intValue;
	if (light_on == true) {
		hsb2rgb_led_open(hue, saturation, bright);
	}
	return kNoErr;
}
mx_status handle_write_cur_bright		(ali_att_val value)
{
	bright = value.intValue;
	if (light_on == true) {
		hsb2rgb_led_open(hue, saturation, bright);
	}
	return kNoErr;
}

mx_status handle_write_cur_hue			(ali_att_val value)
{
	hue = (float)value.intValue;
	if (light_on == true) {
		app_log("Light: %.2f, %.2f, %.2f", hue, saturation, bright);
		hsb2rgb_led_open(hue, saturation, bright);
	}
	return kNoErr;
}

mx_status handle_write_cur_light_switch(ali_att_val value)
{
	light_on = value.boolValue;
	if (light_on == true) {
		hsb2rgb_led_open(hue, saturation, bright);
	}
	else {
		hsb2rgb_led_close();
	}
	return kNoErr;
}


mx_status handle_read_cur_saturation(ali_att_val *value)
{
	(*value).intValue = saturation;
	return kNoErr;
}

mx_status handle_read_cur_bright(ali_att_val *value)
{
	(*value).intValue = bright;
	return kNoErr;
}

mx_status handle_read_cur_hue(ali_att_val *value)
{
	(*value).intValue = hue;
	return kNoErr;
}

mx_status handle_read_cur_light_switch(ali_att_val *value)
{
	(*value).boolValue = light_on;
	return kNoErr;
}

/* Switch attribute handlers */
mx_status handle_read_cur_switch_1		(ali_att_val *value)
{
	(*value).boolValue = switch_1;
	return kNoErr;
}

mx_status handle_read_cur_switch_2		(ali_att_val *value)
{
	(*value).boolValue = switch_2;
	return kNoErr;	
}

char console_buff[128]="hello, world!";
const char oled_clear_line[OLED_DISPLAY_MAX_CHAR_PER_ROW]="                ";
mx_status handle_read_console (ali_att_val *value)
{
	(*value).stringValue = console_buff;
	return kNoErr;
}

mx_status handle_write_console(ali_att_val value)
{
	app_log("recv %s", value.stringValue);
	strncpy(console_buff, value.stringValue, 128);
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, oled_clear_line);
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, console_buff);
	return kNoErr;
}


void usr_btn_isr(void)
{
	button_irq_handler(&usr_btn);
}

void usr_clicked_handler(void)
{
	alicloud_provision();
}

const char oled_default_line[OLED_DISPLAY_MAX_CHAR_PER_ROW]="Restore default";
void usr_long_pressed_handler(void)
{
	cc_log("Restore default settings");
	
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, oled_clear_line);
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, oled_default_line);
	
	alicloud_restore();
	
	//_reset_mcu();
}

/* Attribute write functions */