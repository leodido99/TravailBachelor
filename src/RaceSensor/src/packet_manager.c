/** 
 * @file packet_manager.c
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 23, 2018
 */

#include "packet_manager.h"

#include "debug.h"
#include "RN2483_lora.h"
#include "sensor_config.h"

#include <kernel.h>

/**
 * Set debug
 */
#define DEBUG DEBUG_PKT_MNGR

/**
 * Stack size allocated for the packet manager thread
 */
#define PKT_MANAGER_STACK_SIZE 512

/**
 * Thread priority for the packet manager thread
 */
#define PKT_MANAGER_PRIORITY 10

/**
 * The interval between two thread execution
 */
#define PKT_MANAGER_THREAD_INTERVAL_MS 500

/**
 * Declares the stack for the packet manager thread
 */
//K_THREAD_STACK_DEFINE(pkt_mngr_thread_stack, PKT_MANAGER_STACK_SIZE);

/* The thread id for LoRa */
extern const k_tid_t packet_mngr_thread_id;

struct pkt_mngr_data {
	bool initialized;
	k_tid_t thread_id;
	struct k_thread *thread_data;

} pkt_mngr;


static void pkt_mngr_thread(void)
{
	DBG_PRINTK("%s: Enter thread\n", __func__);

	while (1) {
		k_sleep(K_MSEC(PKT_MANAGER_THREAD_INTERVAL_MS));
		/* Build packet */

		/* Send through LoRa */

	}
}

int configure_rn2483(void)
{
	int err;

	/* Pause mac layer */
	err = rn2483_lora_pause_mac();
	if (err) {
		DBG_PRINTK("%s: Can't pause mac layer %d\n", __func__, err);
		return PKT_MNGR_DEVICE_FAILURE;
	}

	/* Set spreading factor */
	err = rn2483_lora_radio_set_sf(LORA_SPREADING_FACTOR);
	if (err) {
		DBG_PRINTK("%s: Can't set spreading factor %d\n", __func__, err);
		return PKT_MNGR_DEVICE_FAILURE;
	}

	/* Set power output */
	err = rn2483_lora_radio_set_pwr(LORA_POWER_OUTPUT);
	if (err) {
		DBG_PRINTK("%s: Can't set power output %d\n", __func__, err);
		return PKT_MNGR_DEVICE_FAILURE;
	}

	return PKT_MNGR_SUCCESS;
}

int pkt_mngr_init(void)
{
	int err;

	if (pkt_mngr.initialized) {
		return PKT_MNGR_SUCCESS;
	}

	/* Initialize the UART */
	err = rn2483_lora_init(CONFIG_UART_SAM0_SERCOM2_LABEL);
	if (err) {
		DBG_PRINTK("%s: Can't init RN2483 %d\n", __func__, err);
		return PKT_MNGR_DEVICE_FAILURE;
	}

	/* Configure the LoRa chip */
	err = configure_rn2483();
	if (err) {
		return err;
	}

	pkt_mngr.initialized = true;

	return PKT_MNGR_SUCCESS;
}

int pkt_mngr_start(void)
{
	if (!pkt_mngr.initialized) {
		return PKT_MNGR_NOT_INITIALIZED;
	}

	k_thread_start(packet_mngr_thread_id);

	return PKT_MNGR_SUCCESS;
}

/* Thread definition for the RN2483 LoRa module */
K_THREAD_DEFINE(packet_mngr_thread_id, PKT_MANAGER_STACK_SIZE, pkt_mngr_thread, NULL, NULL, NULL, PKT_MANAGER_PRIORITY, 0, K_FOREVER);
