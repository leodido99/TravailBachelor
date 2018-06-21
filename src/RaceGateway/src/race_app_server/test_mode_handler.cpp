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
#include <iostream>

test_mode_handler::test_mode_handler() {
	this->cnt = 0;
	this->nb_errors = 0;
	this->nb_processed_bytes = 0;
}

test_mode_handler::~test_mode_handler() {
}

uint32_t test_mode_handler::getNext32bits(std::vector<unsigned char> data) {
	uint32_t result = 0;
	result |= static_cast<uint32_t>(data[this->nb_processed_bytes]) << 24;
	result |= static_cast<uint32_t>(data[this->nb_processed_bytes+1]) << 16;
	result |= static_cast<uint32_t>(data[this->nb_processed_bytes+2]) << 8;
	result |= static_cast<uint32_t>(data[this->nb_processed_bytes+3]);
	this->nb_processed_bytes += 4;
	return result;
}

void test_mode_handler::handle(lora_rxpk_parser rxpk) {
	std::cout << "I handle this: " << rxpk << std::endl;
	this->nb_processed_bytes = 0;
	std::cout << "data0: " << std::hex << this->getNext32bits(rxpk.get_decoded_data()) << std::endl;
	std::cout << "data1: " << std::hex << this->getNext32bits(rxpk.get_decoded_data()) << std::endl;
	std::cout << "data2: " << std::hex << this->getNext32bits(rxpk.get_decoded_data()) << std::endl;
	std::cout << "data3: " << std::hex << this->getNext32bits(rxpk.get_decoded_data()) << std::endl;
	std::cout << "data4: " << std::hex << this->getNext32bits(rxpk.get_decoded_data()) << std::endl;
	std::cout << "data5: " << std::hex << this->getNext32bits(rxpk.get_decoded_data()) << std::endl;
	std::cout << "data6: " << std::hex << this->getNext32bits(rxpk.get_decoded_data()) << std::endl;
	std::cout << "data7: " << std::hex << this->getNext32bits(rxpk.get_decoded_data()) << std::endl;
}
