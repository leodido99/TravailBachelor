/** 
 * @file testmodehandler.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 21, 2018
 */
#ifndef SRC_RACE_APP_SERVER_TEST_MODE_HANDLER_H_
#define SRC_RACE_APP_SERVER_TEST_MODE_HANDLER_H_

#include "rxpk_handler.h"

class test_mode_handler: public rxpk_handler {
public:
	/**
	 * Constructor
	 */
	test_mode_handler();

	~test_mode_handler();

	/**
	 * Handles an rxpk packet
	 * @param rxpk
	 */
	void handle(lora_rxpk_parser rxpk);
};

#endif /* SRC_RACE_APP_SERVER_TEST_MODE_HANDLER_H_ */
