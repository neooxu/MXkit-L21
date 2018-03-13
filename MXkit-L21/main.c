#include <atmel_start.h>
#include "oled.h"
#include "color_led.h"
#include "sht2x.h"

char sensor_display[2][OLED_DISPLAY_MAX_CHAR_PER_ROW + 1];
			
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	color_led_init();
	color_led_open(128, 128, 128);
	
	/* Temperature and huminity sensor  */
	SHT2x_Init();

	/* init OLED */
	OLED_Init();
    //OLED_FillAll();
    OLED_Clear();

	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_1, "MicroChip");
	
	STDIO_REDIRECT_0_example();

	/* Replace with your application code */
	while (1) {
		
		sprintf(sensor_display[0], "Temp: %.2f C", SHT2x_GetTempPoll());
		sprintf(sensor_display[1], "humi: %.2f %%", SHT2x_GetHumiPoll());
		
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2, sensor_display[0]);
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, sensor_display[1]);
		
		delay_ms(500);
	}
}
