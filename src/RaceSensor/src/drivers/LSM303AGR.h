/** 
 * @file LSM303AGR.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 2, 2018
 */
#ifndef SRC_LSM303AGR_H_
#define SRC_LSM303AGR_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * Return types of the LSM303AGR functions
 */
#define LSM303AGR_SUCCESS 0
#define LSM303AGR_BINDING_FAILED -1
#define LSM303AGR_CONFIGURATION_FAILED -2
#define LSM303AGR_NOT_INITIALIZED -3
#define LSM303AGR_REG_ACCESS_FAILED -4
#define LSM303AGR_UNKNOWN_DEVICE -5

/**
 * Device mode
 */
typedef enum {
	LSM303AGR_NORMAL_MODE,
	LSM303AGR_HIGH_RES_MODE,
	LSM303AGR_LOW_PWR_MODE
} lsm303agr_modes_t;

/**
 * Data rate and operating modes of the accelerometer
 */
typedef enum {
	LSM303AGR_PWR_DOWN_MODE,
	LSM303AGR_HIGH_RES_1HZ,
	LSM303AGR_HIGH_RES_10HZ,
	LSM303AGR_HIGH_RES_25HZ,
	LSM303AGR_HIGH_RES_50HZ,
	LSM303AGR_HIGH_RES_100HZ,
	LSM303AGR_HIGH_RES_200HZ,
	LSM303AGR_HIGH_RES_400HZ,
	LSM303AGR_LOW_PWR_MODE_1620HZ,
	LSM303AGR_HIGH_RES_5376HZ
} lsm303agr_datarate_modes_t;

/* Accelerometer axes */
#define LSM303AGR_Z_AXIS (1 << 2)
#define LSM303AGR_Y_AXIS (1 << 1)
#define LSM303AGR_X_AXIS (1 << 0)

/**
 * Initialize the LSM303AGR Magnetometer and accelerometer module
 * @param device_name I2C device name
 * @return
 */
int lsm303agr_init(const char *device_name);

/**
 * Returns the accelerometer device ID
 * @return
 */
uint8_t lsm303agr_accel_get_device_id();

/**
 * Enable the LSM303AGR Accelerometer
 */
int lsm303agr_accel_enable(lsm303agr_modes_t mode, lsm303agr_datarate_modes_t data_rate, bool low_pwr_mode, uint8_t axis_enable);

/**
 * Returns the magnetometerdevice ID
 * @return
 */
uint8_t lsm303agr_mag_get_device_id();

#endif /* SRC_LSM303AGR_H_ */
