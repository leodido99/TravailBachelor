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

#include "sensor_config.h"
#include "RN2483_lora.h"
#include "UBloxEVA8M.h"
#include "LSM303AGR.h"

#include <kernel.h>
#include <string.h>

/**
 * Set debug
 */
#define DEBUG DEBUG_PKT_MNGR

/**
 * Marker used to signal the beginning of the packet
 */
#define PKT_MANAGER_SYNC_MARKER 0xF0CACC1A

/**
 * Size of the packet buffer
 */
#define PKT_MANAGER_BUFFER_SIZE 56

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
#define PKT_MANAGER_THREAD_INTERVAL K_MSEC(1000)

/* The thread id for LoRa */
extern const k_tid_t packet_mngr_thread_id;

/* Semaphore used to wait for GPS updates */
K_SEM_DEFINE(pkt_mngr_gps_sem, 0, 1);

struct pkt_mngr_data {
	bool initialized;
	k_tid_t thread_id;
	struct k_thread *thread_data;
	struct k_sem *gps_sem;
	ubloxeva8m_nav_pvt_t last_pvt_msg;
};

struct pkt_mngr_data pkt_mngr = {
	.initialized = false,
	.gps_sem = &pkt_mngr_gps_sem
};

/**
 * Buffer used to hold a packet
 */
static u8_t buf[PKT_MANAGER_BUFFER_SIZE];

#ifdef DEBUG
static void print_nav_pvt_msg(char *txt, ubloxeva8m_nav_pvt_t* msg) {
	DBG_PRINTK("UBX-NAV-PVT: %d.%d.%d %02d:%02d:%02d validity=%d fixType=%d numSV=%d lat=%d lon=%d \n", msg->day, msg->month, msg->year, msg->hour, msg->minute, msg->seconds, msg->valid, msg->fixType, msg->numSV, msg->lat, msg->lon);
}
#endif

static void gps_msg_callback(ubloxeva8m_ubx_msg* msg)
{
	if (msg->class_id == UBLOXEVA8M_CLASS_NAV && msg->message_id == UBLOXEVA8M_MSG_NAV_PVT) {
		memcpy(&pkt_mngr.last_pvt_msg, msg->payload, msg->length);
		print_nav_pvt_msg("UBX-NAV-PVT: ", &pkt_mngr.last_pvt_msg);
	}
}

static void wait_for_gps_update()
{
	/* Wait for GPS message */
	k_sem_take(pkt_mngr.gps_sem, K_FOREVER);
}

static int build_packet(u8_t *buffer, int buffer_size)
{
	int pkt_size = 0;

	wait_for_gps_update();
	/* Write packet synch marker */
	buffer[pkt_size] = (PKT_MANAGER_SYNC_MARKER >> 24) & 0xFF;
	pkt_size++;
	buffer[pkt_size] = (PKT_MANAGER_SYNC_MARKER >> 16) & 0xFF;
	pkt_size++;
	buffer[pkt_size] = (PKT_MANAGER_SYNC_MARKER >> 8) & 0xFF;
	pkt_size++;
	buffer[pkt_size] = PKT_MANAGER_SYNC_MARKER & 0xFF;
	pkt_size++;
	/* */


	return PKT_MNGR_SUCCESS;
}

static void pkt_mngr_thread(void)
{
	int pkt_size;
	DBG_PRINTK("%s: Enter thread\n", __func__);

	while (1) {
		k_sleep(PKT_MANAGER_THREAD_INTERVAL);
		/* Build packet */
		if (build_packet(buf, PKT_MANAGER_BUFFER_SIZE) < 0) {
			DBG_PRINTK("Couldn't build packet\n");
		}
		/* Send through LoRa */
		if (rn2483_lora_radio_tx(buf, PKT_MANAGER_BUFFER_SIZE)) {
			DBG_PRINTK("Couldn't send packet\n");
		}
	}
}

static int configure_lsm303agr(void)
{
	int err;

	/* Initialize the LSM303AGR module */
	err = lsm303agr_init(CONFIG_I2C_SAM0_SERCOM3_LABEL);
	if (err) {
		DBG_PRINTK("%s: Can't init LSM303AGR %d\n", __func__, err);
		return PKT_MNGR_ACCEL_DEVICE_FAILURE;
	}

	/* Enable the accelerometer */
	err = lsm303agr_accel_enable(LSM303AGR_NORMAL_MODE, LSM303AGR_HIGH_RES_100HZ, false, (LSM303AGR_Z_AXIS | LSM303AGR_Y_AXIS | LSM303AGR_X_AXIS));
	if (err) {
		DBG_PRINTK("%s: Can't enable accelerometer %d\n", __func__, err);
		return PKT_MNGR_ACCEL_DEVICE_FAILURE;
	}

	/* Set accelerometer scale */
	if (lsm303agr_accel_set_scale(LSM303AGR_8G)) {
		printk("Couldn't set accelerometer scale\n");
	}

	return PKT_MNGR_SUCCESS;
}

static int configure_rn2483(void)
{
	int err;

	/* Initialize the UART */
	err = rn2483_lora_init(CONFIG_UART_SAM0_SERCOM2_LABEL);
	if (err) {
		DBG_PRINTK("%s: Can't init RN2483 %d\n", __func__, err);
		return PKT_MNGR_LORA_DEVICE_FAILURE;
	}

	/* Pause mac layer */
	err = rn2483_lora_pause_mac();
	if (err) {
		DBG_PRINTK("%s: Can't pause mac layer %d\n", __func__, err);
		return PKT_MNGR_LORA_DEVICE_FAILURE;
	}

	/* Set spreading factor */
	err = rn2483_lora_radio_set_sf(LORA_SPREADING_FACTOR);
	if (err) {
		DBG_PRINTK("%s: Can't set spreading factor %d\n", __func__, err);
		return PKT_MNGR_LORA_DEVICE_FAILURE;
	}

	/* Set power output */
	err = rn2483_lora_radio_set_pwr(LORA_POWER_OUTPUT);
	if (err) {
		DBG_PRINTK("%s: Can't set power output %d\n", __func__, err);
		return PKT_MNGR_LORA_DEVICE_FAILURE;
	}

	return PKT_MNGR_SUCCESS;
}

static int configure_gps(void)
{
	int err;

	/* Initialize the gps */
	err = ubloxeva8m_init(CONFIG_I2C_SAM0_SERCOM3_LABEL);
	if (err) {
		DBG_PRINTK("%s: Can't initialize UBloxEVA8M %d\n", __func__, err);
		return PKT_MNGR_GPS_DEVICE_FAILURE;
	}

	/* Set callback function */
	ubloxeva8m_set_callback(gps_msg_callback);

	/* Start module */
	err = ubloxeva8m_start();
	if (err) {
		DBG_PRINTK("%s: Can't start GPS module %d\n", __func__, err);
		return PKT_MNGR_GPS_DEVICE_FAILURE;
	}

	/* Set the dynamic model used by the GPS */
	err = ubloxeva8m_set_dynamic_model(UBLOXEVA8M_DYNMODEL_AUTOMOTIVE);
	if (err) {
		DBG_PRINTK("%s: Can't set dynamic model %d\n", __func__, err);
		return PKT_MNGR_GPS_DEVICE_FAILURE;
	}

	return PKT_MNGR_SUCCESS;
}

int pkt_mngr_init(void)
{
	int err;

	if (pkt_mngr.initialized) {
		return PKT_MNGR_SUCCESS;
	}

	DBG_PRINTK("%s: Initializing RN2483\n", __func__);
	/* Configure the LoRa chip */
	err = configure_rn2483();
	if (err) {
		return err;
	}

	DBG_PRINTK("%s: Initializing UBloxEVA8M\n", __func__);
	/* Configure the GPS */
	err = configure_gps();
	if (err) {
		return err;
	}

	DBG_PRINTK("%s: Initializing LSM303AGR\n", __func__);
	/* Configure the accelerometer */
	err = configure_lsm303agr();
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
