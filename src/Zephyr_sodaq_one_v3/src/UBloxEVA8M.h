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
#include "UBloxEVA8M_protocol.h"

#define UBLOXEVA8M_SUCCESS 0
#define UBLOXEVA8M_BINDING_FAILED -1
#define UBLOXEVA8M_CONFIGURATION_FAILED -2
#define UBLOXEVA8M_GPIO_ACCESS_FAILED -3
#define UBLOXEVA8M_NOT_INITIALIZED -4
#define UBLOXEVA8M_I2C_ACCESS_FAILED -5
#define UBLOXEVA8M_NOT_ANSWERING -6
#define UBLOXEVA8M_NULL_POINTER -7
#define UBLOXEVA8M_NACK -8
#define UBLOXEVA8M_BUSY -9

/**
 * Size of a message payload
 */
#define UBLOXEVA8M_MSG_PAYLOAD_BUFFER_SIZE 128

/**
 * Structure used to store a decoded message
 */
typedef struct __attribute__((packed,aligned(1))) {
	uint8_t class_id;
	uint8_t message_id;
	uint16_t length;
	uint8_t payload[UBLOXEVA8M_MSG_PAYLOAD_BUFFER_SIZE];
	uint16_t checksum;
	uint16_t stored_length;
} ubloxeva8m_ubx_msg;

/**
 * The dynamic models that can be configured
 */
typedef enum {
	UBLOXEVA8M_DYNMODEL_PORTABLE,
	UBLOXEVA8M_DYNMODEL_STATIONARY = 2,
	UBLOXEVA8M_DYNMODEL_PEDESTRIAN,
	UBLOXEVA8M_DYNMODEL_AUTOMOTIVE,
	UBLOXEVA8M_DYNMODEL_AT_SEA,
	UBLOXEVA8M_DYNMODEL_AIRBORNE_1G,
	UBLOXEVA8M_DYNMODEL_AIRBORNE_2G,
	UBLOXEVA8M_DYNMODEL_AIRBORNE_4G,
	UBLOXEVA8M_DYNMODEL_WRIST
} ubloxeva8m_dynamic_models;

/**
 * Type used for the callbacks
 * @param msg Message received
 */
typedef void (*ubloxeva8m_msg_callback)(ubloxeva8m_ubx_msg* msg);

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

/**
 * Sets a callback function on message received
 */
void ubloxeva8m_set_callback(ubloxeva8m_msg_callback handler);

/**
 * Sets the dynamic model to be used by the module
 * @param dyn_model
 * @return
 */
int ubloxeva8m_set_dynamic_model(ubloxeva8m_dynamic_models dyn_model);

#endif /* SRC_UBLOXEVA8M_H_ */
