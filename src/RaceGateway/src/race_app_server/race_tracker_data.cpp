/** 
 * @file race_tracker_data.cpp
 * @brief Handles the race tracker data
 *
 * This module handles the race tracker data
 * and the accesses to the database
 *
 * @author Léonard Bise
 * @date   Aug 6, 2018
 */
#include "race_tracker_data.h"

#include <string>
#include <exception>

race_tracker_data::race_tracker_data(std::string connection_str)
{
	this->connection_str = connection_str;	
}

race_tracker_data::~race_tracker_data()
{
	// TODO Auto-generated destructor stub
}

int race_tracker_data::insert_data_point(race_mode_record* data_point)
{
	pqxx::connection C(connection_str);
	if (!C.is_open()) {
		throw std::runtime_error("Cannot connect to database " + connection_str);
	}


	std::string insert_query = "INSERT INTO race_tracker.data_point (competitor_id, competition_id, sequence, time_stamp, position, heart_rate, cadence, nb_satellites, position_dop, status)" \
			 	   "VALUES (1, 1, 0, '2018-05-26 08:30:00-00', ST_MakePoint(46.9933, 6.91612), 130, 160, 5, 9.9, 0);";


	// INSERT INTO race_tracker.data_point (competitor_id, competition_id, sequence, time_stamp, position, heart_rate, cadence, nb_satellites, position_dop, status)
	// VALUES (1, 1, 0, '2018-05-26 08:30:00-00', ST_MakePoint(46.9933, 6.91612), 130, 160, 5, 9.9, 0);


	return 0;
}
