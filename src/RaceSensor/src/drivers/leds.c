/** 
 * @file leds.c
 * @brief LED Driver
 *
 * Provides a driver to manipulate the LEDs
 *
 * @author Léonard Bise
 * @date   Jul 26, 2018
 */

#include "leds.h"

#include <board.h>
#include <device.h>
#include <gpio.h>

#include "debug.h"
#define DEBUG DEBUG_LEDS

struct leds_private_t {
	struct device *led_red;
	struct device *led_green;
	struct device *led_blue;
};

struct leds_private_t leds_private;

int leds_init()
{
	int err;

	leds_private.led_red = device_get_binding(LED0_GPIO_PORT);
	if (!leds_private.led_red) {
		DBG_PRINTK("Couldn't bind to LED 0\n");
		return LEDS_BINDING_FAILED;
	}

	gpio_pin_configure(leds_private.led_red, LED0_GPIO_PIN, GPIO_DIR_OUT);

	leds_private.led_green = device_get_binding(LED1_GPIO_PORT);
	if (!leds_private.led_green) {
		DBG_PRINTK("Couldn't bind to LED 1\n");
		return LEDS_BINDING_FAILED;
	}

	gpio_pin_configure(leds_private.led_green, LED1_GPIO_PIN, GPIO_DIR_OUT);

	leds_private.led_blue = device_get_binding(LED2_GPIO_PORT);
	if (!leds_private.led_blue) {
		DBG_PRINTK("Couldn't bind to LED 2\n");
		return LEDS_BINDING_FAILED;
	}

	gpio_pin_configure(leds_private.led_blue, LED2_GPIO_PIN, GPIO_DIR_OUT);

	err = leds_set(LED_RED, false);
	if (err < 0) {
		return err;
	}

	err = leds_set(LED_GREEN, false);
	if (err < 0) {
		return err;
	}

	err = leds_set(LED_BLUE, false);
	if (err < 0) {
		return err;
	}

	return LEDS_SUCCESS;
}

int leds_set(enum leds_t led, bool enable)
{
	int err;

	switch(led) {
	case LED_RED:
		err = gpio_pin_write(leds_private.led_red, LED0_GPIO_PIN, enable ? 0 : 1);
		break;
	case LED_GREEN:
		err = gpio_pin_write(leds_private.led_green, LED1_GPIO_PIN, enable ? 0 : 1);
		break;
	case LED_BLUE:
		err = gpio_pin_write(leds_private.led_blue, LED2_GPIO_PIN, enable ? 0 : 1);
		break;
	default:
		DBG_PRINTK("Unknown LED\n");
		err = LEDS_UNKNOWN_LED;
		break;
	}

	return err;
}
