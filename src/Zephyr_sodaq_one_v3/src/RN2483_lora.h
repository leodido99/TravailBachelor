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

#define RN2483_LORA_SUCCESS 0
#define RN2483_LORA_BINDING_FAILED -1
#define RN2483_LORA_BUSY -2
#define RN2483_LORA_CMD_TOO_LONG -3


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
 * Send a command to the LoRa module
 * \note The function automatically adds the '\r' and '\n' characters
 * @param cmd
 * @return
 */
int rn2483_lora_cmd(char* cmd);

/**
 * Wait for command to be sent
 */
void rn2483_lora_wait_cmd();

/**
 * Wait for any reply
 */
void rn2483_lora_wait_for_any_reply();

/**
 * Wait for the given reply
 * @param reply
 */
void rn2483_lora_wait_for_reply(char* reply);

/**
 * Send data through LoRa radio
 * @return
 */
int rn2483_lora_radio_tx(u8_t* data, u32_t size);

#endif /* SRC_RN2483_LORA_H_ */
