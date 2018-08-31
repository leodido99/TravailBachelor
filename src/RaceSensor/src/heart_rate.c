/*
 * heart_rate.c
 *
 *  Created on: 17 Aug 2018
 *      Author: leonard.bise
 */

#include "heart_rate.h"

#include "debug.h"
#include <gpio.h>
#include <pinmux.h>

/**
 * Set debug
 */
#define DEBUG DEBUG_HR

struct heart_rate {
	struct device *gpio_dev;
	struct gpio_callback gpio_cb;
	int pin;
	int beat_cnt;
	s64_t start_time;
};

static struct heart_rate hr = {
	.beat_cnt = 0,
	.start_time = 0
};

void hr_gpio_callback(struct device *port, struct gpio_callback *cb, u32_t pins)
{
	if (hr.start_time == 0) {
		hr.start_time = k_uptime_get();
	}

	hr.beat_cnt++;
}

u8_t hr_get(void)
{
	u8_t bpm;
	s64_t elapsed_s = (k_uptime_get() - hr.start_time) / 1000;

	bpm = (60 * hr.beat_cnt) / elapsed_s;

	DBG_PRINTK("%s: BPM=%d beats=%d elapsed=%lld s\n", __func__, bpm, hr.beat_cnt, elapsed_s);

	hr.beat_cnt = 0;
	hr.start_time = 0;

	return bpm;
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
	struct device *muxa;

	hr.pin = pin;

	DBG_PRINTK("HR device %s pin %d\n", device, hr.pin);

	/* Update pin configuration to be EXTINT */
	muxa = device_get_binding(CONFIG_PINMUX_SAM0_A_LABEL);
	if (!muxa) {
		DBG_PRINTK("%s: Binding to pinmux failed\n", __func__);
		return HR_BINDING_FAILED;
	}

	err = pinmux_pin_set(muxa, 11, PINMUX_FUNC_A);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot change pin function\n", __func__);
		return err;
	}

	hr.gpio_dev = device_get_binding(device);
	if (!hr.gpio_dev) {
		DBG_PRINTK("%s: Binding to gpio failed\n", __func__);
		return HR_BINDING_FAILED;
	}

	gpio_init_callback(&hr.gpio_cb, hr_gpio_callback, BIT(hr.pin));

	err = gpio_add_callback(hr.gpio_dev, &hr.gpio_cb);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot add callback\n", __func__);
		return err;
	}

	err = gpio_pin_configure(hr.gpio_dev, hr.pin, (GPIO_DIR_IN | GPIO_INT |
			 	 GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH));
	if (err < 0) {
		DBG_PRINTK("%s: Cannot configure gpio pin %d", __func__, hr.pin);
		return err;
	}

	err = gpio_pin_enable_callback(hr.gpio_dev, hr.pin);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot add callback\n", __func__);
		return err;
	}

	return HR_SUCCESS;
}
