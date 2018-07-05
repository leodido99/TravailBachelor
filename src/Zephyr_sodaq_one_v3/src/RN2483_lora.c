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

#include "debug.h"

#include <string.h>
#include <stdio.h>

#include <device.h>
#include <board.h>
#include <gpio.h>
#include <uart.h>
#include <misc/printk.h>
#include <kernel.h>

/**
 * Pin used for the reset of the module
 */
#define RN2483_LORA_RESET_PORT LORA_RESET_GPIO_PORT
#define RN2483_LORA_RESET_PIN LORA_RESET_GPIO_PIN

/**
 * Command to transmit radio
 */
#define RN2483_LORA_RADIO_TX_CMD "radio tx "
/**
 * Size of the command to transmit radio (nb chars)
 */
#define RN2483_LORA_RADIO_TX_CMD_SIZE 9

/**
 * Maximum payload size
 */
#define RN2483_LORA_MAX_PAYLOAD_SIZE 222

/**
 * LoRa UART device
 */
static struct device *lora_uart;

/**
 * Thread stack size
 */
#define RN2483_LORA_STACKSIZE 1024

/**
 * Thread priority
 */
#define RN2483_LORA_PRIORITY 7

/**
 * Amount of time the thread sleeps after each iterations
 */
#define RN2483_LORA_THREAD_SLEEP_MS 100

/**
 *  Buffer size
 */
#define BUFF_SIZE 256

/**
 * RX Data Buffer
 */
u32_t rx_data_buf_idx = 0;
u8_t rx_data_buf[BUFF_SIZE];

/**
 * RX data is ready
 */
static volatile bool rx_data_rdy = false;

/**
 * Ready to send (i.e. received system ver at beginning)
 */
static volatile bool ready_to_send = false;

/**
 * New command is ready to be sent
 */
static volatile bool new_cmd_rdy = false;

/**
 * Command index and buffer
 */
u32_t cmd_idx = 0;
u8_t cmd_buf[BUFF_SIZE];

/**
 * Wait for buffer
 */
static volatile bool wait_for = false;
char wait_for_buf[BUFF_SIZE];

static void rn2483_lora_irq_handler(struct device *dev) {
	int bytes_read;

	uart_irq_update(dev);

	/* TODO IRQ on TX seem to spin forever */
	/*if (uart_irq_tx_ready(dev)) {
	}*/

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
		}
	}
}

void rn2483_lora_thread(void) {
	char sent;

	printk("%s\n", __func__);

	while(1) {
		if (rx_data_rdy) {
			if (!ready_to_send) {
				/* Before being able to send anything, we need
				 * to wait for the sys version from the RN2483 */
				ready_to_send = true;
			}

			if (wait_for) {
				if (wait_for_buf[0] == '\0') {
					/* Wait for any reply */
					wait_for = false;
				} else {
					/* Check the reply */
					if (strcmp(wait_for_buf, rx_data_buf) == 0) {
						wait_for = false;
					}
				}
			}
			/* setup for new reading */
			rx_data_rdy = false;
			rx_data_buf_idx = 0;

			DBG_PRINTK("%s: RX: %s\n", __func__, rx_data_buf);
		}

		if (ready_to_send && new_cmd_rdy) {
			/* Send command by polling */
			sent = uart_poll_out(lora_uart, cmd_buf[cmd_idx]);
			if (sent == '\n') {
				new_cmd_rdy = false;
				DBG_PRINTK("%s: TX: %s\n", __func__, cmd_buf);
			}
			cmd_idx++;
		}
		/* TODO uart_err_check */
		k_sleep(RN2483_LORA_THREAD_SLEEP_MS);
	}
}

int rn2483_lora_cmd(char* cmd) {
	if (!new_cmd_rdy) {
		if (strlen(cmd) <= (BUFF_SIZE - 2)) {
			memcpy(cmd_buf, cmd, strlen(cmd));
			cmd_buf[strlen(cmd)] = '\r';
			cmd_buf[strlen(cmd) + 1] = '\n';
			new_cmd_rdy = true;
			cmd_idx = 0;
		} else {
			DBG_PRINTK("%s: Cmd too long %d (max:%d)\n", __func__, strlen(cmd), BUFF_SIZE);
			return RN2483_LORA_CMD_TOO_LONG;
		}
	} else {
		DBG_PRINTK("%s: Module busy\n", __func__);
		return RN2483_LORA_BUSY;
	}
	return RN2483_LORA_SUCCESS;
}

void rn2483_lora_wait_for_any_reply() {
	if (!wait_for) {
		wait_for_buf[0] = '\0';
		wait_for = true;
		while(wait_for) {
			k_sleep(200);
		}
	}
}

void rn2483_lora_wait_for_reply(char* reply) {
	if (!wait_for && strlen(reply) <= (BUFF_SIZE - 2)) {
		strcpy(wait_for_buf, reply);
		wait_for_buf[strlen(wait_for_buf)] = '\r';
		wait_for_buf[strlen(wait_for_buf)+1] = '\0';
		wait_for = true;
		while(wait_for) {
			k_sleep(200);
		}
	}
}

void rn2483_lora_wait_cmd() {
	while(new_cmd_rdy) {
		k_sleep(200);
	}
}

int rn2483_lora_init(const char *device_name) {
	rx_data_rdy = false;
	rx_data_buf_idx = 0;
	new_cmd_rdy = false;
	ready_to_send = false;

	/* Reset device */
	if (rn2483_lora_reset()) {
		return RN2483_LORA_BINDING_FAILED;
	}
	/* Bind to LoRa UART */
	lora_uart = device_get_binding(device_name);
	if (!lora_uart) {
		DBG_PRINTK("%s: Couldn't bind LoRa UART device\n", __func__);
		return RN2483_LORA_BINDING_FAILED;
	}
	/* Configure interrupt handler */
	uart_irq_callback_set(lora_uart, rn2483_lora_irq_handler);

	/* Enable interrupts */
	uart_irq_rx_enable(lora_uart);

	/* Wait for the reading of the system version */
	while(!ready_to_send) {
		k_sleep(200);
	}

	return RN2483_LORA_SUCCESS;
}

int rn2483_lora_reset() {
	static struct device *lora_reset;
	/* Reset LoRa module */
	lora_reset = device_get_binding(RN2483_LORA_RESET_PORT);
	if (!lora_reset) {
		DBG_PRINTK("%s: Couldn't bind LoRa Reset device\n", __func__);
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
	char cmd_buf[BUFF_SIZE];
	char tmp[3];
	int i;

	if (!new_cmd_rdy) {
		if (size <= RN2483_LORA_MAX_PAYLOAD_SIZE) {
			strcpy(cmd_buf, RN2483_LORA_RADIO_TX_CMD);
			for(i = 0; i < size; i++) {
				snprintf(tmp, 3, "%02X", data[i]);
				strcat(cmd_buf, tmp);
			}
			/* Send command */
			return rn2483_lora_cmd(cmd_buf);
		} else {
			DBG_PRINTK("%s: Command too long %d (max %d)\n", __func__, size, RN2483_LORA_MAX_PAYLOAD_SIZE);
			return RN2483_LORA_CMD_TOO_LONG;
		}
	} else {
		DBG_PRINTK("%s: Module busy\n", __func__);
		return RN2483_LORA_BUSY;
	}
	return RN2483_LORA_SUCCESS;
}

/* Thread definition for the RN2483 LoRa module */
K_THREAD_DEFINE(rn2483_lora_thread_id, RN2483_LORA_STACKSIZE, rn2483_lora_thread, NULL, NULL, NULL, RN2483_LORA_PRIORITY, 0, K_NO_WAIT);
