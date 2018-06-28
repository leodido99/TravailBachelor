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

#define RN2483_LORA_SUCCESS 0
#define RN2483_LORA_BINDING_FAILED -1

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



#endif /* SRC_RN2483_LORA_H_ */
