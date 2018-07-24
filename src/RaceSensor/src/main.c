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

#include "packet_manager.h"
#include "debug.h"

#include "RN2483_lora.h"
#define LORA_SPREADING_FACTOR "sf7"
#define LORA_POWER 1
#include "UBloxEVA8M.h"

/**
 * Set debug
 */
#define DEBUG DEBUG_MAIN

/**
 * Function called in case of a fatal error
 */
static void fatal_error(char *msg)
{
	DBG_PRINTK("FATAL ERROR! %s\n", msg);

	while(1);
}

/* TODO Move to debug */
#ifdef DEBUG
int thread_cnt = 0;
static void print_thread(const struct k_thread *thread, void *user_data) {
	printk("%d - %p : priority=%d", thread_cnt, thread, thread->base.prio);
	if (thread == k_current_get()) {
		printk(" <- Current Thread");
	}
	printk("\n");
	thread_cnt++;
}
#endif

void main(void)
{
	int err;

#ifdef DEBUG
	/* Define CONFIG_OBJECT_TRACING=y CONFIG_THREAD_MONITOR=y for this feature to work */
	printk("Threads defined:\n");
	k_thread_foreach(print_thread, NULL);
#endif

	DBG_PRINTK("%s: Initializing packet manager\n", __func__);
	err = pkt_mngr_init();
	if (err) {
		fatal_error("Cannot init packet manager");
	}

	DBG_PRINTK("%s: Starting packet manager\n", __func__);
	err = pkt_mngr_start();
	if (err) {
		fatal_error("Cannot start packet manager");
	}

	DBG_PRINTK("%s: Starting\n", __func__);

}
