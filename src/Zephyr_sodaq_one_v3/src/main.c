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

#include "debug.h"
#include "RN2483_lora.h"
#include "LSM303AGR.h"
#include "UBloxEVA8M.h"

#define LORA_SPREADING_FACTOR "radio set sf sf7"
#define LORA_POWER "radio set pwr 1"

#ifdef DEBUG
static void print_nav_pvt_msg(char *txt, ubloxeva8m_nav_pvt_t* msg);
#endif

void gps_msg_handler(ubloxeva8m_ubx_msg* msg) {
	ubloxeva8m_nav_pvt_t pvt_msg;

	if (msg->class_id == UBLOXEVA8M_CLASS_NAV && msg->message_id == UBLOXEVA8M_MSG_NAV_PVT) {
		memcpy(&pvt_msg, msg->payload, msg->length);
		print_nav_pvt_msg("UBX-NAV-PVT: ", &pvt_msg);
	}

}

void main(void)
{
	static struct device *led0, *led1, *led2;
	int ret;
	u8_t data_buf[20] = { 0x00, 0x01, 0x02, 0x03, 0x04 };
	lsm303agr_status_t accel_status;
	int16_t accel[3];

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

#if 0
	if (rn2483_lora_init(CONFIG_UART_SAM0_SERCOM2_LABEL)) {
		printk("Couldn't initialize LoRa\n");
	} else {
		printk("LoRa initialized\n");
	}
	/* Setup the RN2483 for radio */
	/* Pause mac layer */
	if (rn2483_lora_cmd("mac pause\r\n")) {
		printk("Couldn't mac pause\n");
	}
	rn2483_lora_wait_for_any_reply();
	/* Set spreading factor */
	if (rn2483_lora_cmd(LORA_SPREADING_FACTOR)) {
		printk("Couldn't set spreading factor\n");
	}
	rn2483_lora_wait_for_reply("ok");
	/* Set power output */
	if (rn2483_lora_cmd(LORA_POWER)) {
		printk("Couldn't set power output\n");
	}
	rn2483_lora_wait_for_reply("ok");

	rn2483_lora_radio_tx(data_buf, 5);
	rn2483_lora_wait_for_reply("radio_tx_ok");

	data_buf[0] = 0x05;
	data_buf[1] = 0x06;
	data_buf[2] = 0x07;
	data_buf[3] = 0x08;
	data_buf[4] = 0x09;
	rn2483_lora_radio_tx(data_buf, 5);
	rn2483_lora_wait_for_reply("radio_tx_ok");
#endif

	ubloxeva8m_init(CONFIG_I2C_SAM0_SERCOM3_LABEL);
	/* Configure message handler */
	ubloxeva8m_set_callback(gps_msg_handler);
	if (ubloxeva8m_start()) {
		printk("Couldn't start UBLOXEVA8M\n");
	}
	/* Change dynamic model */
	if (ubloxeva8m_set_dynamic_model(UBLOXEVA8M_DYNMODEL_AUTOMOTIVE)) {
		printk("Couldn't set dynamic model\n");
	}

	lsm303agr_init(CONFIG_I2C_SAM0_SERCOM3_LABEL);
	printk("LSM303AGR Accelerometer Device ID: %X\n", lsm303agr_accel_get_device_id());
	printk("LSM303AGR Magnetometer Device ID: %X\n", lsm303agr_mag_get_device_id());
	if (lsm303agr_accel_enable(LSM303AGR_NORMAL_MODE, LSM303AGR_HIGH_RES_100HZ, false, (LSM303AGR_Z_AXIS | LSM303AGR_Y_AXIS | LSM303AGR_X_AXIS))) {
		printk("Couldn't enable accelerometer\n");
	}
	if (lsm303agr_accel_set_scale(LSM303AGR_8G)) {
		printk("Couldn't set accelerometer scale\n");
	}
	if (lsm303agr_get_status(&accel_status)) {
		printk("Couldn't read status");
	}
	printk("Acelerometer status: Overrun (all=%d x=%d y=%d z=%d) Data Available (all=%d x=%d y=%d =z%d)\n", accel_status.z_y_x_overrun, accel_status.x_overrun, accel_status.y_overrun, accel_status.z_overrun, accel_status.z_y_x_available, accel_status.x_available, accel_status.y_available, accel_status.z_available);
	if (lsm303agr_get_x_acceleration(&accel[0])) {
		printk("Couldn't read acceleration");
	}
	if (lsm303agr_get_y_acceleration(&accel[1])) {
		printk("Couldn't read acceleration");
	}
	if (lsm303agr_get_z_acceleration(&accel[2])) {
		printk("Couldn't read acceleration");
	}
	printk("Acceleration x=%d y=%d z=%d\n", accel[0], accel[1], accel[2]);

	while(1) {
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
	}
}

#ifdef DEBUG
static void print_nav_pvt_msg(char *txt, ubloxeva8m_nav_pvt_t* msg) {
	DBG_PRINTK("UBX-NAV-PVT: %d.%d.%d %02d:%02d:%02d validity=%d fixType=%d numSV=%d lat=%d lon=%d \n", msg->day, msg->month, msg->year, msg->hour, msg->minute, msg->seconds, msg->valid, msg->fixType, msg->numSV, msg->lat, msg->lon);
}
#endif
