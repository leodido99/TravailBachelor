/* File: race_app_server.h
 * Date: 
 * Author: Léonard Bise
 * Description: 
 */

#include <string>
#include <thread>
#include <cstdint>

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
