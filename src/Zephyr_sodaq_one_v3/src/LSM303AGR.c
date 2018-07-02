/** 
 * @file LSM303AGR.c
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 2, 2018
 */

#include "LSM303AGR.h"

#include "debug.h"

#include <zephyr.h>
#include <i2c.h>

/**
 * i2c address for the LSM303AGR Accelerometer
 */
#define LSM303AGR_ACCEL_ADDR 0x19

/**
 * i2c address for the LSM303AGR Magnetometer
 */
#define LSM303AGR_MAGN_ADDR 0x1E

/**
 * Register addresses
 */
#define LSM303AGR_ACCEL_WHO_AM_I_REG_ADDR 0x0F



typedef struct {
	struct device *i2c_dev;

} lsm303agr_priv_t;

static lsm303agr_priv_t lsm303agr_priv;

int lsm303agr_init(const char *device_name) {
	lsm303agr_priv.i2c_dev = device_get_binding(device_name);
	u32_t i2c_cfg = I2C_SPEED_SET(I2C_SPEED_STANDARD) | I2C_MODE_MASTER;

	if (!lsm303agr_priv.i2c_dev) {
		DBG_PRINTK("%s: Binding to i2c failed\n", __func__);
		return LSM303AGR_BINDING_FAILED;
	}

	if (i2c_configure(lsm303agr_priv.i2c_dev, i2c_cfg)) {
		DBG_PRINTK("%s: i2c configuration failed\n", __func__);
		return LSM303AGR_CONFIGURATION_FAILED;
	}

	return LSM303AGR_SUCCESS;
}

uint8_t lsm303agr_accel_get_device_id() {
	uint8_t dev_id = 0xFF;
	if (lsm303agr_priv.i2c_dev) {
		printk("lsm303agr_accel_get_device_id\n");
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_WHO_AM_I_REG_ADDR, &dev_id)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return 0xFF;
		}
	}
	return dev_id;
}

