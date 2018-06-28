/** 
 * @file RN2483_lora.c
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 28, 2018
 */

#include "RN2483_lora.h"

#include <device.h>
#include <board.h>
#include <gpio.h>
#include <uart.h>

/**
 * Pins used for the reset of the module
 */
#define RN2483_LORA_RESET_PORT LORA_RESET_GPIO_PORT
#define RN2483_LORA_RESET_PIN LORA_RESET_GPIO_PIN

/**
 * LoRa UART device
 */
static struct device *lora_uart;

#define RX_BUFF_SIZE 256

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

u32_t rx_data_buf_idx = 0;
u8_t rx_data_buf[RX_BUFF_SIZE];

static volatile bool rx_data_rdy = false;

static void rn2483_lora_irq_handler(struct device *dev) {
	int bytes_read;

	uart_irq_update(dev);

	if (uart_irq_tx_ready(dev)) {



	}

	if (uart_irq_rx_ready(dev)) {
		/* Data is available for reading */
		while ((bytes_read = uart_fifo_read(dev, (u8_t *)(&rx_data_buf[rx_data_buf_idx]), sizeof(rx_data_buf) - rx_data_buf_idx))) {
			rx_data_buf_idx += bytes_read;
			if (rx_data_buf[rx_data_buf_idx] == '\n') {
				rx_data_rdy = true;
			}
			//write_data(dev, data_buf, *bytes_read);
			printk("%s", &rx_data_buf[rx_data_buf_idx]);

		}
		printk("\n");
	}
}

void rn2483_lora_thread(void) {
	printk("%s\n", __func__);
	while(1) {
		if (rx_data_rdy) {
			printk("RX Data: %s\n", rx_data_buf);
			rx_data_rdy = false;
		}
	}
}

int rn2483_lora_init(const char *device_name) {
	/* Reset device */
	if (rn2483_lora_reset()) {
		return RN2483_LORA_BINDING_FAILED;
	}
	/* Bind to LoRa UART */
	lora_uart = device_get_binding(device_name);
	if (!lora_uart) {
		return RN2483_LORA_BINDING_FAILED;
	}
	/* Configure interrupt handler */
	uart_irq_callback_set(lora_uart, rn2483_lora_irq_handler);




	/* Enable interrupts */
	/*uart_irq_rx_enable(lora_uart);
	uart_irq_tx_enable(lora_uart);*/

	return RN2483_LORA_SUCCESS;
}

int rn2483_lora_reset() {
	static struct device *lora_reset;
	/* Reset LoRa module */
	lora_reset = device_get_binding(RN2483_LORA_RESET_PORT);
	if (!lora_reset) {
		//printk("Couldn't get LoRa reset IO\n");
		return RN2483_LORA_BINDING_FAILED;
	}
	/* Set pin direction */
	gpio_pin_configure(lora_reset, RN2483_LORA_RESET_PIN, GPIO_DIR_OUT);

	/* Set pin low */
	gpio_pin_write(lora_reset, RN2483_LORA_RESET_PIN, 0);
	/* Wait 100ms */
	k_busy_wait(100000);
	/* Set pin high */
	gpio_pin_write(lora_reset, RN2483_LORA_RESET_PIN, 1);
	/* Wait 100ms */
	k_busy_wait(100000);

	return RN2483_LORA_SUCCESS;
}

K_THREAD_DEFINE(rn2483_lora_thread_id, STACKSIZE, rn2483_lora_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
