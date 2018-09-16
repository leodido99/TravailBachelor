/** 
 * @file main.c
 * @brief Entry point
 *
 * Entry point for the RaceSensor application
 *
 * @author Léonard Bise
 * @date   Jul 23, 2018
 */

#include <kernel.h>
#include <shell/shell.h>

#include "debug.h"
/**
 * Set debug
 */
#define DEBUG DEBUG_MAIN

#include "race_sensor_manager.h"
#include "RN2483_lora.h"
#include "UBloxEVA8M.h"
#include "leds.h"
#include "race_sensor_shell.h"

/**
 * Delay used to make the LED blink on fatal error
 */
#define FATAL_ERR_LED_DELAY 100

#define RACE_SENSOR_SHELL "race-sensor"

/**
 * Function called in case of a fatal error
 */
static void fatal_error(char *msg)
{
	DBG_PRINTK("FATAL ERROR! %s\n", msg);

	while(1) {
		leds_set(LED_RED, true);
		k_sleep(FATAL_ERR_LED_DELAY);
		leds_set(LED_RED, false);
		k_sleep(FATAL_ERR_LED_DELAY);
	}
}

static struct shell_cmd commands[] = {
	{ "set_lora_sf", race_sensor_shell_set_lora_sf, "<sf>\n\nSets the LoRa spreading factor (sf7 to sf12)"},
	{ "set_lora_pwr", race_sensor_shell_set_lora_pwr, "<pwr>\n\nSets the LoRa power output (-3 to 15)"},
	{ "set_msg_interval", race_sensor_shell_set_msg_interval , "<ms>\n\nSets the sensor message interval" },
	{ "cadence_samples" , race_sensor_shell_cadence_samples, ""},
	{ "cadence_reset" , race_sensor_shell_cadence_reset, ""},
	{ "cadence_scale", race_sensor_shell_cadence_scale, " <value>" },
	{ NULL, NULL, NULL }
};

void main(void)
{
	int err;

#if defined(CONFIG_OBJECT_TRACING) && defined(CONFIG_THREAD_MONITOR)
	dbg_print_threads();
#endif

	err = leds_init();
	if (err < 0) {
		fatal_error("Cannot init LEDs");
	}

	DBG_PRINTK("%s: Initializing race sensor manager\n", __func__);
	err = race_sensor_mngr_init();
	if (err < 0) {
		fatal_error("Cannot init race sensor manager");
	}

	DBG_PRINTK("%s: Starting race sensor manager\n", __func__);
	err = race_sensor_mngr_start();
	if (err < 0) {
		fatal_error("Cannot start race sensor manager");
	}

	DBG_PRINTK("%s: Starting\n", __func__);

	/* Setup race sensor shell */
	SHELL_REGISTER(RACE_SENSOR_SHELL, commands);
	shell_register_default_module(RACE_SENSOR_SHELL);

}
