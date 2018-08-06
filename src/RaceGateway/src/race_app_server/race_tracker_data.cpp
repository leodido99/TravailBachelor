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

race_tracker_data::race_tracker_data()
{
	pqxx::connection C("dbname = testdb user = postgres password = cohondob \
	      hostaddr = 127.0.0.1 port = 5432");

}

race_tracker_data::~race_tracker_data()
{
	// TODO Auto-generated destructor stub
}

int race_tracker_data::insert_data_point(race_mode_record* data_point)
{
	std::string insert_query = "INSERT INTO race_tracker.data_point (competitor_id, competition_id, sequence, time_stamp, position, heart_rate, cadence, nb_satellites, position_dop, status)" \
			 	   "VALUES (1, 1, 0, '2018-05-26 08:30:00-00', ST_MakePoint(46.9933, 6.91612), 130, 160, 5, 9.9, 0);";


	// INSERT INTO race_tracker.data_point (competitor_id, competition_id, sequence, time_stamp, position, heart_rate, cadence, nb_satellites, position_dop, status)
	// VALUES (1, 1, 0, '2018-05-26 08:30:00-00', ST_MakePoint(46.9933, 6.91612), 130, 160, 5, 9.9, 0);

	return 0;
}
