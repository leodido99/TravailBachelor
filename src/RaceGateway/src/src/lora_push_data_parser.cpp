/**
 * Project Untitled
 */


#include "lora_push_data_parser.h"

#include "logger.h"

#include <iostream>
#include <cstring>

#include <rapidjson/document.h>

/**
 * Size of the gateway address
 */
#define LORA_PKT_GATEWAY_ADDR_SIZE (sizeof(uint8_t) * 8)

/**
 * lora_push_data_parser implementation
 * 
 * LoRa packet forwarder PUSH_DATA parser. This class provides a way to parse packets sent by the packet forwarder.
 */

/**
 * Names associated with lora_push_data_types
 */
std::string lora_push_data_types_names[3] = { "stat", "rxpk", "unknown type" };

/**
 * Constructor
 */
lora_push_data_parser::lora_push_data_parser() {
	this->data_type = UNKNOWN;
	this->gateway_mac_addr = 0;
}

/**
 * Parse the packet data of a PUSH_DATA
 * @param data
 * @param size
 */
void lora_push_data_parser::parse(uint8_t* data, int size) {
	rapidjson::Document doc;

	/* Extract parameters from data array */
	std::memcpy(&this->gateway_mac_addr, data, LORA_PKT_GATEWAY_ADDR_SIZE);
	/* Ensure string is zero terminated */
	data[size] = '\0';
	this->json_string = std::string((char *)&data[LORA_PKT_GATEWAY_ADDR_SIZE]);

	log(logDEBUG4) << "lora_push_data_parser::parse: Parsing " << this->json_string;
	rapidjson::StringStream packet_stream(this->json_string.c_str());
	doc.ParseStream(packet_stream);
	/* Get main json member name which define what type of packet has been received */
	rapidjson::Value::ConstMemberIterator fileIt = doc.MemberBegin();
	std::string member(fileIt->name.GetString());
	if (member.compare("stat") == 0) {
		this->data_type = STAT;
	} else if (member.compare("rxpk") == 0) {
		this->data_type = RXPK;
	}
	log(logDEBUG4) << "lora_push_data_parser::parse: Main json member name: " << lora_push_data_types_names[this->data_type];
}

/**
 * Returns the type of the packet data
 * @return lora_push_data_types
 */
lora_push_data_types lora_push_data_parser::get_data_type() const {
    return this->data_type;
}

/**
 * Returns the json string of the packet data
 * @return string
 */
std::string lora_push_data_parser::get_json_string() const {
    return this->json_string;
}

std::ostream& operator<<(std::ostream &strm, const lora_push_data_parser &a) {
	// "decoded data=" << a.get_decoded_data() <<
	return strm << "lora_push_data_parser(" << "data type=" << lora_push_data_types_names[a.data_type] << " gateway mac addr=" << a.gateway_mac_addr << " json=" << a.json_string << ")";
}
