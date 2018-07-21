/** 
 * @file RN2483_lora.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 28, 2018
 */
#ifndef SRC_RN2483_LORA_H_
#define SRC_RN2483_LORA_H_

#include <zephyr.h>

/* Error Statuses returned by the RN2483 LoRa module */
#define RN2483_LORA_SUCCESS 0
#define RN2483_LORA_BINDING_FAILED -1
#define RN2483_LORA_BUSY -2
#define RN2483_LORA_CMD_TOO_LONG -3
#define RN2483_LORA_TOO_MANY_REPLIES -4
#define RN2483_LORA_ERR_REPLY -6

/**
 * Initialze the RN2483 LoRa module
 * @param device_name UART device name
 * @return
 */
int rn2483_lora_init(const char *device_name);

/**
 * Reset the RN2483 Lora module
 * @return
 */
int rn2483_lora_reset();

/**
 * Pause the MAC layer
 * \note Use when you only want LoRa radio
 * @return
 */
int rn2483_lora_pause_mac();

/**
 * Sets the spreading factor of the LoRa radio
 * @param sf Spreading factor (sf7 .. sf12)
 * @return
 */
int rn2483_lora_radio_set_sf(char* sf);

/**
 * Sets the power of the LoRa radio output
 * @param pwr
 * @return
 */
int rn2483_lora_radio_set_pwr(u8_t pwr);

/**
 * Send data through LoRa radio
 * @return
 */
int rn2483_lora_radio_tx(u8_t* data, u32_t size);

#endif /* SRC_RN2483_LORA_H_ */
