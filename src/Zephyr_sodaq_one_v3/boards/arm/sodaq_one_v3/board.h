/*
 * Copyright (c) 2018 Henrik Brix Andersen <henrik@brixandersen.dk>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __INC_BOARD_H
#define __INC_BOARD_H

#include <soc.h>

/* LED 0 (Red) on PA15 */
#define LED0_GPIO_PORT CONFIG_GPIO_SAM0_PORTA_LABEL
#define LED0_GPIO_PIN 15

/* LED 1 (Green) on PB10 */
#define LED1_GPIO_PORT CONFIG_GPIO_SAM0_PORTB_LABEL
#define LED1_GPIO_PIN 10

/* LED 2 (Blue) on PB11 */
#define LED2_GPIO_PORT CONFIG_GPIO_SAM0_PORTB_LABEL
#define LED2_GPIO_PIN 11

/* SERCOM0 on RX=PA11/pad 3, TX=PA10/pad 2 */
//#define CONFIG_UART_SAM0_SERCOM0_PADS \
/	(SERCOM_USART_CTRLA_RXPO(3) | SERCOM_USART_CTRLA_TXPO(1))

/* SERCOM5 on RX=PB3/pad 1, TX=PB2/pad 0 */
#define CONFIG_UART_SAM0_SERCOM5_PADS \
	(SERCOM_USART_CTRLA_RXPO(1) | SERCOM_USART_CTRLA_TXPO(0))

#define CONFIG_SPI_SAM0_SERCOM4_PADS \
	(SERCOM_SPI_CTRLA_DIPO(0) | SERCOM_SPI_CTRLA_DOPO(1))

#endif /* __INC_BOARD_H */
