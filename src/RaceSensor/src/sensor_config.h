/** 
 * @file sensor_config.h
 * @brief RaceSensor configuration
 *
 * This file provides the configuration of the RaceSensor
 *
 * @author Léonard Bise
 * @date   Jul 23, 2018
 */
#ifndef SRC_SENSOR_CONFIG_H_
#define SRC_SENSOR_CONFIG_H_

/* ************************************************************************* */
/* GPIO									     */
/* ************************************************************************* */
/**
 * Heart Rate connected on PA7 GPIO
 */
#define GPIO_HR_DEV CONFIG_GPIO_SAM0_PORTB_LABEL
#define GPIO_HR_PIN 11
//#define GPIO_HR_DEV CONFIG_GPIO_SAM0_PORTB_LABEL
//#define GPIO_HR_PIN 8
//#define GPIO_HR_DEV CONFIG_GPIO_SAM0_PORTB_LABEL
//#define GPIO_HR_PIN 8

/* ************************************************************************* */
/* LoRa									     */
/* ************************************************************************* */
/**
 * LoRa spreading factor
 * (Between sf7 and sf12)
 */
#define LORA_SPREADING_FACTOR "sf7"

/**
 * LoRa power output
 */
#define LORA_POWER_OUTPUT 1

#endif /* SRC_SENSOR_CONFIG_H_ */
