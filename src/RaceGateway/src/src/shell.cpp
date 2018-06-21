/**
 * @file shell.cpp
 * @brief Shell
 *
 * The application shell provides a way for the user
 * to enter commands to perform various operations on the race_app_server.
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */

#include "shell.h"

#include <cstring>
#include <iostream>

shell::shell() {
	this->shell_thread = NULL;
	this->shell_thread_running = true;
}

void shell::shell_handler() {
	std::string cmd;

	while(this->shell_thread_running) {
		std::getline(std::cin, cmd);
		std::cout << "Entered text: " << cmd << std::endl;
	}
}

void shell::start() {
	/* Create shell thread */
	this->shell_thread = new std::thread(&shell::shell_handler, this);
	this->shell_thread->join();
}
