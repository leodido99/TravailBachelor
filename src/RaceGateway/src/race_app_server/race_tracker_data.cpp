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

	c.prepare("insert_data_point", "INSERT INTO race_tracker.data_point "
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

	if (r.size() == 0) {
		log(logWARNING) << "Cannot find competition id";
		return -1;
	} else if (r.size() > 1) {
		log(logWARNING) << r.size() << " competition ids found instead of expected 1";
	}

	/* Only take the first result, if there are more than one it means the system is badly configured (Several sensors with the same ID) */
	auto row = r[0];

	log(logINFO) << "competitor_id = " << row["competitor_id"].c_str() << " competition_id = " << row["competition_id"].c_str();

	/* Create timestamp */
	std::stringstream mystr;
	mystr << unsigned(data_point->get_timestamp().get_year()) << "-" << unsigned(data_point->get_timestamp().get_month()) << "-";
	mystr << unsigned(data_point->get_timestamp().get_day()) << " " << unsigned(data_point->get_timestamp().get_hour()) << ":" << unsigned(data_point->get_timestamp().get_min());
	mystr << ":" << unsigned(data_point->get_timestamp().get_sec()) << "-00";

	t.prepared("insert_data_point")(row["competitor_id"].as<uint16_t>())
				       (row["competition_id"].as<uint16_t>())
				       (data_point->get_seq())
				       (mystr.str())
				       (data_point->get_lat())
				       (data_point->get_lon())
				       (static_cast<uint16_t>(data_point->get_heart_rate()))
				       (static_cast<uint16_t>(data_point->get_cadence()))
				       (static_cast<uint16_t>(data_point->get_nb_sv()))
				       (data_point->get_hdop())
				       (static_cast<uint16_t>(data_point->get_status())).exec();

	t.commit();

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
