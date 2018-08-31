/**
 * @file heart_rate.h
 * @brief Heart Rate module
 *
 * Handles the IO associated with the Heart Rate module
 *
 * @author Léonard Bise
 * @date   Jul 23, 2018
 */

#ifndef SRC_HEART_RATE_H_
#define SRC_HEART_RATE_H_

#include <zephyr.h>

#define HR_SUCCESS 0
#define HR_BINDING_FAILED -1
#define HR_GPIO_ACCESSS_FAILED -2

/**
 * Initialize the heart rate module
 * Configures the GPIO and add an interruption that is
 * triggered on rising edges
 *
 * @param device GPIO device
 * @param pin GPIO pin
 * @return 0 if successful, error code otherwise
 */
int hr_init(const char* device, int pin);

/**
 * Returns the heart rate in beats per minutes
 *
 * @return Heart rate
 */
u8_t hr_get(void);

#endif /* SRC_HEART_RATE_H_ */
