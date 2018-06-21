/**
 * @file lora_pkt_fwd_parser.cpp
 * @brief LoRa packet forwarder parser
 *
 * Parses packets produced by a LoRa packet forwarder.
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */

#include "lora_pkt_fwd_parser.h"

#include "logger.h"

#include <stdexcept>
#include <cstring>

/**
 * Names associated with lora_pkt_fwd_types
 */
std::string lora_pkt_fwd_names[7] = { "PUSH_DATA", "PUSH_ACK", "PULL_DATA", "PULL_ACK", "PULL_RESP", "TX_ACK", "UNKNOWN_TYPE" };

/**
 * lora_pkt_fwd_parser implementation
 * 
 * LoRa packet forwader parser.
 * This class provides a way to parse packets sent by the packet forwarder.
 */

/**
 * LoRa Packet minimum size
 */
#define LORA_PKT_MIN_SIZE 13

/**
 * Constructor
 */
lora_pkt_fwd_parser::lora_pkt_fwd_parser() {
	this->protocol_version = -1;
	this->random_token = 0;
	this->pkt_type = UNKNOWN_TYPE;
	this->pkt_data = NULL;
	this->pkt_data_size = 0;
}

/**
 * Parse the provided data
 * @param data
 * @param size
 * @return void
 */
void lora_pkt_fwd_parser::parse(uint8_t* data, int size) {
	if (size >= LORA_PKT_MIN_SIZE) {
		log(logDEBUG2) << "lora_pkt_fwd_parser::parse: Extracting information from packet";
		/* Extract informations from the packet
		 * This format is described in this file https://github.com/Lora-net/packet_forwarder/blob/master/PROTOCOL.TXT */
		this->protocol_version = data[0];
		std::memcpy(&this->random_token, &data[1], sizeof(this->random_token));
		this->pkt_type = static_cast<lora_pkt_fwd_types>(data[3]);
		/* Set pointer to beginning of the data */
		this->pkt_data = &data[4];
		/* Set data size */
		this->pkt_data_size = size - sizeof(uint8_t) - sizeof(this->random_token) - sizeof(uint8_t);
	} else {
		throw std::runtime_error("Not enough data (actual=" + std::to_string(size) + " minimum=" + std::to_string(LORA_PKT_MIN_SIZE) + ")");
	}
    return;
}

/**
 * Returns the packet protocol version
 * @return int
 */
int lora_pkt_fwd_parser::get_protocol_version() const {
    return this->protocol_version;
}

/**
 * Returns the packet random token
 * @return uint16_t
 */
uint16_t lora_pkt_fwd_parser::get_random_token() const {
    return this->random_token;
}

/**
 * Returns the packet type
 * @return lora_pkt_fwd_types
 */
lora_pkt_fwd_types lora_pkt_fwd_parser::get_pkt_type() const {
    return this->pkt_type;
}

/**
 * Returns the packet data without the common header (Protocol version, random token and packet type)
 * @return uint8_t*
 */
uint8_t* lora_pkt_fwd_parser::get_pkt_data() const {
    return this->pkt_data;
}

/**
 * Returns the packet data size without the common header.
 * @return int
 */
int lora_pkt_fwd_parser::get_pkt_data_size() const {
    return this->pkt_data_size;
}

std::ostream& operator<<(std::ostream &strm, const lora_pkt_fwd_parser &a) {
  return strm << "lora_pkt_fwd_parser(protocol version=" << a.protocol_version << " random token=" << a.random_token << " packet type=" << lora_pkt_fwd_names[a.pkt_type];
}
