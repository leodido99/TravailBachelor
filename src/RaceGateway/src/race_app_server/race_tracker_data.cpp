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

#include "logger.h"

pqxx::prepare::invocation& prep_dynamic(unsigned char data, pqxx::prepare::invocation& inv)
{
    inv(data);
    return inv;
}

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
	pqxx::connection c(connection_str);
	pqxx::work t{c};

	if (!c.is_open()) {
		throw std::runtime_error("Cannot connect to database " + connection_str);
	}

	/*c.prepare("insert_data_point", "INSERT INTO race_tracker.data_point "
		  "(competitor_id, competition_id, sequence, time_stamp, position, heart_rate, cadence, nb_satellites, position_dop, status)"
		  "VALUES ($1, $2, $3, $4, ST_MakePoint($5, $6), $7, $8, $9, $10, $11);");

	c.prepare("get_ids", "select race_tracker.competitor_registration.competition_id, "
			     "race_tracker.competitor_registration.competitor_id from "
			     "race_tracker.competitor_registration INNER JOIN "
			     "race_tracker.competitor ON "
			     "(race_tracker.competitor_registration.competitor_id = "
			     "race_tracker.competitor.competitor_id) INNER JOIN "
			     "race_tracker.competition ON (race_tracker.competition.competition_id "
			     "= race_tracker.competitor_registration.competition_id) WHERE "
			     "race_tracker.competitor_registration.sensor_id = $1 "
			     "AND race_tracker.competition.active = True;");

	pqxx::result r = t.prepared("get_ids")(data_point->get_id()).exec();
	if (r.size() > 1) {
		throw std::runtime_error("Sensor active on " + std::to_string(r.size()) + " competitions");
	}

	auto row = r[0];

	log(logINFO) << "competitor_id = " << row["competitor_id"].c_str() << "competition_id = " << row["competition_id"].c_str();*/

uint16_t hr = static_cast<uint16_t>(data_point->get_heart_rate());
uint16_t cad = static_cast<uint16_t>(data_point->get_cadence());
uint16_t nbsv = static_cast<uint16_t>(data_point->get_nb_sv());
uint16_t status = static_cast<uint16_t>(data_point->get_status());

	/*t.prepared("insert_data_point")(row["competitor_id"].as<uint16_t>())
				       (row["competition_id"].as<uint16_t>())
				       (data_point->get_seq())
				       (0)*/ /* TODO Timestamp */
				       /*(data_point->get_lat())
				       (data_point->get_lon())
				       (0)
				       (0)
				       (0)
				       (data_point->get_hdop())
				       (0).exec();*/






	// INSERT INTO race_tracker.data_point (competitor_id, competition_id, sequence, time_stamp, position, heart_rate, cadence, nb_satellites, position_dop, status)
	// VALUES (1, 1, 0, '2018-05-26 08:30:00-00', ST_MakePoint(46.9933, 6.91612), 130, 160, 5, 9.9, 0);


	return 0;
}

pqxx::result race_tracker_data::get_active_competitions(uint16_t sensor_id)
{
	pqxx::connection c(connection_str);
	pqxx::work t{c};

	if (!c.is_open()) {
		throw std::runtime_error("Cannot connect to database " + connection_str);
	}

	c.prepare("get_active_competitions", "select race_tracker.competition.competition_id from "
			  	  	     "race_tracker.competitor_registration INNER JOIN "
			                     "race_tracker.competitor ON "
			                     "(race_tracker.competitor_registration.competitor_id = "
			                     "race_tracker.competitor.competitor_id) INNER JOIN "
			                     "race_tracker.competition ON (race_tracker.competition.competition_id "
			                     "= race_tracker.competitor_registration.competition_id) WHERE "
			                     "race_tracker.competitor_registration.sensor_id = $1 "
			                     "AND race_tracker.competition.active = True;");

	return t.prepared("get_active_competitions")(sensor_id).exec();
}
