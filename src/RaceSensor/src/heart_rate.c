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
	int pin;
};

static struct heart_rate hr;

void hr_gpio_callback(struct device *port, struct gpio_callback *cb, u32_t pins)
{
	printk("beat!\n");
}

int hr_get_io(void)
{
	int value;
	int err;

	err = gpio_pin_read(hr.gpio_dev, hr.pin, &value);
	if (err < 0) {
		DBG_PRINTK("Cannot read IO");
		return HR_GPIO_ACCESSS_FAILED;
	}

	return value;
}

int hr_init(const char* device, int pin)
{
	int err;

	hr.gpio_dev = device_get_binding(device);
	if (!hr.gpio_dev) {
		DBG_PRINTK("%s: Binding to gpio failed\n", __func__);
		return HR_BINDING_FAILED;
	}

	gpio_init_callback(&hr.gpio_cb, hr_gpio_callback, 0xFFFFFFFF);

	err = gpio_add_callback(hr.gpio_dev, &hr.gpio_cb);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot add callback", __func__);
		return err;
	}

	//int pins[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	int pins[] = { 11 };

	for (int i = 0; i < ARRAY_SIZE(pins); i++) {
		err = gpio_pin_configure(hr.gpio_dev, pin, (GPIO_DIR_IN | GPIO_INT |
						 GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH));

		err = gpio_pin_enable_callback(hr.gpio_dev, pin);
		if (err < 0) {
			DBG_PRINTK("%s: Cannot add callback", __func__);
			return err;
		}


	}
	/*err = gpio_pin_configure(hr.gpio_dev, pin, (GPIO_DIR_IN | GPIO_INT |
					 GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH));*/

	//GPIO_PUD_PULL_DOWN
	if (err < 0) {
		DBG_PRINTK("%s: Cannot configure gpio pin %d", __func__, pin);
		return err;
	}

	/*gpio_init_callback(&hr.gpio_cb, hr_gpio_callback, BIT(pin));

	err = gpio_add_callback(hr.gpio_dev, &hr.gpio_cb);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot add callback", __func__);
		return err;
	}

	err = gpio_pin_enable_callback(hr.gpio_dev, pin);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot add callback", __func__);
		return err;
	}*/

	hr.pin = pin;

	return HR_SUCCESS;
}
