#include <stdio.h>
#include "mx_hal.h"

#include <driver_init.h>
#include <hal_i2c_m_sync.h>

extern struct i2c_m_sync_desc I2C_0;


struct _i2c_instance {
    uint32_t place_holder;
};


/* NULL handler for this driver, complete in future */
static struct _i2c_instance i2c;

void* mx_hal_i2c_init(void *config)
{
    UNUSED_PARAMETER(config);

	I2C_0_init();
	i2c_m_sync_set_baudrate(&I2C_0, 0, 100);
	i2c_m_sync_enable(&I2C_0);

    return &i2c;
}


/**
 * \brief Sync version of i2c write command
 */
int32_t mx_hal_i2c_cmd_write(void *instance, uint16_t slave_addr, uint8_t reg, uint8_t *buffer, uint8_t length)
{
    UNUSED_PARAMETER(instance);
	
    mx_status err = kNoErr;
	i2c_m_sync_set_slaveaddr(&I2C_0, slave_addr, 1);
	err = i2c_m_sync_cmd_write(&I2C_0, reg, buffer, length);
	require_noerr(err, exit);
	
exit:
	return err;	
}

int32_t mx_hal_i2c_cmd_read(void *instance, uint16_t slave_addr, uint8_t reg, uint8_t *buffer, uint8_t length)
{
	UNUSED_PARAMETER(instance);

	mx_status err = kNoErr;
	i2c_m_sync_set_slaveaddr(&I2C_0, slave_addr, 1);
		
	err = i2c_m_sync_cmd_read(&I2C_0, reg, buffer, length);
	require_noerr(err, exit);

exit:
	return err;
}
