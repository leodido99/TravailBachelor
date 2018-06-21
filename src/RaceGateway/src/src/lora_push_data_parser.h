/**
 * @file lora_push_data_parser.h
 * @brief LoRa PUSH_DATA packet forwarder parser
 *
 * Parses packets produced by a LoRa packet forwarder and which
 * are of type PUSH_DATA.
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */

#ifndef _LORA_PUSH_DATA_PARSER_H
#define _LORA_PUSH_DATA_PARSER_H

#include <string>
#include <cstdint>

enum lora_push_data_types { STAT, RXPK, UNKNOWN };

/**
 * Names associated with lora_push_data_types
 */
extern std::string lora_push_data_types_names[];

class lora_push_data_parser {
private:
    /**
	 * Packet json string
	 */
	std::string json_string;
	/**
	 * Type of PUSH_DATA
	 */
	lora_push_data_types data_type;
	/**
	 * Gateway MAC address
	 */
	uint64_t gateway_mac_addr;
public: 
	/**
	 * Constructor
	 */
	lora_push_data_parser();
    
	/**
	 * Parse the packet data of a PUSH_DATA
	 * @param data
	 * @param size
	 */
	void parse(uint8_t* data, int size);

	/**
	 * Returns the type of the packet data
	 */
	lora_push_data_types get_data_type() const;

	/**
	 * Returns the json string of the packet data
	 */
	std::string get_json_string() const;

	/**
	 * Friend insertion operator to be able to print class content
	 * @param strm
	 * @param a
	 * @return
	 */
	friend std::ostream& operator<<(std::ostream &strm, const lora_push_data_parser &a);
};

#endif //_LORA_PUSH_DATA_PARSER_H
