/** 
 * @file debug.h
 * @brief Debug module
 *
 * Provides debug functions
 *
 * @author Léonard Bise
 * @date   Jun 29, 2018
 */
#ifndef SRC_DEBUG_H_
#define SRC_DEBUG_H_

/* Modules */
#define DEBUG_MAIN 1
#define DEBUG_RACE_SENSOR_MNGR 1

/* Drivers */
#define DEBUG_RN2483_LORA 1
#define DEBUG_LSM303AGR 1
#define DEBUG_UBLOXEVA8M 1
#define DEBUG_LEDS 1
#define DEBUG_HR 1

#define DBG_PRINTK(fmt, ...) \
            do { if (DEBUG) printk(fmt, ## __VA_ARGS__); } while (0)

#if defined(CONFIG_OBJECT_TRACING) && defined(CONFIG_THREAD_MONITOR)
/**
 * Prints all the threads
 */
void dbg_print_threads(void);
#endif

#endif /* SRC_DEBUG_H_ */
