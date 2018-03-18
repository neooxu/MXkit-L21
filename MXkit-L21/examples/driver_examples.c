/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

void I2C_0_example(void)
{
	struct io_descriptor *I2C_0_io;

	i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_0_io);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, 0x12, I2C_M_SEVEN);
	io_write(I2C_0_io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using TARGET_IO to write "Hello World" using the IO abstraction.
 */
void TARGET_IO_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&TARGET_IO, &io);
	usart_sync_enable(&TARGET_IO);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using USART_AT to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_USART_AT[12] = "Hello World!";

static void tx_cb_USART_AT(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void USART_AT_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&USART_AT, USART_ASYNC_TXC_CB, tx_cb_USART_AT);
	/*usart_async_register_callback(&USART_AT, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&USART_AT, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&USART_AT, &io);
	usart_async_enable(&USART_AT);

	io_write(io, example_USART_AT, 12);
}

/**
 * Example of using PWM_B.
 */
void PWM_B_example(void)
{
	pwm_set_parameters(&PWM_B, 10000, 5000);
	pwm_enable(&PWM_B);
}

/**
 * Example of using PWM_R.
 */
void PWM_R_example(void)
{
	pwm_set_parameters(&PWM_R, 10000, 5000);
	pwm_enable(&PWM_R);
}

/**
 * Example of using PWM_G.
 */
void PWM_G_example(void)
{
	pwm_set_parameters(&PWM_G, 10000, 5000);
	pwm_enable(&PWM_G);
}
