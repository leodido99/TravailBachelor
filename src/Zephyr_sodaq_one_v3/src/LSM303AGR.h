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

#define LSM303AGR_SUCCESS 0
#define LSM303AGR_BINDING_FAILED -1
#define LSM303AGR_CONFIGURATION_FAILED -2
#define LSM303AGR_NOT_INITIALIZED -3
#define LSM303AGR_REG_ACCESS_FAILED -4

/**
 * Initialize the LSM303AGR Magnetometer and accelerometer module
 * @param device_name I2C device name
 * @return
 */
int lsm303agr_init(const char *device_name);

/**
 * Returns the device ID
 * @return
 */
uint8_t lsm303agr_accel_get_device_id();


#endif /* SRC_LSM303AGR_H_ */
