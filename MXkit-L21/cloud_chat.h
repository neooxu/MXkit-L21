
#ifndef _CLOUD_CHAT_H_
#define _CLOUD_CHAT_H_


#define APP_DEBUG MX_DEBUG_ON
#define cc_log(M, ...) MX_LOG(APP_DEBUG, "CC", M, ##__VA_ARGS__)

typedef enum
{
	eState_M1_initialize           = 1,
	eState_M2_provision            = 2,
	eState_M3_normal               = 3,
	eState_M4_disconnected         = 4,
} cc_device_state_e;

typedef struct {
	cc_device_state_e device_state;
} cc_context_t;

uint8_t cc_init(void);
//void Process_InputData(uint8_t* data_buffer, uint16_t Nb_bytes);
mx_status cc_tick(void);

#endif
