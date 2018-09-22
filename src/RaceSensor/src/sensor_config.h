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
/* General								     */
/* ************************************************************************* */
/**
 * ID of the sensor
 */
#define SENSOR_ID 0x1

/**
 * Race managers wait for GPS fix before starting to send packets
 */
#define RACE_SENSOR_DO_GPS_FIX 1

/* ************************************************************************* */
/* GPIO									     */
/* ************************************************************************* */
/**
 * Heart Rate connected on PA7 GPIO
 */
/* TODO: Something is wrong because the pin on which the HR is supposed
 * to be PA11 but it doesn't work... Only setting PB11 works */
#define GPIO_HR_DEV CONFIG_GPIO_SAM0_PORTB_LABEL
#define GPIO_HR_PIN 11
//#define GPIO_HR_DEV CONFIG_GPIO_SAM0_PORTA_LABEL
//#define GPIO_HR_PIN 8

/**
 * Accelerometer interrupts connected on PA20 (INT2) and PA21 (INT1)
 */
#define ACCEL_INT_DEV CONFIG_GPIO_SAM0_PORTA_LABEL
#define ACCEL_INT1_PIN 21
#define ACCEL_INT2_PIN 20

/* ************************************************************************* */
/* LoRa									     */
/* ************************************************************************* */
/**
 * LoRa spreading factor
 * (Between sf7 and sf12)
 */
#define LORA_SPREADING_FACTOR "sf12"

/**
 * LoRa power output
 */
#define LORA_POWER_OUTPUT 1

#endif /* SRC_SENSOR_CONFIG_H_ */
