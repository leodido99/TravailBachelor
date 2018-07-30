/** 
 * @file race_mode_handler.cpp
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 30, 2018
 */

#include "race_mode_handler.h"
#include "vector_reader.h"
#include "race_mode_record.h"

#include <iostream>
#include <sstream>

#define RACE_PKT_MARKER 0xF0CACC1A

#define RACE_PKT_LATLON_SCALE 1.0e-7

#define RACE_PKT_PDOP_SCALE 0.01

race_mode_handler::race_mode_handler()
{
	this->nb_discarded = 0;

}

race_mode_handler::~race_mode_handler()
{


}

void race_mode_handler::handle(lora_rxpk_parser* rxpk)
{
	vector_reader myreader(rxpk->get_decoded_data());
	race_mode_record race_record;

	if (myreader.get_next_32bits() == RACE_PKT_MARKER) {
		race_record.set_id(myreader.get_next_16bits());
		/* TODO Timestamp */
		myreader.get_next_64bits();
		//race_record.set_timestamp();
		race_record.set_status(myreader.get_next_8bits());
		race_record.set_seq(myreader.get_next_16bits());
		int32_t lat_raw = myreader.get_next_signed_32bits();
		race_record.set_lat((double)lat_raw * RACE_PKT_LATLON_SCALE);
		int32_t lon_raw = myreader.get_next_signed_32bits();
		race_record.set_lon((double)lon_raw * RACE_PKT_LATLON_SCALE);
		race_record.set_nb_sv(myreader.get_next_8bits());
		uint16_t hdop_raw = myreader.get_next_16bits();
		race_record.set_hdop((double)hdop_raw * RACE_PKT_PDOP_SCALE);
		race_record.set_heart_rate(myreader.get_next_8bits());
		race_record.set_cadence(myreader.get_next_8bits());

		std::cout << race_record.to_string();
	} else {
		this->nb_discarded++;
	}
}

std::string race_mode_handler::print()
{
	std::stringstream mystr;
	mystr << "race_mode_handler(nb discarded=" << this->nb_discarded << ")";
	return mystr.str();
}
