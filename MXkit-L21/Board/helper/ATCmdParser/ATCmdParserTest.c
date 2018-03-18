
#include <stdio.h>
#include "ATCmdParser.h"

char* return_str;

void ATCmdParser_test(void)
{
	mx_status ret = kNoErr;
	
	board_init();
	
	ret = emw_module_init();
	if (ret == kNoErr) printf("module init success\r\n");
	
	printf("FW version: %s\r\n", emw_module_get_fw_version());
	printf("System tick: %d\r\n", emw_module_get_tick());

	while(1);
}
