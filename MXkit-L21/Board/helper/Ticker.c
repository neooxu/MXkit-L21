
#include <stdio.h>
#include <hpl_core.h>
#include <peripheral_clk_config.h>
#include "mx_common.h"

static volatile uint32_t _ticker_count = 0;

void SysTick_Handler(void)
{
	_ticker_count++;
}

void ms_ticker_init(void)
{
	SysTick_Config( CONF_CPU_FREQUENCY / 1000 );
}

uint32_t ms_ticker_read(void)
{
	return _ticker_count;
}

MX_WEAK void mx_delay(volatile uint32_t delay)
{
	uint32_t tickstart = 0;
	tickstart = ms_ticker_read();
	while((ms_ticker_read() - tickstart) < delay);
}