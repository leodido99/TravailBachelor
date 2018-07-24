/** 
 * @file packet_manager.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 23, 2018
 */
#ifndef SRC_PACKET_MANAGER_H_
#define SRC_PACKET_MANAGER_H_

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


#endif /* SRC_PACKET_MANAGER_H_ */
