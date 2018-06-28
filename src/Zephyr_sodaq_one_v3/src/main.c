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

static int idx = 0;
static const char *sys_ver = "sys get ver\r\n";

static volatile bool data_transmitted;
static volatile bool data_arrived;
static	char data_buf[64];

static volatile int cnt_rx = 0;
static volatile int cnt_tx = 0;

static void interrupt_handler(struct device *dev)
{
	uart_irq_update(dev);

	if (uart_irq_tx_ready(dev)) {
		data_transmitted = true;
		cnt_tx++;
	}

	if (uart_irq_rx_ready(dev)) {
		data_arrived = true;
		cnt_rx++;
	}
}

static void write_data(struct device *dev, const char *buf, int len)
{
	printk("Enable IRQ TX\n");
	uart_irq_tx_enable(dev);

	printk("write_data start\n");
	while (len) {
		int written;

		data_transmitted = false;
		written = uart_fifo_fill(dev, (const u8_t *)buf, len);
		while (data_transmitted == false) {
			printk("yield\n");
			k_yield();
		}

		len -= written;
		buf += written;
	}

	printk("write_data end\n");
	uart_irq_tx_disable(dev);
}

static void read_and_echo_data(struct device *dev, int *bytes_read)
{
	while (data_arrived == false)
		;

	data_arrived = false;

	/* Read all data and echo it back */
	while ((*bytes_read = uart_fifo_read(dev,
	    (u8_t *)data_buf, sizeof(data_buf)))) {
		//write_data(dev, data_buf, *bytes_read);
		printk("rx: %s", data_buf);
	}
}

static void reset_lora() {
	static struct device *lora_reset;
	/* Reset LoRa module */
	lora_reset = device_get_binding(LORA_RESET_GPIO_PORT);
	if (!lora_reset) {
		printk("Couldn't get LoRa reset IO\n");
	}
	gpio_pin_configure(lora_reset, LORA_RESET_GPIO_PIN, GPIO_DIR_OUT);

	gpio_pin_write(lora_reset, LORA_RESET_GPIO_PIN, 0);
	/* Wait 100ms */
	k_busy_wait(100000);
	gpio_pin_write(lora_reset, LORA_RESET_GPIO_PIN, 1);
	/* Wait 100ms */
	k_busy_wait(100000);
}

void main(void)
{
	static struct device *led0, *led1, *led2, *lora;
	int on = 1;
	u32_t baudrate, bytes_read;
	int ret;
	unsigned char recvChar, sentChar;

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

	if (rn2483_lora_init(ATMEL_SAM0_UART_42001000_LABEL)) {
		printk("Couldn't initialize LoRa\n");
	} else {
		printk("LoRa initialized\n");
	}










	//reset_lora();

	/*lora = device_get_binding(ATMEL_SAM0_UART_42001000_LABEL);
	if (!lora) {
		printk("Couldn't get UART device SERCOM2\n");
	} else {
		printk("Got UART device SERMCOM2 binding\n");
	}*/

	//gpio_pin_write(led0, LED0_GPIO_PIN, 0);

	/*ret = uart_line_ctrl_get(lora, LINE_CTRL_BAUD_RATE, &baudrate);
	if (ret) {
		printk("Failed to get baudrate (err=%d)\n", ret);
	} else {
		printk("Baudrate detected: %d\n", baudrate);
	}*/

	/*printk("Setup IRQ Handler\n");
	uart_irq_callback_set(lora, interrupt_handler);*/

	//gpio_pin_write(led0, LED0_GPIO_PIN, 1);
	//gpio_pin_write(led1, LED1_GPIO_PIN, 0);

	/*printk("Write data\n");
	write_data(lora, sys_ver, strlen(sys_ver));

	printk("Enable IRQ RX\n");
	uart_irq_rx_enable(lora);*/

	//gpio_pin_write(led1, LED1_GPIO_PIN, 1);
	//gpio_pin_write(led2, LED2_GPIO_PIN, 0);

	//printk("sys_ver strlen %d\n", strlen(sys_ver));

	while(1) {
		//read_and_echo_data(lora, (int *) &bytes_read);
		/*gpio_pin_write(led2, LED2_GPIO_PIN, on);
		k_sleep(200);
		on = (on == 1) ? 0 : 1;*/
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
		/*if (uart_poll_in(lora, &recvChar) == 0) {
			printk("%c", recvChar);
		}*/
		/*if (idx < strlen(sys_ver)) {
			sentChar = uart_poll_out(lora, sys_ver[idx]);
			if (sentChar == sys_ver[idx]) {
				printk("%c", sentChar);
			} else {
				printk("Sent %c instead of %c\n", sentChar, sys_ver[idx]);
			}
			idx++;
		}*/
	}
}
