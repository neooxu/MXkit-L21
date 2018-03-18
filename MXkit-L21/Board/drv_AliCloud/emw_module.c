

#include "ATCmdParser.h"
#include "emw_api.h"

static char _fw_version[32];

extern void emw_ali_event_handler(void);
extern void emw_wlan_event_handler(void);


mx_status emw_module_reset(void)
{
	if (!(ATCmdParser_send("AT+REBOOT")
	&&  ATCmdParser_recv("OK\n"))) {
		return kGeneralErr;
	}
	mx_delay(500);
	for (int i = 0; i < 2; i++) {
		if (ATCmdParser_send("AT")
		&&  ATCmdParser_recv("OK\n")
		&&  ATCmdParser_send("AT+UARTE=ON")
		&&  ATCmdParser_recv("OK\n")) {
			return kNoErr;
		}
	}
	return kGeneralErr;
}

mx_status emw_module_init(void)
{
	ATCmdParser_init("\r\n", 1000, false);
		
	for (int i = 0; i < 2; i++) {
		if ( kNoErr == emw_module_reset()) {
			ATCmdParser_add_oob("+ALINKEVENT:",	emw_ali_event_handler);
			ATCmdParser_add_oob("+WEVENT:",		emw_wlan_event_handler);
			return kNoErr;
		}
	}
		
	return kGeneralErr;
}

mx_status emw_module_restore_settings(void)
{
	if (!(ATCmdParser_send("AT+FACTORY")
	&&  ATCmdParser_recv("OK\n"))) {
		return kGeneralErr;
	}
	
	for (int i = 0; i < 2; i++) {
		if ( kNoErr == emw_module_reset()) {
			return kNoErr;
		}
	}	
		
	return kGeneralErr;
}

const char *emw_module_get_fw_version(void)
{
	if (!(ATCmdParser_send("AT+FWVER?")
       && ATCmdParser_recv("+FWVER:%32[^\r]\r\nOK\r\n", _fw_version))) {
        return NULL;
    }	
	return _fw_version;
}

uint32_t emw_module_get_tick(void)
{
	uint32_t tick;
	if (!(ATCmdParser_send("AT+SYSTIME?")
	   && ATCmdParser_recv("+SYSTIME:%d\r\nOK\r\n", &tick))) {
		return 0;
	}
	return tick;	
}