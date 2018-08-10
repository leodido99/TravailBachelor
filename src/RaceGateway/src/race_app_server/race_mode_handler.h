/** 
 * @file race_mode_handler.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 30, 2018
 */
#ifndef SRC_RACE_APP_SERVER_RACE_MODE_HANDLER_H_
#define SRC_RACE_APP_SERVER_RACE_MODE_HANDLER_H_

#include "rxpk_handler.h"
#include "race_tracker_data.h"

class race_mode_handler: public rxpk_handler {
private:
	int nb_discarded;
	int tot_pkt;
	race_tracker_data *db;

public:
	/**
	 * Constructor
	 */
	race_mode_handler();

	/**
	 * Destructor
	 */
	~race_mode_handler();

	/**
	 * Handles an rxpk packet
	 * @param rxpk
	 */
	void handle(lora_rxpk_parser* rxpk);

	/**
	 * Print the class
	 */
	std::string print();

	/**
	 * Returns the number of discarded packets
	 * @return
	 */
	int get_nb_discarded() const {
		return nb_discarded;
	}
};

#endif /* SRC_RACE_APP_SERVER_RACE_MODE_HANDLER_H_ */
