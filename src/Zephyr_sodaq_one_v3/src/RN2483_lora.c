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

#include <string.h>

#include <device.h>
#include <board.h>
#include <gpio.h>
#include <uart.h>
#include <misc/printk.h>


/**
 * Pins used for the reset of the module
 */
#define RN2483_LORA_RESET_PORT LORA_RESET_GPIO_PORT
#define RN2483_LORA_RESET_PIN LORA_RESET_GPIO_PIN

/**
 * LoRa UART device
 */
static struct device *lora_uart;

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

/* Buffer size */
#define BUFF_SIZE 256

u32_t rx_data_buf_idx = 0;
u8_t rx_data_buf[BUFF_SIZE];

u32_t tx_data_buf_idx = 0;
u8_t tx_data_buf[BUFF_SIZE];

static volatile bool rx_data_rdy = false;

static volatile bool tx_data_rdy = false;
u32_t cmd_idx = 0;
u8_t cmd[] = { "sys get hweui\r\n" };

static void rn2483_lora_irq_handler(struct device *dev) {
	int bytes_read;

	uart_irq_update(dev);

	if (uart_irq_tx_ready(dev)) {

	}

	if (uart_irq_rx_ready(dev)) {
		/* Data is available for reading */
		while ((bytes_read = uart_fifo_read(dev, (u8_t *)(&rx_data_buf[rx_data_buf_idx]), sizeof(rx_data_buf) - rx_data_buf_idx))) {
			/* Check last char */
			if (rx_data_buf[rx_data_buf_idx + bytes_read - 1] == '\n') {
				rx_data_rdy = true;
				/* Discard carriage return */
				rx_data_buf[rx_data_buf_idx + bytes_read - 1] = '\0';
			}
			rx_data_buf_idx += bytes_read;
			//printk("%s", &rx_data_buf[rx_data_buf_idx]);
		}
		//printk("\n");
	}
}

void rn2483_lora_thread(void) {
	char sent;
	printk("%s\n", __func__);
	while(1) {
		if (rx_data_rdy) {
			printk("RX Data: %s\n", rx_data_buf);
			rx_data_rdy = false;
			rx_data_buf_idx = 0;
			tx_data_rdy = true;
		}
		if (tx_data_rdy) {
			//unsigned char uart_poll_out(struct device *dev, unsigned char out_char)
			sent = uart_poll_out(lora_uart, cmd[cmd_idx]);
			if (sent == '\n') {
				tx_data_rdy = false;
				printk("Message sent %s\n", cmd);
			}
			cmd_idx++;

		}
		/* TODO uart_err_check */
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
	uart_irq_rx_enable(lora_uart);
	//uart_irq_tx_enable(lora_uart);

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

int rn2483_lora_radio_tx(u8_t* data, u32_t size) {

}

K_THREAD_DEFINE(rn2483_lora_thread_id, STACKSIZE, rn2483_lora_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
