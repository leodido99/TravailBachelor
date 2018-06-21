/**
 * @file lora_pkt_fwd_parser.h
 * @brief LoRa packet forwarder parser
 *
 * Parses packets produced by a LoRa packet forwarder.
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */

#ifndef _LORA_PKT_FWD_PARSER_H
#define _LORA_PKT_FWD_PARSER_H

#include <string>
#include <cstdint>

/**
 * The different types of packets used by the LoRa packet forwarder UDP protocol
 */
enum lora_pkt_fwd_types {
	PUSH_DATA,
	PUSH_ACK,
	PULL_DATA,
	PULL_ACK,
	PULL_RESP,
	TX_ACK,
	UNKNOWN_TYPE
};

/**
 * Names associated with lora_pkt_fwd_types
 */
extern std::string lora_pkt_fwd_names[];

class lora_pkt_fwd_parser {

private:
    /**
	 * LoRa Packet Forwarder protocol version
	 */
	int protocol_version;
	/**
	 * Random token used to identify the packet
	 */
	uint16_t random_token;
	/**
	 * Type of the packet
	 */
	lora_pkt_fwd_types pkt_type;
	/**
	 * Pointer to the beginning of the packet data
	 */
	uint8_t* pkt_data;
	/**
	 * Size of the packet data
	 */
	int pkt_data_size;
public: 
	/**
	 * Constructor
	 */
	lora_pkt_fwd_parser();

	/**
	 * Parse the provided data
	 * @param data
	 * @param size
	 */
	void parse(uint8_t* data, int size);

	/**
	 * Returns the packet protocol version
	 */
	int get_protocol_version() const;

	/**
	 * Returns the packet random token
	 */
	uint16_t get_random_token() const;

	/**
	 * Returns the packet type
	 */
	lora_pkt_fwd_types get_pkt_type() const;

	/**
	 * Returns the packet data without the common header (Protocol version, random token and packet type)
	 */
	uint8_t* get_pkt_data() const;

	/**
	 * Returns the packet data size without the common header.
	 */
	int get_pkt_data_size() const;

	/**
	 * Friend insert operator to be able to print class to stream
	 */
	friend std::ostream& operator<<(std::ostream &strm, const lora_pkt_fwd_parser &a);
};

#endif //_LORA_PKT_FWD_PARSER_H
