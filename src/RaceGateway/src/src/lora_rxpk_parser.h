/**
 * Project Untitled
 */


#ifndef _LORA_RXPK_PARSER_H
#define _LORA_RXPK_PARSER_H

#include <cstdint>
#include <string>
#include <vector>

class lora_rxpk_parser {
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
    unsigned int size;
    std::string data;
    std::vector<char> decoded_data;
    int protocol_version;
public: 
    /**
     * Constructor
     * @param protocol_version Protocol version of the packet
     */
	lora_rxpk_parser(int protocol_version);

	/**
	 * Parse the push data provided
	 * @param data
	 * @param size
	 */
	void parse(uint8_t* data, int size);

	/**
	 * UTC time of pkt RX, us precision, ISO 8601 'compact' format
	 */
	std::string get_time() const;

	/**
	 * GPS time of pkt RX, number of milliseconds since 06.Jan.1980
	 */
	unsigned int get_tmms() const;

	/**
	 * Internal timestamp of "RX finished" event (32b unsigned)
	 */
	unsigned int get_tmst() const;

	/**
	 * RX central frequency in MHz (unsigned float, Hz precision)
	 */
	float get_freq() const;

	/**
	 * Concentrator "IF" channel used for RX (unsigned integer)
	 */
	unsigned int get_chan() const;

	/**
	 * Concentrator "RF chain" used for RX (unsigned integer)
	 */
	unsigned int get_rf_chain() const;

	/**
	 * CRC status: 1 = OK, -1 = fail, 0 = no CRC
	 */
	int get_stat() const;

	/**
	 * Modulation identifier "LORA" or "FSK"
	 */
	std::string get_modu() const;

	/**
	 * LoRa datarate identifier (eg. SF12BW500) or FSK datarate (unsigned, in bits per second)
	 */
	std::string get_datr() const;

	/**
	 * LoRa ECC coding rate identifier
	 */
	std::string get_codr() const;

	/**
	 * RSSI in dBm (signed integer, 1 dB precision)
	 */
	int get_RSSI() const;

	/**
	 * Lora SNR ratio in dB (signed float, 0.1 dB precision)
	 */
	float get_lsnr() const;

	/**
	 * RF packet payload size in bytes (unsigned integer)
	 */
	unsigned int get_size() const;

	/**
	 * Base64 encoded RF packet payload, padded
	 */
	std::string get_data() const;

	/**
	 * Decoded RF packet payload as a vector of bytes
	 */
	std::vector<char> get_decoded_data() const;

	/**
	 * Friend insertion operator to be able to print private members to stream
	 * @param strm
	 * @param a
	 * @return
	 */
	friend std::ostream& operator<<(std::ostream &strm, const lora_rxpk_parser &a);
};

#endif //_LORA_RXPK_PARSER_H
