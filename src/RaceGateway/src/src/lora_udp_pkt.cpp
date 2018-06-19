/**
 * Project Untitled
 */


#include "lora_udp_pkt.h"
#include <stdexcept>
#include <cstdio>
#include <string>

#include <rapidjson/document.h>

#include "logger.h"

#include <rapidjson/stringbuffer.h>// debug
#include <rapidjson/writer.h>//debug
#include <iostream> //debug
/**
 * lora_udp_pkt implementation
 */

/**
 * LoRa UDP Packet minimum size
 */
#define LORA_UDP_PKT_MIN_SIZE 13

/**
 * Size of the gateway address
 */
#define LORA_UDP_PKT_GATEWAY_ADDR_SIZE (sizeof(uint8_t) * 8)

/**
 * Names associated with lora_udp_pkt_types
 */
std::string lora_udp_pkt_names[7] = { "PUSH_DATA", "PUSH_ACK", "PULL_DATA", "PULL_ACK", "PULL_RESP", "TX_ACK", "UNKNOWN_TYPE" };

/**
 * Creates a new lora_udp_pkt instance
 * @return void
 */
lora_udp_pkt::lora_udp_pkt() {
	this->protocol_version = -1;
	this->random_token = 0;
	this->gateway_mac_addr = 0;
	this->rapidjson_doc = NULL;
	this->packet_type = UNKNOWN_TYPE;
	this->json_obj = "unknown";
}

/**
 * Destructor
 */
lora_udp_pkt::~lora_udp_pkt() {
	if (this->rapidjson_doc != NULL) {
		delete(this->rapidjson_doc);
	}
}

/**
 * @param uint8_t* data
 * @param int size
 * @return void
 */
void lora_udp_pkt::parse(uint8_t* data, int size) {
	log(logDEBUG4) << "lora_udp_pkt::parse";
	if (size >= LORA_UDP_PKT_MIN_SIZE) {
		this->protocol_version = data[0];
		std::memcpy(&this->random_token, &data[1], sizeof(this->random_token));
		this->packet_type = static_cast<lora_udp_pkt_types>(data[3]);
		std::memcpy(&this->gateway_mac_addr, &data[4], LORA_UDP_PKT_GATEWAY_ADDR_SIZE);

		log(logDEBUG4) << "lora_udp_pkt::parse: Parsing json";
		rapidjson::StringStream packet_stream((char *)(&data[4 + LORA_UDP_PKT_GATEWAY_ADDR_SIZE]));
		this->rapidjson_doc = new rapidjson::Document();
		this->rapidjson_doc->ParseStream(packet_stream);
		rapidjson::Value::ConstMemberIterator fileIt = this->rapidjson_doc->MemberBegin();
		this->json_obj = fileIt->name.GetString();
		log(logDEBUG4) << "lora_udp_pkt::parse: Finished parsing json: First member " << this->json_obj;

		std::cout << "Dumping json" << std::endl;
		rapidjson::StringBuffer buffer;
		buffer.Clear();
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		this->rapidjson_doc->Accept(writer);
		std::cout << "Doc: " << buffer.GetString() << std::endl;
	} else {
		throw std::runtime_error("Not enough data (actual=" + std::to_string(size) + " minimum=" + std::to_string(LORA_UDP_PKT_MIN_SIZE) + ")");
	}
    return;
}

/**
 * Returns the type of packet
 * @return lora_udp_pkt_types
 */
lora_udp_pkt_types lora_udp_pkt::get_pkt_type() {
    return this->packet_type;
}

/**
 * Returns a rapidjson Document instance containing the packet info as described by the LoRa packet forwarder UDP protocol
 * @return Document
 */
rapidjson::Document* lora_udp_pkt::get_rapidjson_doc() {
    return this->rapidjson_doc;
}

/**
 * Returns the protocol version
 * @return int
 */
int lora_udp_pkt::get_protocol_version() {
    return this->protocol_version;
}

/**
 * Returns the gateway mac address if applicable
 * @return uint64_t
 */
uint64_t lora_udp_pkt::get_gateway_mac_addr() {
    return this->gateway_mac_addr;
}

/**
 * Returns the packet random token
 * @return uint16_t
 */
uint16_t lora_udp_pkt::get_random_token() {
    return this->random_token;
}

/**
 * Returns the main json object name
 * @return
 */
std::string lora_udp_pkt::get_json_obj_name() {
	return this->json_obj;
}

/**
 * Returns a string representing the class instance
 * @return
 */
std::string lora_udp_pkt::get_string() {
	return lora_udp_pkt_names[this->packet_type] + " protocol version=" + std::to_string(this->protocol_version) + " gateway id=" + std::to_string(this->gateway_mac_addr) + " random token=" + std::to_string(this->random_token) + " json object name=" + this->json_obj;
}

std::ostream& operator<<(std::ostream &strm, const lora_udp_pkt &a) {
  return strm;// << "A(" << a.i << ")";
}
