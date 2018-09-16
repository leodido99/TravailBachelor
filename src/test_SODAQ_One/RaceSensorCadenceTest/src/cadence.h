/*
 * cadence.h
 *
 *  Created on: 17 Aug 2018
 *      Author: leonard.bise
 */

#ifndef SRC_CADENCE_H_
#define SRC_CADENCE_H_

#include <zephyr.h>

#define CADENCE_SUCCESS 0
#define CADENCE_BINDING_FAILED -1
#define CADENCE_CONFIGURATION_ERROR -2

/**
 * Initialize the cadence module
 * @param device Device on which the LSM303AGR is connected
 * @param irq_device Device on which the IRQ is connected (GPIO port)
 * @param irq_pin Pin of the GPIO port on which IRQ is
 * @return 0 if success, negative errorno otherwise
 */
int cadence_init(const char* device, const char *irq_device, int irq_pin);

/**
 * Return the cadence in step per min
 * @return Cadence
 */
u8_t cadence_get(void);

void cadence_print(void);

void cadence_reset_samples(void);

#endif /* SRC_CADENCE_H_ */
