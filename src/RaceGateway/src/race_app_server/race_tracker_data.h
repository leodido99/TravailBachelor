/** 
 * @file race_tracker_data.h
 * @brief Handles the race tracker data
 *
 * This module handles the race tracker data
 * and the accesses to the database
 *
 * @author Léonard Bise
 * @date   Aug 6, 2018
 */
#ifndef SRC_RACE_APP_SERVER_RACE_TRACKER_DATA_H_
#define SRC_RACE_APP_SERVER_RACE_TRACKER_DATA_H_

#include <pqxx/pqxx>
#include "race_mode_record.h"

class race_tracker_data {
private:
	pqxx::connection connection;

public:
	/**
	 * Constructor
	 */
	race_tracker_data(std::string connection_str);

	/**
	 * Destructor
	 */
	virtual ~race_tracker_data();

	/**
	 * Insert a data point into the database
	 * @param data_point
	 * @return
	 */
	int insert_data_point(race_mode_record *data_point);

};

#endif /* SRC_RACE_APP_SERVER_RACE_TRACKER_DATA_H_ */
