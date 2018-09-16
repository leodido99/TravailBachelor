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
 * Module status
 */
typedef struct {
	bool z_y_x_overrun;
	bool z_overrun;
	bool y_overrun;
	bool x_overrun;
	bool z_y_x_available;
	bool z_available;
	bool y_available;
	bool x_available;
} lsm303agr_status_t;

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

typedef enum {
	LSM303AGR_2G,
	LSM303AGR_4G,
	LSM303AGR_8G,
	LSM303AGR_16G
} lsm303agr_scales_t;

typedef enum {
	LSM303AGR_INT_OR_COMBINATION,
	LSM303AGR_INT_MOVEMENT_RECOG,
	LSM303AGR_INT_AND_COMBINATION,
	LSM303AGR_INT_POSITION_RECOG
} lsm303agr_interrupt_modes_t;

typedef enum {
	LSM303AGR_INT_DIR_Z_HIGH = 32,
	LSM303AGR_INT_DIR_Z_LOW = 16,
	LSM303AGR_INT_DIR_Y_HIGH = 8,
	LSM303AGR_INT_DIR_Y_LOW = 4,
	LSM303AGR_INT_DIR_X_HIGH = 2,
	LSM303AGR_INT_DIR_X_LOW = 1,
} lsm303agr_interrupt_directions_t;

typedef enum {
	LSM303AGR_INT_CLICK = 7,
	LSM303AGR_INT_AOI1 = 6,
	LSM303AGR_INT_AOI2 = 5,
	LSM303AGR_INT_DRDY1 = 4,
	LSM303AGR_INT_DRDY2 = 3,
	LSM303AGR_INT_WTM = 2,
	LSM303AGR_INT_OVERRUN = 1
} lsm303agr_interrupts_t;

/**
 * Initialize the LSM303AGR Magnetometer and accelerometer module
 * @param device_name I2C device name
 * @return 0 on success, error code otherwise 0 on success, error code otherwise
 */
int lsm303agr_init(const char *device_name);

/**
 * Returns the accelerometer device ID
 * @return 0 on success, error code otherwise 0 on success, error code otherwise
 */
uint8_t lsm303agr_accel_get_device_id();

/**
 * Enable the LSM303AGR Accelerometer
 * @param mode Mode of operation
 * @param data_rate Data rate
 * @param low_pwr_mode Low power mode
 * @param axis_enable Axis to enable
 * @return 0 on success, error code otherwise 0 on success, error code otherwise
 */
int lsm303agr_accel_enable(lsm303agr_modes_t mode, lsm303agr_datarate_modes_t data_rate, bool low_pwr_mode, uint8_t axis_enable);

/**
 * Sets the accelerometer scale
 * @param scale
 * @return 0 on success, error code otherwise 0 on success, error code otherwise
 */
int lsm303agr_accel_set_scale(lsm303agr_scales_t scale);

/**
 * Configure interrupt 1 generation
 * @param interrupt_mode
 * @param interrupt_directions
 * @return 0 on success, error code otherwise
 */
int lsm303agr_configure_interrupt1(lsm303agr_interrupt_modes_t interrupt_mode, uint8_t interrupt_directions);

/**
 * Sets the threshold at which the interrupt triggers
 * @param value Threshold value see data sheet
 * @return 0 on success, error code otherwise
 */
int lsm303agr_set_interrupt_threshold(uint8_t value);

/**
 * Sets the duration of the interrupt
 * @param value Duration value see data sheet
 * @return 0 on success, error code otherwise
 */
int lsm303agr_set_interrupt_duration(uint8_t value);

/**
 * Enable the interrupt generation
 * @param interrupt
 * @return 0 on success, error code otherwise
 */
int lsm303agr_enable_interrupt(lsm303agr_interrupts_t interrupt);

/**
 * Converts raw value to scaled value
 * @param acceleration Acceleration as retrieved with lsm303agr_get_(x/y/z)_acceleration
 * @return
 */
double lsm303agr_get_scaled_value(int16_t acceleration);

/**
 * Returns the raw Z axis acceleration
 * @param acceleration
 * @return 0 on success, error code otherwise
 */
int lsm303agr_get_z_acceleration(int16_t* acceleration);

/**
 * Returns the Z axis in Gs
 * @param acceleration acceleration
 * @return 0 on success, error code otherwise
 */
int lsm303agr_get_scaled_z_acceleration(double *acceleration);

/**
 * Returns the Y axis acceleration
 * @param acceleration
 * @return 0 on success, error code otherwise
 */
int lsm303agr_get_y_acceleration(int16_t* acceleration);

/**
 * Returns the Y axis in Gs
 * @param acceleration acceleration
 * @return 0 on success, error code otherwise
 */
int lsm303agr_get_scaled_y_acceleration(double *acceleration);

/**
 * Returns the X axis acceleration
 * @param acceleration
 * @return 0 on success, error code otherwise
 */
int lsm303agr_get_x_acceleration(int16_t* acceleration);

/**
 * Returns the X axis in Gs
 * @param acceleration acceleration
 * @return 0 on success, error code otherwise
 */
int lsm303agr_get_scaled_x_acceleration(double *acceleration);

/**
 * Returns the status of the accelerometer
 * @param status
 * @return 0 on success, error code otherwise
 */
int lsm303agr_get_status(lsm303agr_status_t* status);

/**
 * Returns the magnetometerdevice ID
 * @return 0 on success, error code otherwise
 */
uint8_t lsm303agr_mag_get_device_id();

#endif /* SRC_LSM303AGR_H_ */
