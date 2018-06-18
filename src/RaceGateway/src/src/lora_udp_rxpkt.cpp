/**
 * Project Untitled
 */


#include "lora_udp_rxpkt.h"

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
	/*std::string time;
		unsigned int tmms;
		unsigned int tmst;
		float freq;
		unsigned int chan;
		unsigned int rf_chain;
		int stat;
		std::string modu;
		std::string datr;
		std::string codr;
		int RSSI;
		float lsnr;
		int size;
		std::string data;
		std::vector<char> decoded_data;*/






    return;
}

/**
 * Returns the time of the packet reception
 * @return string
 */
std::string lora_udp_rxpkt::get_time() {
    return this->time;
}

/**
 * Returns the GPS time of pkt RX, number of milliseconds since 06.Jan.1980
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_tmms() {
    return this->tmms;
}

/**
 * Returns Internal timestamp of "RX finished" event (32b unsigned)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_tmst() {
    return this->tmst;
}

/**
 * Returns RX central frequency in MHz (unsigned float, Hz precision)
 * @return float
 */
float lora_udp_rxpkt::get_freq() {
    return this->freq;
}

/**
 * Returns the concentrator "IF" channel used for RX (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_chan() {
    return this->chan;
}

/**
 * Returns the concentrator "RF chain" used for RX (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_rf_chain() {
    return this->rf_chain;
}

/**
 * Returns CRC status: 1 = OK, -1 = fail, 0 = no CRC
 * @return int
 */
int lora_udp_rxpkt::get_stat() {
    return this->stat;
}

/**
 * Returns the modulation identifier "LORA" or "FSK"
 * @return string
 */
std::string lora_udp_rxpkt::get_modu() {
    return this->modu;
}

/**
 * Returns LoRa datarate identifier (eg. SF12BW500) or FSK datarate (unsigned, in bits per second) depending on the packet modulation
 * @return string
 */
std::string lora_udp_rxpkt::get_datr() {
    return this->datr;
}

/**
 * Returns the LoRa ECC coding rate identifier
 * @return string
 */
std::string lora_udp_rxpkt::get_codr() {
    return this->codr;
}

/**
 * Returns the RSSI in dBm (signed integer, 1 dB precision)
 * @return int
 */
int lora_udp_rxpkt::get_RSSI() {
    return this->RSSI;
}

/**
 * Returns the Lora SNR ratio in dB (signed float, 0.1 dB precision)
 * @return float
 */
float lora_udp_rxpkt::get_lsnr() {
    return this->lsnr;
}

/**
 * Returns the RF packet payload size in bytes (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_size() {
    return this->size;
}

/**
 * Returns the Base64 encoded RF packet payload, padded
 * @return string
 */
std::string lora_udp_rxpkt::get_data() {
    return this->data;
}

/**
 * Returns the decoded packet data as  a vector of bytes
 * @return vector<char>
 */
std::vector<char> lora_udp_rxpkt::get_decoded_data() {
    return this->decoded_data;
}
