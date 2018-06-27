/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <gpio.h>
#include <board.h>

void main(void)
{
	static struct device *led0, *led1, *led2;
	int on = 1;

	printk("Hello World! %s\n", CONFIG_ARCH);

	led0 = device_get_binding(LED0_GPIO_PORT);
	gpio_pin_configure(led0, LED0_GPIO_PIN, GPIO_DIR_OUT);
	led1 = device_get_binding(LED1_GPIO_PORT);
	gpio_pin_configure(led1, LED1_GPIO_PIN, GPIO_DIR_OUT);
	led2 = device_get_binding(LED2_GPIO_PORT);
	gpio_pin_configure(led2, LED2_GPIO_PIN, GPIO_DIR_OUT);

	gpio_pin_write(led0, LED0_GPIO_PIN, 1);
	gpio_pin_write(led1, LED1_GPIO_PIN, 1);
	gpio_pin_write(led2, LED2_GPIO_PIN, 1);

	while(1) {
		/*gpio_pin_write(led2, LED2_GPIO_PIN, on);
		k_sleep(200);
		on = (on == 1) ? 0 : 1;*/
		gpio_pin_write(led2, LED2_GPIO_PIN, 1);
		gpio_pin_write(led0, LED0_GPIO_PIN, 0);
		k_sleep(200);
		gpio_pin_write(led0, LED0_GPIO_PIN, 1);
		gpio_pin_write(led1, LED1_GPIO_PIN, 0);
		k_sleep(200);
		gpio_pin_write(led1, LED1_GPIO_PIN, 1);
		gpio_pin_write(led2, LED2_GPIO_PIN, 0);
		k_sleep(200);
	}
}
