/*
 * cadence.h
 *
 *  Created on: 17 Aug 2018
 *      Author: leonard.bise
 */

#ifndef SRC_CADENCE_H_
#define SRC_CADENCE_H_

#include <zephyr.h>

/**
 * Initialize the cadence modle
 * @param device Device on which the LSM303AGR is connected
 * @return 0 if success, negative errorno otherwise
 */
int cadence_init(const char* device);

/**
 * Return the cadence in step per min
 * @return Cadence
 */
u8_t cadence_get(void);

#endif /* SRC_CADENCE_H_ */
