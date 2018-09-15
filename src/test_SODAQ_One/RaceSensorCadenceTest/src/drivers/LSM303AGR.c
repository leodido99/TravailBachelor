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

#define DEBUG DEBUG_LSM303AGR

#include <zephyr.h>
#include <i2c.h>

/* LSM303AGR is connected on i2c bus
 * PA20: INT1_XL
 * PA21: INT2_XL
 */

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

#define LSM303AGR_ACCEL_CFG3_ADDR 0x22

#define LSM303AGR_ACCEL_CFG4_ADDR 0x23
#define LSM303AGR_ACCEL_CFG4_HR_POS 3
#define LSM303AGR_ACCEL_CFG4_FS_POS 4

#define LSM303AGR_ACCEL_INT1_CFG_ADDR 0x30
#define LSM303AGR_ACCEL_INT1_CFG_AOI_POS 7
#define LSM303AGR_ACCEL_INT1_CFG_6D_POS 6
#define LSM303AGR_ACCEL_INT1_CFG_Z_HIGH_POS 5
#define LSM303AGR_ACCEL_INT1_CFG_Z_LOW_POS 4
#define LSM303AGR_ACCEL_INT1_CFG_Y_HIGH_POS 3
#define LSM303AGR_ACCEL_INT1_CFG_Y_LOW_POS 2
#define LSM303AGR_ACCEL_INT1_CFG_X_HIGH_POS 1
#define LSM303AGR_ACCEL_INT1_CFG_X_LOW_POS 0

#define LSM303AGR_ACCEL_INT1_THS_A 0x32

#define LSM303AGR_ACCEL_INT1_DURATION_A 0x33

#define LSM303AGR_ACCEL_STATUS_ADDR 0x27
#define LSM303AGR_ACCEL_STATUS_ZYXOR_POS 7
#define LSM303AGR_ACCEL_STATUS_ZOR_POS 6
#define LSM303AGR_ACCEL_STATUS_YOR_POS 5
#define LSM303AGR_ACCEL_STATUS_XOR_POS 4
#define LSM303AGR_ACCEL_STATUS_ZYXDA_POS 3
#define LSM303AGR_ACCEL_STATUS_ZDA_POS 2
#define LSM303AGR_ACCEL_STATUS_YDA_POS 1
#define LSM303AGR_ACCEL_STATUS_XDA_POS 0

#define LSM303AGR_ACCEL_OUT_X_L_ADDR 0x28
#define LSM303AGR_ACCEL_OUT_X_H_ADDR 0x29

#define LSM303AGR_ACCEL_OUT_Y_L_ADDR 0x2A
#define LSM303AGR_ACCEL_OUT_Y_H_ADDR 0x2B

#define LSM303AGR_ACCEL_OUT_Z_L_ADDR 0x2C
#define LSM303AGR_ACCEL_OUT_Z_H_ADDR 0x2D


/* Magnetometer */
#define LSM303AGR_MAG_WHO_AM_I_REG_ADDR 0x4F


typedef struct {
	struct device *i2c_dev;
	lsm303agr_scales_t scale;
	lsm303agr_modes_t mode;
} lsm303agr_priv_t;

static lsm303agr_priv_t lsm303agr_priv = {
	.scale = LSM303AGR_2G,
	.mode = LSM303AGR_NORMAL_MODE
};

u8_t getAccelScaleMax(lsm303agr_scales_t scale)
{
    return (1 << (scale + 1));
}

double map_double(int16_t x, double in_min, double in_max, double out_min, double out_max)
{
    return ((double)x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*double getScaledAcceleration(int16_t acceleration)
{
	int8_t scale_max;

	printk("%s: %d\n", __func__, acceleration);

	scale_max = getAccelScaleMax(lsm303agr_priv.scale);

	return map_double((int16_t)acceleration, INT16_MIN, INT16_MAX, (double)-scale_max, (double)scale_max);
}*/

double getScaledAcceleration(int16_t acceleration)
{
	double result;

	if (lsm303agr_priv.mode == LSM303AGR_HIGH_RES_MODE) {
		result = acceleration / 16; // 12-bit value

		switch (lsm303agr_priv.scale)
		{
		case LSM303AGR_2G: return (double)result * 1.0f / 1000.0f;
		case LSM303AGR_4G: return (double)result * 2.0f / 1000.0f;
		case LSM303AGR_8G: return (double)result * 4.0f / 1000.0f;
		case LSM303AGR_16G: return (double)result * 12.0f / 1000.0f;
		default:
			break;
		}
	}
	else if (lsm303agr_priv.mode == LSM303AGR_NORMAL_MODE) {
		result = acceleration / 64; // 10-bit value

		switch (lsm303agr_priv.scale)
		{
		case LSM303AGR_2G: return (double)result * 4.0f / 1000.0f;
		case LSM303AGR_4G: return (double)result * 8.0f / 1000.0f;
		case LSM303AGR_8G: return (double)result * 16.0f / 1000.0f;
		case LSM303AGR_16G: return (double)result * 48.0f / 1000.0f;
		default:
			break;
		}
	}
	else if (lsm303agr_priv.mode == LSM303AGR_LOW_PWR_MODE) {
		result = acceleration / 256; // 8-bit value

		switch (lsm303agr_priv.scale)
		{
		case LSM303AGR_2G: return (double)result * 16.0f / 1000.0f;
		case LSM303AGR_4G: return (double)result * 32.0f / 1000.0f;
		case LSM303AGR_8G: return (double)result * 64.0f / 1000.0f;
		case LSM303AGR_16G: return (double)result * 192.0f / 1000.0f;
		default:
			break;
		}
	}

	return 0;
}

double lsm303agr_get_scaled_value(int16_t acceleration)
{
	//printk("%s: %d\n", __func__, acceleration);
	return getScaledAcceleration(acceleration);
}

int lsm303agr_init(const char *device_name)
{
	uint8_t accel_id;
	uint8_t magn_id;
	lsm303agr_priv.i2c_dev = device_get_binding(device_name);
	u32_t i2c_cfg = I2C_SPEED_SET(I2C_SPEED_FAST) | I2C_MODE_MASTER;

	if (!lsm303agr_priv.i2c_dev) {
		DBG_PRINTK("%s: Binding to i2c failed\n", __func__);
		return LSM303AGR_BINDING_FAILED;
	}

	if (i2c_configure(lsm303agr_priv.i2c_dev, i2c_cfg)) {
		DBG_PRINTK("%s: i2c configuration failed\n", __func__);
		return LSM303AGR_CONFIGURATION_FAILED;
	}

	/* Check device IDs */
	accel_id = lsm303agr_accel_get_device_id();
	magn_id = lsm303agr_mag_get_device_id();
	if (accel_id != LSM303AGR_ACCEL_DEVICE_ID ||
	    magn_id != LSM303AGR_MAGN_DEVICE_ID) {
		DBG_PRINTK("%s: Unknown device IDs Accelereometer: 0x%X Magnetometer: 0x%X\n", __func__, accel_id, magn_id);
		return LSM303AGR_UNKNOWN_DEVICE;
	}

	return LSM303AGR_SUCCESS;
}

uint8_t lsm303agr_accel_get_device_id()
{
	uint8_t dev_id = 0xFF;
	if (lsm303agr_priv.i2c_dev) {
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_WHO_AM_I_REG_ADDR, &dev_id)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return 0xFF;
		}
	}

	return dev_id;
}

int lsm303agr_accel_enable(lsm303agr_modes_t mode, lsm303agr_datarate_modes_t data_rate, bool low_pwr_mode, uint8_t axis_enable)
{
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

		lsm303agr_priv.mode = mode;
	} else {
		DBG_PRINTK("%s: I2C bus not initialized\n", __func__);
		return LSM303AGR_NOT_INITIALIZED;
	}

	return LSM303AGR_SUCCESS;
}

int lsm303agr_accel_set_scale(lsm303agr_scales_t scale)
{
	uint8_t reg = 0;
	if (lsm303agr_priv.i2c_dev) {
		reg = scale << LSM303AGR_ACCEL_CFG4_FS_POS;
		if (i2c_reg_update_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_CFG4_ADDR, (1 << LSM303AGR_ACCEL_CFG4_FS_POS), reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}

		lsm303agr_priv.scale = scale;
	} else {
		DBG_PRINTK("%s: I2C bus not initialized\n", __func__);
		return LSM303AGR_NOT_INITIALIZED;
	}

	return LSM303AGR_SUCCESS;
}

int lsm303agr_enable_interrupt(lsm303agr_interrupts_t interrupt)
{
	uint8_t reg = 0;

	if (lsm303agr_priv.i2c_dev) {
		reg = 1 << interrupt;
		if (i2c_reg_write_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_CFG3_ADDR, reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}
	} else {
		DBG_PRINTK("%s: I2C bus not initialized\n", __func__);
		return LSM303AGR_NOT_INITIALIZED;
	}

	return LSM303AGR_SUCCESS;
}

int lsm303agr_configure_interrupt1(lsm303agr_interrupt_modes_t interrupt_mode, uint8_t interrupt_directions)
{
	uint8_t reg = 0;

	if (lsm303agr_priv.i2c_dev) {
		reg = (interrupt_mode << LSM303AGR_ACCEL_INT1_CFG_6D_POS) | (interrupt_directions & 0x3F);
		if (i2c_reg_write_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_INT1_CFG_ADDR, reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}
	} else {
		DBG_PRINTK("%s: I2C bus not initialized\n", __func__);
		return LSM303AGR_NOT_INITIALIZED;
	}

	return LSM303AGR_SUCCESS;
}

int lsm303agr_set_interrupt_threshold(uint8_t value)
{
	if (lsm303agr_priv.i2c_dev) {
		if (i2c_reg_write_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_INT1_THS_A, value)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}
	} else {
		DBG_PRINTK("%s: I2C bus not initialized\n", __func__);
		return LSM303AGR_NOT_INITIALIZED;
	}

	return 0;
}

int lsm303agr_set_interrupt_duration(uint8_t value)
{
	if (lsm303agr_priv.i2c_dev) {
		if (i2c_reg_write_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_INT1_DURATION_A, value)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}
	} else {
		DBG_PRINTK("%s: I2C bus not initialized\n", __func__);
		return LSM303AGR_NOT_INITIALIZED;
	}

	return 0;
}

int lsm303agr_get_z_acceleration(int16_t* acceleration)
{
	uint8_t reg = 0;

	if (lsm303agr_priv.i2c_dev) {
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_OUT_Z_L_ADDR, &reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}

		*acceleration = reg;
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_OUT_Z_H_ADDR, &reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}

		*acceleration |= (reg << 8);
	} else {
		DBG_PRINTK("%s: I2C bus not initialized\n", __func__);
		return LSM303AGR_NOT_INITIALIZED;
	}

	return LSM303AGR_SUCCESS;
}

int lsm303agr_get_scaled_z_acceleration(double *acceleration)
{
	uint16_t accel;
	int err;

	err = lsm303agr_get_z_acceleration(&accel);
	if (err < 0) {
		return err;
	}

	*acceleration = getScaledAcceleration(accel);

	return 0;
}

int lsm303agr_get_y_acceleration(int16_t* acceleration)
{
	uint8_t reg = 0;

	if (lsm303agr_priv.i2c_dev) {
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_OUT_Y_L_ADDR, &reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}

		*acceleration = reg;
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_OUT_Y_H_ADDR, &reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}

		*acceleration |= (reg << 8);
	} else {
		DBG_PRINTK("%s: I2C bus not initialized\n", __func__);
		return LSM303AGR_NOT_INITIALIZED;
	}

	return LSM303AGR_SUCCESS;
}

int lsm303agr_get_scaled_y_acceleration(double *acceleration)
{
	int16_t accel;
	int err;

	err = lsm303agr_get_y_acceleration(&accel);
	if (err < 0) {
		return err;
	}

	*acceleration = getScaledAcceleration(accel);

	return 0;
}

int lsm303agr_get_x_acceleration(int16_t* acceleration) {
	uint8_t reg = 0;

	if (lsm303agr_priv.i2c_dev) {
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_OUT_X_L_ADDR, &reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}

		*acceleration = reg;
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_OUT_X_H_ADDR, &reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}

		*acceleration |= (reg << 8);
	} else {
		DBG_PRINTK("%s: I2C bus not initialized\n", __func__);
		return LSM303AGR_NOT_INITIALIZED;
	}
	return LSM303AGR_SUCCESS;
}

int lsm303agr_get_scaled_x_acceleration(double *acceleration)
{
	int16_t accel;
	int err;

	err = lsm303agr_get_x_acceleration(&accel);
	if (err < 0) {
		return err;
	}

	*acceleration = getScaledAcceleration(accel);

	return 0;
}

int lsm303agr_get_status(lsm303agr_status_t* status) {
	uint8_t reg = 0;

	if (lsm303agr_priv.i2c_dev) {
		if (i2c_reg_read_byte(lsm303agr_priv.i2c_dev, LSM303AGR_ACCEL_ADDR, LSM303AGR_ACCEL_STATUS_ADDR, &reg)) {
			DBG_PRINTK("%s: Register access failed\n", __func__);
			return LSM303AGR_REG_ACCESS_FAILED;
		}
		status->z_y_x_overrun = (reg & (1 << LSM303AGR_ACCEL_STATUS_ZYXOR_POS)) ? true : false;
		status->z_overrun = (reg & (1 << LSM303AGR_ACCEL_STATUS_ZOR_POS)) ? true : false;
		status->y_overrun = (reg & (1 << LSM303AGR_ACCEL_STATUS_YOR_POS)) ? true : false;
		status->x_overrun = (reg & (1 << LSM303AGR_ACCEL_STATUS_XOR_POS)) ? true : false;
		status->z_y_x_available = (reg & (1 << LSM303AGR_ACCEL_STATUS_ZYXDA_POS)) ? true : false;
		status->z_available = (reg & (1 << LSM303AGR_ACCEL_STATUS_ZDA_POS)) ? true : false;
		status->y_available = (reg & (1 << LSM303AGR_ACCEL_STATUS_YDA_POS)) ? true : false;
		status->x_available = (reg & (1 << LSM303AGR_ACCEL_STATUS_XDA_POS)) ? true : false;
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

