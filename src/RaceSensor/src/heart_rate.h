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

#define HR_SUCCESS 0
#define HR_BINDING_FAILED -1
#define HR_GPIO_ACCESSS_FAILED -2

int hr_init(const char* device, int pin);



#endif /* SRC_HEART_RATE_H_ */
