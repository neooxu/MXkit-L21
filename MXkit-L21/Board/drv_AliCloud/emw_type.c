#include "emw_type.h"



const char* EMW_TYPE_ALI_FORMAT[] =
{
	[EMW_TYPE_ALI_FORMAT_JSON] = "JSON",
	[EMW_TYPE_ALI_FORMAT_RAW]  = "RAW",
	[EMW_TYPE_ALI_FORMAT_MAX]  = "\0",
};

const char* EMW_TYPE_ALI_CONN[] =
{
	[EMW_TYPE_ALI_CONN_CONNECTED]     = "CONNECT",
	[EMW_TYPE_ALI_CONN_DISCONNECTED]  = "DISCONNECT",
	[EMW_TYPE_ALI_CONN_MAX]           = "\0",
};

const char* EMW_TYPE_ALI_EV[] =
{
	[EMW_TYPE_ALI_EV_ALINK]     = "ALINK",
	[EMW_TYPE_ALI_EV_GET]       = "GET",
	[EMW_TYPE_ALI_EV_SET]       = "SET",
	[EMW_TYPE_ALI_EV_MAX]       = "\0",
};

const char* EMW_TYPE_WLAN_EV[] =
{
	[EMW_TYPE_WLAN_EV_UAP_ESTABLISHED]     = "AP_UP",
	[EMW_TYPE_WLAN_EV_UAP_DESTROYED]       = "AP_DOWN",
	[EMW_TYPE_WLAN_EV_STA_CONNECTED]       = "STATION_UP",
	[EMW_TYPE_WLAN_EV_STA_DISCONNECTED]    = "STATION_DOWN",
	[EMW_TYPE_WLAN_EV_MAX]                 = "\0",
};

const char* EMW_TYPE_WLAN_STA[] =
{
	[EMW_TYPE_WLAN_STA_CONNECTED]       = "STATION_UP",
	[EMW_TYPE_WLAN_STA_DISCONNECTED]    = "STATION_DOWN",
	[EMW_TYPE_WLAN_STA_DISCONNECTED]    = "CONNECTING",
	[EMW_TYPE_WLAN_STA_MAX]             = "\0",
};



const char* emw_arg_for_type(char* list[], uint8_t type)
{
	return list[type];
}

uint8_t emw_type_for_arg( char* list[], char *arg)
{
	for (uint8_t i = 0; i < EMW_TYPE_ERR; i++) {
		if (*list[i] == 0x0) break;
		if (memcmp(list[i], arg, strlen(list[i])) == 0) {
			return i;
		}
		
	}
	return EMW_TYPE_ERR;
}


