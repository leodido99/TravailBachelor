/**
 * Project Untitled
 */


#ifndef _LORA_UDP_RXPKT_H
#define _LORA_UDP_RXPKT_H

#include <cstdint>
#include <string>
#include <vector>
#include <rapidjson/document.h>

class lora_udp_rxpkt {
private:
	std::string time;
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
	std::vector<char> decoded_data;
public: 
    
/**
 * Builds a UDP rxpkt from a rapidjson Document instance
 * @param doc
 */
lora_udp_rxpkt(rapidjson::Document* doc);

/**
 * Parse a json document representing a LoRa UDP rxpk packet
 * @param doc
 */
void parse_json(rapidjson::Document* doc);
    
/**
 * Returns the time of the packet reception
 */
std::string get_time() const;
    
/**
 * Returns the GPS time of pkt RX, number of milliseconds since 06.Jan.1980
 */
unsigned int get_tmms() const;
    
/**
 * Returns Internal timestamp of "RX finished" event (32b unsigned)
 */
unsigned int get_tmst() const;
    
/**
 * Returns RX central frequency in MHz (unsigned float, Hz precision)
 */
float get_freq() const;
    
/**
 * Returns the concentrator "IF" channel used for RX (unsigned integer)
 */
unsigned int get_chan() const;
    
/**
 * Returns the concentrator "RF chain" used for RX (unsigned integer)
 */
unsigned int get_rf_chain() const;
    
/**
 * Returns CRC status: 1 = OK, -1 = fail, 0 = no CRC
 */
int get_stat() const;
    
/**
 * Returns the modulation identifier "LORA" or "FSK"
 */
std::string get_modu() const;
    
/**
 * Returns LoRa datarate identifier (eg. SF12BW500) or FSK datarate (unsigned, in bits per second) depending on the packet modulation
 */
std::string get_datr() const;
    
/**
 * Returns the LoRa ECC coding rate identifier
 */
std::string get_codr() const;
    
/**
 * Returns the RSSI in dBm (signed integer, 1 dB precision)
 */
int get_RSSI() const;
    
/**
 * Returns the Lora SNR ratio in dB (signed float, 0.1 dB precision)
 */
float get_lsnr() const;
    
/**
 * Returns the RF packet payload size in bytes (unsigned integer)
 */
unsigned int get_size() const;
    
/**
 * Returns the Base64 encoded RF packet payload, padded
 */
std::string get_data() const;
    
/**
 * Returns the decoded packet data as  a vector of bytes
 */
std::vector<char> get_decoded_data() const;

/**
 * Insertion operator
 * @param strm
 * @param a
 * @return
 */
friend std::ostream& operator<<(std::ostream &strm, const lora_udp_rxpkt &a);
};

#endif //_LORA_UDP_RXPKT_H
