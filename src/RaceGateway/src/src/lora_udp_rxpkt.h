/**
 * Project Untitled
 */


#ifndef _LORA_UDP_RXPKT_H
#define _LORA_UDP_RXPKT_H

#include <cstdint>
#include <string>
#include <rapidjson/document.h>

class lora_udp_rxpkt {
public: 
    
/**
 * Builds a UDP rxpkt from a rapidjson Document instance
 * @param doc
 */
lora_udp_rxpkt(rapidjson::Document doc);
    
/**
 * Returns the time of the packet reception
 */
std::string get_time();
    
/**
 * Returns the GPS time of pkt RX, number of milliseconds since 06.Jan.1980
 */
unsigned int get_tmms();
    
/**
 * Returns Internal timestamp of "RX finished" event (32b unsigned)
 */
unsigned int get_tmst();
    
/**
 * Returns RX central frequency in MHz (unsigned float, Hz precision)
 */
float get_freq();
    
/**
 * Returns the concentrator "IF" channel used for RX (unsigned integer)
 */
unsigned int get_chan();
    
/**
 * Returns the concentrator "RF chain" used for RX (unsigned integer)
 */
unsigned int get_rf_chain();
    
/**
 * Returns CRC status: 1 = OK, -1 = fail, 0 = no CRC
 */
int get_stat();
    
/**
 * Returns the modulation identifier "LORA" or "FSK"
 */
std::string get_modu();
    
/**
 * Returns LoRa datarate identifier (eg. SF12BW500) or FSK datarate (unsigned, in bits per second) depending on the packet modulation
 */
std::string get_datr();
    
/**
 * Returns the LoRa ECC coding rate identifier
 */
std::string get_codr();
    
/**
 * Returns the RSSI in dBm (signed integer, 1 dB precision)
 */
int get_RSSI();
    
/**
 * Returns the Lora SNR ratio in dB (signed float, 0.1 dB precision)
 */
float get_lsnr();
    
/**
 * Returns the RF packet payload size in bytes (unsigned integer)
 */
unsigned int get_size();
    
/**
 * Returns the Base64 encoded RF packet payload, padded
 */
std::string get_data();
    
/**
 * Returns the decoded packet data as  a vector of bytes
 */
std::vector<char> get_decoded_data();
};

#endif //_LORA_UDP_RXPKT_H
