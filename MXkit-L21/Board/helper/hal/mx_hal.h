#ifndef _MX_HAL_H_
#define _MX_HAL_H_

#include "mx_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

void mx_hal_ms_ticker_init(void);

uint32_t mx_hal_ms_ticker_read(void);

void mx_hal_delay_ms(volatile uint32_t delay);


void mx_hal_serial_init(int timeout);
void mx_hal_serial_set_timeout(int timeout);
int mx_hal_serial_putc(char c);
int mx_hal_serial_getc(void);
bool mx_hal_serial_readable(void);
void mx_hal_serial_flush(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif