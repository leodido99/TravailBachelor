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
#include <sstream>

shell::shell() {
	this->shell_thread = NULL;
	this->shell_thread_running = true;
}



void shell::shell_handler() {
	std::string cmd_line;

	while(this->shell_thread_running) {
		std::cout << ">>";
		std::getline(std::cin, cmd_line);
		/* Find first word */
		std::string cmd = cmd_line.substr(0, cmd_line.find(" "));
		/* Remove command word */
		cmd_line=cmd_line.substr(cmd_line.find_first_of(" \t")+1);
		this->execute_cmd(cmd, this->get_parameters(cmd_line));
	}
}

void shell::execute_cmd(std::string cmd, std::list<std::string>* params) {
	for (std::list<shell_command*>::iterator it=this->commands.begin(); it != this->commands.end(); ++it) {
		if (**it == cmd) {
			try {
				(**it).exec(params);
			} catch (std::runtime_error& e) {
				std::cout << ">> Error: " << cmd << " : " << e.what() << std::endl;
			}
			break;
		}
	}
}

std::list<std::string>* shell::get_parameters(std::string cmd_line) {
	std::list<std::string>* mylist = new std::list<std::string>();
	std::stringstream mystream(cmd_line);
	std::string str;
	while(mystream >> str) {
		mylist->push_back(str);
	}
	return mylist;
}

void shell::start() {
	/* Create shell thread */
	this->shell_thread = new std::thread(&shell::shell_handler, this);
	this->shell_thread->join();
}

void shell::add_cmd(shell_command* cmd) {
	this->commands.push_back(cmd);
}
