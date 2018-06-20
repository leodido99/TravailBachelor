/* File: main.cpp
 * Date: 
 * Author: Léonard Bise
 * Description: 
 */

#include "race_app_server.h"


#include <iostream>
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>

#include "logger.h"
/* Define logger level */
//loglevel_e loglevel = logDEBUG4;
loglevel_e loglevel = logINFO;

int main(int argc, char **argv) {
	race_app_server myAppServer;

	/* TODO Check arguments for :
	 * Config file
	 * verbose lvl */

	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "Current working dir: %s\n", cwd);

	try {
		/* Load configuration */
		myAppServer.load_configuration("global_conf.json");
	} catch(std::runtime_error &err) {
		std::cout << "Problem when loading configuration: " << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	/* Print configuration */
	myAppServer.print_configuration();
	/* Set verbose TODO From arguments */
	myAppServer.set_verbose(true);

	try {
		/* Start application server */
		myAppServer.start();
	} catch(std::runtime_error &err) {
		std::cout << "Application server error: " << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
