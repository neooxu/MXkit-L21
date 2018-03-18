#include <atmel_start.h>
#include "mx_debug.h"
#include "cloud_chat.h"

#define APP_DEBUG MX_DEBUG_ON
#define app_log(M, ...) MX_LOG(APP_DEBUG, "APP", M, ##__VA_ARGS__)
			
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	//board_test();
	//ATCmdParser_test();
	board_init();
	cc_init();
	
	while(1)
	{
		/* Application tick */
		cc_tick();
		ATCmdParser_process_oob();
	}
}
