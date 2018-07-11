/** 
 * @file vector_reader.h
 * @brief Class to manipulate vectors of bytes
 *
 * This class provides a way to extract data from a vector of byte
 *
 * @author Léonard Bise
 * @date   Jul 09, 2018
 */
#ifndef SRC_TOOLS_VECTOR_READER_H_
#define SRC_TOOLS_VECTOR_READER_H_

#include <cstdint>
#include <vector>

class vector_reader {
private:
	int curr_pos;
	const std::vector<unsigned char>& data;

	/**
	 * Returns the number of bytes left in the vector
	 * @return
	 */
	int get_size_left();
public:
	/**
	 * Constructor
	 */
	vector_reader(const std::vector<unsigned char>& data);

	/**
	 * Resets the current position in the vector
	 */
	void reset();

	/**
	 * Read the next 8 bits
	 * @return
	 */
	uint8_t get_next_8bits();

	/**
	 * Read the next 16 bits
	 * @return
	 */
	uint16_t get_next_16bits();

	/**
	 * Read the next 32 bits
	 * @return
	 */
	uint32_t get_next_32bits();

	/**
	 * Read the next 64 bits
	 * @return
	 */
	uint64_t get_next_64bits();

	/**
	 * Read the next double
	 * @return
	 */
	double get_next_double();
};

#endif /* SRC_TOOLS_VECTOR_READER_H_ */
