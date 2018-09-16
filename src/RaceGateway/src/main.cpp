/**
 * @file main.c
 * @brief Main file
 *
 * Entry point for the race_app_server application
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */

#include "race_app_server.h"
#include "test_mode_handler.h"
#include "race_mode_handler.h"

#include "shell.h"
#include "test_mode_cmd.h"

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
	shell myShell;
	bool enableShell = true;

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

	if (argc > 1) {
		if (strcmp(argv[1], "-noshell") == 0) {
			std::cout << "No shell mode enabled" << std::endl;
			enableShell = false;
		}
	}

	/* Print configuration */
	myAppServer.print_configuration();
	myAppServer.set_verbose(true);

	/* Test mode */
	test_mode_handler* test_handler = new test_mode_handler();
	race_mode_handler* race_handler = new race_mode_handler();
	myAppServer.set_rxpk_handler(race_handler);

	try {
		/* Start application server */
		myAppServer.start();
	} catch(std::runtime_error &err) {
		std::cout << "Application server error: " << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	if (enableShell) {
		/* Configure shell */
		test_mode_cmd* test_cmd = new test_mode_cmd(test_handler);
		myShell.add_cmd(test_cmd);

		/* Start shell - blocking */
		myShell.start();
	} else {
		myAppServer.join();
	}

	return EXIT_SUCCESS;
}
