/**
 * Project Untitled
 */


#include "lora_udp_pkt.h"
#include <stdexcept>
#include <cstdio>
#include <string>
#include <rapidjson/document.h>

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
 * Creates a new lora_udp_pkt instance
 * @return void
 */
lora_udp_pkt::lora_udp_pkt() {
	this->protocol_version = -1;
	this->random_token = 0;
	this->gateway_mac_addr = 0;
	this->rapidjson_doc = NULL;
	this->packet_type = UNKNOWN_TYPE;
}

/**
 * @param uint8_t* data
 * @param int size
 * @return void
 */
void lora_udp_pkt::parse(uint8_t* data, int size) {
	if (size >= LORA_UDP_PKT_MIN_SIZE) {
		this->protocol_version = data[0];
		std::memcpy(&this->random_token, &data[1], sizeof(this->random_token));
		this->packet_type = static_cast<lora_udp_pkt_types>(data[3]);
		std::memcpy(&this->gateway_mac_addr, &data[4], LORA_UDP_PKT_GATEWAY_ADDR_SIZE);
		rapidjson::StringStream s((char *)(&data[4 + LORA_UDP_PKT_GATEWAY_ADDR_SIZE]));



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
