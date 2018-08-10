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
#include <string>

class race_tracker_data {
private:
	std::string connection_str;

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

	/**
	 * Returns a list of active competitions for the given sensor ID
	 * @param sensor_id Sensor ID
	 * @return List of active competitions
	 */
	pqxx::result get_active_competitions(uint16_t sensor_id);

};

#endif /* SRC_RACE_APP_SERVER_RACE_TRACKER_DATA_H_ */
