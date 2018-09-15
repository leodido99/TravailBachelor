/*
 * cadence.c
 *
 *  Created on: 17 Aug 2018
 *      Author: leonard.bise
 */

#include "cadence.h"

#include <gpio.h>
#include <stdio.h>

#include "LSM303AGR.h"
#include "leds.h"
#include "sensor_config.h"

#include "debug.h"

#define DEBUG 1

extern const k_tid_t cadence_thread_id;

struct cadence {
	struct gpio_callback int1_cb;
	int step_cnt;
	s64_t start_time;
};

struct cadence cadence_priv;

//#define CADENCE_SAMPLING 0
#ifdef CADENCE_SAMPLING
/**
 * Stack size allocated for the packet manager thread
 */
#define CADENCE_STACK_SIZE 1024

/**
 * Thread priority for the packet manager thread
 */
#define CADENCE_PRIORITY 5000

#define CADENCE_INTERVAL K_MSEC(50)

#define CADENCE_NB_DATA_POINT 200 /* 10 seconds */

struct cadence_data {
	u8_t dev;
	/*double accel_x;
	double accel_y;
	double accel_z;*/
	u32_t ts;
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
};

struct cadence_data_points {
	u32_t nb_data_point;
	struct cadence_data data[CADENCE_NB_DATA_POINT];
};

struct cadence_data_points data_points = {
	.nb_data_point = 0
};

void cadence_print(void)
{
	char buffer[256];

	printk("Printing %d Cadence points:\ndevice\nx\ty\tz\n", data_points.nb_data_point);

	for (int i = 0; i < data_points.nb_data_point; i++) {
		snprintf(buffer, 256, "%d\t%d\t%f\t%f\t%f\n", data_points.data[i].dev, data_points.data[i].ts, lsm303agr_get_scaled_value(data_points.data[i].accel_x),
				lsm303agr_get_scaled_value(data_points.data[i].accel_y),
				lsm303agr_get_scaled_value(data_points.data[i].accel_z));
		printk("%s", buffer);
	}
}

void cadence_reset_samples(void)
{
	data_points.nb_data_point = 0;
}

static void cadence_thread(void)
{
	bool msg = true;
	bool led_state = false;

	while (1) {
		if (data_points.nb_data_point < CADENCE_NB_DATA_POINT) {
			data_points.data[data_points.nb_data_point].dev = lsm303agr_accel_get_device_id();

			if (lsm303agr_get_x_acceleration(&data_points.data[data_points.nb_data_point].accel_x) < 0) {
				DBG_PRINTK("Cannot get x accel\n");
			}

			if (lsm303agr_get_y_acceleration(&data_points.data[data_points.nb_data_point].accel_y) < 0) {
				DBG_PRINTK("Cannot get y accel\n");
			}

			if (lsm303agr_get_z_acceleration(&data_points.data[data_points.nb_data_point].accel_z) < 0) {
				DBG_PRINTK("Cannot get z accel\n");
			}

			data_points.data[data_points.nb_data_point].ts = k_uptime_get_32();

			data_points.nb_data_point++;

			leds_set(LED_RED, led_state);
			led_state = led_state ? false : true;
		} else if (msg) {
			DBG_PRINTK("Got all samples\n");
			msg = false;
			leds_set(LED_GREEN, led_state);
		}

		k_sleep(CADENCE_INTERVAL);
	}
}
#else
void cadence_print(void) {};
void cadence_reset_samples(void) {};
#endif

void cadence_gpio_callback(struct device *port, struct gpio_callback *cb, u32_t pins)
{
	if (cadence_priv.start_time == 0) {
		cadence_priv.start_time = k_uptime_get();
	}

	cadence_priv.step_cnt++;

	printk("Step!\n");
}

static int configure_lsm303agr(const char* device)
{
	int err;

	/* Initialize the LSM303AGR module */
	err = lsm303agr_init(device);
	if (err) {
		DBG_PRINTK("%s: Can't init LSM303AGR %d\n", __func__, err);
		return CADENCE_CONFIGURATION_ERROR;
	}

	/* Enable the accelerometer */
	err = lsm303agr_accel_enable(LSM303AGR_NORMAL_MODE, LSM303AGR_HIGH_RES_100HZ, false, (LSM303AGR_Z_AXIS | LSM303AGR_Y_AXIS | LSM303AGR_X_AXIS));
	if (err) {
		DBG_PRINTK("%s: Can't enable accelerometer %d\n", __func__, err);
		return CADENCE_CONFIGURATION_ERROR;
	}

	/* Configure interruption */
	err = lsm303agr_configure_interrupt1(LSM303AGR_INT_AND_COMBINATION,
			LSM303AGR_INT_DIR_Z_HIGH);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot configure interrupt 1", __func__);
		return CADENCE_CONFIGURATION_ERROR;
	}

	/* Set interrupt threshold */
	err = lsm303agr_set_interrupt_threshold(ACCELEROMETER_Z_THRESHOLD);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot set threshold", __func__);
	}

	/* Set duration of the interrupt */
	err = lsm303agr_set_interrupt_duration(ACCELEROMETER_INT_DURATION);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot set duration", __func__);
	}

	/* Enable interrupt */
	err = lsm303agr_enable_interrupt(LSM303AGR_INT_AOI1);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot enable interrupt 1", __func__);
		return CADENCE_CONFIGURATION_ERROR;
	}

	return 0;
}

static int setup_gpio_irq(const char* device, int pin)
{
	struct device *dev;
	int err;

	dev = device_get_binding(device);
	if (!dev) {
		DBG_PRINTK("%s: Binding to gpio failed\n", __func__);
		return CADENCE_BINDING_FAILED;
	}

	gpio_init_callback(&cadence_priv.int1_cb, cadence_gpio_callback, BIT(pin));

	err = gpio_add_callback(dev, &cadence_priv.int1_cb);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot add callback\n", __func__);
		return err;
	}

	/* Interruption on rising edge */
	err = gpio_pin_configure(dev, pin, (GPIO_DIR_IN | GPIO_INT |
			 	 GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH));
	if (err < 0) {
		DBG_PRINTK("%s: Cannot configure gpio pin %d", __func__, pin);
		return err;
	}

	err = gpio_pin_enable_callback(dev, pin);
	if (err < 0) {
		DBG_PRINTK("%s: Cannot add callback\n", __func__);
		return err;
	}

	return 0;
}

int cadence_init(const char* device, const char *irq_device, int irq_pin) {
	int err;

	err = configure_lsm303agr(device);
	if (err < 0) {
		return err;
	}

	err = setup_gpio_irq(irq_device, irq_pin);
	if (err < 0) {
		return err;
	}

#ifdef CADENCE_SAMPLING
	k_thread_start(cadence_thread_id);
#endif

	return 0;
}

u8_t cadence_get(void)
{
	return 0;
}

#ifdef CADENCE_SAMPLING
K_THREAD_DEFINE(cadence_thread_id, CADENCE_STACK_SIZE, cadence_thread, NULL, NULL, NULL, CADENCE_PRIORITY, 0, K_FOREVER);
#endif
