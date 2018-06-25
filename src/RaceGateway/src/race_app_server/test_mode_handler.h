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
#include "test_mode_record.h"

#include <list>
#include <fstream>

class test_mode_handler: public rxpk_handler {
private:
	uint32_t exp_cnt;
	int nb_errors;
	int tot_pkt;
	int nb_discarded;
	bool init;
	std::list<test_mode_record*> positions;
public:
	/**
	 * Constructor
	 */
	test_mode_handler();

	/**
	 * Destructor
	 */
	~test_mode_handler();

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
	 * Reset the class
	 */
	void reset();

	/**
	 * Returns the percentage of errors
	 * @return
	 */
	double get_percent_error() const;

	/**
	 * Dump all the positions to a file
	 * @param file
	 */
	void dump_positions(std::ofstream& file);

	/**
	 * Returns the number of position
	 * @return
	 */
	int get_nb_positions();

	/**
	 * Returns the expected counter value for the next packet
	 * @return
	 */
	uint32_t get_exp_cnt() const {
		return exp_cnt;
	}

	/**
	 * Set the expected counter value for the next packet
	 * @return
	 */
	void set_exp_cnt(uint32_t expCnt) {
		exp_cnt = expCnt;
	}

	/**
	 * Returns the number of errors detected
	 * @return
	 */
	int get_nb_errors() const {
		return nb_errors;
	}

	/**
	 * Set the number of errors detected
	 * @return
	 */
	void set_nb_errors(int nbErrors) {
		nb_errors = nbErrors;
	}

	/**
	 * Returns the total number of packet received
	 * @return
	 */
	int get_tot_pkt() const {
		return tot_pkt;
	}

	/**
	 * Set the total number of packet received
	 * @param totPkt
	 */
	void set_tot_pkt(int totPkt) {
		tot_pkt = totPkt;
	}

	/**
	 * Friend insertion operator to be able to print private members to stream
	 * @param strm
	 * @param a
	 * @return
	 */
	friend std::ostream& operator<<(std::ostream &strm, const test_mode_handler &a);

	/**
	 * Returns the number of discarded packets
	 * @return
	 */
	int get_nb_discarded() const {
		return nb_discarded;
	}

	/**
	 * Set the number of discarded packets
	 * @param nbDiscarded
	 */
	void set_nb_discarded(int nbDiscarded) {
		nb_discarded = nbDiscarded;
	}
};

#endif /* SRC_RACE_APP_SERVER_TEST_MODE_HANDLER_H_ */
