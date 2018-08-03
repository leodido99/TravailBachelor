/** 
 * @file race_sensor_manager.h
 * @brief Race Sensor Manager module
 *
 * The Race Sensor Manager module is responsible for periodically
 * performing the required acquisitions, creating and sending the
 * Race Tracking Packet containing the information about the competitor
 *
 * @author Léonard Bise
 * @date   Jul 23, 2018
 */
#ifndef SRC_RACE_SENSOR_MANAGER_H_
#define SRC_RACE_SENSOR_MANAGER_H_

#include <zephyr.h>

/**
 * Statuses of the race sensor manager
 */
#define RACE_SENSOR_MNGR_SUCCESS 0
#define RACE_SENSOR_MNGR_NOT_INITIALIZED -1
#define RACE_SENSOR_MNGR_LORA_DEVICE_FAILURE -2
#define RACE_SENSOR_MNGR_GPS_DEVICE_FAILURE -3
#define RACE_SENSOR_MNGR_ACCEL_DEVICE_FAILURE -4

/**
 * Initialze the race sensor manager
 * @return One of race sensor manager statuses
 */
int race_sensor_mngr_init(void);

/**
 * Starts the race sensor manager
 */
int race_sensor_mngr_start(void);

/**
 * Sets the heart rate sent in the packets
 * @param heart_rate Heart Rate
 */
void race_sensor_mngr_set_heart_rate(u8_t heart_rate);

/**
 * Sets the cadence sent in the packets
 * @param cadence Cadence
 */
void race_sensor_mngr_set_cadence(u8_t cadence);

#endif /* SRC_RACE_SENSOR_MANAGER_H_ */
