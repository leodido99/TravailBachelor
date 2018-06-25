/** 
 * @file rxpkhandler.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 09, 2018
 */

#ifndef SRC_RACE_APP_SERVER_RXPK_HANDLER_H_
#define SRC_RACE_APP_SERVER_RXPK_HANDLER_H_

#include "lora_rxpk_parser.h"

#include <string>

class rxpk_handler {

public:
	/**
	 * Destructor
	 */
	virtual ~rxpk_handler() {}

	/**
	 * Handles an rxpk packet
	 * @param rxpk
	 */
	virtual void handle(lora_rxpk_parser* rxpk) = 0;

	/**
	 * Print the class
	 */
	virtual std::string print() = 0;
};

#endif /* SRC_RACE_APP_SERVER_RXPK_HANDLER_H_ */
