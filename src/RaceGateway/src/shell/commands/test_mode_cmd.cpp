/** 
 * @file test_mode_cmd.cpp
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 25, 2018
 */
#include "test_mode_cmd.h"

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>

test_mode_cmd::test_mode_cmd(test_mode_handler* handler) : handler(handler) {
	this->name = "test";
	this->cnt = 0;
}

test_mode_cmd::~test_mode_cmd() {
	// TODO Auto-generated destructor stub
}

std::string test_mode_cmd::get_curr_time() {
	std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char buffer [80];
	struct tm * timeinfo;
	timeinfo = localtime (&now_c);
	strftime(buffer,80,"%Y-%m-%d.%H-%M-%S",timeinfo);
	return std::string(buffer);
}

void test_mode_cmd::exec(std::list<std::string>* parameters) {
	/* Iterate over parameters */
	for (std::list<std::string>::iterator it=parameters->begin(); it != parameters->end(); ++it) {
		if ((*it).compare("dump") == 0) {
			if (this->handler->get_nb_positions() > 0) {
				std::ofstream myfile;
				std::string filename("dump/");
				filename.append(this->get_curr_time());
				/* Check if file name is provided */
				if (parameters->size() > 1) {
					it++;
					/* file name provided */
					filename.append(std::string("_") + *(it));
				} else {
					filename.append(std::string("_dump") + std::to_string(this->cnt));
				}
				filename.append(".log");
				std::cout << ">> Dumping " << this->handler->get_nb_positions() << " positions to " << filename << std::endl;
				myfile.open(filename);
				this->handler->dump_positions(myfile);
				myfile.close();
				this->cnt++;
			} else {
				std::cout << ">> Nothing to dump" << std::endl;
			}
		} else if ((*it).compare("reset") == 0) {
			std::cout << ">> Resetting test mode" << std::endl;
			this->handler->reset();
		} else {
			/* Unknown */
			throw std::runtime_error("Unknown parameter " + (*it));
		}
	}
}
