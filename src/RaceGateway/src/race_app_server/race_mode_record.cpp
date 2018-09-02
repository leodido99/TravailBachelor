/** 
 * @file race_mode_record.cpp
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 30, 2018
 */
#include "race_mode_record.h"

#include <sstream>
#include <iostream>
#include <iomanip>

race_mode_record::race_mode_record()
{
	this->id = RACE_PKT_NO_ID;
	this->status = 0;
	this->seq = 0;
	this->lat = 0;
	this->lon = 0;
	this->nb_sv = 0;
	this->hdop = 0;
	this->heart_rate = 0;
	this->cadence = 0;
}

race_mode_record::~race_mode_record()
{
	// TODO Auto-generated destructor stub
}

std::string race_mode_record::to_string()
{
	std::stringstream mystr;

	mystr << "race_mode_record(" << "id: " << this->id << " Timestamp: " << unsigned(this->timestamp.get_year()) << "." << unsigned(this->timestamp.get_month());
	mystr << "." << unsigned(this->timestamp.get_day()) << " " << unsigned(this->timestamp.get_hour()) << ":" << unsigned(this->timestamp.get_min()) << ":" << unsigned(this->timestamp.get_sec());
	mystr << " Status: " << unsigned(this->status) << " Seq: " << this->seq;
	mystr << " Latitude: " << this->lat << " Longitude: " << this->lon << " Nb SV: " << unsigned(this->nb_sv) << " Position DOP: " << this->hdop;
	mystr << " Heart Rate: " << unsigned(this->heart_rate) << "  Cadence: " << unsigned(this->cadence);
	mystr << ")";

	return mystr.str();
}
