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

#define DEBUG 1

#define DBG_PRINTK(fmt, ...) \
            do { if (DEBUG) printk(fmt, __VA_ARGS__); } while (0)

#endif /* SRC_DEBUG_H_ */
