/**
 * @file lora_rxpk_parser.cpp
 * @brief LoRa rxpk packet parser
 *
 * Parses rxpk packets produced by a LoRa packet forwarder which
 * contains an RF packet sent by a node.
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */

#include "lora_rxpk_parser.h"

#include "logger.h"
#include "base64.h"

#include <iostream>
#include <stdexcept>

/**
 * The maximum size of a LoRa RF packet
 */
#define LORA_MAX_PAYLOAD_SIZE 222

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
void lora_rxpk_parser::parse(std::string json_str) {
	rapidjson::StringStream packet_stream(json_str.c_str());
	rapidjson::Document packet;

	//log(logDEBUG4) << "lora_rxpk_parser::parse: Parsing " << json_str;

	/*Parse json */
	packet.ParseStream(packet_stream);

	/* Ensure that the main member is rxpk */
	if (packet.HasMember("rxpk")) {
		if (!packet["rxpk"].IsArray()) {
			throw std::runtime_error("json member rxpk is not an array");
		}
	} else {
		throw std::runtime_error("json member rxpk missing");
	}

	if (this->protocol_version == 1) {
		this->parse_prot_v1(&packet);
	} else {
		throw std::runtime_error("Unknown protocol verison " + this->protocol_version);
	}
}

/**
 * Parse packet following version 1 of the protocol
 * @param packet Packet to parse
 */
void lora_rxpk_parser::parse_prot_v1(rapidjson::Document* packet) {
	/* Iterate over all members */
	for (rapidjson::SizeType i = 0; i < (*packet)["rxpk"].Size(); i++){
		const rapidjson::Value& c = (*packet)["rxpk"][i];
		for (rapidjson::Value::ConstMemberIterator itr = c.MemberBegin(); itr != c.MemberEnd(); ++itr){
			std::string objectType(itr->name.GetString());
			if (objectType.compare("time") == 0) {
				this->time = itr->value.GetString();
			} else if (objectType.compare("tmms") == 0) {
				this->tmms = itr->value.GetUint();
			} else if (objectType.compare("tmst") == 0) {
				this->tmst = itr->value.GetUint();
			} else if (objectType.compare("freq") == 0) {
				this->freq = itr->value.GetDouble();
			} else if (objectType.compare("chan") == 0) {
				this->chan = itr->value.GetUint();
			} else if (objectType.compare("rf_chain") == 0) {
				this->rf_chain = itr->value.GetUint();
			} else if (objectType.compare("stat") == 0) {
				this->stat = itr->value.GetInt();
			} else if (objectType.compare("modu") == 0) {
				this->modu = itr->value.GetString();
			} else if (objectType.compare("datr") == 0) {
				this->datr = itr->value.GetString();
			} else if (objectType.compare("codr") == 0) {
				this->codr = itr->value.GetString();
			} else if (objectType.compare("rssi") == 0) {
				this->RSSI = itr->value.GetInt();
			} else if (objectType.compare("lsnr") == 0) {
				this->lsnr = itr->value.GetDouble();
			} else if (objectType.compare("size") == 0) {
				this->size = itr->value.GetUint();
			} else if (objectType.compare("data") == 0) {
				this->data = itr->value.GetString();
			}
		}
	}

	/* Decode data from base64 */
	uint8_t buffer[LORA_MAX_PAYLOAD_SIZE];
	int nb_bytes_conv = b64_to_bin(this->data.c_str(), this->data.size(), buffer, LORA_MAX_PAYLOAD_SIZE);
	if (nb_bytes_conv == -1 || (unsigned int)nb_bytes_conv != this->size) {
		throw std::runtime_error("Couldn't convert the base64 data (actual=" + std::to_string(nb_bytes_conv) + " expected=" + std::to_string(this->size));
	}
	this->decoded_data.assign(buffer, buffer + nb_bytes_conv);
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
std::vector<unsigned char> lora_rxpk_parser::get_decoded_data() const {
	return this->decoded_data;
}

/**
 * Returns a string representing the decoded data
 * @return
 */
std::string lora_rxpk_parser::get_decoded_data_string() const {
	std::stringstream datastream;
	for (std::vector<unsigned char>::const_iterator i = this->decoded_data.begin(); i != this->decoded_data.end(); ++i) {
		datastream << std::hex << static_cast<unsigned>(*i);
	}
	return datastream.str();
}

/**
 * Insertion operator used to print class content
 * @param strm
 * @param a
 * @return
 */
std::ostream& operator<<(std::ostream &strm, const lora_rxpk_parser &a) {
	return strm << "lora_rxpk_parser(" << "time=" << a.get_time() << " tmms=" << a.get_tmms() << " tmst=" << a.get_tmst() << " freq=" << a.get_freq() << " chan=" << a.get_chan() << " rf_chain=" << a.get_rf_chain() << " stat=" << a.get_stat() << " modu=" << a.get_modu() << " datr=" << a.get_datr() << " codr=" << a.get_codr() << " RSSI=" << a.get_RSSI() << " lsnr=" << a.get_lsnr() << " size=" << a.get_size() << " data=" << a.get_data() << " decoded=" << a.get_decoded_data_string() << ")";
}
