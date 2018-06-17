/**
 * Project Untitled
 */


#include "lora_udp_pkt.h"

/**
 * lora_udp_pkt implementation
 */


/**
 * Creates a new lora_udp_pkt instance
 * @return void
 */
lora_udp_pkt::lora_udp_pkt() {
    return;
}

/**
 * @param uint8_t* data
 * @param int size
 * @return void
 */
void lora_udp_pkt::parse(uint8_t* data, int size) {
    return;
}

/**
 * Returns the type of packet
 * @return lora_udp_pkt_types
 */
lora_udp_pkt_types lora_udp_pkt::get_pkt_type() {
    return null;
}

/**
 * Returns a rapidjson Document instance containing the packet info as described by the LoRa packet forwarder UDP protocol
 * @return Document
 */
rapidjson::Document lora_udp_pkt::get_rapidjson_doc() {
    return null;
}

/**
 * Returns the protocol version
 * @return int
 */
int lora_udp_pkt::get_protocol_version() {
    return 0;
}

/**
 * Returns the gateway mac address if applicable
 * @return uint64_t
 */
uint64_t lora_udp_pkt::get_gateway_mac_addr() {
    return null;
}

/**
 * Returns the packet random token
 * @return uint16_t
 */
uint16_t lora_udp_pkt::get_random_token() {
    return null;
}
