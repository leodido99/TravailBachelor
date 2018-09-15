/** 
 * @file debug.c
 * @brief Debug module
 *
 * Provides debug functions
 *
 * @author Léonard Bise
 * @date   Jul 26, 2018
 */

#include "debug.h"

#include <zephyr.h>

static int dbg_thread_cnt = 0;

#if defined(CONFIG_OBJECT_TRACING) && defined(CONFIG_THREAD_MONITOR)
static void print_thread(const struct k_thread *thread, void *user_data) {
	printk("%d - %p : priority=%d", dbg_thread_cnt, thread, thread->base.prio);
	if (thread == k_current_get()) {
		printk(" <- Current Thread");
	}
	printk("\n");
	dbg_thread_cnt++;
}

void dbg_print_threads(void)
{
	printk("---------------------------------\n");
	printk("Threads:\n");
	printk("---------------------------------\n");
	dbg_thread_cnt = 0;
	k_thread_foreach(print_thread, NULL);
	printk("---------------------------------\n");
}
#endif
