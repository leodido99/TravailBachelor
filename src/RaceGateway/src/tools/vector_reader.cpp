/** 
 * @file vector_reader.cpp
 * @brief Class to manipulate vectors of bytes
 *
 * This class provides a way to extract data from a vector of byte
 *
 * @author Léonard Bise
 * @date   Jun 25, 2018
 */
#include "vector_reader.h"

#include <stdexcept>

vector_reader::vector_reader(const std::vector<unsigned char>& data) : data(data) {
	this->curr_pos = 0;
}

void vector_reader::reset() {
	this->curr_pos = 0;
}

uint16_t vector_reader::get_next_16bits() {
	uint16_t result = 0;
	if ((unsigned)this->get_size_left() >= sizeof(uint16_t)) {
		result |= static_cast<uint16_t>(data[this->curr_pos]) << 8;
		result |= static_cast<uint16_t>(data[this->curr_pos+1]);
		this->curr_pos += sizeof(uint16_t);
	} else {
		throw std::runtime_error("Not enough data");
	}
	return result;
}

uint32_t vector_reader::get_next_32bits() {
	uint32_t result = 0;
	if ((unsigned)this->get_size_left() >= sizeof(uint32_t)) {
		result |= static_cast<uint32_t>(data[this->curr_pos]) << 24;
		result |= static_cast<uint32_t>(data[this->curr_pos+1]) << 16;
		result |= static_cast<uint32_t>(data[this->curr_pos+2]) << 8;
		result |= static_cast<uint32_t>(data[this->curr_pos+3]);
		this->curr_pos += sizeof(uint32_t);
	} else {
		throw std::runtime_error("Not enough data");
	}
	return result;
}

uint64_t vector_reader::get_next_64bits() {
	uint64_t result = 0;
	if ((unsigned)this->get_size_left() >= sizeof(uint64_t)) {
		result |= static_cast<uint64_t>(data[this->curr_pos]) << 56;
		result |= static_cast<uint64_t>(data[this->curr_pos+1]) << 48;
		result |= static_cast<uint64_t>(data[this->curr_pos+2]) << 40;
		result |= static_cast<uint64_t>(data[this->curr_pos+3]) << 32;
		result |= static_cast<uint64_t>(data[this->curr_pos+4]) << 24;
		result |= static_cast<uint64_t>(data[this->curr_pos+5]) << 16;
		result |= static_cast<uint64_t>(data[this->curr_pos+6]) << 8;
		result |= static_cast<uint64_t>(data[this->curr_pos+7]);
		this->curr_pos += sizeof(uint64_t);
	} else {
		throw std::runtime_error("Not enough data");
	}
	return result;
}

int vector_reader::get_size_left() {
	return this->data.size() - this->curr_pos;
}
