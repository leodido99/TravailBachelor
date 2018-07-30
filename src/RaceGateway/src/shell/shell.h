/**
 * @file shell.h
 * @brief Shell
 *
 * The application shell provides a way for the user
 * to enter commands to perform various operations on the race_app_server.
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */

#ifndef SRC_SHELL_H_
#define SRC_SHELL_H_

#include "shell_command.h"

#include <thread>
#include <list>

class shell {
private:
	std::thread* shell_thread;
	bool shell_thread_running;
	std::list<shell_command*> commands;

	/**
	 * Shell thread method
	 */
	void shell_handler();

	/**
	 * Returns a list of parameters
	 * @return
	 */
	std::list<std::string>* get_parameters(std::string cmd_line);

	/**
	 * Executes a command
	 * @param cmd
	 * @param params
	 */
	void execute_cmd(std::string cmd, std::list<std::string>* params);
public:
	/**
	 * Constructor
	 */
	shell();

	/**
	 * Starts the shell
	 */
	void start();

	/**
	 * Adds a new command to the shell
	 * @param
	 */
	void add_cmd(shell_command* cmd);

	/**
	 * Print all the registered commands
	 */
	void print_all_commands();
};

#endif /* SRC_SHELL_H_ */
