/** 
 * @file race_sensor_shell.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 29, 2018
 */
#ifndef SRC_RACE_SENSOR_SHELL_H_
#define SRC_RACE_SENSOR_SHELL_H_

/**
 * Set the LoRa spreading factor
 * @param argc Number of arguments
 * @param argv Arguments array
 * @return 0 on success, negative errno code on failure
 */
int race_sensor_shell_set_lora_sf(int argc, char *argv[]);

/**
 * Set the LoRa power output
 * @param argc Number of arguments
 * @param argv Arguments array
 * @return 0 on success, negative errno code on failure
 */
int race_sensor_shell_set_lora_pwr(int argc, char *argv[]);

#endif /* SRC_RACE_SENSOR_SHELL_H_ */
