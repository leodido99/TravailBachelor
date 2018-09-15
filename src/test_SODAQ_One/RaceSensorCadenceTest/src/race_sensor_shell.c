/** 
 * @file race_sensor_shell.c
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 29, 2018
 */

#include "race_sensor_shell.h"

#include <zephyr.h>
#include <stdlib.h>
#include <stdio.h>

#include "RN2483_lora.h"
#include "race_sensor_manager.h"
#include "cadence.h"
#include "LSM303AGR.h"

int race_sensor_shell_set_lora_sf(int argc, char *argv[]) {
	int err;

	if (argc != 2) {
		printk("Wrong number of arguments\n");
		return -EINVAL;
	}

	err = rn2483_lora_radio_set_sf(argv[1]);
	if (err < 0) {
		printk("Couldn't set spreading factor %s : %d\n", argv[1], err);
		return err;
	} else {
		printk("Set LoRa spreading factor %s successfully\n", argv[1]);
	}

	return 0;
}

int race_sensor_shell_set_lora_pwr(int argc, char *argv[]) {
	int err;
	s8_t pwr;
	char *end;

	if (argc != 2) {
		printk("Wrong number of arguments\n");
		return -EINVAL;
	}

	pwr = strtol(argv[1], &end, 0);

	err = rn2483_lora_radio_set_pwr(pwr);
	if (err < 0) {
		printk("Couldn't set power output %d : %d\n", pwr, err);
		return err;
	} else {
		printk("Set LoRa power output %d successfully\n", pwr);
	}

	return 0;
}

int race_sensor_shell_set_msg_interval(int argc, char *argv[]) {
	int interval;
	char *end;

	if (argc != 2) {
		printk("Wrong number of arguments\n");
		return -EINVAL;
	}

	interval = strtol(argv[1], &end, 0);
	race_sensor_mngr_set_msg_interval(interval);

	return 0;
}

int race_sensor_shell_cadence_samples(int argc, char *argv[])
{
	cadence_print();
	return 0;
}

int race_sensor_shell_cadence_reset(int argc, char *argv[])
{
	cadence_reset_samples();
	return 0;
}

int race_sensor_shell_cadence_scale(int argc, char *argv[])
{
	int16_t value;
	double scaled = 0;
	char buffer[56];

	if (argc < 2) {
		printk("Wrong number of arguments\n");
		return -EINVAL;
	}

	value = strtol(argv[1], NULL, 0);
	scaled = lsm303agr_get_scaled_value(value);

	snprintf(buffer, 56, "value=%d scaled=%f\n", value, scaled);
	printk("%s", buffer);

	return 0;
}

