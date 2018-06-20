/**
 * Project Untitled
 */


#include "lora_rxpk_parser.h"

#include <iostream>

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
 * Constructor
 */
lora_rxpk_parser::lora_rxpk_parser(int protocol_version) {
	this->protocol_version = protocol_version;
	this->time = "";
	this->tmms = 0;
	this->tmst = 0;
	this->RSSI = 0;
	this->chan = 0;
	this->codr = "";
	this->data = "";
	this->datr = "";
	this->freq = 0.0;
	this->lsnr = 0.0;
	this->modu = "";
	this->size = 0;
	this->stat = -10;
	this->rf_chain = 0;
	this->decoded_data.clear();
}

/**
 * Parse the push data provided
 * @param data
 * @param size
 */
void lora_rxpk_parser::parse(uint8_t* data, int size) {
//	std::memcpy(&this->gateway_mac_addr, &data[4], LORA_UDP_PKT_GATEWAY_ADDR_SIZE);
//	this->json_string = std::string((char *)&data[4 + LORA_UDP_PKT_GATEWAY_ADDR_SIZE]);
//
//
//	log(logDEBUG4) << "lora_udp_pkt::parse: Parsing with rapidjson: " << this->json_string;
//	rapidjson::StringStream packet_stream(this->json_string.c_str());
//	rapidjson::Document doc;
//	/* Parsing json */
//	doc.ParseStream(packet_stream);
//	/* Get main json member name which define what type of packet has been received */
//	rapidjson::Value::ConstMemberIterator fileIt = doc.MemberBegin();
//	this->json_obj = fileIt->name.GetString();
//	log(logDEBUG4) << "lora_udp_pkt::parse: Main json member name: " << this->json_obj;
}

/**
 * UTC time of pkt RX, us precision, ISO 8601 'compact' format
 * @return string
 */
std::string lora_rxpk_parser::get_time() const {
    return this->time;
}

/**
 * GPS time of pkt RX, number of milliseconds since 06.Jan.1980
 * @return unsigned int
 */
unsigned int lora_rxpk_parser::get_tmms() const {
    return this->tmms;
}

/**
 * Internal timestamp of "RX finished" event (32b unsigned)
 * @return unsigned int
 */
unsigned int lora_rxpk_parser::get_tmst() const {
    return this->tmst;
}

/**
 * RX central frequency in MHz (unsigned float, Hz precision)
 * @return float
 */
float lora_rxpk_parser::get_freq() const {
    return this->freq;
}

/**
 * Concentrator "IF" channel used for RX (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_rxpk_parser::get_chan() const {
    return this->chan;
}

/**
 * Concentrator "RF chain" used for RX (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_rxpk_parser::get_rf_chain() const {
    return this->rf_chain;
}

/**
 * CRC status: 1 = OK, -1 = fail, 0 = no CRC
 * @return int
 */
int lora_rxpk_parser::get_stat() const {
    return this->stat;
}

/**
 * Modulation identifier "LORA" or "FSK"
 * @return string
 */
std::string lora_rxpk_parser::get_modu() const {
    return this->modu;
}

/**
 * LoRa datarate identifier (eg. SF12BW500) or FSK datarate (unsigned, in bits per second)
 * @return string
 */
std::string lora_rxpk_parser::get_datr() const {
    return this->datr;
}

/**
 * LoRa ECC coding rate identifier
 * @return string
 */
std::string lora_rxpk_parser::get_codr() const {
    return this->codr;
}

/**
 * RSSI in dBm (signed integer, 1 dB precision)
 * @return int
 */
int lora_rxpk_parser::get_RSSI() const {
    return this->RSSI;
}

/**
 * Lora SNR ratio in dB (signed float, 0.1 dB precision)
 * @return float
 */
float lora_rxpk_parser::get_lsnr() const {
    return this->lsnr;
}

/**
 * RF packet payload size in bytes (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_rxpk_parser::get_size() const {
    return this->size;
}

/**
 * Base64 encoded RF packet payload, padded
 * @return string
 */
std::string lora_rxpk_parser::get_data() const {
    return this->data;
}

/**
 * Decoded RF packet payload as a vector of bytes
 * @return vector<char>
 */
std::vector<char> lora_rxpk_parser::get_decoded_data() const {
	return this->decoded_data;
}

std::ostream& operator<<(std::ostream &strm, const lora_rxpk_parser &a) {
	// "decoded data=" << a.get_decoded_data() <<
	return strm << "lora_push_data_parser(" << "time=" << a.get_time() << " tmms=" << a.get_tmms() << " tmst=" << a.get_tmst() << "freq=" << a.get_freq() << " chan=" << a.get_chan() << "rf_chain=" << a.get_rf_chain() << " stat=" << a.get_stat() << " modu=" << a.get_modu() << " datr=" << a.get_datr() << " codr=" << a.get_codr() << " RSSI=" << a.get_RSSI() << " lsnr=" << a.get_lsnr() << " size=" << a.get_size() << "data=" << a.get_data() << ")";
}