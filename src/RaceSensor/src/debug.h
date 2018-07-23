/** 
 * @file debug.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 29, 2018
 */
#ifndef SRC_DEBUG_H_
#define SRC_DEBUG_H_

/* Modules */
#define DEBUG_MAIN 1
#define DEBUG_PKT_MNGR 1

/* Drivers */
#define DEBUG_RN2483_LORA 1
#define DEBUG_LSM303AGR 1
#define DEBUG_UBLOXEVA8M 1

#define DBG_PRINTK(fmt, ...) \
            do { if (DEBUG) printk(fmt, ## __VA_ARGS__); } while (0)



#endif /* SRC_DEBUG_H_ */
