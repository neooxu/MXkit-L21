

#include "ATCmdParser.h"
#include "emw_api.h"

static char _ssid[33];
static char _pwd[33];


mx_status emw_wlan_get_para(char ssid[33], char pwd[33])
{
	char args[100], *arg_list[2];

	if (!(ATCmdParser_send("AT+WJAP=?")
	   && ATCmdParser_recv("+WJAP:%100[^\r]\r\n",args)
	   && ATCmdParser_recv("OK\r\n"))) {
		return kReadErr;
	}
	
	if (2 != ATCmdParser_analyse_args(args, arg_list, 2)) {
		return kMalformedErr;
	};
	if (ssid) strcpy(ssid, arg_list[0]);
	if (pwd)  strcpy(pwd,  arg_list[1]);
	
	return kNoErr;
}

emw_arg_wlan_sta_e emw_wlan_get_sta_status(void)
{
	char arg[20];

	if (!(ATCmdParser_send("AT+WJAPS")
	   && ATCmdParser_recv("+WJAPS:%20[^\r]\r\n",arg)
	   && ATCmdParser_recv("OK\r\n"))) {
		return EMW_ARG_ERR;
	}
	
	return emw_arg_for_arg( EMW_ARG_WLAN_STA, arg);
}

void emw_wlan_event_handler(void)
{
	mx_status err = kNoErr;
	char arg[15];

	// parse out the packet
	require_action(ATCmdParser_recv("%100[^\r]\r\n", arg), exit, err = kMalformedErr);
		
	emw_arg_wlan_ev_e event = emw_arg_for_arg(EMW_ARG_WLAN_EV, arg);
	require_action(event != EMW_ARG_ERR, exit,  err = kMalformedErr);
	
	emw_ev_wlan(event);
	
exit:
	if (err == kMalformedErr) emw_ev_unknown();
	return;
}
