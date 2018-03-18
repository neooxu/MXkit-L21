
#include <stdio.h>
#include <hpl_core.h>
#include <peripheral_clk_config.h>

uint32_t _ticker_count = 0;

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
