/** 
 * @file test_mode_record.cpp
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 25, 2018
 */
#include "test_mode_record.h"

#include <sstream>
#include <iostream>

test_mode_record::test_mode_record(std::string name, double lat, double lon, uint32_t cnt, lora_rxpk_parser* pkt) : pkt(pkt) {
	this->cnt = cnt;
	this->lat = lat;
	this->lon = lon;
	this->name = name;
}

test_mode_record::~test_mode_record() {
	delete(this->pkt);
}

std::string test_mode_record::get_line() {
	// mymaps.us lat, lng, name, color, note
	std::stringstream mystr;
	//mystr << this->lat << "," << this->lon << "," << this->name << "," << "#FF0000" << ",\"" << *(this->pkt) << "\"";
	// http://www.gpsvisualizer.com/map_input?form=googleearth
	// name,desc,latitude,longitude
	mystr << this->name << ",\"" << "cnt=" << this->cnt << " " << *(this->pkt) << "\"," << this->lat << "," << this->lon;
	return mystr.str();
}