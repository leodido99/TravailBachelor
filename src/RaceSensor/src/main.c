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

#include "debug.h"
/**
 * Set debug
 */
#define DEBUG DEBUG_MAIN

#include "packet_manager.h"
#include "RN2483_lora.h"
#include "UBloxEVA8M.h"
#include "leds.h"

#define FATAL_ERR_LED_DELAY 100

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

	DBG_PRINTK("%s: Initializing packet manager\n", __func__);
	err = pkt_mngr_init();
	if (err < 0) {
		fatal_error("Cannot init packet manager");
	}

	DBG_PRINTK("%s: Starting packet manager\n", __func__);
	err = pkt_mngr_start();
	if (err < 0) {
		fatal_error("Cannot start packet manager");
	}

	DBG_PRINTK("%s: Starting\n", __func__);

}
