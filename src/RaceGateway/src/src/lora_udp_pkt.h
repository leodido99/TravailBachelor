/**
 * Project Untitled
 */

#ifndef _LORA_UDP_PKT_H
#define _LORA_UDP_PKT_H

#include <cstdint>
#include <string>
#include <rapidjson/document.h>

/**
 * The different types of packets used by the LoRa packet forwarder UDP protocol
 */
enum lora_udp_pkt_types {
	PUSH_DATA,		  //!< PUSH_DATA
	PUSH_ACK,          //!< PUSH_ACK
	PULL_DATA,         //!< PULL_DATA
	PULL_ACK,          //!< PULL_ACK
	PULL_RESP,         //!< PULL_RESP
	TX_ACK,             //!< TX_ACK
	UNKNOWN_TYPE
};

/**
 * Names associated with lora_udp_pkt_types
 */
extern std::string lora_udp_pkt_names[];

class lora_udp_pkt {
private:
    int protocol_version;
    uint16_t random_token;
    uint64_t gateway_mac_addr;
    lora_udp_pkt_types packet_type;
    std::string json_string;
    std::string json_obj;
public: 
    
/**
 * Constructor
 */
lora_udp_pkt();

/**
 * Destructor
 */
~lora_udp_pkt();

/**
 * @param uint8_t* data
 * @param int size
 */
void parse(uint8_t* data, int size);
    
/**
 * Returns the type of packet
 */
lora_udp_pkt_types get_pkt_type();
    
/**
 * Returns the protocol version
 */
int get_protocol_version();
    
/**
 * Returns the gateway mac address if applicable
 */
uint64_t get_gateway_mac_addr();
    
/**
 * Returns the packet random token
 */
uint16_t get_random_token();

/**
 * Returns the main json object name
 * @return
 */
std::string get_json_obj_name();

/**
 * Returns the complete json string
 * @return
 */
std::string get_json_string();

/**
 * Returns a string representing the class instance
 * @return
 */
std::string get_string();
};

#endif //_LORA_UDP_PKT_H
