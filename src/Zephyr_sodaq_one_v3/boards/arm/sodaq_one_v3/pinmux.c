/*
 * Copyright (c) 2018 Henrik Brix Andersen <henrik@brixandersen.dk>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <init.h>
#include <pinmux.h>

static int board_pinmux_init(struct device *dev)
{
	struct device *muxa = device_get_binding(CONFIG_PINMUX_SAM0_A_LABEL);
	struct device *muxb = device_get_binding(CONFIG_PINMUX_SAM0_B_LABEL);

	ARG_UNUSED(dev);

#if CONFIG_UART_SAM0_SERCOM2_BASE_ADDRESS
	/* SERCOM2 LoRa UART on RX=PA13/pad 1, TX=PA12/pad 0 */
	pinmux_pin_set(muxa, 12, PINMUX_FUNC_D);
	pinmux_pin_set(muxa, 13, PINMUX_FUNC_D);
#endif

#if CONFIG_UART_SAM0_SERCOM5_BASE_ADDRESS
	/* SERCOM5 on RX=PB3/pad 1, TX=PB2/pad 0 */
	pinmux_pin_set(muxb, 2, PINMUX_FUNC_D);
	pinmux_pin_set(muxb, 3, PINMUX_FUNC_D);
#endif

#if CONFIG_UART_SAM0_SERCOM0_BASE_ADDRESS
#error Pin mapping is not configured
#endif
#if CONFIG_UART_SAM0_SERCOM1_BASE_ADDRESS
#error Pin mapping is not configured
#endif
#if CONFIG_UART_SAM0_SERCOM3_BASE_ADDRESS
#error Pin mapping is not configured
#endif
#if CONFIG_UART_SAM0_SERCOM4_BASE_ADDRESS
#error Pin mapping is not configured
#endif

#if CONFIG_SPI_SAM0_SERCOM4_BASE_ADDRESS
	/* SPI SERCOM2 on MISO=PA8/pad 0, MOSI=PA10/pad 2, SCK=PA11/pad 3 */
	/* TODO */
	pinmux_pin_set(muxa, 8, PINMUX_FUNC_D);
	pinmux_pin_set(muxa, 10, PINMUX_FUNC_D);
	pinmux_pin_set(muxa, 11, PINMUX_FUNC_D);
#endif

#if CONFIG_SPI_SAM0_SERCOM0_BASE_ADDRESS
#error Pin mapping is not configured
#endif
#if CONFIG_SPI_SAM0_SERCOM1_BASE_ADDRESS
#error Pin mapping is not configured
#endif
#if CONFIG_SPI_SAM0_SERCOM2_BASE_ADDRESS
#error Pin mapping is not configured
#endif
#if CONFIG_SPI_SAM0_SERCOM3_BASE_ADDRESS
#error Pin mapping is not configured
#endif
#if CONFIG_SPI_SAM0_SERCOM5_BASE_ADDRESS
#error Pin mapping is not configured
#endif

#ifdef CONFIG_USB_DC_SAM0
	/* USB DP on PA25, USB DM on PA24 */
	pinmux_pin_set(muxa, 25, PINMUX_FUNC_G);
	pinmux_pin_set(muxa, 24, PINMUX_FUNC_G);
#endif

	return 0;
}

SYS_INIT(board_pinmux_init, PRE_KERNEL_1, CONFIG_PINMUX_INIT_PRIORITY);
