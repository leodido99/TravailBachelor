/* File: race_app_server.h
 * Date: 
 * Author: Léonard Bise
 * Description: 
 */

#include <string>

using namespace std;

class race_app_server {
	private:
		int listen_port;
		int listen_socket;
		bool verbose;

		/**
		 * Setup and connect to the port to listen for incoming packets
		 */
		void connect_listen();

		/**
		 * Log a message to the console
		 * If the verbose flag is false nothing is printed
		 * @param msg Message to print
		 */
		void log(string msg);

	public:
		/**
		 * Create a new instance of the class
		 */
		race_app_server();

		/**
		 * Loads a configuration file
		 * @param configfile Path to the configuration file
		 */
		void load_configuration(string configfile);
	
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
		 * Set the verbosity
		 * @param verbose true = verbose mode on; false = verbose mode off
		 */
		void set_verbose(bool verbose);
};
