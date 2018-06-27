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

static const char *sys_ver = "sys get ver\r\n";

static volatile bool data_transmitted;
static volatile bool data_arrived;
static	char data_buf[64];

static void interrupt_handler(struct device *dev)
{
	uart_irq_update(dev);

	if (uart_irq_tx_ready(dev)) {
		data_transmitted = true;
	}

	if (uart_irq_rx_ready(dev)) {
		data_arrived = true;
	}
}

static void write_data(struct device *dev, const char *buf, int len)
{
	uart_irq_tx_enable(dev);

	while (len) {
		int written;

		data_transmitted = false;
		written = uart_fifo_fill(dev, (const u8_t *)buf, len);
		while (data_transmitted == false) {
			k_yield();
		}

		len -= written;
		buf += written;
	}

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
		printk("%s", data_buf);
	}
}

void main(void)
{
	static struct device *led0, *led1, *led2, *lora;
	int on = 1;
	u32_t baudrate, bytes_read;
	int ret;

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

	lora = device_get_binding(ATMEL_SAM0_UART_42001000_LABEL);
	if (!lora) {
		printk("Couldn't get UART device SERCOM2\n");
	}

	gpio_pin_write(led0, LED0_GPIO_PIN, 0);

	uart_irq_callback_set(lora, interrupt_handler);
	uart_irq_rx_enable(lora);

	gpio_pin_write(led0, LED0_GPIO_PIN, 1);
	gpio_pin_write(led1, LED1_GPIO_PIN, 0);

	write_data(lora, sys_ver, strlen(sys_ver));

	gpio_pin_write(led1, LED1_GPIO_PIN, 1);
	gpio_pin_write(led2, LED2_GPIO_PIN, 0);

	/*ret = uart_line_ctrl_get(lora, LINE_CTRL_BAUD_RATE, &baudrate);
	if (ret) {
		printk("Failed to get baudrate (err=%d)\n", ret);
	} else {
		printk("Baudrate detected: %d\n", baudrate);
	}*/



	while(1) {
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
		read_and_echo_data(lora, (int *) &bytes_read);
	}
}
