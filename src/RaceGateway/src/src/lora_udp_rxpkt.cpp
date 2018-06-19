/**
 * Project Untitled
 */


#include "lora_udp_rxpkt.h"

#include <iostream>

#include <rapidjson/document.h>

/**
 * lora_udp_rxpkt implementation
 * 
 * Class representing a LoRa UDP protocol rxpkt
 */

/**
 * Builds a UDP rxpkt from a rapidjson Document instance
 * @param doc
 * @return void
 */
lora_udp_rxpkt::lora_udp_rxpkt(rapidjson::Document* doc) {

	//std::cout << doc->GetString() << std::endl;
	//const rapidjson::Value& rxpkt = (*doc)["rxpkt"];
	//assert(rxpkt.IsArray());
	std::cout << "lora_udp_rxpkt" << std::endl;
	for (rapidjson::Value::ConstMemberIterator itr = doc->MemberBegin(); itr !=  doc->MemberEnd(); ++itr) {
		std::string objectType(itr->name.GetString());
		std::cout << "Member: " << objectType << std::endl;
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
		} else if (objectType.compare("RSSI") == 0) {
			this->RSSI = itr->value.GetInt();
		} else if (objectType.compare("lsnr") == 0) {
			this->lsnr = itr->value.GetDouble();
		} else if (objectType.compare("size") == 0) {
			this->size = itr->value.GetInt();
		} else if (objectType.compare("data") == 0) {
			this->data = itr->value.GetString();
		}
	}
}

/**
 * Returns the time of the packet reception
 * @return string
 */
std::string lora_udp_rxpkt::get_time() const {
    return this->time;
}

/**
 * Returns the GPS time of pkt RX, number of milliseconds since 06.Jan.1980
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_tmms() const {
    return this->tmms;
}

/**
 * Returns Internal timestamp of "RX finished" event (32b unsigned)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_tmst() const {
    return this->tmst;
}

/**
 * Returns RX central frequency in MHz (unsigned float, Hz precision)
 * @return float
 */
float lora_udp_rxpkt::get_freq() const {
    return this->freq;
}

/**
 * Returns the concentrator "IF" channel used for RX (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_chan() const {
    return this->chan;
}

/**
 * Returns the concentrator "RF chain" used for RX (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_rf_chain() const {
    return this->rf_chain;
}

/**
 * Returns CRC status: 1 = OK, -1 = fail, 0 = no CRC
 * @return int
 */
int lora_udp_rxpkt::get_stat() const {
    return this->stat;
}

/**
 * Returns the modulation identifier "LORA" or "FSK"
 * @return string
 */
std::string lora_udp_rxpkt::get_modu() const {
    return this->modu;
}

/**
 * Returns LoRa datarate identifier (eg. SF12BW500) or FSK datarate (unsigned, in bits per second) depending on the packet modulation
 * @return string
 */
std::string lora_udp_rxpkt::get_datr() const {
    return this->datr;
}

/**
 * Returns the LoRa ECC coding rate identifier
 * @return string
 */
std::string lora_udp_rxpkt::get_codr() const {
    return this->codr;
}

/**
 * Returns the RSSI in dBm (signed integer, 1 dB precision)
 * @return int
 */
int lora_udp_rxpkt::get_RSSI() const {
    return this->RSSI;
}

/**
 * Returns the Lora SNR ratio in dB (signed float, 0.1 dB precision)
 * @return float
 */
float lora_udp_rxpkt::get_lsnr() const {
    return this->lsnr;
}

/**
 * Returns the RF packet payload size in bytes (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_size() const {
    return this->size;
}

/**
 * Returns the Base64 encoded RF packet payload, padded
 * @return string
 */
std::string lora_udp_rxpkt::get_data() const {
    return this->data;
}

/**
 * Returns the decoded packet data as  a vector of bytes
 * @return vector<char>
 */
std::vector<char> lora_udp_rxpkt::get_decoded_data() const {
    return this->decoded_data;
}

std::ostream& operator<<(std::ostream &strm, const lora_udp_rxpkt &a) {
	// "decoded data=" << a.get_decoded_data() <<
	return strm << "lora_udp_rxpkt(" << "time=" << a.get_time() << " tmms=" << a.get_tmms() << " tmst=" << a.get_tmst() << "freq=" << a.get_freq() << " chan=" << a.get_chan() << "rf_chain=" << a.get_rf_chain() << " stat=" << a.get_stat() << " modu=" << a.get_modu() << " datr=" << a.get_datr() << " codr=" << a.get_codr() << " RSSI=" << a.get_RSSI() << " lsnr=" << a.get_lsnr() << " size=" << a.get_size() << "data=" << a.get_data() << ")";
}
