/** 
 * @file leds.h
 * @brief LED Driver
 *
 * Provides a driver to manipulate the LEDs
 *
 * @author Léonard Bise
 * @date   Jul 26, 2018
 */
#ifndef SRC_DRIVERS_LEDS_H_
#define SRC_DRIVERS_LEDS_H_

#include <zephyr.h>

#define LEDS_SUCCESS 0
#define LEDS_BINDING_FAILED -1
#define LEDS_UNKNOWN_LED -2

enum leds_t {
	LED_RED,
	LED_GREEN,
	LED_BLUE
};

/**
 * Initialize the leds
 * @return
 */
int leds_init();

/**
 * Switch-on or off a LED
 * @param led LED to set
 * @param enable true = switch-on, false = switch-off
 * @return
 */
int leds_set(enum leds_t led, bool enable);

#endif /* SRC_DRIVERS_LEDS_H_ */
