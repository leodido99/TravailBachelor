/** 
 * @file packet_manager.h
 * @brief Packet Manager module
 *
 * The Packet Manager module is responsible for periodically
 * creating and sending the Race Tracking Packet containing
 * the information about the competitor
 *
 * @author Léonard Bise
 * @date   Jul 23, 2018
 */
#ifndef SRC_PACKET_MANAGER_H_
#define SRC_PACKET_MANAGER_H_

#include <zephyr.h>

/**
 * Statuses of the packet manager
 */
#define PKT_MNGR_SUCCESS 0
#define PKT_MNGR_NOT_INITIALIZED -1
#define PKT_MNGR_LORA_DEVICE_FAILURE -2
#define PKT_MNGR_GPS_DEVICE_FAILURE -3
#define PKT_MNGR_ACCEL_DEVICE_FAILURE -4

/**
 * Initialze the packet manager
 * @return One of packet manager statuses
 */
int pkt_mngr_init(void);

/**
 * Starts the packet manager
 */
int pkt_mngr_start(void);

/**
 * Sets the heart rate sent in the packets
 * @param heart_rate Heart Rate
 */
void pkt_mngr_set_heart_rate(u8_t heart_rate);

/**
 * Sets the cadence sent in the packets
 * @param cadence Cadence
 */
void pkt_mngr_set_cadence(u8_t cadence);

#endif /* SRC_PACKET_MANAGER_H_ */
