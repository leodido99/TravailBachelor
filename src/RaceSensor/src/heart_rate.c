/*
 * heart_rate.c
 *
 *  Created on: 17 Aug 2018
 *      Author: leonard.bise
 */

#include "heart_rate.h"

#include "debug.h"
#include <gpio.h>

/**
 * Set debug
 */
#define DEBUG DEBUG_HR

struct heart_rate {
	struct device *gpio_dev;
	struct gpio_callback gpio_cb;
};

static struct heart_rate hr;

void hr_gpio_callback(struct device *port, struct gpio_callback *cb, u32_t pins)
{

}

int hr_init(const char* device, int pin)
{
	int err;

	hr.gpio_dev = device_get_binding(device);
	if (!hr.gpio_dev) {
		DBG_PRINTK("%s: Binding to gpio failed\n", __func__);
		return HR_BINDING_FAILED;
	}

	err = gpio_pin_configure(hr.gpio_dev, pin, (GPIO_DIR_IN | GPIO_INT |
				 GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH | GPIO_PUD_PULL_UP));
	if (err < 0) {
		DBG_PRINTK("%s: Cannot configure gpio pin %d", __func__, pin);
		return err;
	}

	gpio_init_callback(&hr.gpio_cb, hr_gpio_callback, BIT(pin));

	/*err = gpio_add_callback(hr.gpio_dev, &hr.gpio_cb);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot add callback", __func__);
		return err;
	}

	err = gpio_pin_enable_callback(hr.gpio_dev, pin);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot add callback", __func__);
		return err;
	}*/

	return HR_SUCCESS;
}


