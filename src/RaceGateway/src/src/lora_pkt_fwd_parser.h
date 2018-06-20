/**
 * Project Untitled
 */


#ifndef _LORA_PKT_FWD_PARSER_H
#define _LORA_PKT_FWD_PARSER_H

#include <string>
#include <stdint.h>

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
	int get_protocol_version();

	/**
	 * Returns the packet random token
	 */
	uint16_t get_random_token();

	/**
	 * Returns the packet type
	 */
	lora_pkt_fwd_types get_pkt_type();

	/**
	 * Returns the packet data without the common header (Protocol version, random token and packet type)
	 */
	uint8_t* get_pkt_data();

	/**
	 * Returns the packet data size without the common header.
	 */
	int get_pkt_data_size();
};

#endif //_LORA_PKT_FWD_PARSER_H
