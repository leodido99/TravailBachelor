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
 * Device ID for the LSM303AGR Accelerometer
 */
#define LSM303AGR_ACCEL_DEVICE_ID 0x33
/**
 * i2c address for the LSM303AGR Magnetometer
 */
#define LSM303AGR_MAGN_ADDR 0x1E

/**
 * Device ID for the LSM303AGR Magnetometer
 */
#define LSM303AGR_MAGN_DEVICE_ID 0x40

/**
 * Register addresses
 */
/* Accelerometer */
#define LSM303AGR_ACCEL_WHO_AM_I_REG_ADDR 0x0F
#define LSM303AGR_ACCEL_CFG1_ADDR 0x20
#define LSM303AGR_ACCEL_CFG_ODR_POS 4
#define LSM303AGR_ACCEL_CFG_LPEN_POS 3
#define LSM303AGR_ACCEL_CFG_ZEN_POS 2
#define LSM303AGR_ACCEL_CFG_YEN_POS 1
#define LSM303AGR_ACCEL_CFG_XEN_POS 0

#define LSM303AGR_ACCEL_CFG4_ADDR 0x23
#define LSM303AGR_ACCEL_CFG4_HR_POS 3

/* Magnetometer */
#define LSM303AGR_MAG_WHO_AM_I_REG_ADDR 0x4F


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

	/* Check device IDs */
	if (lsm303agr_accel_get_device_id() != LSM303AGR_ACCEL_DEVICE_ID ||
			lsm303agr_mag_get_device_id() != LSM303AGR_MAGN_DEVICE_ID) {
		DBG_PRINTK("%s: Unknown device IDs Accelereometer: 0x%X Magnetometer: 0x%X\n", __func__, lsm303agr_accel_get_device_id(), lsm303agr_mag_get_device_id());
		return LSM303AGR_UNKNOWN_DEVICE;
	}

	return LSM303AGR_SUCCESS;
}

uint8_t lsm303agr_accel_get_device_id() {
	uint8_t dev_id = 0xFF;
	if (lsm303agr_priv.i2c_dev) {
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_WHO_AM_I_REG_ADDR, &dev_id)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return 0xFF;
		}
	}
	return dev_id;
}

int lsm303agr_accel_enable(lsm303agr_modes_t mode, lsm303agr_datarate_modes_t data_rate, bool low_pwr_mode, uint8_t axis_enable) {
	uint8_t reg = 0;
	if (lsm303agr_priv.i2c_dev) {
		reg = (data_rate << LSM303AGR_ACCEL_CFG_ODR_POS) | ((low_pwr_mode ? 1 : 0) << LSM303AGR_ACCEL_CFG_LPEN_POS) | axis_enable;
		if (i2c_reg_write_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_CFG1_ADDR, reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}
		/* Set operating mode */
		if (mode & 0x1) {
			reg = 1 << LSM303AGR_ACCEL_CFG4_HR_POS;
		} else {
			reg = 0;
		}
		if (i2c_reg_update_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_CFG4_ADDR, (1 << LSM303AGR_ACCEL_CFG4_HR_POS), reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}
	} else {
		DBG_PRINTK("%s: I2C bus not initialized\n", __func__);
		return LSM303AGR_NOT_INITIALIZED;
	}
	return LSM303AGR_SUCCESS;
}

uint8_t lsm303agr_mag_get_device_id() {
	uint8_t dev_id = 0xFF;
	if (lsm303agr_priv.i2c_dev) {
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_MAGN_ADDR, LSM303AGR_MAG_WHO_AM_I_REG_ADDR, &dev_id)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return 0xFF;
		}
	}
	return dev_id;
}

