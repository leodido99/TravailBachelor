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

#define DEBUG DEBUG_RN2483_LORA

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
 * Command to set the spreading factor
 */
#define RN2483_LORA_RADIO_SET_SF "radio set sf "

/**
 * Command to pause the MAC layer
 */
#define RN2483_LORA_MAC_PAUSE "mac pause"

/**
 * Command to set the radio output power
 */
#define RN2483_LORA_RADIO_SET_PWR "radio set pwr "

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
#define RN2483_LORA_PRIORITY 0

/**
 * Amount of time the thread sleeps after each iterations
 */
#define RN2483_LORA_THREAD_SLEEP_MS 100

/**
 *  Buffer size
 */
#define RN2483_LORA_RX_BUFF_SIZE 128

/**
 * Number of buffers
 */
#define BUFF_NB 4

/**
 * RX Data index
 */
u32_t rx_data_buf_idx = 0;

/**
 * RX Data Buffer used by the irq
 */
u8_t rx_data_buf[RN2483_LORA_RX_BUFF_SIZE];

/**
 * New command is ready to be sent
 */
static volatile bool new_cmd_rdy = false;

/**
 * Maximum payload size that can be stored in the cmd buffer
 * \note Each byte of data is actually represented as two
 * chars (i.e. 16 bits)
 */
#define RN2483_LORA_CMD_BUFFER_PAYLOAD_MAX_SIZE (2 * RN2483_LORA_MAX_PAYLOAD_SIZE)

/**
 * The largest command we can hold in the command buffer
 * \note The largest command we can hold is a radio tx with max size payload
 */
#define RN2483_LORA_MAX_CMD_SIZE (sizeof(RN2483_LORA_RADIO_TX_CMD) + RN2483_LORA_CMD_BUFFER_PAYLOAD_MAX_SIZE)

/**
 * Size of the CMD buffer
 */
#define RN2483_LORA_CMD_BUFFER_SIZE (RN2483_LORA_MAX_CMD_SIZE + sizeof('\r') + sizeof('\n') + sizeof('\0'))

/**
 * Command index
 */


/**
 * Buffer used to store the command to send
 */


struct tx_cmd_buffer_t {
	u8_t cmd_buf[RN2483_LORA_CMD_BUFFER_SIZE];
	u32_t cmd_idx;
	bool cmd_rdy;
};

struct tx_cmd_buffer_t tx_cmd_buffer = {
	.cmd_idx = 0,
	.cmd_rdy = false
};

/**
 * Size of the largest message we can receive from the LoRa module
 * "RN2483 1.0.3 Mar 22 2017 06:00:42\r\n"
 */
#define RN2483_LORA_MSG_SIZE 40

/**
 * Size of the receive message queue
 */
#define RN2483_LORA_RX_MSG_QUEUE_SIZE 2

/**
 * Structure containing an rx message
 */
typedef struct {
	char msg[RN2483_LORA_MSG_SIZE];
} rn2483_lora_rx_msg;

/* Define a message queue to hold the rx messages */
K_MSGQ_DEFINE(rn2483_lora_rx_msgs, sizeof(rn2483_lora_rx_msg), RN2483_LORA_RX_MSG_QUEUE_SIZE, 4);

/* Semaphore used to wait for a certain message */
K_SEM_DEFINE(sem_lora_wait_for_rx_msg, 0, 1);

/* Semaphore used to protect access to the UART */
K_SEM_DEFINE(sem_lora_tx_msg, 1, 1);

/* The thread id for LoRa */
extern const k_tid_t rn2483_lora_thread_id;

/**
 * Wait for mechanism structure
 */
typedef struct {
	bool wait_for;
	int wait_for_nb;
	char wait_for_buf[BUFF_NB][RN2483_LORA_RX_BUFF_SIZE];
	struct k_sem* wait_for_sem;
	int wait_for_match_idx;
} rn2483_lora_wait_for_priv;

/**
 * Structure used to store the wait for mechanism state
 */
static rn2483_lora_wait_for_priv wait_for_priv = {
	.wait_for = false,
	.wait_for_nb = 0,
	.wait_for_sem = &sem_lora_wait_for_rx_msg
};

static void rn2483_lora_irq_handler(struct device *dev) {
	int bytes_read;
	rn2483_lora_rx_msg rx_msg;

	uart_irq_update(dev);

	/* TODO IRQ on TX seem to spin forever */
	/*if (uart_irq_tx_ready(dev)) {
	}*/

	if (uart_irq_rx_ready(dev)) {
		/* Data is available for reading */
		while ((bytes_read = uart_fifo_read(dev, (u8_t *)(&rx_data_buf[rx_data_buf_idx]), sizeof(rx_data_buf) - rx_data_buf_idx))) {
			/* Check last char */
			if (rx_data_buf[rx_data_buf_idx + bytes_read - 1] == '\n') {
				/* Discard \r\n return */
				rx_data_buf[rx_data_buf_idx + bytes_read - 2] = '\0';
				/* Message is ready, add to queue */
				strcpy(rx_msg.msg, rx_data_buf);
				/* Add message to queue */
				if (k_msgq_put(&rn2483_lora_rx_msgs, &rx_msg, K_NO_WAIT) < 0) {
					DBG_PRINTK("%s: Couldn't store rx msg in queue\n", __func__);
				}
				rx_data_buf_idx = 0;
			} else {
				rx_data_buf_idx += bytes_read;
			}
		}
	}
}

static void check_waiting_for(rn2483_lora_rx_msg* msg) {
	if (wait_for_priv.wait_for) {
		if (wait_for_priv.wait_for_nb == 0) {
			DBG_PRINTK("%s: Received waiting for any message: \"%s\"\n", __func__, msg->msg);
			/* If we are waiting for a message but the count is 0,
			 * it means we are waiting for any messages */
			wait_for_priv.wait_for = false;
			wait_for_priv.wait_for_match_idx = 0;
			k_sem_give(wait_for_priv.wait_for_sem);
			return;
		}

		/* Check message received is one of the expected one */
		for(int i = 0; i < wait_for_priv.wait_for_nb; i++) {
			if (strcmp(msg->msg, wait_for_priv.wait_for_buf[i]) == 0) {
				DBG_PRINTK("%s: Received waiting for message: \"%s\"\n", __func__, msg->msg);
				/* Found message */
				wait_for_priv.wait_for = false;
				wait_for_priv.wait_for_nb = 0;
				wait_for_priv.wait_for_match_idx = i;
				k_sem_give(wait_for_priv.wait_for_sem);
				break;
			}
		}
	}
}

void rn2483_lora_thread(void) {
	char sent;
	rn2483_lora_rx_msg rx_msg;

	DBG_PRINTK("%s: Enter thread\n", __func__);

	/* When the LoRa module is switched on, it will automatically
	 * send its version number */
	if (k_msgq_get(&rn2483_lora_rx_msgs, &rx_msg, K_MSEC(200))) {
		DBG_PRINTK("%s: Couldn't read rx message from queue\n", __func__);
	}
	DBG_PRINTK("%s: %s\n", __func__, rx_msg.msg);

	/* Unblock the init function */
	k_sem_give(wait_for_priv.wait_for_sem);

	while(1) {
		if (tx_cmd_buffer.cmd_rdy) {
			/* Send command */
			do {
				sent = uart_poll_out(lora_uart, tx_cmd_buffer.cmd_buf[tx_cmd_buffer.cmd_idx]);
				tx_cmd_buffer.cmd_idx++;
			} while(sent != '\n');

			tx_cmd_buffer.cmd_rdy = false;

			DBG_PRINTK("%s: TX: %s\n", __func__, tx_cmd_buffer.cmd_buf);
		}

		if (k_msgq_num_used_get(&rn2483_lora_rx_msgs) > 0) {
			/* Data ready */
			if (k_msgq_get(&rn2483_lora_rx_msgs, &rx_msg, K_FOREVER)) {
				DBG_PRINTK("%s: Couldn't read rx message from queue\n", __func__);
			}
			DBG_PRINTK("%s: RX: %s\n", __func__, rx_msg.msg);

			/* Check if we are waiting on this message */
			check_waiting_for(&rx_msg);
		}

		/* TODO uart_err_check */
		k_sleep(RN2483_LORA_THREAD_SLEEP_MS);
	}
}

int rn2483_lora_wait_for_either_replies(char replies[][RN2483_LORA_MSG_SIZE], unsigned int nb_replies, int* reply_match_idx) {

	if (nb_replies > BUFF_NB) {
		DBG_PRINTK("%s: Too many replies : %d > %d\n", __func__, nb_replies, BUFF_NB);
		return RN2483_LORA_TOO_MANY_REPLIES;
	}

	if (!wait_for_priv.wait_for) {
		wait_for_priv.wait_for = true;
		wait_for_priv.wait_for_nb = nb_replies;
		for(int i = 0; i < nb_replies; i++) {
			/* Copy all the replies */
			strcpy(wait_for_priv.wait_for_buf[i], replies[i]);
		}

		DBG_PRINTK("%s: Waiting for replies %d\n", __func__, nb_replies);
		/* Take semaphore */
		k_sem_take(wait_for_priv.wait_for_sem, K_FOREVER);
		*reply_match_idx = wait_for_priv.wait_for_match_idx;
	} else {
		DBG_PRINTK("%s: Already waiting for message(s)\n", __func__);
		return RN2483_LORA_BUSY;
	}

	return RN2483_LORA_SUCCESS;
}

int rn2483_lora_wait_for_any_reply() {
	return rn2483_lora_wait_for_either_replies(NULL, 0, NULL);
}

int rn2483_lora_wait_for_reply(char* reply) {
	return rn2483_lora_wait_for_either_replies((char(*)[RN2483_LORA_MSG_SIZE])reply, 1, NULL);
}

int rn2483_lora_init(const char *device_name) {
	rx_data_buf_idx = 0;
	new_cmd_rdy = false;

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

	/* start LoRa thread */
	k_thread_start(rn2483_lora_thread_id);

	/* Wait for the RN2483 version message */
	k_sem_take(wait_for_priv.wait_for_sem, K_FOREVER);

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

int rn2483_lora_cmd(char* cmd) {
	if (!tx_cmd_buffer.cmd_rdy) {
		if (strlen(cmd) <= RN2483_LORA_MAX_CMD_SIZE) {
			strcat(tx_cmd_buffer.cmd_buf, "\r\n");
			tx_cmd_buffer.cmd_idx = 0;
			tx_cmd_buffer.cmd_rdy = true;
		} else {
			DBG_PRINTK("%s: Cmd too long %d (max:%d)\n", __func__, strlen(cmd), RN2483_LORA_CMD_BUFFER_PAYLOAD_MAX_SIZE);
			return RN2483_LORA_CMD_TOO_LONG;
		}
	} else {
		DBG_PRINTK("%s: Command is currently being sent\n", __func__);
		return RN2483_LORA_BUSY;
	}
	return RN2483_LORA_SUCCESS;
}

int rn2483_lora_radio_tx(u8_t* data, u32_t size) {
	char tmp[3];
	char replies[4][RN2483_LORA_MSG_SIZE] = { "busy", "invalid_param", "radio_err", "radio_tx_ok" };
	int i;
	int err;
	int reply_idx = 0;

	k_sem_take(&sem_lora_tx_msg, K_FOREVER);

	if (size <= RN2483_LORA_MAX_PAYLOAD_SIZE) {
		strcpy(tx_cmd_buffer.cmd_buf, RN2483_LORA_RADIO_TX_CMD);
		for(i = 0; i < size; i++) {
			snprintf(tmp, 3, "%02X", data[i]);
			strcat(tx_cmd_buffer.cmd_buf, tmp);
		}

		/* Send command */
		err = rn2483_lora_cmd(tx_cmd_buffer.cmd_buf);
		if (err) {
			return err;
		}

		/* Wait for reply */
		err = rn2483_lora_wait_for_either_replies(replies, 4, &reply_idx);
		if (err) {
			return err;
		}
		if (reply_idx != 3) {
			DBG_PRINTK("%s: Unexpected reply %s\n", __func__, replies[reply_idx]);
			return RN2483_LORA_ERR_REPLY;
		}
	} else {
		DBG_PRINTK("%s: Command too long %d (max %d)\n", __func__, size, RN2483_LORA_MAX_PAYLOAD_SIZE);
		return RN2483_LORA_CMD_TOO_LONG;
	}

	k_sem_give(&sem_lora_tx_msg);

	return RN2483_LORA_SUCCESS;
}

int rn2483_lora_radio_set_sf(char* sf) {
	char replies[2][RN2483_LORA_MSG_SIZE] = { "ok", "invalid_param" };
	int err;
	int reply_idx = 0;

	k_sem_take(&sem_lora_tx_msg, K_FOREVER);

	/* Build command */
	strcpy(tx_cmd_buffer.cmd_buf, RN2483_LORA_RADIO_SET_SF);
	strcat(tx_cmd_buffer.cmd_buf, sf);

	/* Send command */
	err = rn2483_lora_cmd(tx_cmd_buffer.cmd_buf);
	if (err) {
		return err;
	}

	/* Wait for reply */
	err = rn2483_lora_wait_for_either_replies(replies, 2, &reply_idx);
	if (err) {
		return err;
	}
	if (reply_idx != 0) {
		DBG_PRINTK("%s: Unexpected reply %s\n", __func__, replies[reply_idx]);
		return RN2483_LORA_ERR_REPLY;
	}

	k_sem_give(&sem_lora_tx_msg);

	return RN2483_LORA_SUCCESS;
}

int rn2483_lora_radio_set_pwr(s8_t pwr) {
	char replies[2][RN2483_LORA_MSG_SIZE] = { "ok", "invalid_param" };
	int err;
	int reply_idx = 0;

	k_sem_take(&sem_lora_tx_msg, K_FOREVER);

	snprintf(tx_cmd_buffer.cmd_buf, RN2483_LORA_CMD_BUFFER_SIZE, "%s%d", RN2483_LORA_RADIO_SET_PWR, pwr);

	/* Send command */
	err = rn2483_lora_cmd(tx_cmd_buffer.cmd_buf);
	if (err) {
		return err;
	}

	/* Wait for reply */
	err = rn2483_lora_wait_for_either_replies(replies, 2, &reply_idx);
	if (err) {
		return err;
	}
	if (reply_idx != 0) {
		DBG_PRINTK("%s: Unexpected reply %s\n", __func__, replies[reply_idx]);
		return RN2483_LORA_ERR_REPLY;
	}

	k_sem_give(&sem_lora_tx_msg);

	return RN2483_LORA_SUCCESS;
}

int rn2483_lora_pause_mac() {
	int err;

	k_sem_take(&sem_lora_tx_msg, K_FOREVER);

	strcpy(tx_cmd_buffer.cmd_buf, RN2483_LORA_MAC_PAUSE);

	/* Send command */
	err = rn2483_lora_cmd(tx_cmd_buffer.cmd_buf);
	if (err) {
		return err;
	}

	/* Wait for reply */
	err = rn2483_lora_wait_for_any_reply();

	k_sem_give(&sem_lora_tx_msg);

	return err;
}

/* Thread definition for the RN2483 LoRa module */
K_THREAD_DEFINE(rn2483_lora_thread_id, RN2483_LORA_STACKSIZE, rn2483_lora_thread, NULL, NULL, NULL, RN2483_LORA_PRIORITY, 0, K_FOREVER);
