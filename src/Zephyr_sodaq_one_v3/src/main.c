/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <gpio.h>
#include <board.h>
#include <device.h>
#include <uart.h>
#include <stdio.h>
#include <string.h>

#include "RN2483_lora.h"
#include "LSM303AGR.h"

#define LORA_SPREADING_FACTOR "radio set sf sf7"
#define LORA_POWER "radio set pwr 1"

void main(void)
{
	static struct device *led0, *led1, *led2;
	int ret;
	u8_t data_buf[20] = { 0x00, 0x01, 0x02, 0x03, 0x04 };

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

	if (rn2483_lora_init(CONFIG_UART_SAM0_SERCOM2_LABEL)) {
		printk("Couldn't initialize LoRa\n");
	} else {
		printk("LoRa initialized\n");
	}
	/* Setup the RN2483 for radio */
	/* Pause mac layer */
	if (rn2483_lora_cmd("mac pause\r\n")) {
		printk("Couldn't mac pause\n");
	}
	rn2483_lora_wait_for_any_reply();
	/* Set spreading factor */
	if (rn2483_lora_cmd(LORA_SPREADING_FACTOR)) {
		printk("Couldn't set spreading factor\n");
	}
	rn2483_lora_wait_for_reply("ok");
	/* Set power output */
	if (rn2483_lora_cmd(LORA_POWER)) {
		printk("Couldn't set power output\n");
	}
	rn2483_lora_wait_for_reply("ok");

	rn2483_lora_radio_tx(data_buf, 5);
	rn2483_lora_wait_for_reply("radio_tx_ok");

	data_buf[0] = 0x05;
	data_buf[1] = 0x06;
	data_buf[2] = 0x07;
	data_buf[3] = 0x08;
	data_buf[4] = 0x09;
	rn2483_lora_radio_tx(data_buf, 5);
	rn2483_lora_wait_for_reply("radio_tx_ok");

	lsm303agr_init(CONFIG_I2C_SAM0_SERCOM3_LABEL);
	printk("End Conf\n");
	printk("LSM303AGR Accelerometer Device ID: %X\n", lsm303agr_accel_get_device_id());

	while(1) {
		//printk("Hello World! %s\n", CONFIG_ARCH);
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
