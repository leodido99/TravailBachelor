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
	/* Test mode */
	test_mode_handler* test_handler = new test_mode_handler();
	myAppServer.set_rxpk_handler(test_handler);

	try {
		/* Start application server */
		myAppServer.start();
	} catch(std::runtime_error &err) {
		std::cout << "Application server error: " << err.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	/* Configure shell */
	test_mode_cmd* test_cmd = new test_mode_cmd(test_handler);
	myShell.add_cmd(test_cmd);

	/* Start shell */
	myShell.start();
	return EXIT_SUCCESS;
}
