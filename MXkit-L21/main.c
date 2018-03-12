#include <atmel_start.h>
#include "oled.h"
			
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	/* init OLED */
	OLED_Init();
    OLED_FillAll();
    OLED_Clear();

	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_1, "MicroChip");
	
	STDIO_REDIRECT_0_example();

	/* Replace with your application code */
	while (1) {
	}
}
