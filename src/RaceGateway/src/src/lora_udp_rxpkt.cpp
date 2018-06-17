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
lora_udp_rxpkt::lora_udp_rxpkt(rapidjson::Document doc) {
    return;
}

/**
 * Returns the time of the packet reception
 * @return string
 */
std::string lora_udp_rxpkt::get_time() {
    return "";
}

/**
 * Returns the GPS time of pkt RX, number of milliseconds since 06.Jan.1980
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_tmms() {
    return null;
}

/**
 * Returns Internal timestamp of "RX finished" event (32b unsigned)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_tmst() {
    return null;
}

/**
 * Returns RX central frequency in MHz (unsigned float, Hz precision)
 * @return float
 */
float lora_udp_rxpkt::get_freq() {
    return 0.0;
}

/**
 * Returns the concentrator "IF" channel used for RX (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_chan() {
    return null;
}

/**
 * Returns the concentrator "RF chain" used for RX (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_rf_chain() {
    return null;
}

/**
 * Returns CRC status: 1 = OK, -1 = fail, 0 = no CRC
 * @return int
 */
int lora_udp_rxpkt::get_stat() {
    return 0;
}

/**
 * Returns the modulation identifier "LORA" or "FSK"
 * @return string
 */
std::string lora_udp_rxpkt::get_modu() {
    return "";
}

/**
 * Returns LoRa datarate identifier (eg. SF12BW500) or FSK datarate (unsigned, in bits per second) depending on the packet modulation
 * @return string
 */
std::string lora_udp_rxpkt::get_datr() {
    return "";
}

/**
 * Returns the LoRa ECC coding rate identifier
 * @return string
 */
std::string lora_udp_rxpkt::get_codr() {
    return "";
}

/**
 * Returns the RSSI in dBm (signed integer, 1 dB precision)
 * @return int
 */
int lora_udp_rxpkt::get_RSSI() {
    return 0;
}

/**
 * Returns the Lora SNR ratio in dB (signed float, 0.1 dB precision)
 * @return float
 */
float lora_udp_rxpkt::get_lsnr() {
    return 0.0;
}

/**
 * Returns the RF packet payload size in bytes (unsigned integer)
 * @return unsigned int
 */
unsigned int lora_udp_rxpkt::get_size() {
    return null;
}

/**
 * Returns the Base64 encoded RF packet payload, padded
 * @return string
 */
std::string lora_udp_rxpkt::get_data() {
    return "";
}

/**
 * Returns the decoded packet data as  a vector of bytes
 * @return vector<char>
 */
std::vector<char> lora_udp_rxpkt::get_decoded_data() {
    return null;
}
