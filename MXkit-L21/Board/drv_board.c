
#include <stdio.h>
#include "drv_board.h"

void drv_board_init(void)
{
	/* System time ticker */
	mx_hal_ms_ticker_init();
	
	/* RGB color led initialize */
	color_led_init();
	color_led_open(0, 0, 0);
	
	/* Temperature and humidity sensor  */
	SHT2x_Init();

	/* init OLED */
	OLED_Init();
	OLED_Clear();

}
