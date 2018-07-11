/** 
 * @file testmodehandler.cpp
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 21, 2018
 */
#include "test_mode_handler.h"

#include "vector_reader.h"
#include "logger.h"
#include "test_mode_record.h"

#include <iostream>
#include <stdexcept>

#define TEST_MODE_FIX_VAL1 0xFEEDDEAD
#define TEST_MODE_FIX_VAL2 0xACABFACE

test_mode_handler::test_mode_handler() {
	this->exp_cnt = 0;
	this->nb_errors = 0;
	this->tot_pkt = 0;
	this->nb_discarded = 0;
	this->init = false;
}

test_mode_handler::~test_mode_handler() {
}

void test_mode_handler::handle(lora_rxpk_parser* rxpk) {
	vector_reader myreader(rxpk->get_decoded_data());
	double lat, lon, hdop;
	uint8_t nb_sv;
	uint32_t cnt;
	std::string status("OK");

	if (myreader.get_next_32bits() == TEST_MODE_FIX_VAL1 && myreader.get_next_32bits() == TEST_MODE_FIX_VAL2) {
		/* Extract lat/lon + counter */
		lat = myreader.get_next_double();
		lon = myreader.get_next_double();
		nb_sv = myreader.get_next_8bits();
		hdop = myreader.get_next_double();
		cnt = myreader.get_next_32bits();
		if (!this->init) {
			this->init = true;
			this->exp_cnt = cnt + 1;
		} else {
			if (cnt != this->exp_cnt) {
				this->nb_errors++;
				status = "NOK";
			}
			this->exp_cnt = cnt + 1;
		}
		/* Save data point */
		this->positions.push_back(new test_mode_record(std::string("Packet #") + std::to_string(this->tot_pkt), lat, lon, hdop, nb_sv, cnt, rxpk));
		this->tot_pkt++;
		log(logINFO) << "test_mode_handler::handle: status=" << status << " cnt=" << cnt << " expcnt=" << this->exp_cnt << " tot_pkt=" << this->tot_pkt << " lat=" << lat << " lon=" << lon << " hdop=" << hdop << " nb_sv=" << unsigned(nb_sv);
	} else {
		this->nb_discarded++;
	}
}

double test_mode_handler::get_percent_error() const {
	return (double)this->nb_errors/this->tot_pkt;
}

std::string test_mode_handler::print() {
	std::stringstream mystr;
	mystr << "test_mode_handler(exp_cnt=" << this->exp_cnt << " total packet=" << this->tot_pkt << " nb errors=" << this->nb_errors << " percent error=" << this->get_percent_error() << " nb discarded=" << this->nb_discarded << ")";
	return mystr.str();
}

void test_mode_handler::reset() {
	this->init = false;
	this->exp_cnt = 0;
	this->nb_discarded = 0;
	this->nb_errors = 0;
	this->tot_pkt = 0;
	this->positions.clear();
}

void test_mode_handler::dump_positions(std::ofstream& file) {
	if (file.is_open()) {
		// Header https://mymaps.us/
		// mymaps.us file << "lat,lng,name,color,note" << std::endl;
		// http://www.gpsvisualizer.com/map_input?form=googleearth
		// name,desc,latitude,longitude
		file << "# " << *this << std::endl;
		file << "name,desc,latitude,longitude" << std::endl;
		for (std::list<test_mode_record*>::iterator it=this->positions.begin(); it != this->positions.end(); ++it) {
			std::cout << ">> pos:" << (*it)->get_line() << std::endl;
			file << (*it)->get_line() << std::endl;
		}
	} else {
		throw std::runtime_error("File is not open!:");
	}
}

/**
 * Insertion operator used to print class content
 * @param strm
 * @param a
 * @return
 */
std::ostream& operator<<(std::ostream &strm, const test_mode_handler &a) {
	return strm << "test_mode_handler(exp_cnt=" << a.exp_cnt << " total packet=" << a.tot_pkt << " nb errors=" << a.nb_errors << " percent error=" << a.get_percent_error() << " nb discarded=" << a.nb_discarded << ")";
}

int test_mode_handler::get_nb_positions() {
	return this->positions.size();
}
