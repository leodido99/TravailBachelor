/** 
 * @file UBloxEVA8M.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */
#ifndef SRC_UBLOXEVA8M_H_
#define SRC_UBLOXEVA8M_H_

#include <zephyr.h>

#define UBLOXEVA8M_SUCCESS 0
#define UBLOXEVA8M_BINDING_FAILED -1
#define UBLOXEVA8M_CONFIGURATION_FAILED -2
#define UBLOXEVA8M_GPIO_ACCESS_FAILED -3
#define UBLOXEVA8M_NOT_INITIALIZED -4
#define UBLOXEVA8M_I2C_ACCESS_FAILED -5
#define UBLOXEVA8M_NOT_ANSWERING -6
#define UBLOXEVA8M_NULL_POINTER -7
#define UBLOXEVA8M_NACK -8

/**
 * Initialze the UBlox EVA 8M module
 * @param device_name I2C device name
 * @return
 */
int ubloxeva8m_init(const char *device_name);

/**
 * Powers on the UBlox EVA 8M device
 * @return
 */
int ubloxeva8m_power_on();

/**
 * Powers off the UBlox EVA 8M device
 * @return
 */
int ubloxeva8m_power_off();

/**
 * Starts the module
 * @return
 */
int ubloxeva8m_start();

/**
 * Returns if the module is answering on the i2c bus
 * @return
 */
bool ubloxeva8m_is_alive();

#endif /* SRC_UBLOXEVA8M_H_ */
