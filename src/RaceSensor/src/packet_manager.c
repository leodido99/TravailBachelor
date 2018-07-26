/** 
 * @file packet_manager.c
 * @brief Packet Manager module
 *
 * The Packet Manager module is responsible for periodically
 * creating and sending the Race Tracking Packet containing
 * the information about the competitor
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
#include "race_tracking_packet.h"
#include "leds.h"

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
 * Stack size allocated for the packet manager thread
 */
#define PKT_MANAGER_STACK_SIZE 1024

/**
 * Thread priority for the packet manager thread
 */
#define PKT_MANAGER_PRIORITY 10

/**
 * ID of the sensor
 */
#define PKT_MANAGER_SENSOR_ID 0xBEEF

/**
 * The interval between two thread execution
 */
#define PKT_MANAGER_THREAD_INTERVAL K_MSEC(5000)

/**
 * The minimum number of satellites we are waiting for during GPS fix
 */
#define PKT_MANAGER_GPS_FIX_NB_SV 4

/**
 * The fix type we are waiting for during GPS fix
 */
#define PKT_MANAGER_GPS_FIX_TYPE (UBLOXEVA8M_NAV_PVT_FIXTYPE_2D_FIX_MASK \
				| UBLOXEVA8M_NAV_PVT_FIXTYPE_3D_FIX_MASK)

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
	u16_t pkt_seq;
	u8_t heart_rate;
	u8_t cadence;
};

struct pkt_mngr_data pkt_mngr = {
	.initialized = false,
	.gps_sem = &pkt_mngr_gps_sem,
	.pkt_seq = 0,
	.heart_rate = 0,
	.cadence = 0
};

/**
 * Buffer used to hold a packet
 */
struct pkt_mngr_race_pkt pkt_buffer;

#ifdef DEBUG
static void print_nav_pvt_msg(char *txt, ubloxeva8m_nav_pvt_t* msg) {
	DBG_PRINTK("UBX-NAV-PVT: %d.%d.%d %02d:%02d:%02d:%03d validity=%d fixType=%d numSV=%d lat=%d lon=%d \n", msg->day, msg->month, msg->year, msg->hour, msg->minute, msg->seconds, msg->nano, msg->valid, msg->fixType, msg->numSV, msg->lat, msg->lon);
}
#endif

static void gps_msg_callback(ubloxeva8m_ubx_msg* msg)
{
	if (msg->class_id == UBLOXEVA8M_CLASS_NAV && msg->message_id == UBLOXEVA8M_MSG_NAV_PVT) {
		memcpy(&pkt_mngr.last_pvt_msg, msg->payload, sizeof(ubloxeva8m_nav_pvt_t));
		print_nav_pvt_msg("UBX-NAV-PVT: ", &pkt_mngr.last_pvt_msg);
		k_sem_give(pkt_mngr.gps_sem);
	}
}

static void wait_for_gps_update()
{
	/* Wait for new GPS message */
	k_sem_reset(pkt_mngr.gps_sem);
	k_sem_take(pkt_mngr.gps_sem, K_FOREVER);
}

static void get_timestamp(u8_t *buffer) {
	/* TODO */
}

static void wait_for_fix() {
	bool led_state = false;

	DBG_PRINTK("%s: Waiting for GPS fix...\n", __func__);

	do {
		wait_for_gps_update();
		leds_set(LED_GREEN, led_state);
		led_state = led_state ? false : true;
	} while(pkt_mngr.last_pvt_msg.numSV < PKT_MANAGER_GPS_FIX_NB_SV ||
		pkt_mngr.last_pvt_msg.fixType & PKT_MANAGER_GPS_FIX_TYPE);

	leds_set(LED_GREEN, true);

	DBG_PRINTK("%s: Found GPS fix Num SV=%d FixType=0x%x\n", __func__, pkt_mngr.last_pvt_msg.numSV, pkt_mngr.last_pvt_msg.fixType);
}

static int build_packet(struct pkt_mngr_race_pkt *packet)
{
	/* In order to have the most precise GPS position we
	 * wait for a new one */
	wait_for_gps_update();

	packet->marker = PKT_MANAGER_SYNC_MARKER;

	packet->id = PKT_MANAGER_SENSOR_ID;

	get_timestamp(packet->timestamp);

	packet->status = 0;

	packet->counter = pkt_mngr.pkt_seq;
	pkt_mngr.pkt_seq++;

	packet->latitude = pkt_mngr.last_pvt_msg.lat;

	packet->longitude = pkt_mngr.last_pvt_msg.lon;

	packet->nb_sv = pkt_mngr.last_pvt_msg.numSV;

	packet->pdop = pkt_mngr.last_pvt_msg.pDOP;

	packet->heart_rate = pkt_mngr.heart_rate;

	packet->cadence = pkt_mngr.cadence;

	return PKT_MNGR_SUCCESS;
}

static void pkt_mngr_thread(void)
{
	DBG_PRINTK("%s: Enter thread\n", __func__);

	/* Before sending packets we are going to wait to have a good GPS fix
	 * in order to provide precise positions */
	wait_for_fix();

	while (1) {
		k_sleep(PKT_MANAGER_THREAD_INTERVAL);
		/* Build packet */
		if (build_packet(&pkt_buffer) < 0) {
			DBG_PRINTK("Couldn't build packet\n");
		}
		/* Send through LoRa */
		if (rn2483_lora_radio_tx((u8_t*)&pkt_buffer, sizeof(struct pkt_mngr_race_pkt))) {
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

void pkt_mngr_set_heart_rate(u8_t heart_rate) {
	pkt_mngr.heart_rate = heart_rate;
}

void pkt_mngr_set_cadence(u8_t cadence) {
	pkt_mngr.cadence = cadence;
}

/* Thread definition for the RN2483 LoRa module */
K_THREAD_DEFINE(packet_mngr_thread_id, PKT_MANAGER_STACK_SIZE, pkt_mngr_thread, NULL, NULL, NULL, PKT_MANAGER_PRIORITY, 0, K_FOREVER);
