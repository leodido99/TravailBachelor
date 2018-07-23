/** 
 * @file UBloxEVA8M.c
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */

#include "UBloxEVA8M.h"

#include "debug.h"

#define DEBUG DEBUG_UBLOXEVA8M

#include "UBloxEVA8M_protocol.h"

#include <i2c.h>
#include <board.h>
#include <gpio.h>
#include <string.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/**
 * Thread stack size
 */
#define UBLOXEVA8M_STACKSIZE 1024

/**
 * Thread priority
 */
#define UBLOXEVA8M_PRIORITY 7

/**
 * Amount of milliseconds the thread sleeps after each iteration
 */
#define UBLOXEVA8M_THREAD_SLEEP_MS 100

/**
 * I2C Device Address
 */
#define UBLOXEVA8M_ADDR 0x42

/**
 * I2C Register Addresses
 */
/* 16 bits number of bytes in the message stream */
#define UBLOXEVA8M_NB_BYTES_STREAM_ADDR 0xFD
/* Address where the stream can be read from */
#define UBLOXEVA8M_STREAM_ADDR 0xFF

/* Sync char 1 character */
#define UBLOXEVA8M_SYNCH_CHAR_1 0xB5
/* Sync char 2 character */
#define UBLOXEVA8M_SYNCH_CHAR_2 0x62

/**
 * Number of iterations before declaring the module is dead
 */
#define UBLOXEVA8M_IS_ALIVE_TIMEOUT_ITER 50

/**
 * Structure used internally
 */
typedef struct {
	struct device *i2c_dev;
	struct device *gps_enable;
	struct device *gps_timepulse;
	bool is_thread_running;
} ubloxeva8m_priv_t;

/**
 * Internal structure
 */
static ubloxeva8m_priv_t ubloxeva8m_priv;

/**
 * States used during message processing
 */
#define UBLOXEVA8M_STATE_SYNCH_CHAR_1 0
#define UBLOXEVA8M_STATE_SYNCH_CHAR_2 1
#define UBLOXEVA8M_STATE_CLASS 2
#define UBLOXEVA8M_STATE_MSG_ID 3
#define UBLOXEVA8M_STATE_LEN_1 4
#define UBLOXEVA8M_STATE_LEN_2 5
#define UBLOXEVA8M_STATE_PAYLOAD 6
#define UBLOXEVA8M_STATE_CSUM_A 7
#define UBLOXEVA8M_STATE_CSUM_B 8

/**
 * Size of a message
 */
#define UBLOXEVA8M_MSG_BUFFER_SIZE (4 + UBLOXEVA8M_MSG_PAYLOAD_BUFFER_SIZE + 2)

/**
 * Size of the SYNCH chars
 */
#define UBLOXEVA8M_SYNC_CHAR_SIZE 1

/**
 * Size of the class ID
 */
#define UBLOXEVA8M_CLASS_ID_SIZE 1

/**
 * Size of the message ID
 */
#define UBLOXEVA8M_MSG_ID_SIZE 1

/**
 * Size of the length
 */
#define UBLOXEVA8M_LENGTH_SIZE 2

/**
 * Size of the checksum
 */
#define UBLOXEVA8M_CSUM_SIZE 2

/**
 * The size of a message with no payload
 */
#define UBLOXEVA8M_MSG_PARAM_SIZE (2 * UBLOXEVA8M_SYNC_CHAR_SIZE + UBLOXEVA8M_CLASS_ID_SIZE + UBLOXEVA8M_MSG_ID_SIZE + UBLOXEVA8M_LENGTH_SIZE + UBLOXEVA8M_CSUM_SIZE)

/**
 * Current state of process data
 */
static uint8_t current_state = 0;

/**
 * Current UBX message being processed
 */
static ubloxeva8m_ubx_msg current_msg;

/**
 * Callback function
 */
static ubloxeva8m_msg_callback msg_handler = NULL;

/* Forward declarations */
#ifdef DEBUG_MSG
static void print_msg(char* txt, ubloxeva8m_ubx_msg* msg);
#endif

#define UBLOXEVA8M_NB_CLASS_IDS 4
#define UBLOXEVA8M_NB_MSG_IDS 4

typedef struct {
	uint8_t class_ids[UBLOXEVA8M_NB_CLASS_IDS];
	uint8_t message_ids[UBLOXEVA8M_NB_MSG_IDS];
	int nb;
	ubloxeva8m_ubx_msg* msg_recv;
} ubloxeva8m_wait_for_t;

static ubloxeva8m_wait_for_t wait_for;

/**
 * Define semaphore used for wait for mechanism
 */
K_SEM_DEFINE(sem_wait_for, 0, 1);

typedef struct {
	uint8_t msg[UBLOXEVA8M_MSG_BUFFER_SIZE];
	int length;
} ubloxeva8m_send_msg_t;

static ubloxeva8m_send_msg_t msg_to_send;

/* The thread id for UBloxEVA8M */
extern const k_tid_t ubloxeva8m_thread_id;

static uint16_t calc_csum(uint8_t* data, uint16_t length) {
	uint8_t csum_a = 0, csum_b = 0;
	for(int i = 0; i < length; i++) {
		csum_a = csum_a + data[i];
		csum_b = csum_b + csum_a;
	}
	return (uint16_t)(csum_a << 8 | (csum_b & 0xFF));
}

static uint16_t get_nb_available_data() {
	uint8_t tmp;
	uint16_t reg;

	if (!ubloxeva8m_priv.i2c_dev) {
		DBG_PRINTK("%s: Module not initialized\n", __func__);
		return 0;
	}
	if (i2c_reg_read_byte(ubloxeva8m_priv.i2c_dev, UBLOXEVA8M_ADDR, UBLOXEVA8M_NB_BYTES_STREAM_ADDR, &tmp)) {
		DBG_PRINTK("%s: Register #1 access failed\n", __func__);
		return 0;
	}
	reg = tmp << 8;
	if (i2c_reg_read_byte(ubloxeva8m_priv.i2c_dev, UBLOXEVA8M_ADDR, UBLOXEVA8M_NB_BYTES_STREAM_ADDR + 1, &tmp)) {
		DBG_PRINTK("%s: Register #2 access failed\n", __func__);
		return 0;
	}
	reg |= tmp;
	return reg;
}

static uint8_t read_stream_byte() {
	uint8_t data;

	if (i2c_reg_read_byte(ubloxeva8m_priv.i2c_dev, UBLOXEVA8M_ADDR, UBLOXEVA8M_STREAM_ADDR, &data)) {
		DBG_PRINTK("%s: Register access failed\n", __func__);
	}
	return data;
}

/**
 * Reset a message
 * @param msg Message to reset
 */
static void reset_msg(ubloxeva8m_ubx_msg* msg) {
	msg->class_id = 0xFF;
	msg->message_id = 0xFF;
	msg->length = 0;
	msg->stored_length = 0;
	msg->checksum = 0;
}

/**
 * Process a new byte of data
 * The data received uses the UBX protocol as defined in document
 * u-blox8-M8_ReceiverDescrProtSpec_(UBX-13003221)_Public.pdf
 *
 * The format of an UBX message is described here:
 * 	- Synch Char #1 : 1 byte : 0xB5
 * 	- Synch Char #2 : 1 byte : 0x62
 * 	- Class 		: 1 byte : Message Class
 * 	- ID 			: 1 byte : Message ID
 * 	- Length		: 2 byte : UBX Message payload length (little endian)
 * 	- Payload		: X byte : UBX Message payload
 * 	- Checksum A	: 1 byte : Checksum part A
 * 	- Checksum B	: 1 byte : Checksum part B
 *
 * 	The 16bit checksum is calculated over the Class, ID, Length and Payload fields.
 *
 * @param data
 * @return true = msg ready, false = msg incomplete
 */
static bool process_data(uint8_t data) {
	bool ret = false;
	switch(current_state) {
	case UBLOXEVA8M_STATE_SYNCH_CHAR_1:
		if (data == UBLOXEVA8M_SYNCH_CHAR_1) {
			current_state = UBLOXEVA8M_STATE_SYNCH_CHAR_2;
			reset_msg(&current_msg);
		}
		break;
	case UBLOXEVA8M_STATE_SYNCH_CHAR_2:
		if (data == UBLOXEVA8M_SYNCH_CHAR_2) {
			current_state = UBLOXEVA8M_STATE_CLASS;
		}
		break;
	case UBLOXEVA8M_STATE_CLASS:
		current_msg.class_id = data;
		current_state = UBLOXEVA8M_STATE_MSG_ID;
		break;
	case UBLOXEVA8M_STATE_MSG_ID:
		current_msg.message_id = data;
		current_state = UBLOXEVA8M_STATE_LEN_1;
		break;
	case UBLOXEVA8M_STATE_LEN_1:
		current_msg.length = data & 0xFF;
		current_state = UBLOXEVA8M_STATE_LEN_2;
		break;
	case UBLOXEVA8M_STATE_LEN_2:
		current_msg.length |= (data << 8);
		current_state = UBLOXEVA8M_STATE_PAYLOAD;
		if (current_msg.length > UBLOXEVA8M_MSG_PAYLOAD_BUFFER_SIZE) {
			DBG_PRINTK("%s: Payload length %d larger than buffer. Resetting to inital state\n", __func__, current_msg.length);
			current_state = UBLOXEVA8M_STATE_SYNCH_CHAR_1;
		}
		break;
	case UBLOXEVA8M_STATE_PAYLOAD:
		current_msg.payload[current_msg.stored_length] = data;
		current_msg.stored_length++;
		if (current_msg.length == current_msg.stored_length) {
			current_state = UBLOXEVA8M_STATE_CSUM_A;
		}
		break;
	case UBLOXEVA8M_STATE_CSUM_A:
		current_msg.checksum = data << 8;
		current_state = UBLOXEVA8M_STATE_CSUM_B;
		break;
	case UBLOXEVA8M_STATE_CSUM_B:
		current_msg.checksum |= data;
		current_state = UBLOXEVA8M_STATE_SYNCH_CHAR_1;
		ret = true;
		break;
	default:
		DBG_PRINTK("%s: Unknown State. Resetting to inital state\n", __func__);
		current_state = UBLOXEVA8M_STATE_SYNCH_CHAR_1;
		break;
	}
	return ret;
}

static void process_msg(ubloxeva8m_ubx_msg* msg) {
	for(int i = 0; i < wait_for.nb; i++) {
		if (wait_for.class_ids[i] == msg->class_id && wait_for.message_ids[i] == msg->message_id) {
			if (wait_for.msg_recv != NULL) {
				memcpy(wait_for.msg_recv, msg, sizeof(msg));
			}
			wait_for.nb = 0;
			k_sem_give(&sem_wait_for);
			break;
		}
	}
	if (msg_handler) {
		msg_handler(msg);
	}
}

static bool wait_for_messages(uint8_t* class_id, uint8_t* message_id, int nb, ubloxeva8m_ubx_msg* msg_recv) {
	if (wait_for.nb > 0) {
		DBG_PRINTK("%s: Already waiting!\n", __func__);
		return false;
	}
	wait_for.nb = nb;
	wait_for.msg_recv = msg_recv;
	for(int i = 0; i < wait_for.nb; i++) {
		wait_for.class_ids[i] = class_id[i];
		wait_for.message_ids[i] = message_id[i];
	}
	/* Wait for message */
	k_sem_take(&sem_wait_for, K_FOREVER);
	return true;
}

static bool wait_for_message(uint8_t class_id, uint8_t message_id, ubloxeva8m_ubx_msg* msg_recv) {
	return wait_for_messages(&class_id, &message_id, 1, msg_recv);
}

/**
 * Wait for ACK or NAK
 * @return true = ACK, false = NAK
 */
static bool wait_for_ack() {
	uint8_t class_ids[2] = { UBLOXEVA8M_CLASS_ACK, UBLOXEVA8M_CLASS_ACK };
	uint8_t message_ids[2] = { UBLOXEVA8M_MSG_ACK_ACK, UBLOXEVA8M_MSG_ACK_NAK };
	ubloxeva8m_ubx_msg msg_recv;

	if (wait_for_messages(class_ids, message_ids, 2, &msg_recv)) {
		if (msg_recv.class_id == UBLOXEVA8M_CLASS_ACK && msg_recv.message_id == UBLOXEVA8M_MSG_ACK_ACK) {
			return true;
		}
		if (msg_recv.class_id == UBLOXEVA8M_CLASS_ACK && msg_recv.message_id == UBLOXEVA8M_MSG_ACK_NAK) {
			return false;
		}
	}
	return false;
}

static int send_msg(uint8_t class_id, uint8_t message_id, uint8_t* payload, uint16_t length) {
	uint16_t csum;

	if (msg_to_send.length > 0) {
		DBG_PRINTK("%s: I2C Busy\n", __func__);
		return UBLOXEVA8M_BUSY;
	}
	/* Create message */
	msg_to_send.msg[0] = UBLOXEVA8M_SYNCH_CHAR_1;
	msg_to_send.msg[1] = UBLOXEVA8M_SYNCH_CHAR_2;
	msg_to_send.msg[2] = class_id;
	msg_to_send.msg[3] = message_id;
	/* Little endian */
	/* 0x0102 */
	msg_to_send.msg[4] = length & 0xFF;
	msg_to_send.msg[5] = length >> 8;
	/* Copy payload */
	memcpy(&msg_to_send.msg[6], payload, length);
	csum = calc_csum(&msg_to_send.msg[2], length + 4);
	msg_to_send.msg[6 + length] = csum >> 8;
	msg_to_send.msg[6 + 1 + length] = csum & 0xFF;
#ifdef DEBUG
	DBG_PRINTK("%s: Sending: ", __func__);
	for(int i = 0; i < (length + UBLOXEVA8M_MSG_PARAM_SIZE); i++) {
		DBG_PRINTK("%02X", msg_to_send.msg[i]);
	}
	DBG_PRINTK("\n");
#endif
	msg_to_send.length = length + UBLOXEVA8M_MSG_PARAM_SIZE;
	return UBLOXEVA8M_SUCCESS;
}

static int get_protocol_configuration(ubloxeva8m_cfg_prt_t* msg) {
	int status;

	if (msg == NULL) {
		DBG_PRINTK("%s: NULL pointer\n", __func__);
		return UBLOXEVA8M_NULL_POINTER;
	}

	/* Request protocol configuration from device */
	status = send_msg(UBLOXEVA8M_CLASS_CFG, UBLOXEVA8M_MSG_CFG_PRT, NULL, 0);
	if (status) {
		return status;
	}

	/* Wait for device answer containing the payload */
	wait_for_message(UBLOXEVA8M_CLASS_CFG, UBLOXEVA8M_MSG_CFG_PRT, NULL);

	/* Wait for Acknowledgment */
	if (!wait_for_ack()) {
		DBG_PRINTK("%s: Module returned NACK\n", __func__);
		return UBLOXEVA8M_NACK;
	}

	/* Copy message */
	memcpy(msg, &current_msg.payload, current_msg.length);

	return UBLOXEVA8M_SUCCESS;
}

static void flush_stream() {
	uint16_t nb_byte;
	uint8_t data;
	nb_byte = get_nb_available_data();
	for(int i = 0; i < nb_byte; i++) {
		data = read_stream_byte();
	}
}

static int configure_protocol() {
	int status;
	ubloxeva8m_cfg_prt_t msg;

	/* Read current configuration from device */
	status = get_protocol_configuration(&msg);
	if (status) {
		return status;
	}

	/* Enable only UBX as output protocol */
	msg.outProtoMask = UBLOXEVA8M_CFG_PRT_OUTPROTOMASK_UBX_MASK;

	/* Send back message to update the configuration */
	status = send_msg(UBLOXEVA8M_CLASS_CFG, UBLOXEVA8M_MSG_CFG_PRT, (uint8_t*)&msg, sizeof(msg));
	if (status) {
		return status;
	}

	/* Wait for Acknowledgment */
	if (!wait_for_ack()) {
		DBG_PRINTK("%s: Module returned NACK\n", __func__);
		return UBLOXEVA8M_NACK;
	}

	return UBLOXEVA8M_SUCCESS;
}

static int configure_msg_rates(uint8_t class_id, uint8_t message_id, uint8_t rate) {
	int status;
	ubloxeva8m_cfg_msg_t msg;

	msg.msgClass = class_id;
	msg.msgID = message_id;
	msg.rate = rate;

	status = send_msg(UBLOXEVA8M_CLASS_CFG, UBLOXEVA8M_MSG_CFG_MSG, (uint8_t*)&msg, sizeof(msg));
	if (status) {
		return status;
	}

	/* Wait for Acknowledgment */
	if (!wait_for_ack()) {
		DBG_PRINTK("%s: Module returned NACK\n", __func__);
		return UBLOXEVA8M_NACK;
	}

	return UBLOXEVA8M_SUCCESS;
}

void ubloxeva8m_thread(void) {
	uint16_t nb_byte;
	uint8_t data;

	DBG_PRINTK("%s: Enter thread\n", __func__);

	while(1) {
		if (ubloxeva8m_priv.is_thread_running) {
			nb_byte = get_nb_available_data();
			if (nb_byte > 0) {
				for(int i = 0; i < nb_byte; i++) {
					/* Read a byte from the device stream */
					data = read_stream_byte();
					if (process_data(data)) {
						/* Message complete */
						process_msg(&current_msg);
					}
				}
			}
			/* Check if new command to send */
			if (msg_to_send.length > 0) {
				/* Send message over i2c */
				if (i2c_write(ubloxeva8m_priv.i2c_dev, msg_to_send.msg, msg_to_send.length, UBLOXEVA8M_ADDR)) {
					DBG_PRINTK("%s: I2C access failed\n", __func__);
				}
				msg_to_send.length = 0;
			}
		}
		k_sleep(UBLOXEVA8M_THREAD_SLEEP_MS);
	}
}

bool ubloxeva8m_is_alive() {
	u8_t val;
	uint32_t cnt = 0;
	while(cnt < UBLOXEVA8M_IS_ALIVE_TIMEOUT_ITER && i2c_reg_read_byte(ubloxeva8m_priv.i2c_dev, UBLOXEVA8M_ADDR, UBLOXEVA8M_NB_BYTES_STREAM_ADDR, &val)) {
		cnt++;
	}
	return cnt == UBLOXEVA8M_IS_ALIVE_TIMEOUT_ITER ? false : true;
}

int ubloxeva8m_init(const char *device_name) {
	ubloxeva8m_priv.i2c_dev = device_get_binding(device_name);
	u32_t i2c_cfg = I2C_SPEED_SET(I2C_SPEED_FAST) | I2C_MODE_MASTER;

	DBG_PRINTK("%s: Thread id %p\n", __func__, ubloxeva8m_thread_id);

	current_state = 0;
	ubloxeva8m_priv.is_thread_running = false;
	wait_for.nb = 0;

	/* Configure I2C Device */
	if (!ubloxeva8m_priv.i2c_dev) {
		DBG_PRINTK("%s: Binding to i2c failed\n", __func__);
		return UBLOXEVA8M_BINDING_FAILED;
	}
	if (i2c_configure(ubloxeva8m_priv.i2c_dev, i2c_cfg)) {
		DBG_PRINTK("%s: i2c configuration failed\n", __func__);
		return UBLOXEVA8M_CONFIGURATION_FAILED;
	}

	/* Configure GPIO GPS Enable Device */
	ubloxeva8m_priv.gps_enable = device_get_binding(GPS_EN_GPIO_PORT);
	if (!ubloxeva8m_priv.gps_enable) {
		DBG_PRINTK("%s: Binding to gpio failed\n", __func__);
		return UBLOXEVA8M_BINDING_FAILED;
	}
	if (gpio_pin_configure(ubloxeva8m_priv.gps_enable, GPS_EN_GPIO_PIN, GPIO_DIR_OUT)) {
		DBG_PRINTK("%s: Couldn't configure gpio\n", __func__);
		return UBLOXEVA8M_GPIO_ACCESS_FAILED;
	}

	/* Configure GPIO Timepulse Device */
	ubloxeva8m_priv.gps_timepulse = device_get_binding(GPS_TIMEPULSE_GPIO_PORT);
	if (!ubloxeva8m_priv.gps_timepulse) {
		DBG_PRINTK("%s: Binding to gpio failed\n", __func__);
		return UBLOXEVA8M_BINDING_FAILED;
	}
	if (gpio_pin_configure(ubloxeva8m_priv.gps_timepulse, GPS_TIMEPULSE_GPIO_PIN, GPIO_DIR_IN)) {
		DBG_PRINTK("%s: Couldn't configure gpio\n", __func__);
		return UBLOXEVA8M_GPIO_ACCESS_FAILED;
	}

	return UBLOXEVA8M_SUCCESS;
}

int ubloxeva8m_power_on() {
	if (!ubloxeva8m_priv.gps_enable) {
		DBG_PRINTK("%s: Module not initialized\n", __func__);
		return UBLOXEVA8M_NOT_INITIALIZED;
	}
	if (gpio_pin_write(ubloxeva8m_priv.gps_enable, GPS_EN_GPIO_PIN, 1)) {
		DBG_PRINTK("%s: Couldn't write to gpio\n", __func__);
		return UBLOXEVA8M_GPIO_ACCESS_FAILED;
	}
	return UBLOXEVA8M_SUCCESS;
}

int ubloxeva8m_power_off() {
	if (!ubloxeva8m_priv.gps_enable) {
		DBG_PRINTK("%s: Module not initialized\n", __func__);
		return UBLOXEVA8M_NOT_INITIALIZED;
	}
	if (gpio_pin_write(ubloxeva8m_priv.gps_enable, GPS_EN_GPIO_PIN, 0)) {
		DBG_PRINTK("%s: Couldn't write to gpio\n", __func__);
		return UBLOXEVA8M_GPIO_ACCESS_FAILED;
	}
	return UBLOXEVA8M_SUCCESS;
}


int ubloxeva8m_start() {
	int status = ubloxeva8m_power_on();
	if (status != UBLOXEVA8M_SUCCESS) {
		return status;
	}
	/* Wait for the device to be ready */
	if (!ubloxeva8m_is_alive()) {
		DBG_PRINTK("%s: Module is not responding over i2c\n", __func__);
		return UBLOXEVA8M_NOT_ANSWERING;
	}

	/* Flush the stream */
	flush_stream();

	/* Start thread */
	ubloxeva8m_priv.is_thread_running = true;

	/* Configure protocol */
	status = configure_protocol();
	if (status != UBLOXEVA8M_SUCCESS) {
		return status;
	}

	/* Configuration message rates */
	status = configure_msg_rates(UBLOXEVA8M_CLASS_NAV, UBLOXEVA8M_MSG_NAV_PVT, 1);
	if (status != UBLOXEVA8M_SUCCESS) {
		return status;
	}

	return UBLOXEVA8M_SUCCESS;
}

void ubloxeva8m_set_callback(ubloxeva8m_msg_callback handler) {
	msg_handler = handler;
}

int ubloxeva8m_set_dynamic_model(ubloxeva8m_dynamic_models dyn_model) {
	ubloxeva8m_cfg_nav5_t msg;
	int status;

	msg.mask = UBLOXEVA8M_CFG_NAV5_MASK_DYN_MASK;
	msg.dynModel = dyn_model;

	status = send_msg(UBLOXEVA8M_CLASS_CFG, UBLOXEVA8M_MSG_CFG_NAV5, (uint8_t*)&msg, sizeof(msg));
	if (status) {
		return status;
	}

	/* Wait for Acknowledgment */
	if (!wait_for_ack()) {
		DBG_PRINTK("%s: Module returned NACK\n", __func__);
		return UBLOXEVA8M_NACK;
	}

	return UBLOXEVA8M_SUCCESS;
}

#ifdef DEBUG_MSG
static void print_msg(char* txt, ubloxeva8m_ubx_msg* msg) {
	char buf[256];
	snprintf(buf, 256, "%s Class: 0x%X Msg: 0x%X Len: %d Checksum: 0x%X Payload: 0x", txt, msg->class_id, msg->message_id, msg->length, msg->checksum);
	for(int i = 0; i < msg->length; i++) {
		snprintf(buf, 256, "%s%02X", buf, msg->payload[i]);
	}
	DBG_PRINTK("%s\n", buf);
}
#endif

/* Thread definition for the UBlox EVA 8M module */
K_THREAD_DEFINE(ubloxeva8m_thread_id, UBLOXEVA8M_STACKSIZE, ubloxeva8m_thread, NULL, NULL, NULL, UBLOXEVA8M_PRIORITY, 0, K_NO_WAIT);
