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
 * Pwr Output dBm mA
 * -3 -4.0 17.3
 * -2 -2.9 18.0
 * -1 -1.9 18.7
 * 0 -1.7 20.2
 * 1 -0.6 21.2
 * 2 0.4 22.3
 * 3 1.4 23.5
 * 4 2.5 24.7
 * 5 3.6 26.1
 * 6 4.7 27.5
 * 7 5.8 28.8
 * 8 6.9 30.0
 * 9 8.1 31.2
 * 10 9.3 32.4
 * 11 10.4 33.7
 * 12 11.6 35.1
 * 13 12.5 36.5
 * 14 13.5 38.0
 * 15 14.1 38.9
 */
#define LORA_POWER_OUTPUT 14

#endif /* SRC_SENSOR_CONFIG_H_ */
