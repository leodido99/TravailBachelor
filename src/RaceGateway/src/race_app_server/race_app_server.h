/**
 * @file race_app_server.h
 * @brief Race Application Server
 *
 * The Race Application Server is the main class used for the reception of the packets
 * produced by the race sensors. Packets are received on the LoRa receiver, a packet forwarder
 * then creates a json and sends it through a socket as an udp datagram. This class
 * listens on this socket, retrieves the udp packet, decodes it and extracts the json string
 * using various classes. After the json string is extracted, the data inside is pushed to
 * the database.
 *
 * Semtech's packet forwarder (https://github.com/Lora-net/packet_forwarder)
 * was used as a base to develop this class, in particular the util_sink application.
 *
 * @hallard Single channel gateway (https://github.com/hallard/single_chan_pkt_fwd) implementation was also
 * used as a base (in particular the rapidjson usage for the configuration)
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */

#include <string>
#include <thread>
#include <cstdint>

#include "lora_rxpk_parser.h"

/**
 * Class handling the race application server
 * It listens on the provided port for UDP packets
 * which are sent by the packet forwarder. Once received, the packet is
 * processed and the encapsulated data is extracted and pushed to the database
 */
class race_app_server {
	private:
		int listen_port;
		int listen_socket;
		bool verbose;
		bool is_thread_running;
		std::thread* listening_thread;

		/**
		 * Setup and connect to the port to listen for incoming packets
		 */
		void connect_listen();

		/**
		 * Listen on the port for new data
		 * When new data is received it will process it
		 */
		void listen();

		/**
		 * Process an UDP datagram sent by the packet forwarder
		 * @param data Buffer containing the data
		 * @param size Size of the data
		 */
		void process_datagram(uint8_t* data, int size);

		/**
		 * Closes the socket
		 */
		void end_listen();

		/**
		 * Handles an rxpk packet
		 */
		void handle_rxpk(lora_rxpk_parser rxpk);
	public:
		/**
		 * Create a new instance of the class
		 */
		race_app_server();

		/**
		 * Loads a configuration file
		 * @param configfile Path to the configuration file
		 */
		void load_configuration(std::string configfile);
	
		/**
		 * Prints the class configuration
		 */
		void print_configuration();

		/**
		 * Starts the application server
		 * It will start listening for incoming packets from
		 * the packet forwarder and place the data in the database.
		 */
		void start();

		/**
		 * Stops the application server
		 */
		void stop();

		/**
		 * Set the verbosity
		 * @param verbose true = verbose mode on; false = verbose mode off
		 */
		void set_verbose(bool verbose);
};
